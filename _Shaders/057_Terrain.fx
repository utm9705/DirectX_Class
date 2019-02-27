#include "000_Header.fx"

struct TerrainDesc
{
    float MinDistance;
    float MaxDistance;
    float MinTesselation;
    float MaxTesselation;

    float TexelCellSpaceU;
    float TexelCellSpaceV;
    float WorldCellSpace;

    float2 TexScale = 66.0f;
};
TerrainDesc Desc;


Texture2D HeightMap;
SamplerState HeightMapSampler
{
    Filter = MIN_MAG_MIP_POINT;
};

struct VertexOutput
{
    float4 Position : Position0;
    float2 Uv : Uv0;
};

VertexOutput VS(VertexTexture input)
{
    VertexOutput output;
    output.Position = input.Position;
    output.Uv = input.Uv;

    return output;
}

struct ConstantHullOutput
{
    float Edge[4] : SV_TessFactor;
    float Inside[2] : SV_InsideTessFactor;
};

float CalcTessFactor(float3 position)
{
    float d = distance(position, ViewPosition);
    float s = saturate((d - Desc.MinDistance) / (Desc.MaxDistance - Desc.MinDistance));

    return pow(2, lerp(Desc.MaxTesselation, Desc.MinTesselation, s));
}

ConstantHullOutput HS_Constant(InputPatch<VertexOutput, 4> input)
{
    ConstantHullOutput output;
    
    float3 e0 = (input[0].Position + input[2].Position).xyz * 0.5f;
    float3 e1 = (input[0].Position + input[1].Position).xyz * 0.5f;
    float3 e2 = (input[1].Position + input[3].Position).xyz * 0.5f;
    float3 e3 = (input[2].Position + input[3].Position).xyz * 0.5f;
    float3 c = (input[0].Position + input[1].Position + input[2].Position + input[3].Position).xyz * 0.25f;

    output.Edge[0] = CalcTessFactor(e0);
    output.Edge[1] = CalcTessFactor(e1);
    output.Edge[2] = CalcTessFactor(e2);
    output.Edge[3] = CalcTessFactor(e3);

    output.Inside[0] = CalcTessFactor(c);
    output.Inside[1] = output.Inside[0];

    return output;
}


struct HullOutput
{
    float4 Position : Position0;
    float2 Uv : Uv0;
};

[domain("quad")]
[partitioning("integer")]
//[outputtopology("triangle_cw")]
[partitioning("fractional_even")]
[outputcontrolpoints(4)]
[patchconstantfunc("HS_Constant")]
HullOutput HS(InputPatch<VertexOutput, 4> input, uint pointID : SV_OutputControlPointID)
{
    HullOutput output;
    output.Position = input[pointID].Position;
    output.Uv = input[pointID].Uv;

    return output;
}


struct DomainOutput
{
    float4 Position : SV_Position0;
    float3 wPosition : Position1;
    float2 Uv : Uv0;
    float2 TiledUv : Uv1;
};

[domain("quad")]
DomainOutput DS(ConstantHullOutput input, float2 uv : SV_DomainLocation, const OutputPatch<HullOutput, 4> patch)
{
    DomainOutput output;

    float3 p0 = lerp(patch[0].Position, patch[1].Position, uv.x).xyz;
    float3 p1 = lerp(patch[2].Position, patch[3].Position, uv.x).xyz;
    float3 position = lerp(p0, p1, uv.y);
    output.wPosition = position;

    float2 uv0 = lerp(patch[0].Uv, patch[1].Uv, uv.x);
    float2 uv1 = lerp(patch[2].Uv, patch[3].Uv, uv.x);
    output.Uv = lerp(uv0, uv1, uv.y);


    output.wPosition.y = HeightMap.SampleLevel(HeightMapSampler, output.Uv, 0).r;

    output.Position = mul(float4(output.wPosition, 1), View);
    output.Position = mul(output.Position, Projection);

    output.TiledUv = output.Uv * Desc.TexScale;

    return output;
}


SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};
float4 PS(DomainOutput input) : SV_TARGET
{
    float2 left = input.Uv + float2(-Desc.TexelCellSpaceU, 0.0f);
    float2 right = input.Uv + float2(Desc.TexelCellSpaceU, 0.0f);
    float2 top = input.Uv + float2(0.0f, -Desc.TexelCellSpaceV);
    float2 bottom = input.Uv + float2(0.0f, Desc.TexelCellSpaceV);

    float leftY = HeightMap.SampleLevel(HeightMapSampler, left, 0).r;
    float rightY = HeightMap.SampleLevel(HeightMapSampler, right, 0).r;
    float topY = HeightMap.SampleLevel(HeightMapSampler, top, 0).r;
    float bottomY = HeightMap.SampleLevel(HeightMapSampler, bottom, 0).r;

    float3 tangent = normalize(float3(Desc.WorldCellSpace * 2.0f, rightY - leftY, 0.0f));
    float3 biTangent = normalize(float3(0.0f, bottomY - topY, Desc.WorldCellSpace * -2.0f));
    float3 normalW = cross(tangent, biTangent);


    return float4(1, 0, 0, 1);
}

///////////////////////////////////////////////////////////////////////////////

RasterizerState RS
{
    FillMode = Wireframe;
};

technique11 T0
{
    pass P0
    {
        SetRasterizerState(RS);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetHullShader(CompileShader(hs_5_0, HS()));
        SetDomainShader(CompileShader(ds_5_0, DS()));

        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
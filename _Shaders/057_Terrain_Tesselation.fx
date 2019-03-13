#include "000_Header_N.fx"
#include "000_Terrain.fx"

Texture2D HeightMap;
SamplerState HeightMapSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

VertexOutput_Terrain VS(VertexInput_Terrain input)
{
    VertexOutput_Terrain output;
    output.Position = input.Position;
    output.Uv = input.Uv;
    output.BoundY = input.BoundY;

    return output;
}

ConstantHullOutput_Terrain HS_Constant(InputPatch<VertexOutput_Terrain, 4> input)
{
    ConstantHullOutput_Terrain output;

    float minY = input[0].BoundY.x;
    float maxY = input[0].BoundY.y;

    float3 vMin = float3(input[2].Position.x, minY, input[2].Position.z);
    float3 vMax = float3(input[1].Position.x, maxY, input[1].Position.z);


    float3 boxCenter = (vMin + vMax) * 0.5f;
    float3 boxExtents = (vMax - vMin) * 0.5f;

    if (AabbOutsideFrustumTest(boxCenter, boxExtents))
    {
        output.Edge[0] = 0.0f;
        output.Edge[1] = 0.0f;
        output.Edge[2] = 0.0f;
        output.Edge[3] = 0.0f;

        output.Inside[0] = 0.0f;
        output.Inside[1] = 0.0f;

        return output;
    }

    
    float3 e0 = (input[0].Position + input[2].Position).xyz * 0.5f;
    float3 e1 = (input[0].Position + input[1].Position).xyz * 0.5f;
    float3 e2 = (input[1].Position + input[3].Position).xyz * 0.5f;
    float3 e3 = (input[2].Position + input[3].Position).xyz * 0.5f;
    float3 c = (input[0].Position + input[1].Position + input[2].Position + input[3].Position).xyz * 0.25f;

    output.Edge[0] = TerrainTessFactor(e0, ViewPosition());
    output.Edge[1] = TerrainTessFactor(e1, ViewPosition());
    output.Edge[2] = TerrainTessFactor(e2, ViewPosition());
    output.Edge[3] = TerrainTessFactor(e3, ViewPosition());

    output.Inside[0] = TerrainTessFactor(c, ViewPosition());
    output.Inside[1] = output.Inside[0];

    return output;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HS_Constant")]
[maxtessfactor(64.0f)]
HullOutput_Terrain HS(InputPatch<VertexOutput_Terrain, 4> input, uint pointID : SV_OutputControlPointID)
{
    HullOutput_Terrain output;
    output.Position = input[pointID].Position;
    output.Uv = input[pointID].Uv;

    return output;
}

[domain("quad")]
DomainOutput_Terrain DS
(
    ConstantHullOutput_Terrain input, 
    float2 uv : SV_DomainLocation, 
    const OutputPatch<HullOutput_Terrain, 4> patch
)
{
    DomainOutput_Terrain output;
    
    float3 p0 = lerp(patch[0].Position, patch[1].Position, uv.x).xyz;
    float3 p1 = lerp(patch[2].Position, patch[3].Position, uv.x).xyz;
    float3 position = lerp(p0, p1, uv.y);
    output.wPosition = position;

    float2 uv0 = lerp(patch[0].Uv, patch[1].Uv, uv.x);
    float2 uv1 = lerp(patch[2].Uv, patch[3].Uv, uv.x);
    output.Uv = lerp(uv0, uv1, uv.y);


    output.wPosition.y = HeightMap.SampleLevel(HeightMapSampler, output.Uv, 0).b * HeightRatio;

    output.Position = mul(float4(output.wPosition, 1), View);
    output.Position = mul(output.Position, Projection);

    output.TiledUv = output.Uv * TexScale;

    return output;
}


SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

float4 PS(DomainOutput_Terrain input) : SV_TARGET
{
    float2 left = input.Uv + float2(-TexelCellSpaceU, 0.0f);
    float2 right = input.Uv + float2(TexelCellSpaceU, 0.0f);
    float2 top = input.Uv + float2(0.0f, -TexelCellSpaceV);
    float2 bottom = input.Uv + float2(0.0f, TexelCellSpaceV);

    float leftY = HeightMap.SampleLevel(HeightMapSampler, left, 0).b * HeightRatio;
    float rightY = HeightMap.SampleLevel(HeightMapSampler, right, 0).b * HeightRatio;
    float topY = HeightMap.SampleLevel(HeightMapSampler, top, 0).b * HeightRatio;
    float bottomY = HeightMap.SampleLevel(HeightMapSampler, bottom, 0).b * HeightRatio;

    float3 tangent = normalize(float3(WorldCellSpace * 2.0f, rightY - leftY, 0.0f));
    float3 biTangent = normalize(float3(0.0f, bottomY - topY, WorldCellSpace * -2.0f));
    float3 normalW = cross(tangent, biTangent);


    float4 diffuse = BaseMap.Sample(LinearSampler, input.TiledUv);
    float NdotL = dot(normalize(normalW), -LightDirection);

    return diffuse * NdotL;
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
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetHullShader(CompileShader(hs_5_0, HS()));
        SetDomainShader(CompileShader(ds_5_0, DS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetRasterizerState(RS);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetHullShader(CompileShader(hs_5_0, HS()));
        SetDomainShader(CompileShader(ds_5_0, DS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
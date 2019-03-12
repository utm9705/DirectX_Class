#include "000_Header.fx"

float Ratio = 20;
uint MaxPiece = 10;

struct VertexOutput
{
    float4 Position : Position0;
};

VertexOutput VS(Vertex input)
{
    VertexOutput output;
    output.Position = input.Position;

    return output;
}


struct ConstantHullOutput
{
    float Edge[4] : SV_TessFactor;
    float Inside[2] : SV_InsideTessFactor;
};

float ComputeAmount(float3 position)
{
    float dist = distance(position, ViewPosition);
    float s = saturate((dist - Ratio) / (50 - Ratio));

    return lerp(MaxPiece, 1, s);
}

ConstantHullOutput HS_Constant(InputPatch<VertexOutput, 4> input)
{
    ConstantHullOutput output;
   
    float3 center = 0;
    center = (input[0].Position + input[1].Position) * 0.5f;
    output.Edge[0] = ComputeAmount(center);

    center = (input[1].Position + input[3].Position) * 0.5f;
    output.Edge[1] = ComputeAmount(center);

    center = (input[3].Position + input[2].Position) * 0.5f;
    output.Edge[2] = ComputeAmount(center);

    center = (input[2].Position + input[0].Position) * 0.5f;
    output.Edge[3] = ComputeAmount(center);


    center = (input[0].Position + input[1].Position + input[2].Position + input[3].Position) * 0.25f;
    output.Inside[0] = ComputeAmount(center);
    output.Inside[1] = output.Inside[0];

    return output;
}


struct HullOutput
{
    float4 Position : Position0;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HS_Constant")]
HullOutput HS(InputPatch<VertexOutput, 4> input, uint pointID : SV_OutputControlPointID)
{
    HullOutput output;
    output.Position = input[pointID].Position;

    return output;
}


struct DomainOutput
{
    float4 Position : SV_Position0;
};

[domain("quad")]
DomainOutput DS(ConstantHullOutput input, float2 uv : SV_DomainLocation, const OutputPatch<HullOutput, 4> patch)
{
    DomainOutput output;

    //float3 position = uvw.x * patch[0].Position + uvw.y * patch[1].Position + uvw.z * patch[2].Position;
    float3 v1 = lerp(patch[0].Position.xyz, patch[1].Position.xyz, 1 - uv.y);
    float3 v2 = lerp(patch[2].Position.xyz, patch[3].Position.xyz, 1 - uv.y);
    float3 position = lerp(v1, v2, uv.x);

    output.Position = mul(float4(position, 1), World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    return output;
}


SamplerState Sampler;
float4 PS(DomainOutput input) : SV_TARGET
{
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
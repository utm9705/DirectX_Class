#include "000_Header.fx"

uint TsAmount = 2;
uint TsAmountInside = 2;

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
    float Edge[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

ConstantHullOutput HS_Constant(InputPatch<VertexOutput, 3> input)
{
    ConstantHullOutput output;
    output.Edge[0] = TsAmount;
    output.Edge[1] = TsAmount;
    output.Edge[2] = TsAmount;

    output.Inside = TsAmountInside;
    
    return output;
}


struct HullOutput
{
    float4 Position : Position0;
};

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HS_Constant")]
HullOutput HS(InputPatch<VertexOutput, 3> input, uint pointID : SV_OutputControlPointID)
{
    HullOutput output;
    output.Position = input[pointID].Position;

    return output;
}


struct DomainOutput
{
    float4 Position : SV_Position0;
};

[domain("tri")]
DomainOutput DS(ConstantHullOutput input, float3 uvw : SV_DomainLocation, const OutputPatch<HullOutput, 3> patch)
{
    DomainOutput output;

    float3 position = uvw.x * patch[0].Position + uvw.y * patch[1].Position + uvw.z * patch[2].Position;

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
#include "000_Header_N.fx"

matrix View2;
matrix Projection2;

struct VertexOuput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
};

VertexOuput VS(VertexTexture input)
{
    VertexOuput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View2);
    output.Position = mul(output.Position, Projection2);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

Texture2D Map;

float4 PS(VertexOuput input) : SV_TARGET
{
    return Map.Sample(Sampler, input.Uv);
}

///////////////////////////////////////////////////////////////////////////////

DepthStencilState Depth
{
    DepthEnable = false;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));

        SetDepthStencilState(Depth, 0);
    }
}
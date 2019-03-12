#include "000_Header_N.fx"

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float3 oPosition : POSITION1;
};

VertexOutput VS(VertexTextureNormalTangent input)
{
    VertexOutput output;

    output.oPosition = input.Position.xyz;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

TextureCube CubeMap;

float4 PS(VertexOutput input) : SV_TARGET
{
    return CubeMap.Sample(Sampler, input.oPosition);
}

///////////////////////////////////////////////////////////////////////////////

DepthStencilState Depth
{
    DepthEnable = false;
};

RasterizerState Cull
{
    FrontCounterClockwise = true;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));

        SetDepthStencilState(Depth, 0);
        SetRasterizerState(Cull);
    }
}
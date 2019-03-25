#include "000_Header_N.fx"

struct VertexOuput
{
    float4 Position : SV_POSITION0;
    float4 pPosition : Position1;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;

    float Depth : Depth0;
};

VertexOuput VS(VertexTextureNormalTangent input)
{
    VertexOuput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.pPosition = output.Position;

    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;

float4 PS(VertexOuput input) : SV_TARGET
{
    return input.Position.z / input.Position.w;
}

///////////////////////////////////////////////////////////////////////////////

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
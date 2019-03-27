#include "000_Header_N.fx"

struct VertexOuput
{
    float4 Position : SV_POSITION0;
    float4 sPosition : Position1;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
};

VertexOuput VS(VertexTextureNormalTangent input)
{
    VertexOuput output;

    output.Position = ShadowPosition(input.Position);
    output.sPosition = input.Position;

    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS(VertexOuput input) : SV_TARGET
{
    float depth = input.sPosition.z / input.sPosition.w;

    return float4(depth, depth, depth, 1);
}

///////////////////////////////////////////////////////////////////////////////

RasterizerState RS
{
    CullMode = Front;
};

technique11 T0
{
    pass P0
    {
        SetRasterizerState(RS);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
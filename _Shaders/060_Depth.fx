#include "000_Header_N.fx"

cbuffer CB_ShadowDepth
{
    matrix ShadowView;
    matrix ShadowProjection;
};

struct VertexOuput
{
    float4 Position : SV_POSITION0;
    float4 oPosition : Position1;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
};

VertexOuput VS(VertexTextureNormalTangent input)
{
    VertexOuput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;

    output.oPosition = mul(input.Position, World);
    output.oPosition = mul(output.Position, ShadowView);
    output.oPosition = mul(output.Position, ShadowProjection);

    return output;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS(VertexOuput input) : SV_TARGET
{
    float depth = input.oPosition.z / input.oPosition.w;

    return float4(1, 1, 0, 1);
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
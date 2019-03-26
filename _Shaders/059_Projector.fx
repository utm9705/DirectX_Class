#include "000_Header_N.fx"

matrix View2;
matrix Projection2;

float4 Color = float4(1, 0, 0, 1);

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float4 pPosition : Position1;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
};

VertexOutput VS(VertexTextureNormalTangent input)
{
    VertexOutput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;

    output.pPosition = mul(input.Position, World);
    output.pPosition = mul(output.pPosition, View2);
    output.pPosition = mul(output.pPosition, Projection2);

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;
Texture2D ProjectionMap;

float4 PS(VertexOutput input) : SV_TARGET
{
    //화면 공간에서의 uv
    float2 uv = 0;
    uv.x = input.pPosition.x / input.pPosition.w * 0.5f + 0.5f;
    uv.y = -input.pPosition.y / input.pPosition.w * 0.5f + 0.5f;

    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
    if (saturate(uv.x) == uv.x && saturate(uv.y) == uv.y)
    {
        float4 color = ProjectionMap.Sample(Sampler, uv);
        color *= Color;

        diffuse = lerp(diffuse, color, color.a);
    }

    return diffuse;
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
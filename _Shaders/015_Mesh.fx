#include "000_Header.fx"

struct VertexOuput
{
    float4 Position : SV_POSITION;
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

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;

float4 PS(VertexOuput input) : SV_TARGET
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);

    //float4 diffuse = 1;
    //float3 direction = normalize(float3(-1, -1, -1));
    //float NdotL = dot(-direction, input.Normal);

    //return diffuse * NdotL;
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
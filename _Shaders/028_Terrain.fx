#include "000_Header_N.fx"

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
struct VertexOuput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};

VertexOuput VS(VertexTextureNormal input)
{
    VertexOuput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = WorldNormal(input.Normal);
    output.Uv = input.Uv;

    return output;
}

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
Texture2D BaseMap;
SamplerState Sampler;

float4 PS(VertexOuput input) : SV_TARGET
{
    float4 diffuse = BaseMap.Sample(Sampler, input.Uv);

    float3 direction = normalize(-LightDirection);
    float NdotL = dot(direction, input.Normal);

    return (diffuse * NdotL);
}

//-----------------------------------------------------------------------------
// Technique
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
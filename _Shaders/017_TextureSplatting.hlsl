cbuffer VS_ViewProjection : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer VS_World : register(b1)
{
    matrix World;
}

struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

cbuffer PS_Light : register(b0)
{
    float3 Direction;
    float PS_Light_Padding;
}

cbuffer PS_Material : register(b1)
{
    float4 Diffuse;
    float4 Specular;
    float Shininess;
}

Texture2D DiffuseMap : register(t0);
Texture2D SpecularMap : register(t1);
Texture2D NormalMap : register(t2);

SamplerState Sampler : register(s0);

Texture2D ColorMap : register(t5);
Texture2D AlphaMap : register(t6);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 A = DiffuseMap.Sample(Sampler, input.Uv);
    float4 B = ColorMap.Sample(Sampler, input.Uv);
    float4 t = AlphaMap.Sample(Sampler, input.Uv);

    float4 color = (1 - t) * A + t * B;


    float3 direction = normalize(-Direction);
    float NdotL = dot(direction, input.Normal);

    return (color * NdotL);
}
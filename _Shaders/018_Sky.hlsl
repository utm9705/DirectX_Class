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
    float3 Tangent : TANGENT0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
    float4 oPosition : POSITION0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.oPosition = input.Position;

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

cbuffer PS_Sky : register(b10)
{
    float4 Center;
    float4 Apex;

    float Height;
}

Texture2D DiffuseMap : register(t0);
Texture2D SpecularMap : register(t1);
Texture2D NormalMap : register(t2);

SamplerState Sampler : register(s0);

Texture2D ColorMap : register(t5);
Texture2D AlphaMap : register(t6);

float4 PS(PixelInput input) : SV_TARGET
{
    float y = saturate(input.oPosition.y);

    return lerp(Center, Apex, y * Height);
}
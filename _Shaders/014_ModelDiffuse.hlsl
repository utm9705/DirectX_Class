cbuffer VS_ViewProjection : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer VS_World : register(b1)
{
    matrix World;
}

cbuffer VS_Bones : register(b2)
{
    matrix Bones[128];   
}

cbuffer VS_BoneIndex : register(b3)
{
    int BoneIndex;
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

    matrix world = Bones[BoneIndex];

    output.Position = mul(input.Position, world);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) world);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

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

float4 PS(PixelInput input) : SV_TARGET
{
    //float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);

    float4 diffuse = 1;
    float3 direction = normalize(float3(-1, -1, -1));
    float NdotL = dot(-direction, input.Normal);

    return diffuse * NdotL;
}
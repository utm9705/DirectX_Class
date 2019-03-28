//-----------------------------------------------------------------------------
// Constant Buffers
//-----------------------------------------------------------------------------
cbuffer CB_PerFrame
{
    matrix World;
    matrix View;
    matrix ViewInverse;
    matrix Projection;
    matrix VP;

    float4 LightColor;
    float3 LightDirection;
    float Time;

    float3 LightPosition;
}

//-----------------------------------------------------------------------------
// Texture Buffers
//-----------------------------------------------------------------------------
cbuffer CB_Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float Shininess;
}

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;


cbuffer CB_ShadowDepth
{
    matrix ShadowView;
    matrix ShadowProjection;
};

cbuffer CB_Shadow
{
    float2 ShadowMapSize;
    float ShadowBias;
};

Texture2D ShadowMap;
SamplerComparisonState ShadowSampler
{
    Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
    //AddressU = BORDER;
    //AddressV = BORDER;
    //AddressW = BORDER;
    //BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComparisonFunc = LESS_EQUAL;
};

//-----------------------------------------------------------------------------
// Vertex Input
//-----------------------------------------------------------------------------
struct Vertex
{
    float4 Position : POSITION0;
};

struct VertexNormal
{
    float4 Position : POSITION0;
    float3 Normal : COLOR0;
};

struct VertexColor
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
};

struct VertexColorNormal
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
};

struct VertexTexture
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
};

struct VertexTextureColor
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float4 Color : COLOR0;
};

struct VertexTextureColorNormal
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
};

struct VertexTextureNormal
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
};

struct VertexColorTextureNormal
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
};

struct VertexTextureNormalBlend
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
    float4 BlendIndices : BLENDINDICES0;
    float4 BlendWeights : BLENDWEIGHTS0;
};

struct VertexTextureNormalTangent
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
};

struct VertexTextureNormalTangentBlend
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
    float4 BlendIndices : BLENDINDICES0;
    float4 BlendWeights : BLENDWEIGHTS0;
};


//-----------------------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------------------
float4 WorldPosition(float4 position)
{
    return mul(position, World);
}

float3 ViewPosition()
{
    return ViewInverse._41_42_43;
}

float3 ViewPosition(float3 position)
{
    float3 camera = 0;
    camera.x = ViewInverse._41;
    camera.y = ViewInverse._42;
    camera.z = ViewInverse._43;

    return normalize(camera - position);
}

float4 ViewProjection(float4 position)
{
    return mul(position, VP);
}

float3 WorldNormal(float3 normal)
{
    return mul(normal, (float3x3) World);
}

float3 WorldTangent(float3 tangent)
{
    return mul(tangent, (float3x3) World);
}

float3 TangentSpace(float3 normalMap, float3 normal, float3 tangent)
{
    float3 coord = 2.0f * normalMap - 1.0f;

    float3 N = normalize(normal); //Z
    float3 T = normalize(tangent - dot(tangent, N) * N); //X
    float3 B = cross(N, T); //Y

    float3x3 TBN = float3x3(T, B, N);

    return mul(coord, TBN);
}
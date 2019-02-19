cbuffer VS_ViewProjection : register(b0)
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1)
{
    matrix _world;
}

cbuffer PS_Color : register(b0)
{
    float X;
}

struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : UV0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;

    return output;
}

Texture2D Map : register(t0);
Texture2D Map2 : register(t1);

Texture2D Maps[2] : register(t2);

SamplerState MapSampler : register(s0);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 color = 0;
    //float4 color2 = Maps[1].Sample(MapSampler, input.uv);

    if(input.uv.x < 1.0f)
        color = Maps[0].Sample(MapSampler, input.uv);
    
    if(input.uv.x >= 1.0f && input.uv.x < 2.0f)
        color = Maps[1].Sample(MapSampler, input.uv - 1.0f);

    return color;
}
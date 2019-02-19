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
    
}

struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : UV0;
    float4 color : COLOR0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV0;
    float4 color : COLOR0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.color = input.color;
    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return input.color;
}
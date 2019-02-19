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
    float3 _direction;
}

struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : UV0;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV0;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
};

PixelInput VS(VertexInput input)
{
    float3 wPosition;

    PixelInput output;
    output.position = mul(input.position, _world);
    wPosition = output.position.xyz;

    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.normal = mul(input.normal, (float3x3) _world);
    output.normal = normalize(output.normal);

    //if(wPosition.y > 30.0f)
    //    output.color = float4(1, 1, 1, 1);
    //else if(wPosition.y > 20.0f)
    //    output.color = float4(0, 0, 1, 1);
    //else if (wPosition.y > 10.0f)
    //    output.color = float4(1, 0, 0, 1);
    //else if (wPosition.y > 0.0f)
    //    output.color = float4(0, 1, 0, 1);

    output.color = input.color;
    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 color = float4(1, 1, 1, 1);
    float i = dot(input.normal, -_direction);

    return color * i;
}
cbuffer VS_ViewProjection : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer VS_World : register(b1)
{
    matrix World;
}

cbuffer VS_Brush : register(b2)
{
    int BrushType;
    float3 BrushLocation;
    
    int BrushRange;
    float3 BrushColor;
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
    float3 Color : COLOR0;
    float4 oPosition : POSITION0;
};

float3 GetBrushColor(float3 location)
{
    if(BrushType == 0)
        return float3(0, 0, 0);

    if (BrushType == 1)
    {
        if((location.x >= (BrushLocation.x - BrushRange)) &&
           (location.x <= (BrushLocation.x + BrushRange)) &&
           (location.z >= (BrushLocation.z - BrushRange)) &&
           (location.z <= (BrushLocation.z + BrushRange)))
        {
            return BrushColor;
        }            
    }

    if(BrushType == 2)
    {
        float dx = location.x - BrushLocation.x;
        float dz = location.z - BrushLocation.z;

        float dist = sqrt(dx * dx + dz * dz);

        if(dist <= BrushRange)
            return BrushColor;
    }

    return float3(0, 0, 0);
}

PixelInput VS(VertexInput input)
{
    PixelInput output;

    output.oPosition = input.Position;

    output.Position = mul(input.Position, World);
    output.Color = GetBrushColor(input.Position.xyz);

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

float4 PS(PixelInput input) : SV_TARGET
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);

    float3 direction = normalize(-Direction);
    float NdotL = dot(direction, input.Normal);

    float4 lineColor = float4(1, 1, 1, 1);

    //float2 grid = 0;
    //grid.x = frac(input.oPosition.x / 5);
    //grid.y = frac(input.oPosition.z / 5);

    //if(grid.x < 0.1f || grid.y < 0.1f)
    //    return lineColor;


    float2 grid = input.oPosition.xz / (float) 5;
    float2 range = abs(frac(grid - 0.5f) - 0.5f);
    float2 speed = fwidth(grid);

    float2 pixel = range / speed;
    float weights = saturate(min(pixel.x, pixel.y) - 0.1f);

    lineColor = lerp(float4(1, 1, 1, 1), float4(0, 0, 0, 0), weights);

    return (diffuse * NdotL) + float4(input.Color, 1) + lineColor;
}
#include "000_Header.fx"

matrix View2;
matrix Projection2;

struct VertexOuput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
};

VertexOuput VS(VertexTexture input)
{
    VertexOuput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View2);
    output.Position = mul(output.Position, Projection2);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

Texture2D Map;

float2 Size = float2(1, 1);

float4 PS(VertexOuput input) : SV_TARGET
{
    float x = input.Uv.x;
    float y = input.Uv.y;

    float4 color = Map.Sample(Sampler, float2(x, y));

    float ratioX = x / Size.x;
    float ratioY = y / Size.y;

    float2 left = float2(x - ratioX, y);
    float2 right = float2(x + ratioX, y);
    float2 up = float2(x, y - ratioY);
    float2 down = float2(x, y + ratioY);

    color += Map.Sample(Sampler, left);
    color += Map.Sample(Sampler, right);
    color += Map.Sample(Sampler, up);
    color += Map.Sample(Sampler, down);

    color.rgb /= 5;
    
    color.a = 1.0f;
    return color;
}

int Count = 10;
float4 PS_Blur(VertexOuput input) : SV_TARGET
{
    float2 arr[9] =
    {
        float2(-1, -1), float2(0, -1), float2(1, -1),
        float2(-1,  0), float2(0,  0), float2(1,  0),
        float2(-1,  1), float2(0,  1), float2(1,  1),
    };


    float3 color = 0;
    for (int blur = 0; blur < Count; blur++)
    {
        for (int i = 0; i < 9; i++)
        {
            float x = arr[i].x * (float) blur / (float) Size.x;
            float y = arr[i].y * (float) blur / (float) Size.y;

            float2 uv = input.Uv + float2(x, y);
            color += Map.Sample(Sampler, uv).rgb;
        }
    }

    color /= Count * 9;
    return float4(color, 1);
}

///////////////////////////////////////////////////////////////////////////////

DepthStencilState Depth
{
    DepthEnable = false;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));

        SetDepthStencilState(Depth, 0);
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Blur()));

        SetDepthStencilState(Depth, 0);
    }
}
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

int BlurCount = 2;
const float Weights[13] =
{
    0.0561f, 0.1353f, 0.2780f, 0.4868f, 0.7261f, 0.9231f,
    1.0f,
    0.9231f, 0.7261f, 0.4868f, 0.2780f, 0.1353f, 0.0561f
};

float4 PS_X(VertexOuput input) : SV_TARGET
{
    float2 uv = input.Uv;
    float u = 1.0f / (float) Size.x;

    float sum = 0;
    float3 diffuse = 0;
    
    //[unroll]
    for (int i = -BlurCount; i <= BlurCount; i++)
    {
        float2 temp = uv + float2(u * (float) i, 0.0f);
        diffuse += Weights[6 + i] * Map.Sample(Sampler, temp).xyz;
        sum += Weights[6 + i];
    }
    diffuse /= sum;
    
    return float4(diffuse, 1);
}

float4 PS_Y(VertexOuput input) : SV_TARGET
{
    float2 uv = input.Uv;
    float v = 1.0f / (float) Size.y;

    float sum = 0;
    float3 diffuse = 0;
    
    //[unroll]
    for (int i = -BlurCount; i <= BlurCount; i++)
    {
        float2 temp = uv + float2(0.0f, v * (float) i);
        diffuse += Weights[6 + i] * Map.Sample(Sampler, temp);
        sum += Weights[6 + i];
    }
    diffuse /= sum;
    
    return float4(diffuse, 1);
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
        SetPixelShader(CompileShader(ps_5_0, PS_X()));

        SetDepthStencilState(Depth, 0);
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Y()));

        SetDepthStencilState(Depth, 0);
    }
}
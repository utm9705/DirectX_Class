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

float4 PS(VertexOuput input) : SV_TARGET
{
    return Map.Sample(Sampler, input.Uv);
}

float4 PS_Invert(VertexOuput input) : SV_TARGET
{
    return saturate(1.0f - Map.Sample(Sampler, input.Uv));
}

float4 PS_ToggleInvert(VertexOuput input) : SV_TARGET
{
    float val = sin(Time * 2.5f);
    float4 color = saturate(val - Map.Sample(Sampler, input.Uv));
    color.r += sin(Time * 2.5f);

    return color;
}

float4 PS_Average(VertexOuput input) : SV_TARGET
{
    float4 color = Map.Sample(Sampler, input.Uv);
    color.rgb = (color.r + color.g + color.b) / 3.0f;

    return color;
}

float4 PS_Grayscale(VertexOuput input) : SV_TARGET
{
    float4 color = Map.Sample(Sampler, input.Uv);

    float3 monotone = float3(0.299f, 0.587f, 0.114f);
    color.rgb = dot(color.rgb, monotone);

    return color;
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
        SetPixelShader(CompileShader(ps_5_0, PS_Invert()));

        SetDepthStencilState(Depth, 0);
    }

    pass P2
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_ToggleInvert()));

        SetDepthStencilState(Depth, 0);
    }

    pass P3
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Average()));

        SetDepthStencilState(Depth, 0);
    }

    pass P4
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Grayscale()));

        SetDepthStencilState(Depth, 0);
    }
}
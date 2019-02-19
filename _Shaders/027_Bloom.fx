#include "000_Header.fx"

matrix View2;
matrix Projection2;


//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
// Pixel Shader - LuminosityExtract
//-----------------------------------------------------------------------------
SamplerState Sampler
{
	Filter = MIN_MAG_MIP_LINEAR;
};

Texture2D Map;
float Threshold = 0.6f;

float4 PS_LuminosityExtract(VertexOuput input) : SV_TARGET
{
	float4 color = Map.Sample(Sampler, input.Uv);

	return saturate((color - Threshold) / (1 - Threshold));
}

static const int MaxSamplingCount = 15;
int SamplingCount = 1;
//-----------------------------------------------------------------------------
// Pixel Shader - Blur
//-----------------------------------------------------------------------------
float2 OffsetsX[MaxSamplingCount];
float WeightsX[MaxSamplingCount];
float4 PS_BlurX(VertexOuput input) : SV_TARGET
{
	float4 color = 0;

	for (int i = 0; i < SamplingCount; i++)
		color += Map.Sample(Sampler, input.Uv + OffsetsX[i]) * WeightsX[i];

    return color;
}

float2 OffsetsY[MaxSamplingCount];
float WeightsY[MaxSamplingCount];
float4 PS_BlurY(VertexOuput input) : SV_TARGET
{
    float4 color = 0;

    for (int i = 0; i < SamplingCount; i++)
        color += Map.Sample(Sampler, input.Uv + OffsetsY[i]) * WeightsY[i];

    return color;
}

//-----------------------------------------------------------------------------
// Pixel Shader - Composite
//-----------------------------------------------------------------------------
Texture2D LuminosityMap;
Texture2D BlurMap;
float Intensity = 1.15f;
float4 PS_Composite(VertexOuput input) : SV_TARGET
{
    float4 l = LuminosityMap.Sample(Sampler, input.Uv) * Intensity;
    float4 b = BlurMap.Sample(Sampler, input.Uv);

    float4 color = (1 - saturate(l)) * b;

    return color + l;
}

//-----------------------------------------------------------------------------
// Technique
//-----------------------------------------------------------------------------
DepthStencilState Depth
{
	DepthEnable = false;
};

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_LuminosityExtract()));

		SetDepthStencilState(Depth, 0);
	}

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_BlurX()));

        SetDepthStencilState(Depth, 0);
    }

    pass P2
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_BlurY()));

        SetDepthStencilState(Depth, 0);
    }

    pass P3
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Composite()));

        SetDepthStencilState(Depth, 0);
    }
}
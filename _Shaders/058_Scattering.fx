#include "000_Header_N.fx"
#include "000_Sky.fx"

VertexOutput_Target VS_Target(VertexTexture input)
{
    VertexOutput_Target output;

    output.Position = input.Position;
    output.Uv = input.Uv;

    return output;
}

PixelOutput_Target PS_Target(VertexOutput_Target input)
{
    PixelOutput_Target output;

    float3 sunDirection = -normalize(LightDirection);
    float2 uv = input.Uv;

    float3 pointPv = float3(0, InnerRadius + 1e-3f, 0.0f);
    float angleXZ = PI * uv.y;
    float angleY = 100.0f * uv.x * PI / 180.0f;


    float3 direction;
    direction.x = sin(angleY) * cos(angleXZ);
    direction.y = cos(angleY);
    direction.z = sin(angleY) * sin(angleXZ);
    direction = normalize(direction);

    float farPvPa = HitOuterSphere(pointPv, direction);
    float3 ray = direction;

    float3 pointP = pointPv;
    float sampleLength = farPvPa / SampleCount;
    float scaledLength = sampleLength * Scale;
    float3 sampleRay = ray * sampleLength;
    pointP += sampleRay * 0.5f;


    float3 rayleighSum = 0;
    float3 mieSum = 0;
    for (int i = 0; i < SampleCount; i++)
    {
        float pHeight = length(pointP);

        float2 densityRatio = GetDensityRatio(pHeight);
        densityRatio *= scaledLength;


        float2 viewerOpticalDepth = GetDistance(pointP, pointPv);

        float farPPc = HitOuterSphere(pointP, sunDirection);
        float2 sunOpticalDepth = GetDistance(pointP, pointP + sunDirection * farPPc);

        float2 opticalDepthP = sunOpticalDepth.xy + viewerOpticalDepth.xy;
        float3 attenuation = exp(-Kr4PI * InvWaveLength * opticalDepthP.x - Km4PI * opticalDepthP.y);

        rayleighSum += densityRatio.x * attenuation;
        mieSum += densityRatio.y * attenuation;

        pointP += sampleRay;
    }

    float3 rayleigh = rayleighSum * KrESun;
    float3 mie = mieSum * KmESun;

    rayleigh *= InvWaveLength;
    mie *= WaveLengthMie;

    output.R = float4(rayleigh, 1);
    output.M = float4(mie, 1);

    return output;
}

///////////////////////////////////////////////////////////////////////////////

VertexOutput_Scattering VS_Scattering(VertexTexture input)
{
    VertexOutput_Scattering output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.oPosition = -input.Position;
    output.Uv = input.Uv;

    return output;
}

float4 PS_Scattering(VertexOutput_Scattering input) : SV_TARGET
{
    float3 sunDirection = -normalize(LightDirection);

    float temp = dot(sunDirection, input.oPosition) / length(input.oPosition);
    float temp2 = temp * temp;

    float3 rSamples = RayleighMap.Sample(SkySampler, input.Uv);
    float3 mSamples = MieMap.Sample(SkySampler, input.Uv);

    float3 color = 0;
    color = GetRayleighPhase(temp2) * rSamples + GetMiePhase(temp, temp2) * mSamples;
    color = HDR(color);

    color += max(0, (1 - color.rgb)) * float3(0.05f, 0.05f, 0.1f);

    float intensity = saturate(StarIntensity);
    return float4(color, 1) + StarMap.Sample(SkySampler, input.Uv) * intensity;
}

///////////////////////////////////////////////////////////////////////////////

VertexOutput_Cloud VS_Cloud(VertexTexture input)
{
    VertexOutput_Cloud output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = (input.Uv * CloudTiles);
    output.oUv = input.Uv;

    return output;
}

float4 PS_Cloud(VertexOutput_Cloud input) : SV_Target0
{
    input.Uv = input.Uv * CloudTiles;

    float n = Noise(input.Uv + Time * CloudSpeed);
    float n2 = Noise(input.Uv * 2 + Time * CloudSpeed);
    float n3 = Noise(input.Uv * 4 + Time * CloudSpeed);
    float n4 = Noise(input.Uv * 8 + Time * CloudSpeed);
	
    float nFinal = n + (n2 / 2) + (n3 / 4) + (n4 / 8);
	
    float c = CloudCover - nFinal;
    if (c < 0) 
        c = 0;
 
    float density = 1.0 - pow(CloudSharpness, c);
    float4 color = density;
    //color *= LightColor;
    
    return color;
}

///////////////////////////////////////////////////////////////////////////////

VertexOutput_Moon VS_Moon(VertexTexture input)
{
    VertexOutput_Moon output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;

    return output;
}

float4 PS_Moon(VertexOutput_Moon input) : SV_Target0
{
    float4 color = MoonMap.Sample(SkySampler, input.Uv);
    color.a *= MoonAlpha;

    return color;
}

///////////////////////////////////////////////////////////////////////////////

DepthStencilState DSS
{
    DepthEnable = false;
};

BlendState AlphaBlend
{
    BlendEnable[0] = true;
    DestBlend[0] = INV_SRC_ALPHA;
    SrcBlend[0] = SRC_ALPHA;
    BlendOp[0] = Add;

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = One;
    RenderTargetWriteMask[0] = 0x0F;
};

///////////////////////////////////////////////////////////////////////////////

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Target()));
        SetPixelShader(CompileShader(ps_5_0, PS_Target()));
    }

    pass P1
    {
        SetDepthStencilState(DSS, 0);

        SetVertexShader(CompileShader(vs_5_0, VS_Scattering()));
        SetPixelShader(CompileShader(ps_5_0, PS_Scattering()));
    }

    pass P2
    {
        SetDepthStencilState(DSS, 0);
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);

        SetVertexShader(CompileShader(vs_5_0, VS_Cloud()));
        SetPixelShader(CompileShader(ps_5_0, PS_Cloud()));
    }

    pass P3
    {
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);

        SetVertexShader(CompileShader(vs_5_0, VS_Moon()));
        SetPixelShader(CompileShader(ps_5_0, PS_Moon()));
    }
}
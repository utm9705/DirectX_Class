#include "000_Header_N.fx"
#include "000_Sky.fx"

///////////////////////////////////////////////////////////////////////////////
//Target
///////////////////////////////////////////////////////////////////////////////

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
//Scattering
///////////////////////////////////////////////////////////////////////////////

VertexOutput_Scattering VS_Scattering(VertexTextureNormalTangent input)
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
//Cloud
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
    float n = Noise(input.Uv + MoonAlpha);
    float n2 = Noise(input.Uv * 2 + MoonAlpha);
    float n3 = Noise(input.Uv * 4 + MoonAlpha);
    float n4 = Noise(input.Uv * 8 + MoonAlpha);
   
    float nFinal = n + (n2 / 2) + (n3 / 4) + (n4 / 8);
   
    float c = CloudCover - nFinal;
    if (c < 0) 
        c = 0;
 
    float CloudDensity = 1.0 - pow(CloudSharpness, c);

    float4 retColor = CloudDensity;
    
    float uvX = abs(0.5f - input.oUv.x) + 0.5f;
    float uvY = abs(0.5f - input.oUv.y) + 0.5f;
    
    retColor.a = uvX > 0.8 ? lerp(0.0f, retColor.a * 0.85f, (1 - uvX) / 0.2f) : retColor.a;
    retColor.a = uvY > 0.8 ? lerp(0.0f, retColor.a * 0.85f, (1 - uvY) / 0.2f) : retColor.a;
    
    return retColor;
}

///////////////////////////////////////////////////////////////////////////////

DepthStencilState DSS
{
    DepthEnable = false;
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
        SetVertexShader(CompileShader(vs_5_0, VS_Cloud()));
        SetPixelShader(CompileShader(ps_5_0, PS_Cloud()));
    }
}
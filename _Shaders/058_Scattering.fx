#include "000_Header_N.fx"

static const float PI = 3.14159265f;
static const float InnerRadius = 6356.7523142f;//���� �ѷ�
static const float OuterRadius = 6356.7523142f * 1.0157313f;

static const float KrESun = 0.0025f * 20.0f; //0.0025f - ���ϸ� ��� * �¾��� ���
static const float KmESun = 0.0010f * 20.0f; //0.0025f - �� ��� * �¾��� ���
static const float Kr4PI = 0.0025f * 4.0f * 3.1415159;
static const float Km4PI = 0.0010f * 4.0f * 3.1415159;

static const float2 RayleighMieScaleHeight = { 0.25f, 0.1f };
static const float Scale = 1.0 / (6356.7523142 * 1.0157313 - 6356.7523142);

static const float g = -0.980f;
static const float g2 = -0.980f * -0.980f;
static const float Exposure = -2.0f;

cbuffer CB_Target
{
    float3 WaveLength;
    int SampleCount;

    float3 InvWaveLength;
    float PS_Target_Padding;

    float3 WaveLengthMie;
}

float StarIntensity;

Texture2D RayleighMap;
Texture2D MieMap;
Texture2D StarMap;

///////////////////////////////////////////////////////////////////////////////

struct VertexTargetOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv0;
};

VertexTargetOutput VS_Target(VertexTexture input)
{
    VertexTargetOutput output;

    output.Position = input.Position;
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

struct PixelTargetOutput
{
    float4 R : SV_TARGET0;
    float4 M : SV_TARGET1;
};

float HitOuterSphere(float3 position, float3 direction)
{
    float3 light = -position;

    float b = dot(light, direction);
    float c = dot(light, light);

    float d = c - b * b;
    float q = sqrt(OuterRadius * OuterRadius - d);

    float t = b;
    t += q;

    return t;
}

float2 GetDensityRatio(float height)
{
    float altitude = (height - InnerRadius) * Scale;

    return exp(-altitude / RayleighMieScaleHeight);
}

float2 GetDistance(float3 p1, float3 p2)
{
    float2 opticalDepth = 0;

    float3 temp = p2 - p1;
    float far = length(temp);
    float3 direction = temp / far;


    float sampleLength = far / SampleCount;
    float scaledLength = sampleLength * Scale;

    float3 sampleRay = direction * sampleLength;
    p1 += sampleRay * 0.5f;

    for (int i = 0; i < SampleCount; i++)
    {
        float height = length(p1);
        opticalDepth += GetDensityRatio(height);

        p1 += sampleRay;
    }

    return opticalDepth * scaledLength;
}

PixelTargetOutput PS_Target(VertexTargetOutput input)
{
    PixelTargetOutput output;

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

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float3 oPosition : POSITION0;
};

VertexOutput VS_Scattering(VertexTexture input)
{
    VertexOutput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.oPosition = -input.Position;
    output.Uv = input.Uv;

    return output;
}

float GetRayleighPhase(float c)
{
    return 0.75f * (1.0f + c);
}

float GetMiePhase(float c, float c2)
{
    float3 result = 0;
    result.x = 1.5f * ((1.0f - g2) / (2.0f + g2));
    result.y = 1.0f + g2;
    result.z = 2.0f * g;

    return result.x * (1.0f + c2) / pow(result.y - result.z * c, 1.5f);
}

float3 HDR(float3 LDR)
{
    return 1.0f - exp(Exposure * LDR);
}

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

float4 PS_Scattering(VertexOutput input) : SV_TARGET
{
    float3 sunDirection = -normalize(LightDirection);

    float temp = dot(sunDirection, input.oPosition) / length(input.oPosition);
    float temp2 = temp * temp;

    float3 rSamples = RayleighMap.Sample(LinearSampler, input.Uv);
    float3 mSamples = MieMap.Sample(LinearSampler, input.Uv);

    float3 color = 0;
    color = GetRayleighPhase(temp2) * rSamples + GetMiePhase(temp, temp2) * mSamples;
    color = HDR(color);

    color += max(0, (1 - color.rgb)) * float3(0.05f, 0.05f, 0.1f);

    float intensity = saturate(StarIntensity);
    return float4(color, 1) + StarMap.Sample(LinearSampler, input.Uv) * intensity;
}

///////////////////////////////////////////////////////////////////////////////

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Target()));
        SetPixelShader(CompileShader(ps_5_0, PS_Target()));
    }
}
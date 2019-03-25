
static const float PI = 3.14159265f;
static const float InnerRadius = 6356.7523142f; //지구 둘레
static const float OuterRadius = 6356.7523142f * 1.0157313f;

static const float KrESun = 0.0025f * 20.0f; //0.0025f - 레일리 상수 * 태양의 밝기
static const float KmESun = 0.0010f * 20.0f; //0.0025f - 미 상수 * 태양의 밝기
static const float Kr4PI = 0.0025f * 4.0f * 3.1415159;
static const float Km4PI = 0.0010f * 4.0f * 3.1415159;

static const float2 RayleighMieScaleHeight = { 0.25f, 0.1f };
static const float Scale = 1.0 / (6356.7523142 * 1.0157313 - 6356.7523142);

static const float g = -0.980f;
static const float g2 = -0.980f * -0.980f;
static const float Exposure = -2.0f;

static const float CloudCover = -0.1f;

static const float ONE = 0.00390625;
static const float ONEHALF = 0.001953125;

///////////////////////////////////////////////////////////////////////////////

SamplerState SkySampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};



///////////////////////////////////////////////////////////////////////////////

cbuffer CB_Target
{
    float3 WaveLength;
    int SampleCount;

    float3 InvWaveLength;
    float PS_Target_Padding;

    float3 WaveLengthMie;
}

cbuffer CB_Night
{
    float StarIntensity;
    float MoonAlpha = 1.0f;
};


cbuffer CB_Cloud
{
    float CloudTiles;
    float cloudCover = -0.1f;
    float CloudSharpness = 0.25f;
};


///////////////////////////////////////////////////////////////////////////////

struct VertexOutput_Target
{
    float4 Position : SV_Position0;
    float2 Uv : Uv0;
};


struct PixelOutput_Target
{
    float4 R : SV_TARGET0;
    float4 M : SV_TARGET1;
};



struct VertexOutput_Scattering
{
    float4 Position : SV_POSITION0;
    float2 Uv : UV0;
    float3 oPosition : POSITION1;
};



struct VertexOutput_Cloud
{
    float4 Position : SV_Position0;
    float2 Uv : Uv0;
    float2 oUv : Uv1;
};

struct VertexOutput_Moon
{
    float4 Position : SV_Position0;
    float2 Uv : Uv0;
    float3 oPosition : POSITION1;
};

Texture2D RayleighMap;
Texture2D MieMap;
Texture2D StarMap;
Texture2D CloudMap;

Texture2D MoonMap;

///////////////////////////////////////////////////////////////////////////////

// target

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


// scattering

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


// cloud(noise)

float Fade(float t)
{
  // return t*t*(3.0-2.0*t);
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}


float Noise(float2 P)
{
    float2 Pi = ONE * floor(P) + ONEHALF;
    float2 Pf = frac(P);

    float2 grad00 = CloudMap.Sample(SkySampler, Pi).rg * 4.0 - 1.0;
    float n00 = dot(grad00, Pf);

    float2 grad10 = CloudMap.Sample(SkySampler, Pi + float2(ONE, 0.0)).rg * 4.0 - 1.0;
    float n10 = dot(grad10, Pf - float2(1.0, 0.0));

    float2 grad01 = CloudMap.Sample(SkySampler, Pi + float2(0.0, ONE)).rg * 4.0 - 1.0;
    float n01 = dot(grad01, Pf - float2(0.0, 1.0));

    float2 grad11 = CloudMap.Sample(SkySampler, Pi + float2(ONE, ONE)).rg * 4.0 - 1.0;
    float n11 = dot(grad11, Pf - float2(1.0, 1.0));

    float2 n_x = lerp(float2(n00, n01), float2(n10, n11), Fade(Pf.x));

    float n_xy = lerp(n_x.x, n_x.y, Fade(Pf.y));

    return n_xy;
}



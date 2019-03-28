#include "000_Header_N.fx"

struct VertexOuput_Depth
{
    float4 Position : SV_POSITION0;
    float4 sPosition : Position1;
};

VertexOuput_Depth VS_Depth(Vertex input)
{
    VertexOuput_Depth output;

    output.Position = WorldPosition(input.Position);
    output.Position = mul(output.Position, ShadowView);
    output.Position = mul(output.Position, ShadowProjection);
    output.sPosition = output.Position;

    return output;
}

float4 PS_Depth(VertexOuput_Depth input) : SV_TARGET
{
    float depth = input.sPosition.z / input.sPosition.w;

    return float4(depth, depth, depth, 1);
}

///////////////////////////////////////////////////////////////////////////////


struct VertexOuput_Shadow
{
    float4 Position : SV_POSITION;
    float4 sPosition : Position1;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
};

VertexOuput_Shadow VS_Shadow(VertexTextureNormalTangent input)
{
    VertexOuput_Shadow output;

    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);

    output.sPosition = mul(input.Position, World);
    output.sPosition = mul(output.sPosition, ShadowView);
    output.sPosition = mul(output.sPosition, ShadowProjection);

    output.Normal = WorldNormal(input.Normal);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;

float4 PS_Shadow(VertexOuput_Shadow input) : SV_TARGET
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);

    input.sPosition.xyz /= input.sPosition.w;

    [flatten]
    if
    (
        input.sPosition.x < -1.0f || input.sPosition.x > 1.0f ||
        input.sPosition.y < -1.0f || input.sPosition.y > 1.0f ||
        input.sPosition.z < 0.0f || input.sPosition.z > 1.0f
    )
    return diffuse;

    input.sPosition.x = input.sPosition.x * 0.5f + 0.5f;
    input.sPosition.y = -input.sPosition.y * 0.5f + 0.5f;
    input.sPosition.z -= ShadowBias;

    //Shadow
    //float depth = ShadowMap.Sample(Sampler, input.sPosition.xy).r;
    //float factor = (float)input.sPosition.z <= depth;

    //PCF
    float depth = input.sPosition.z;
    //float factor = ShadowMap.SampleCmpLevelZero(ShadowSampler, input.sPosition.xy, depth).r;

    //Blur
    float2 size = 1.0f / ShadowMapSize;
    float2 offsets[9] =
    {
        float2(+size.x, -size.y), float2(0.0f, -size.y), float2(+size.x, -size.y),
        float2(+size.x, 0.0f), float2(0.0f, 0.0f), float2(+size.x, 0.0f),
        float2(+size.x, +size.y), float2(0.0f, +size.y), float2(+size.x, +size.y),
    };

    float sum = 0.0f;

    [roll(9)]
    for (int i = 0; i < 9; i++)
    {
        float2 uv = input.sPosition.xy + offsets[i];
        sum += ShadowMap.SampleCmpLevelZero(ShadowSampler, uv, depth).r;

    }

    diffuse.rgb *= sum / 9.0f;
    //diffuse.rgb *= factor;

    //sum¸¸ Ãâ·Â
    //float4 temp = float4(sum / 9.0f, sum / 9.0f, sum / 9.0f, 1);

    return diffuse;
}

///////////////////////////////////////////////////////////////////////////////

RasterizerState RS
{
    CullMode = Front;
};

technique11 T0
{
    pass P0
    {
        SetRasterizerState(RS);

        SetVertexShader(CompileShader(vs_5_0, VS_Depth()));
        SetPixelShader(CompileShader(ps_5_0, PS_Depth()));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Shadow()));
        SetPixelShader(CompileShader(ps_5_0, PS_Shadow()));
    }
}
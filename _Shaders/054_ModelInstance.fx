#include "000_Header.fx"

Texture2D Transforms;

struct VertexInput
{
	float4 Position :Position;
    float2 Uv : Uv0;
    float3 Normal : Normal0;
    float4 BlendIndicies : BlendIndicies0;
    float4 BlendWeights : BlendWeights0;

    matrix Transform : Instance0;
    uint InstId : SV_InstanceID0;
};

struct VertexOuput
{
    float4 Position : SV_Position;
    float2 Uv : Uv0;
};

Texture2D WorldData;
VertexOuput VS(VertexInput input)
{
    VertexOuput output;

    float4 m0 = Transforms.Load(uint3(BoneIndex * 4 + 0, input.InstId, 0));
    float4 m1 = Transforms.Load(uint3(BoneIndex * 4 + 1, input.InstId, 0));
    float4 m2 = Transforms.Load(uint3(BoneIndex * 4 + 2, input.InstId, 0));
    float4 m3 = Transforms.Load(uint3(BoneIndex * 4 + 3, input.InstId, 0));

    matrix transform = matrix(m0, m1, m2, m3); //Bone За·Д
    World = mul(transform, input.Transform);
    
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;

    return output;
}


///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;
float4 PS(VertexOuput input) : SV_TARGET
{
    return DiffuseMap.Sample(Sampler, input.Uv);
}

///////////////////////////////////////////////////////////////////////////////

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
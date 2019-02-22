#include "000_Header.fx"
#define MAX_INST_MODEL 100

Texture2D Transforms;

struct FrameDesc
{
    uint Curr;
    uint Next;
    float Time;
    float FrameTime;
};
FrameDesc Frames[MAX_INST_MODEL];

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

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv0;
};

Texture2D WorldData;
VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;

    float boneIndices[4] =
    {
        input.BlendIndicies.x,
        input.BlendIndicies.y,
        input.BlendIndicies.z,
        input.BlendIndicies.w,
    };

    float boneWeights[4] =
    {
        input.BlendWeights.x,
        input.BlendWeights.y,
        input.BlendWeights.z,
        input.BlendWeights.w,
    };

    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;
    matrix anim = 0;

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        c0 = Transforms.Load(uint3(boneIndices[i] * 4 + 0, Frames[input.InstId].Curr, 0));
        c1 = Transforms.Load(uint3(boneIndices[i] * 4 + 1, Frames[input.InstId].Curr, 0));
        c2 = Transforms.Load(uint3(boneIndices[i] * 4 + 2, Frames[input.InstId].Curr, 0));
        c3 = Transforms.Load(uint3(boneIndices[i] * 4 + 3, Frames[input.InstId].Curr, 0));
        curr = matrix(c0, c1, c2, c3);                                       
                                                                             
        n0 = Transforms.Load(uint3(boneIndices[i] * 4 + 0, Frames[input.InstId].Next, 0));
        n1 = Transforms.Load(uint3(boneIndices[i] * 4 + 1, Frames[input.InstId].Next, 0));
        n2 = Transforms.Load(uint3(boneIndices[i] * 4 + 2, Frames[input.InstId].Next, 0));
        n3 = Transforms.Load(uint3(boneIndices[i] * 4 + 3, Frames[input.InstId].Next, 0));
        next = matrix(n0, n1, n2, n3);

        anim = lerp(curr, next, (matrix) Frames[input.InstId].Time);
        
        transform += mul(boneWeights[i], curr);
    }
    
    World = mul(transform, input.Transform);

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;
    //output.Uv = float2(Frames[input.InstID].Time, Frames[input.InstID].FrameTime);

    return output;
}


///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;
float4 PS(VertexOutput input) : SV_TARGET
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
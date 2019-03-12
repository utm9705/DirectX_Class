//-----------------------------------------------------------------------------
// Define
//-----------------------------------------------------------------------------
#define MAX_INST_MODEL 100

cbuffer CB_Bone
{
    uint BoneIndex;
};

struct AnimationFrame
{
    uint Curr;
    uint Next;
    float Time;
    float FrameTime;
};

cbuffer CB_AnimationFrame
{
    AnimationFrame Frames[MAX_INST_MODEL];
};

///////////////////////////////////////////////////////////////////////////////

struct VertexModel
{
    float4 Position : Position0;
    float2 Uv : Uv0;
    float3 Normal : Normal0;
    float3 Tangent : Tangent0;
    float4 BlendIndices : BlendIndicies0;
    float4 BlendWeights : BlendWeights0;

    matrix Transform : Instance0;
    uint InstId : SV_InstanceID0;
};

///////////////////////////////////////////////////////////////////////////////

void SetModelWorld(inout matrix world, Texture2D textures, VertexModel input)
{
    float4 m0 = textures.Load(uint3(BoneIndex * 4 + 0, input.InstId, 0));
    float4 m1 = textures.Load(uint3(BoneIndex * 4 + 1, input.InstId, 0));
    float4 m2 = textures.Load(uint3(BoneIndex * 4 + 2, input.InstId, 0));
    float4 m3 = textures.Load(uint3(BoneIndex * 4 + 3, input.InstId, 0));
    
    matrix transform = matrix(m0, m1, m2, m3); //Bone За·Д
    world = mul(transform, input.Transform);
}

void SetAnimationWorld(inout matrix world, Texture2D textures, VertexModel input)
{
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;

    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;
    matrix anim = 0;

    float boneIndices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
    float boneWeights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        c0 = textures.Load(uint3(boneIndices[i] * 4 + 0, Frames[input.InstId].Curr, 0));
        c1 = textures.Load(uint3(boneIndices[i] * 4 + 1, Frames[input.InstId].Curr, 0));
        c2 = textures.Load(uint3(boneIndices[i] * 4 + 2, Frames[input.InstId].Curr, 0));
        c3 = textures.Load(uint3(boneIndices[i] * 4 + 3, Frames[input.InstId].Curr, 0));
        curr = matrix(c0, c1, c2, c3);

        n0 = textures.Load(uint3(boneIndices[i] * 4 + 0, Frames[input.InstId].Next, 0));
        n1 = textures.Load(uint3(boneIndices[i] * 4 + 1, Frames[input.InstId].Next, 0));
        n2 = textures.Load(uint3(boneIndices[i] * 4 + 2, Frames[input.InstId].Next, 0));
        n3 = textures.Load(uint3(boneIndices[i] * 4 + 3, Frames[input.InstId].Next, 0));
        next = matrix(n0, n1, n2, n3);

        anim = lerp(curr, next, (matrix) Frames[input.InstId].Time);
        
        transform += mul(boneWeights[i], anim);
    }
    
    world = mul(transform, input.Transform);
}
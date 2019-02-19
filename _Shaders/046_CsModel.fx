#define MAX_THREAD 512

struct Bones
{
    matrix Pose;
};
StructuredBuffer<Bones> BoneBuffer;

int BoneIndex;

ByteAddressBuffer Vertex; //SRV
RWByteAddressBuffer Output; //UAV

struct VertexTextureNormalBlend
{
    float3 Position;
    float2 Uv;
    float3 Normal;
    float4 BlendIndices;
    float4 BlendWeights;
};

[numthreads(MAX_THREAD, 1, 1)]
void CS(uint3 dispatchID : SV_DispatchThreadID)
{
    int fetchAddress = dispatchID.x * 16 * 4;

    VertexTextureNormalBlend vertex;
    
    //Vertex Texture Normal Indices Weights
    //     3       2      3       4       4
    //     0      12     20      32      48
    vertex.Position = asfloat(Vertex.Load3(fetchAddress + 0));
    vertex.Uv = asfloat(Vertex.Load2(fetchAddress + 12));
    vertex.Normal = asfloat(Vertex.Load3(fetchAddress + 20));
    vertex.BlendIndices = asfloat(Vertex.Load4(fetchAddress + 32));
    vertex.BlendWeights = asfloat(Vertex.Load4(fetchAddress + 48));

    vertex.Position = mul(float4(vertex.Position, 1), BoneBuffer[BoneIndex].Pose).xyz;

    Output.Store3(fetchAddress + 0, asuint(vertex.Position));
    Output.Store2(fetchAddress + 12, asuint(vertex.Uv));
    Output.Store3(fetchAddress + 20, asuint(vertex.Normal));
    Output.Store4(fetchAddress + 32, asuint(vertex.BlendIndices));
    Output.Store4(fetchAddress + 48, asuint(vertex.BlendWeights));
}

void Skinning(inout VertexTextureNormalBlend vertex)
{
    float4x4 transform = 0;
    transform += mul(vertex.BlendWeights.x, BoneBuffer[(uint) vertex.BlendIndices.x].Pose);
    transform += mul(vertex.BlendWeights.y, BoneBuffer[(uint) vertex.BlendIndices.y].Pose);
    transform += mul(vertex.BlendWeights.z, BoneBuffer[(uint) vertex.BlendIndices.z].Pose);
    transform += mul(vertex.BlendWeights.w, BoneBuffer[(uint) vertex.BlendIndices.w].Pose);

    vertex.Position = mul(float4(vertex.Position, 1), transform).xyz;
    vertex.Normal = mul(vertex.Normal, (float3x3) transform);
}

[numthreads(MAX_THREAD, 1, 1)]
void CS_Animation(uint3 dispatchID : SV_DispatchThreadID)
{
    int fetchAddress = dispatchID.x * 16 * 4;

    VertexTextureNormalBlend vertex;
    
    //Vertex Texture Normal Indices Weights
    //     3       2      3       4       4
    //     0      12     20      32      48
    vertex.Position = asfloat(Vertex.Load3(fetchAddress + 0));
    vertex.Uv = asfloat(Vertex.Load2(fetchAddress + 12));
    vertex.Normal = asfloat(Vertex.Load3(fetchAddress + 20));
    vertex.BlendIndices = asfloat(Vertex.Load4(fetchAddress + 32));
    vertex.BlendWeights = asfloat(Vertex.Load4(fetchAddress + 48));

    Skinning(vertex);

    Output.Store3(fetchAddress + 0, asuint(vertex.Position));
    Output.Store2(fetchAddress + 12, asuint(vertex.Uv));
    Output.Store3(fetchAddress + 20, asuint(vertex.Normal));
    Output.Store4(fetchAddress + 32, asuint(vertex.BlendIndices));
    Output.Store4(fetchAddress + 48, asuint(vertex.BlendWeights));
}

technique11 T0
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CS()));
    }

    pass P1
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CS_Animation()));
    }
}
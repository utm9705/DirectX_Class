struct Data
{
    uint Number;
    float3 Id;
    uint Data;
};

StructuredBuffer<Data> Input; //SRV
RWStructuredBuffer<Data> Output; //UAV

[numthreads(16, 1, 1)]
void CS(uint3 groupID : SV_GroupID, uint3 groupThreadID : SV_GroupThreadID, uint3 dispatchID : SV_DispatchThreadID)
{
    Output[dispatchID.x].Number = Input[dispatchID.x].Number;

    Output[dispatchID.x].Id.x = groupID.x;
    Output[dispatchID.x].Id.y = groupThreadID.x;
    Output[dispatchID.x].Id.z = dispatchID.x;

    Output[dispatchID.x].Data = Input[dispatchID.x].Data * 10;
}

technique11 T0
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, CS()));
    }
}
#include "000_Header.fx"

struct VertexInput
{
    float4 Position : Position0;
    uint Id : SV_VertexID0;

    float4 Color : Instance0;
    uint InstId : SV_InstanceID0;
};

struct VertexOuput
{
    float4 Position : SV_Position;
    float4 Color : Color0;
    uint Id : VertexID0;
};

Texture2D WorldData;
VertexOuput VS(VertexInput input)
{
    VertexOuput output;

    float3 position = 0;
    position.x = WorldData.Load(uint3(3, input.InstId, 0)).x;
    position.y = WorldData.Load(uint3(7, input.InstId, 0)).y;
    position.z = WorldData.Load(uint3(11, input.InstId, 0)).z;



    output.Position.xyz = input.Position.xyz + position;
    output.Position.w = 1.0f;

    output.Color = input.Color;
    output.Id = input.Id;

    return output;
}


///////////////////////////////////////////////////////////////////////////////

float4 PS(VertexOuput input) : SV_TARGET
{
    return input.Color;
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
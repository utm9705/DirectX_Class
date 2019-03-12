#include "000_Header.fx"

struct VertexInput
{
    float4 Position : Position0;
    float2 Uv : Uv0;

    matrix World : Instance0;
    uint Id : SV_InstanceId0;
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv0;
    uint Id : Id0;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.Position = mul(input.Position, input.World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;
    output.Id = input.Id;

    return output;
}

SamplerState Sampler;
Texture2DArray MapArray;

Texture2D Test;
///////////////////////////////////////////////////////////////////////////////

float4 PS(VertexOutput input) : SV_TARGET
{
    float3 uvw = float3(input.Uv, input.Id % 6);
    float4 color = MapArray.Sample(Sampler, uvw);

    //float4 color = MapArray[input.Id % 6].Sample(Sampler, input.Uv);
    
    return color;
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
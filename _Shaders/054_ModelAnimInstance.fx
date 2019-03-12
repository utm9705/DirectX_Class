#include "000_Header_N.fx"

Texture2D Transforms;

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv0;
};

VertexOutput VS(VertexModel input)
{
    VertexOutput output;

    SetAnimationWorld(Transforms, input);

    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);

    output.Uv = input.Uv;

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
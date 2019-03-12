#include "000_Header_N.fx"

Texture2D Transforms;

struct VertexOuput
{
    float4 Position : SV_Position;
    float2 Uv : Uv0;
};

VertexOuput VS(VertexModel input)
{
    VertexOuput output;

    SetModelWorld(Transforms, input);

    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);

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
#include "000_Header.fx"

struct VertexOuput
{
    float4 Position : SV_POSITION;
};

VertexOuput VS(Vertex input)
{
    VertexOuput output;
    output.Position = input.Position;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS(VertexOuput input) : SV_TARGET
{
    return float4(1, 0, 0, 1);
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
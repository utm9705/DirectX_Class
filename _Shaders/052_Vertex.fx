#include "000_Header.fx"

struct VertexInput
{
    float4 Position : Position0;
    float4 Color : Color0;
};

struct VertexOuput
{
    float4 Position : SV_Position;
    float4 Color : Color0;
};

VertexOuput VS(VertexInput input)
{
    VertexOuput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Color = input.Color;

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
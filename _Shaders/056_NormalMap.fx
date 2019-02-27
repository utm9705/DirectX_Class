#include "000_Header.fx"

struct VertexOuput
{
    float4 Position : SV_Position;
    float2 Uv : Uv0;
};

VertexOuput VS(VertexTexture input)
{
    VertexOuput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;

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
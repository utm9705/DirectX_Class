#include "000_Header_N.fx"

struct VertexOuput
{
    float4 Position : SV_Position;
};

VertexOuput VS(Vertex input)
{
    VertexOuput output;

    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);

    return output;
}

///////////////////////////////////////////////////////////////////////////////

struct PixelOutput
{
    float4 Color : SV_TARGET0;
    float4 Color2 : SV_TARGET1;
};

PixelOutput PS(VertexOuput input)
{
    PixelOutput output;
    output.Color = float4(1, 0, 0, 1);
    output.Color = float4(0, 0, 1, 1);

    return output;
}

///////////////////////////////////////////////////////////////////////////////

RasterizerState RS
{
    FillMode = Wireframe;
};

technique11 T0
{
    pass P0
    {
        SetRasterizerState(RS);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetRasterizerState(RS);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
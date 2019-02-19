#include "000_Header.fx"

int Selected = 0;

struct VertexOuput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
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

SamplerState Sampler0
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

SamplerState Sampler1
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = MIRROR;
    AddressV = MIRROR;
};

SamplerState Sampler2
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

SamplerState Sampler3
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = BORDER;
    AddressV = BORDER;
    BorderColor = float4(1, 0, 0, 1);
};

float4 PS(VertexOuput input) : SV_TARGET
{
    float4 diffuse = 0;
    
    [branch]
    if(Selected == 0)
        diffuse = DiffuseMap.Sample(Sampler0, input.Uv);
    else if (Selected == 1)
        diffuse = DiffuseMap.Sample(Sampler1, input.Uv);
    else if (Selected == 2)
        diffuse = DiffuseMap.Sample(Sampler2, input.Uv);
    else if (Selected == 3)
        diffuse = DiffuseMap.Sample(Sampler3, input.Uv);

    return diffuse;
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
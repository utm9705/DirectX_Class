#include "000_Header.fx"

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
struct VertexOuput
{
    float3 Center : Center0;
    float2 Size : Size0;
};

VertexOuput VS(VertexSize input)
{
    VertexOuput output;
    output.Center = input.Position.xyz;
    output.Size = input.Size;

    return output;
}

//-----------------------------------------------------------------------------
// Geometry Shader
//-----------------------------------------------------------------------------
struct GeometryOutput
{
    float4 Position : SV_Position0;
    float2 Uv : Uv0;
    uint PrimitiveID : Id0;
};

const float2 TexCoord[4] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
    float2(1.0f, 0.0f),
};

[maxvertexcount(4)]
void GS(point VertexOuput input[1], uint primitiveID : SV_PrimitiveID, inout TriangleStream<GeometryOutput> stream)
{
    float3 up = float3(0, 1, 0);
    float3 forward = ViewPosition - input[0].Center;
    forward.y = 0.0f;
    forward = normalize(forward);

    float3 right = cross(forward, up);


    float halfWidth = 0.5f * input[0].Size.x;
    float halfHeight = 0.5f * input[0].Size.y;

    float4 v[4];
    v[0] = float4(input[0].Center.xyz - halfWidth * right - halfHeight * up, 1.0f); //LL
    v[1] = float4(input[0].Center.xyz - halfWidth * right + halfHeight * up, 1.0f); //LU
    v[2] = float4(input[0].Center.xyz + halfWidth * right - halfHeight * up, 1.0f); //RL
    v[3] = float4(input[0].Center.xyz + halfWidth * right + halfHeight * up, 1.0f); //RU    

    
    GeometryOutput output;
    
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        output.Position = mul(v[i], View);
        output.Position = mul(output.Position, Projection);
        output.Uv = TexCoord[i];
        output.PrimitiveID = primitiveID;

        stream.Append(output);
    }
}

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
Texture2DArray Map;
SamplerState Sampler;

float4 PS(GeometryOutput input) : SV_TARGET
{
    float3 uvw = float3(input.Uv, input.PrimitiveID % 6);

    float4 color = Map.Sample(Sampler, uvw);
    clip(color.a - 0.2f);

    return color;
}

//-----------------------------------------------------------------------------
// Technique
//-----------------------------------------------------------------------------
RasterizerState FillMode
{
    //FillMode = Wireframe;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));

        SetRasterizerState(FillMode);
    }
}
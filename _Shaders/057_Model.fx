#include "000_Header_N.fx"
#include "000_Model.fx"

Texture2D Transforms;

struct VertexOuput
{
    float4 Position : SV_Position;
    float3 Normal : Normal0;
    float3 Tangent : Tangent0;
    float2 Uv : Uv0;
};

VertexOuput VS_Model(VertexModel input)
{
    VertexOuput output;

    SetModelWorld(World, Transforms, input);

    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);
    
    output.Normal = WorldNormal(input.Normal);
    output.Tangent = WorldTangent(input.Tangent);

    output.Uv = input.Uv;

    return output;
}

VertexOuput VS_Animation(VertexModel input)
{
    VertexOuput output;

    SetAnimationWorld(World, Transforms, input);

    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);
    
    output.Normal = WorldNormal(input.Normal);
    output.Tangent = WorldTangent(input.Tangent);

    output.Uv = input.Uv;

    return output;
}


///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;
float4 PS(VertexOuput input) : SV_TARGET
{
    float4 diffuseMap = DiffuseMap.Sample(Sampler, input.Uv);
    float4 normalMap = NormalMap.Sample(Sampler, input.Uv);

    float intensity = 1;
    intensity = saturate((dot(normalize(input.Normal), -LightDirection)));
    
    //[flatten]
    //if (length(normalMap) > 0)
    //{
    //    float3 bump = TangentSpace(normalMap.rgb, normalize(input.Normal), input.Tangent);
    //    intensity = saturate(dot(bump, -LightDirection));
    //}

    return diffuseMap * intensity;
}

///////////////////////////////////////////////////////////////////////////////

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Model()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Animation()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
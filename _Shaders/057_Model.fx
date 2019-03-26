#include "000_Header_N.fx"
#include "000_Model.fx"

//float4 Planes;

Texture2D Transforms; //여기에 modelbone의 정보가 들어온다

struct VertexOutput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float2 Uv : Uv0;

    float Culling : SV_CullDistance0; //정점을 자른다
    //float Culling : SV_ClipDistance0; //픽셀을 자른다
};

VertexOutput VS_Model(VertexModel input)
{
    VertexOutput output;

    SetModelWorld(World, Transforms, input);
    
    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);

    output.Normal = WorldNormal(input.Normal);
    output.Tangent = WorldTangent(input.Tangent);

    output.Uv = input.Uv;
    //output.Culling = dot(mul(input.Position, World), Planes);

    return output;
}

VertexOutput VS_Animation(VertexModel input)
{
    VertexOutput output;

    SetAnimationWorld(World, Transforms, input);
    
    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);

    output.Normal = WorldNormal(input.Normal);
    output.Tangent = WorldTangent(input.Tangent);

    output.Uv = input.Uv;
    //output.Culling = dot(mul(input.Position, World), Planes);

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;
float4 PS(VertexOutput input) : SV_TARGET
{
    float4 diffuseMap = DiffuseMap.Sample(Sampler, input.Uv);
    float4 normalMap = NormalMap.Sample(Sampler, input.Uv);

    float intensity = 1;

    //텍스쳐가 들어오면
    [flatten]
    if(length(normalMap) > 0)
    {
        float3 bump = TangentSpace(normalMap.rgb, normalize(input.Normal), input.Tangent);
        intensity = saturate(dot(bump, -LightDirection));
    }

   //return diffuseMap * intensity;
    return diffuseMap;

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


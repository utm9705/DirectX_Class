struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float Shininess;
};

struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Attenuation;
    float PointLight_Padding;
};

void ComputeDirectionalLight(Material m, DirectionalLight l, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0, 0, 0, 0);
    diffuse = float4(0, 0, 0, 0);
    specular = float4(0, 0, 0, 0);

    float3 light = -l.Direction;
    ambient = m.Ambient * l.Ambient;

    float diffuseFactor = dot(light, normal);

    [flatten]
    if(diffuseFactor > 0.0f)
    {
        diffuse = diffuseFactor * m.Diffuse * l.Diffuse;

        
        float3 r = reflect(-light, normal);
        
        float specularFactor = 0;
        specularFactor = saturate(dot(r, toEye));
        specularFactor = pow(specularFactor, m.Specular.a);
        specular = specularFactor * m.Specular * l.Specular;
    }
}

void ComputePointLight(Material m, PointLight l, float3 position, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0, 0, 0, 0);
    diffuse = float4(0, 0, 0, 0);
    specular = float4(0, 0, 0, 0);

     
}
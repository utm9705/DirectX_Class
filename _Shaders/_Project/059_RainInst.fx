#include "000_Header_N.fx"

cbuffer CB_Rain
{
    float4 Color;
    
    float3 Velocity;
    float DrawDistance;

    float3 Origin;
    float CB_Rain_Padding;

    float3 Size;
};

//-----------------------------------------------------------------------------
// Pass0
//-----------------------------------------------------------------------------

struct VertexInput
{
    float4 Position : Position0;
    float2 Uv : Uv0;
    float3 Normal : Normal0;

    matrix World : Instance0;
    uint Id : SV_InstanceId0;
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv0;
    float Alpha : Alpha0;

};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    float3 velocity = Velocity;
    velocity.xz /= input.World._22 * 0.1f; // 보정

    float3 displace = Time * velocity; // 물방울 이동거리

    //input.Position.xyz = Origin + (Size + (input.Position.xyz + displace) % Size) % Size - (Size * 0.5f);
    
    input.World._41 = Origin.x + (Size.x + (input.World._41 + displace.x) % Size.x) % Size.x - (Size.x * 0.5f);
    input.World._42 = Origin.y + (Size.y + (input.World._42 + displace.y) % Size.y) % Size.y - (Size.y * 0.5f);
    input.World._43 = Origin.z + (Size.z + (input.World._43 + displace.z) % Size.z) % Size.z - (Size.z * 0.5f);

    float4 position = mul(input.Position, input.World);

    
    float3 axis = normalize(-velocity); // 방향
    float3 view = position.xyz - ViewPosition();
    float3 side = normalize(cross(axis, view));

    position.xyz += (input.Uv.x - 0.5f) * side * input.World._11;
    position.xyz += (1.5f - input.Uv.y * 1.5f) * axis * input.World._22;
    position.w = 1.0f;




    output.Position = mul(position, View);
    output.Position = mul(output.Position, Projection);

    
    output.Uv = input.Uv;




    float alpha = cos(Time + (input.Position.x + input.Position.z));
    alpha = saturate(1.5f + alpha / DrawDistance * 2);

    output.Alpha = 0.2f * saturate(1 - output.Position.z / DrawDistance) * alpha;

    return output;
}

SamplerState Sampler;

Texture2D Base;
///////////////////////////////////////////////////////////////////////////////

float4 PS(VertexOutput input) : SV_TARGET
{

    float4 color = Base.Sample(Sampler, input.Uv);
    color.rgb += Color.rgb * (1 + input.Alpha) * 2.0f;
    color.a = color.a * (input.Alpha * 1.5f);

    return float4(Color.rgb, color.a); // 이게 더 낫다

    //return float4(1,0,0,1); // 이게 더 낫다
}

BlendState AlphaBlend
{
    BlendEnable[0] = true;
    DestBlend[0] = INV_SRC_ALPHA;
    SrcBlend[0] = SRC_ALPHA;
    BlendOp[0] = Add;

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = One;
    RenderTargetWriteMask[0] = 0x0F;
};

///////////////////////////////////////////////////////////////////////////////
RasterizerState Wire
{
    FillMode = WireFrame;
};

technique11 T0
{
    pass P0
    {
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetRasterizerState(Wire);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
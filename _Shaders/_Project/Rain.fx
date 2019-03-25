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
    float4 Position : POSITION1;
    float2 Uv : Uv0;
    float2 Scale : Scale0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float2 Uv : Uv0;
    float Alpha : Alpha0;
};


// ���� ���ؽ� �ϳ����� �̷� �̵������� ��, ������ġ�� �����Ҷ� �����ϰ� ������Ŵ

// �ν��Ͻ�ȭ �ϸ� �� �ν��Ͻ���ġ�� �����ϰ� �ѱ�� �ɵ�

// �� �������� ������ ���� : ������, uv, ������

VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    float3 velocity = Velocity;
    velocity.xz /= input.Scale.y * 0.1f; // ����

    float3 displace = Time * velocity; // ����� �̵��Ÿ�
    input.Position.xyz = Origin + (Size + (input.Position.xyz + displace) % Size) % Size - (Size * 0.5f);
    

    float4 position = WorldPosition(input.Position);
    


    float3 axis = normalize(-velocity); // ����
    float3 view = position.xyz - ViewPosition(); 
    float3 side = normalize(cross(axis, view));

    position.xyz += (input.Uv.x - 0.5f) * side * input.Scale.x;
    position.xyz += (1.5f - input.Uv.y * 1.5f) * axis * input.Scale.y;
    position.w = 1.0f;

    output.Position = ViewProjection(position);
    output.Uv = input.Uv;


    float alpha = cos(Time + (input.Position.x + input.Position.z));
    alpha = saturate(1.5f + alpha / DrawDistance * 2);

    output.Alpha = 0.2f * saturate(1 - output.Position.z / DrawDistance) * alpha;

    return output;
}


Texture2D Base;

SamplerState Sampler;

float4 PS(VertexOutput input) : SV_TARGET0
{
    float4 color = Base.Sample(Sampler, input.Uv);
    color.rgb += Color.rgb * (1 + input.Alpha) * 2.0f;
    color.a = color.a * (input.Alpha * 1.5f);

    return float4(color.rgb, color.a); // �̰� �� ����


    //return float4(color.rgb, color.a);
   // return float4(1, 1, 1, 1);

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

//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------
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
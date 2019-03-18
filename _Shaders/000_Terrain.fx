//-----------------------------------------------------------------------------
// Textures
//-----------------------------------------------------------------------------
Texture2D BaseMap;
Texture2D AlphaMap;
Texture2D LayerMap;


//-----------------------------------------------------------------------------
// Brush
//-----------------------------------------------------------------------------
cbuffer CB_Brush
{
    float4 BrushColor;
    float3 BrushLocation;
    int BrushType;
    int BrushRange;
};

float3 GetBrushColor(float3 position)
{
    if (BrushType == 0)
        return float3(0, 0, 0);

    if (BrushType == 1)
    {
        if ((position.x >= (BrushLocation.x - BrushRange)) &&
            (position.x <= (BrushLocation.x + BrushRange)) &&
            (position.z >= (BrushLocation.z - BrushRange)) &&
            (position.z <= (BrushLocation.z + BrushRange)))
        {
            return BrushColor.rgb;
        }
    }

    if (BrushType == 2)
    {
        float3 dx = position.x - BrushLocation.x;
        float3 dy = position.z - BrushLocation.z;

        float dist = sqrt(dx * dx + dy * dy);
        //float dist = distance(dx, dy);

        if (dist <= BrushRange)
            return BrushColor.rgb;
    }

    return float3(0, 0, 0);
}


//-----------------------------------------------------------------------------
// GridLine
//-----------------------------------------------------------------------------
cbuffer CB_GridLine
{
    float4 GridLineColor;
    
    int VisibleGridLine;
    float GridLineThickness;
    float GridLineSize;
};

float3 GetGridLineColor(float3 position)
{
    if (VisibleGridLine == 0)
        return float3(0, 0, 0);


    float2 grid = position.xz / GridLineSize;
    float2 range = abs(frac(grid - 0.5f) - 0.5f);
    float2 speed = fwidth(grid);

    float2 pixel = range / speed;
    float weight = saturate(min(pixel.x, pixel.y) - GridLineThickness);

    return lerp(GridLineColor.rgb, float3(0, 0, 0), weight);
}

//-----------------------------------------------------------------------------
// Tesselation
//-----------------------------------------------------------------------------
cbuffer CB_Terrain
{
    float MinDistance;
    float MaxDistance;
    float MinTessellation;
    float MaxTessellation;

    float TexelCellSpaceU;
    float TexelCellSpaceV;
    float WorldCellSpace;
    float HeightRatio;

    float2 TexScale;
    float CB_Terrain_Padding2[2];

    float4 WorldFrustumPlanes[6];
};

struct VertexInput_Terrain
{
    float4 Position : Position0;
    float2 Uv : Uv0;
    float2 BoundY : BoundY0;
};

struct VertexOutput_Terrain
{
    float4 Position : Position0;
    float2 Uv : Uv0;
    float2 BoundY : BoundY0;
};

struct ConstantHullOutput_Terrain
{
    float Edge[4] : SV_TessFactor;
    float Inside[2] : SV_InsideTessFactor;
};

struct HullOutput_Terrain
{
    float4 Position : Position0;
    float2 Uv : Uv0;
};

struct DomainOutput_Terrain
{
    float4 Position : SV_Position0;
    float3 wPosition : Position1;
    float2 Uv : Uv0;
    float2 TiledUv : Uv1;
};

bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
    float3 n = abs(plane.xyz);
    float r = dot(extents, n);
    float s = dot(float4(center, 1), plane);

    return (s + r) < 0.0f;
}

bool AabbOutsideFrustumTest(float3 center, float3 extents)
{
    [roll(6)]
    for (int i = 0; i < 6; i++)
    {
        [flatten]
        if (AabbBehindPlaneTest(center, extents, WorldFrustumPlanes[i]))
            return true;
    }

    return false;
}

float TerrainTessFactor(float3 position, float3 viewPosition)
{
    float d = distance(position, viewPosition);
    float s = saturate((d - MinDistance) / (MaxDistance - MinDistance));

    return pow(2, lerp(MaxTessellation, MinTessellation, s));
}
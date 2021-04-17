//***************************************************************************************
//LU SHUAI QIAO 2020
//Frame reference by Introduction to 3D Game Programming with Directx 12
//***************************************************************************************
#ifndef NUM_DIR_LIGHTS
    #define NUM_DIR_LIGHTS 3
#endif

#ifndef NUM_POINT_LIGHTS
    #define NUM_POINT_LIGHTS 0
#endif

#ifndef NUM_SPOT_LIGHTS
    #define NUM_SPOT_LIGHTS 0
#endif


#include "LightingUtil.hlsli"

struct MaterialData
{
	float4   DiffuseAlbedo;
	float3   FresnelR0;
	float    Roughness;
	float4x4 MatTransform;
	uint     DiffuseMapIndex;
	uint     NormalMapIndex;
	uint     MatPad1;
	uint     MatPad2;
};

TextureCube gCubeMap : register(t0);

Texture2D gShadowMap : register(t1);

Texture2D gTextureMaps[10] : register(t2);

StructuredBuffer<MaterialData> gMaterialData : register(t0, space1);


SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerComparisonState gsamShadow : register(s6);

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
	float4x4 gTexTransform;
	uint gMaterialIndex;
	uint gObjPad0;
	uint gObjPad1;
	uint gObjPad2;
};

cbuffer cbPass : register(b1)
{
    float4x4 gView;
    float4x4 gInvView;
    float4x4 gProj;
    float4x4 gInvProj;
    float4x4 gViewProj;
    float4x4 gInvViewProj;
    float4x4 gShadowTransform;
    float3 gEyePosW;
    float cbPerObjectPad1;
    float2 gRenderTargetSize;
    float2 gInvRenderTargetSize;
    float gNearZ;
    float gFarZ;
    float gTotalTime;
    float gDeltaTime;
    float4 gAmbientLight;
    Light gLights[MaxLights];
};

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N)*N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}


float CalcShadowFactor(float4 shadowPosH
)
{

    shadowPosH.xyz /= shadowPosH.w;

    if (shadowPosH.x > 1.0f || shadowPosH.x < 0.0f)
    {
        return 1.0f;
    }
    else if (shadowPosH.y > 1.0f || shadowPosH.y < 0.0f)
    {
        return 1.0f;
    }
    else
    {
    
        float depth = shadowPosH.z;
        uint width = 0, height = 0, numMips = 0;
        gShadowMap.GetDimensions(0, width, height, numMips);
        float dx = 1.0f / (float) width;
        float percentLit = 0.0f;
        const float2 offsets[9] =
        {
            float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
        };

    [unroll]
        for (int i = 0; i < 9; ++i)
        {
            if (shadowPosH.x + offsets[i].x > 1.0f || shadowPosH.x - offsets[i].x < 0.0f)
            {
                return 1.0f;

            }
            else if (shadowPosH.y + offsets[i].y > 1.0f || shadowPosH.y - offsets[i].y < 0.0f)
            {
                return 1.0f;

            }
            else
            {
            
                percentLit += gShadowMap.SampleCmpLevelZero(gsamShadow,
            shadowPosH.xy + offsets[i], depth).r;
            }
        }
    
        return percentLit / 9.0f;
    }
}

float3 BoxCubeMapLookup(float3 rayOrigin, float3 unitRayDir, float3 boxCenter, float3 boxExtents)
{
    float3 p = rayOrigin - boxCenter;
    
    float3 t1 = (-p + boxExtents) / unitRayDir;
    float3 t2 = (-p - boxExtents) / unitRayDir;

    float3 tmax = max(t1, t2);
    
    float t = min(min(tmax.x, tmax.y), tmax.z);
    
    return p + t * unitRayDir;
    
}

float3 calculateWaveNormal(float2 pos, float dist)
{
    float omega[4] = { 37, 17, 23, 16 };
    float phi[4] = { 7, 5, 3, 11 };
    float2 dir[4] =
    {
        float2(1, 0),
        float2(1, 0.1),
        float2(1, 0.3),
        float2(1, 0.57)
    };
    float t = gTotalTime * 0.61f;
    float3 normal = 0;
    for (int i = 0; i < 4; i++)
    {
        omega[i] *= 0.09f;
        normal += float3(
            omega[i] * dir[i].x * 1 * cos(dot(dir[i], pos) * omega[i] + t * phi[i]),
            1,
            omega[i] * dir[i].y * 1 * cos(dot(dir[i], pos) * omega[i] + t * phi[i])
        );
    }
    normal.xz = normal.xz * 0.1f * exp(-1 / 2.3f * dist);

    return normalize(normal);
}
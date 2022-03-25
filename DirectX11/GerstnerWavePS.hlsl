#include "GerstnerWave.hlsli"

cbuffer CBuf : register(b0)
{
    float3 mCameraPos;
    float3 mDirection;
    float3 mAmbientLight;
    float3 mDiffuseColor;
    float3 mSpecColor;
};

cbuffer CBuf1 : register(b1)
{
    float3 mSeaBaseColor;
    float3 mSeaShallowColor;
    float3 mSkyColor;
    float mTime;
    float mNoiseStrength;
    float mNoiseSizeLerp;
    float mWaveSpeed;
    float mBaseColorStrength;
    float mShallowColorStrength;
    float mShininess;
    float mColorHeightOffset;
    float mF0;
}

static const int waveNumber = 8;
static const int count = 4;

float3 GetSkyColor(float3 dir, float3 c)
{
    dir.y = max(0.0f, dir.y);
    float et = 1.0f - dir.y;
    return (1.0f - c) * et + c;
}

float3 OceanColor(float3 worldPos, float waveHeight, float3 normal)
{
    float3 lightDir = normalize(-mDirection);
    float3 viewDir = normalize(mCameraPos - worldPos);
    float3 mirrorEyeDir = normalize(reflect(-viewDir, normal));

    float facing = saturate(1.0f - dot(viewDir, normal));
    float fresnel = mF0 + (1.0f - mF0) * pow(facing, 5.0f);
    
    float3 diffuse = saturate(dot(normal, lightDir)) * mDiffuseColor;
    
    float dotSpec = saturate(dot(mirrorEyeDir, lightDir) * 0.5f + 0.5f);
    float3 specular = (1.0f - fresnel) * saturate(lightDir.y) * pow(dotSpec, 512.0f) * (mShininess * 1.8f + 0.2f);
    specular += specular * 25.0f * saturate(mShininess - 0.05f) * mSpecColor;

    float3 seaReflectColor = GetSkyColor(mirrorEyeDir, mSkyColor);
    float3 seaBaseColor = mSeaBaseColor * diffuse * mBaseColorStrength + lerp(mSeaBaseColor, mSeaShallowColor * mShallowColorStrength, diffuse);
    float3 waterColor = lerp(seaBaseColor, seaReflectColor, fresnel);
    float3 seaColor = waterColor + mSeaShallowColor * (waveHeight * 0.5f + 0.2f) * mColorHeightOffset;
 
    return mAmbientLight + seaColor + specular;
}

float4 main(float3 worldPos : POSITION) : SV_TARGET
{
    float3 worldPosition = worldPos;
    float time = mTime / 20.0f * mWaveSpeed;    
    float3 p = 0.0;
    float3 pb = float3(0.05f, 0.0f, 0.0f);
    float3 pt = float3(0.0f, 0.0f, 0.05f);
    float3 vBi = worldPosition + float3(0.05f, 0.0f, 0.0f);
    float3 vTan = worldPosition + float3(0.0f, 0.0f, 0.05f);

    for (int m = 0; m < count; m++)
    {
        p += GerstnerWave(amp[m], freq[m], steep[m], spd[m], noiseSize[m], dir[m], worldPosition.xz, time, m, mNoiseStrength);
        pb += GerstnerWave(amp[m], freq[m], steep[m], spd[m], noiseSize[m], dir[m], vBi.xz, time, m, mNoiseStrength);
        pt += GerstnerWave(amp[m], freq[m], steep[m], spd[m], noiseSize[m], dir[m], vTan.xz, time, m, mNoiseStrength);
    }
    for (int n = count; n < waveNumber; n++)
    {
        p += GerstnerWaveCross(amp[n], freq[n], steep[n], spd[n], noiseSize[n], dir[n], worldPosition.xz, time, n, mNoiseStrength, mNoiseSizeLerp);
        pb += GerstnerWaveCross(amp[n], freq[n], steep[n], spd[n], noiseSize[n], dir[n], vBi.xz, time, n, mNoiseStrength, mNoiseSizeLerp);
        pt += GerstnerWaveCross(amp[n], freq[n], steep[n], spd[n], noiseSize[n], dir[n], vTan.xz, time, n, mNoiseStrength, mNoiseSizeLerp);
    }
    worldPosition += p;
    float3 normal = normalize(cross(pt - p, pb - p));

    float waveHeight = worldPosition.y - worldPos.y;
    float3 result = OceanColor(worldPosition, waveHeight, normal);
    float4 col = float4(result, 1.0f);
    
    return col;
}

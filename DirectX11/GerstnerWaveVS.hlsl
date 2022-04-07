#include "GerstnerWave.hlsli"

cbuffer CBuf : register(b0)
{
    matrix mWorldTransform;
    matrix mViewProj;
};

cbuffer CBuf1 : register(b1)
{
    float mTime;
    float mNoiseStrength;
    float mNoiseSizeLerp;
    float mWaveSpeed;
}

static const int waveNumber = 8;
static const int count = 4;

struct VSOut
{
    float3 worldPos : POSITION;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION)
{
    VSOut vso;

    float4 worldPos = mul(float4(pos, 1.0f), mWorldTransform);
    vso.worldPos = (float3) worldPos;

    float time = mTime / 20.0f * mWaveSpeed;    
    float3 position = 0.0f;
    
    for (int i = 0; i < count; i++)
    {
        position += GerstnerWave(amp[i], freq[i], steep[i], spd[i], noiseSize[i], dir[i], worldPos.xz, time, i, mNoiseStrength);
    }
    for (int j = count; j < waveNumber; j++)
    {
        position += GerstnerWaveCross(amp[j], freq[j], steep[j], spd[j], noiseSize[j], dir[j], worldPos.xz, time, j, mNoiseStrength, mNoiseSizeLerp);
    }
    
    worldPos.xyz += position;
    vso.pos = mul(worldPos, mViewProj);

    return vso;
}

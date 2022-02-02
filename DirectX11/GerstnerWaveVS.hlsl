#include "GerstnerWave.hlsli"

cbuffer CBuf : register(b0)
{
    matrix mWorldTransform;
    matrix mViewProj;
};

cbuffer TBuf : register(b1)
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
    float3 world_pos : Position;
    float4 vertex : SV_POSITION;
};

VSOut main(float3 vertex : Position)
{
    VSOut o;

    float4 world_pos = mul(float4(vertex, 1.0f), mWorldTransform);
    o.world_pos = (float3) mul(float4(vertex, 1.0f), mWorldTransform);

    float time = mTime / 20.0f * mWaveSpeed;
    
    float3 p = 0.0;
    for (int i = 0; i < count; i++)
    {
        p += GerstnerWave(amp[i], freq[i], steep[i], speed[i], noise_size[i], dir[i], world_pos.xz, time, i, mNoiseStrength);
    }
    for (int j = waveNumber - count; j < waveNumber; j++)
    {
        p += GerstnerWave_Cross(amp[j], freq[j], steep[j], speed[j], noise_size[j], dir[j], world_pos.xz, time, j, mNoiseStrength, mNoiseSizeLerp);
    }
    world_pos.xyz += p;

    o.vertex = mul(world_pos, mViewProj);

    return o;
}

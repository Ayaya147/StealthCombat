#include "GerstnerWave.hlsli"

cbuffer CBuf : register(b0)
{
    matrix mWorldTransform;
    matrix mViewProj;
};

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

    float time = mTime/20.0f * _WaveSpeed;
    
    float3 p = 0.0;
    for (int i = 0; i < count; i++)
    {
        p += GerstnerWave(amp[i], freq[i], steep[i], speed[i], noise_size[i], dir[i], world_pos.xz, time, i);
    }
    for (int j = wave_number - count; j < wave_number; j++)
    {
        p += GerstnerWave_Cross(amp[j], freq[j], steep[j], speed[j], noise_size[j], dir[j], world_pos.xz, time, j);
    }
    world_pos.xyz += p;

    o.vertex = mul(world_pos, mViewProj);

    return o;
}

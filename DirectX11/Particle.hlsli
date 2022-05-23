static const uint numThreads = 512;

cbuffer SystemBuffer : register(b0)
{
    float mDeltaTime;
    float mRandom;
};

cbuffer ParticleEmitter : register(b1)
{
    float4 mVelocityMin, mVelocityMax;
    float4 mPositionMin, mPositionMax;
    float4 mPosition;
    float mLifeTimeMin, mLifeTimeMax;
    float mScaleMin, mScaleMax;
    int mNewParticles, mMaxParticles;
    float mGravity, mMass;
    uint mEmitRate, mNumdispatch;
};

struct Particle
{
    float4 velocity;
    float4 color;
    float4 position;
    float age;
    float scale;
};

StructuredBuffer<Particle> ParticleIn : register(t0);
StructuredBuffer<int> ParticleCountIn : register(t1);

RWStructuredBuffer<Particle> ParticleOut : register(u0);
RWStructuredBuffer<int> ParticleCountOut : register(u1);

float WangHash(inout uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return (float) seed / (float) 0xFFFFFFFF;
}

void SpawnParticle(uint rand)
{
    uint index = 0;
    InterlockedAdd(ParticleCountOut[0].x, 1, index);

    ParticleOut[index].age = lerp(mLifeTimeMin, mLifeTimeMax, WangHash(rand));
    ParticleOut[index].velocity = (float4(lerp(mVelocityMin.x, mVelocityMax.x, WangHash(rand)), lerp(mVelocityMin.y, mVelocityMax.y, WangHash(rand)), lerp(mVelocityMin.z, mVelocityMax.z, WangHash(rand)), 0));
    ParticleOut[index].scale = lerp(mScaleMin, mScaleMax, WangHash(rand));
    ParticleOut[index].position = mPosition + float4(lerp(mPositionMin.x, mPositionMax.x, WangHash(rand)), lerp(mPositionMin.y, mPositionMax.y, WangHash(rand)), lerp(mPositionMin.z, mPositionMax.z, WangHash(rand)), 0);
    ParticleOut[index].color = float4(lerp(0.7f, 1.0f, WangHash(rand)), lerp(0.45f, 0.75f, WangHash(rand)), lerp(0.55f, 0.85f, WangHash(rand)), 1);
}

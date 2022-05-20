#include "Particle.hlsli"

[numthreads(numThreads, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    int currentParticleAmount = ParticleCountIn[0].x;
    int i = DTid.x;
    if (i == 0)
    {
        uint val = 0;
        InterlockedExchange(ParticleCountOut[0], 0, val);
    }
    AllMemoryBarrier();
	
    if (i < ParticleCountIn[0].x)
    {
        if (ParticleIn[i].age > 0)
        {
            uint index = 0;
            InterlockedAdd(ParticleCountOut[0].x, 1, index);
            ParticleOut[index] = ParticleIn[i];
        }
    }	
    AllMemoryBarrier();

    currentParticleAmount = ParticleCountOut[0].x;
    uint rand = i + (uint)mRandom;
    
    float particlesToEmit = (float)mEmitRate * mDeltaTime;
    if ((float)currentParticleAmount + particlesToEmit >= (float)mMaxParticles)
    {
        particlesToEmit = (float)(mMaxParticles - currentParticleAmount);
    }
    const float totalThreads = (float)(mNumdispatch * numThreads);
    float particlesToEmitPerThread = particlesToEmit / totalThreads;
	
    if (particlesToEmitPerThread <= 1.0)
    {
        if (particlesToEmitPerThread < 0.000001f)
        {
            return;
        }

        const float numSpawningThreads = totalThreads * particlesToEmitPerThread;
        if (i <= floor(numSpawningThreads))
        {
            SpawnParticle(rand);
        }
    }
    else
    {
        const uint numParticlesPerThread = (uint)floor(particlesToEmitPerThread);
        const uint divCutoff = fmod(particlesToEmitPerThread, 1.0) * totalThreads;
        const uint endEmit = (numParticlesPerThread + (i == 0 ? divCutoff : 0));
        for (uint i = 0; i < endEmit; i++)
        {
            SpawnParticle(rand);
        }
    }
}

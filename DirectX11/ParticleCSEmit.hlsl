#include "Particle.hlsli"

[numthreads(numThreads, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    int i = DTid.x;
    
    if (i == 0)
    {
        uint val = 0;
        InterlockedExchange(ParticleCountOut[0].x, 0, val);
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

    int currentParticleAmount = ParticleCountOut[0].x;
    uint rand = i + (uint)mRandom;    
    float particlesToEmit = (float)mEmitRate * mDeltaTime;
    const float totalThreads = (float) (mNumdispatch * numThreads);
    
    if ((float)currentParticleAmount + particlesToEmit >= (float)mMaxParticles)
    {
        particlesToEmit = (float)(mMaxParticles - currentParticleAmount);
    }
    
    float particlesToEmitPerThread = particlesToEmit / totalThreads;
	
    if (particlesToEmitPerThread <= 1.0)
    {
        if (particlesToEmitPerThread >= 0.000001f && i <= floor(particlesToEmit))
        {
            SpawnParticle(rand);
        }
    }
    else
    {
        const uint numParticlesPerThread = (uint)floor(particlesToEmitPerThread);
        const uint divCutoff = fmod(particlesToEmitPerThread, 1.0) * totalThreads;
        const uint endEmit = numParticlesPerThread + (i == 0 ? divCutoff : 0);
        for (uint n = 0; n < endEmit; n++)
        {
            SpawnParticle(rand);
        }
    }
}

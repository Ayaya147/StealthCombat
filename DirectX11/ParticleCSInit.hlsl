#include "Particle.hlsli"

[numthreads(numThreads, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint particlesToEmit = clamp(mNewParticles, 0, mMaxParticles);

    ParticleOut[DTid.x].age = 0.0f;
    ParticleOut[DTid.x].color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    ParticleOut[DTid.x].position = float4(0.0f, 0.0f, 0.0f, 0.0f);
    ParticleOut[DTid.x].scale = 0.0f;
    ParticleOut[DTid.x].velocity = float4(1.0f, 1.0f, 1.0f, 0.0f);
	
    uint rand = DTid.x + 512;

    if (DTid.x <= particlesToEmit)
    {
        SpawnParticle(rand);
    }
}

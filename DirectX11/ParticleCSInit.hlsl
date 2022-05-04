#include "Particle.hlsli"

[numthreads(numThreads, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint particlesToEmit = clamp(_newParticles, 0, _maxParticles);

    ParticleOut[DTid.x].age = 0;
    ParticleOut[DTid.x].color = float4(1, 1, 1, 1);
    ParticleOut[DTid.x].position = float4(0, 0, 0, 0);
    ParticleOut[DTid.x].scale = 0;
    ParticleOut[DTid.x].velocity = float4(1, 1, 1, 0);
	
    uint rand = DTid.x + 512;

    if (DTid.x <= particlesToEmit)
    {
        SpawnParticle(rand);
    }
}
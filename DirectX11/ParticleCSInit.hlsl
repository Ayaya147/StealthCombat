#include "Particle.hlsli"

[numthreads(numThreads, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint i = DTid.x;

    ParticleOut[i].age = 0.0f;
    ParticleOut[i].color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    ParticleOut[i].position = float4(0.0f, 0.0f, 0.0f, 0.0f);
    ParticleOut[i].scale = 0.0f;
    ParticleOut[i].velocity = float4(1.0f, 1.0f, 1.0f, 0.0f);
}

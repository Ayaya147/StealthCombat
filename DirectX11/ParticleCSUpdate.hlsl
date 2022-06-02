#include "Particle.hlsli"

[numthreads(numThreads, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    int i = DTid.x;
    
    if (i == 0)
    {
        uint val = 0;
        InterlockedExchange(ParticleCountOut[0], 0, val);
    }
    AllMemoryBarrier();
    
    if (i < ParticleCountIn[0].x && ParticleIn[i].age > 0.0f)
    {
        InterlockedAdd(ParticleCountOut[0].x, 1);

        ParticleOut[i] = ParticleIn[i];
        
        float3 currentPos = ParticleIn[i].position.xyz;
        float3 oldVelocity = ParticleIn[i].velocity.xyz;		
        float3 direction = normalize(mPosition.xyz - currentPos);
        float distSq = pow(distance(mPosition.xyz, currentPos), 2.0f);		
        float accelScalar = clamp((mGravity * mMass) / (distSq + 0.01f), 0.0f, 750.0f);
        float3 acceleration = direction * accelScalar;
        float3 halfStepVel = oldVelocity + 0.5f * mDeltaTime * acceleration;
        float3 newVelocity = halfStepVel + 0.5f * mDeltaTime * acceleration;

        ParticleOut[i].position += float4(halfStepVel * mDeltaTime, 0.0f);
        ParticleOut[i].velocity = float4(newVelocity, 0.0);
        ParticleOut[i].age -= mDeltaTime;
    }
}

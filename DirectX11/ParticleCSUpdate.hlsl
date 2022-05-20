#include "Particle.hlsli"

float3 GetAcceleration(in float3 movement, in float acceleration, in float3 velocity, in float friction, in float mass)
{
    return movement * acceleration - (friction * mass * velocity);
}

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
        float3 cPos = ParticleIn[i].position.xyz;
        float3 oldVelocity = (ParticleIn[i].velocity.xyz);
		
        float3 dirToCenter = normalize(mPosition.xyz - cPos);
        float dist = distance(mPosition.xyz, cPos);
        dist *= dist;
		
        float force = clamp((mGravity * mMass) / (dist + 0.01f), 0.0f, 750.0f);
        float3 movementVector = dirToCenter;

        float3 acceleration = GetAcceleration(movementVector, force, oldVelocity, 0.0f, mMass);
        float3 halfStepVel = oldVelocity + 0.5f * mDeltaTime * acceleration;

        ParticleOut[i].position += float4(halfStepVel * mDeltaTime, 0.0f);
		
        acceleration = GetAcceleration(movementVector, force, halfStepVel, 0.0f, mMass);
        float3 newVelocity = halfStepVel + 0.5f * mDeltaTime * acceleration;

        ParticleOut[i].velocity = float4(newVelocity, 0.0);
        ParticleOut[i].age -= mDeltaTime;
    }
}

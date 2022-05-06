#include "Particle.hlsli"

float3 GetAcceleration(in float3 movement, in float acceleration, in float3 velocity, in float friction, in float mass)
{
    float3 force = movement * acceleration;
    return force - (friction * mass * velocity);
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
    
    if (i < ParticleCountIn[0].x && ParticleIn[i].age > 0.0)
    {
        InterlockedAdd(ParticleCountOut[0].x, 1);

        ParticleOut[i] = ParticleIn[i];
        float3 cPos = ParticleIn[i].position.xyz;
        float3 oldVelocity = (ParticleIn[i].velocity.xyz);
		
        float3 dirToCenter = normalize(_position.xyz - cPos);
        float dist = distance(_position.xyz, cPos);
        dist *= dist;
		
        float force = clamp((_gravity * _mass) / (dist + 0.01), 0, 750);
        float3 movementVector = dirToCenter;

        float3 acceleration = GetAcceleration(movementVector, force, oldVelocity, 0, _mass);
        float3 halfStepVel = oldVelocity + 0.5 * _deltaTime * acceleration;

        ParticleOut[i].position += float4(halfStepVel * _deltaTime, 0.0);
		
        acceleration = GetAcceleration(movementVector, force, halfStepVel, 0, _mass);
        float3 newVelocity = halfStepVel + 0.5 * _deltaTime * acceleration;

        ParticleOut[i].velocity = float4(newVelocity, 0.0);
        
        if (_random.z)
        {
            ParticleOut[i].age -= _deltaTime * 4.0f;
        }
        else
        {
            ParticleOut[i].age -= _deltaTime;
        }
    }
}

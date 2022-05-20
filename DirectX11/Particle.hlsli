static const uint numThreads = 512;

cbuffer SystemBuffer : register(b0)
{
    float _deltaTime;
    float _random;
};

cbuffer ParticleEmitter : register(b1)
{
    float4 _velocityMin, _velocityMax;
    float4 _positionMin, _positionMax;
    float4 _position;
    float _lifeTimeMin, _lifeTimeMax;
    float _scaleMin, _scaleMax;
    int _newParticles, _maxParticles;
    float _gravity, _mass;
    uint _emitRate, _numdispatch;
};

struct Particle
{
    float4 velocity;
    float4 color;
    float4 position;
    float age, scale;
};

StructuredBuffer<Particle> ParticleIn : register(t0);
StructuredBuffer<int> ParticleCountIn : register(t1);

RWStructuredBuffer<Particle> ParticleOut : register(u0);
RWStructuredBuffer<int> ParticleCountOut : register(u1);

float wang_hash(inout uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return (float) seed / (float) 0xFFFFFFFF;
}

void SpawnParticle(inout uint rand)
{
    uint index = 0;
    InterlockedAdd(ParticleCountOut[0].x, 1, index);

    ParticleOut[index].age = lerp(_lifeTimeMin, _lifeTimeMax, wang_hash(rand));
    ParticleOut[index].velocity = (float4(lerp(_velocityMin.x, _velocityMax.x, wang_hash(rand)), lerp(_velocityMin.y, _velocityMax.y, wang_hash(rand)), lerp(_velocityMin.z, _velocityMax.z, wang_hash(rand)), 0));
    ParticleOut[index].scale = lerp(_scaleMin, _scaleMax, wang_hash(rand));
    ParticleOut[index].position = _position + float4(lerp(_positionMin.x, _positionMax.x, wang_hash(rand)), lerp(_positionMin.y, _positionMax.y, wang_hash(rand)), lerp(_positionMin.z, _positionMax.z, wang_hash(rand)), 0);
    ParticleOut[index].color = float4(lerp(0.7, 1, wang_hash(rand)), lerp(0.45, 0.75, wang_hash(rand)), lerp(0.55, 0.85, wang_hash(rand)), 1);
}

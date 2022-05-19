#define DEBUG_MODE 0
#define PI 3.1415926535897932384626433832795
#define RandomTextureSize 1024.0

static const uint numThreads = 512;

cbuffer SystemBuffer : register(b0)
{
    float _screenWidth;
    float _screenHeight;
    float _deltaTime;
    float _time;
    float _fps;
    float3 _random;
};

cbuffer ParticleEmitter : register(b1)
{
    float _lifeTimeMin, _lifeTimeMax, _scaleMin, _scaleMax; //16
    int _newParticles = 0, _maxParticles = 0; //8 
    float _gravity, _mass; //8
    uint _emitRate, _numdispatch, _d2PE, _d3PE; // 8
    float4 _velocityMin, _velocityMax; //32
    float4 _positionMin, _positionMax; //32
    float4 _position; //16
};

struct Particle
{
    float age, scale, d1, d2;
    float4 velocity;
    float4 color;
    float4 position;
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

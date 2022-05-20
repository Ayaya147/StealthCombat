cbuffer CBuf : register(b0)
{
    float4x4 mViewMatrix;
    float4x4 mProjectionMatrix;
};

struct Particle
{
    float4 velocity;
    float4 color;
    float4 position;
    float age;
    float scale;
};

StructuredBuffer<Particle> ParticleIn : register(t0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 tc : TEXCOORD0;
    float scale : SCALE;
};

PS_INPUT main(uint vIndex : SV_VertexID)
{
    PS_INPUT output;
    output.pos = mul(float4(ParticleIn[vIndex].position.xyz, 1.0f), mul(mViewMatrix, mProjectionMatrix));
    output.col = ParticleIn[vIndex].color;
    output.tc = float2(0.5f, 0.5f);
    output.scale = ParticleIn[vIndex].scale;
    return output;
};

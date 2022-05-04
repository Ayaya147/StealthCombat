#define DEBUG_MODE 0
#pragma pack_matrix( row_major )
#define PI 3.1415926535897932384626433832795

cbuffer CameraBuffer : register(b0)
{
    float4x4 _viewMatrix;
    float4x4 _projectionMatrix;
    float4x4 _invProjectionMatrix;
    float4x4 _invViewMatrix;
    float4 _cameraPosition;
    float4 _cameraDir;
};

struct Particle
{
    float age, scale, d1, d2;
    float3 velocity;
    float d3;
    float4 color;
    float4 position;
};

StructuredBuffer<Particle> ParticleIn : register(t0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
    float scale : SCALE;
};

PS_INPUT main(uint vIndex : SV_VertexID)
{
    PS_INPUT output;
    output.pos = mul(float4(ParticleIn[vIndex].position.xyz, 1.0), mul(_viewMatrix, _projectionMatrix));
    output.col = ParticleIn[vIndex].color;
    output.uv = float2(0.5, 0.5);
    output.scale = ParticleIn[vIndex].scale;
    return output;
};

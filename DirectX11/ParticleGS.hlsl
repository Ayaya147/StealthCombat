#define DEBUG_MODE 0
#pragma pack_matrix( row_major )
#define PI 3.1415926535897932384626433832795

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};
cbuffer ObjectBuffer : register(b0)
{
    float4x4 _modelMatrix; //64
    int _misc0, _misc1, _misc2, _misc3;
};
cbuffer CameraBuffer : register(b1)
{
    float4x4 _viewMatrix;
    float4x4 _projectionMatrix;
    float4x4 _invProjectionMatrix;
    float4x4 _invViewMatrix;
    float4 _cameraPosition;
    float4 _cameraDir;
};
cbuffer ConstantBuffer : register(b2)
{
    float _screenWidth;
    float _screenHeight;
    float _visualType;
    float _time;
    float _fps;
    float3 _dummyCB;
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

[maxvertexcount(6)]
void main(point PS_INPUT input[1], inout TriangleStream<PS_INPUT> outputStream)
{
    float posX = 0.5 * input[0].scale;
    float posY = posX * (_screenWidth / _screenHeight);
    PS_INPUT output;
    output.scale = input[0].scale;
    output.col = input[0].col;
    output.uv = float2(0, 0);
    output.pos = input[0].pos + float4(-posX, posY, 0, 0);
    outputStream.Append(output);
    output.uv = float2(1, 0);
    output.pos = input[0].pos + float4(posX, posY, 0, 0);
    outputStream.Append(output);
    output.uv = float2(0, 1);
    output.pos = input[0].pos + float4(-posX, -posY, 0, 0);
    outputStream.Append(output);
    output.uv = float2(1, 1);
    output.pos = input[0].pos + float4(posX, -posY, 0, 0);
    outputStream.Append(output);
}

struct VS_IN_PARTICLE
{
    float4 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
    uint InstanceID : SV_InstanceID;
};

struct PS_IN_PARTICLE
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

cbuffer CBuf : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

// DrawInstanceópç¿ïW
StructuredBuffer<float3> Position : register(t2);

void main(in VS_IN_PARTICLE In, out PS_IN_PARTICLE Out)
{

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    In.Position.xyz += Position[In.InstanceID];

    Out.Position = mul(In.Position, wvp);
    Out.WorldPosition = mul(In.Position, World);
    Out.TexCoord = In.TexCoord;

}
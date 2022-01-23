cbuffer CBuf : register(b0)
{
    matrix worldTransform;
    matrix viewProj;
};

struct VSOut
{
    float3 worldPos : Position;
    float3 worldNor : Normal;
    float3 worldTan : Tangent;
    float3 worldBitan : Bitangent;
    float2 tc0 : Tex0Coord;
    float2 tc1 : Tex1Coord;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float2 tc0 : Tex0Coord, float2 tc1 : Tex1Coord)
{
    VSOut vso;
    vso.worldPos = (float3)mul(float4(pos, 1.0f), worldTransform);
    vso.worldNor = mul(n, (float3x3) worldTransform);
    vso.worldTan = mul(float3(1.0f, 0.0f, 0.0f), (float3x3) worldTransform);
    vso.worldBitan = mul(float3(0.0f, 0.0f, 1.0f), (float3x3) worldTransform);
    vso.tc0 = tc0;
    vso.tc1 = tc1;
    vso.pos = mul(float4(vso.worldPos, 1.0f), viewProj);

    return vso;
}

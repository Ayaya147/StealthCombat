cbuffer CBuf : register(b0)
{
    matrix worldTransform;
    matrix viewProj;
};

struct VSOut
{
    float3 worldPos : Position;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float2 tc : TexCoord)
{
    VSOut vso;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), worldTransform);
    vso.pos = mul(float4(vso.worldPos, 1.0f), viewProj);
    
    return vso;
}

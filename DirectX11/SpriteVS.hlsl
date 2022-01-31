cbuffer CBuf : register(b0)
{
    matrix mWorldTransform;
    matrix mViewProj;
};

struct VSOut
{
    float2 tc : TexCoord;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tc : TexCoord)
{
    VSOut vso;
    float3 worldPos = (float3) mul(float4(pos, 1.0f), mWorldTransform);
    vso.pos = mul(float4(worldPos, 1.0f), mViewProj);
    vso.tc = tc;
    
    return vso;
}

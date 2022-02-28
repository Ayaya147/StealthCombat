cbuffer CBuf : register(b0)
{
    matrix mWorldTransform;
    matrix mViewProj;
};

struct VSOut
{
    float4 col : COLOR;
    float2 tc : TEXCOORD;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float4 col : COLOR, float2 tc : TEXCOORD)
{
    VSOut vso;
    float3 worldPos = (float3) mul(float4(pos, 1.0f), mWorldTransform);
    vso.pos = mul(float4(worldPos, 1.0f), mViewProj);
    vso.col = col;
    vso.tc = tc;
    
    return vso;
}

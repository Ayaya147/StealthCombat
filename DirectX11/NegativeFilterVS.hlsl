cbuffer CBuf : register(b0)
{
    matrix mWorldTransform;
    matrix mViewProj;
};

struct VSOut
{
    float2 tc : TEXCOORD;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float3 n : NORMAL, float2 tc : TEXCOORD)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), mul(mWorldTransform, mViewProj));
    vso.tc = tc;
    
    return vso;
}

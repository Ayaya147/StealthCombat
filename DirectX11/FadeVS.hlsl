cbuffer CBuf : register(b0)
{
    matrix mWorldTransform;
    matrix mViewProj;
};

struct VSOut
{
    float4 col : COLOR;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float4 col : COLOR)
{
    VSOut vso;
    float3 worldPos = (float3) mul(float4(pos, 1.0f), mWorldTransform);
    vso.pos = mul(float4(worldPos, 1.0f), mViewProj);
    vso.col = col;
    
    return vso;
}

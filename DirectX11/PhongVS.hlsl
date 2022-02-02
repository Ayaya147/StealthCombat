cbuffer CBuf : register(b0)
{
	matrix mWorldTransform;
	matrix mViewProj;
};

struct VSOut
{
    float3 worldPos : POSITION;
    float3 worldNor : NORMAL;
    float2 tc : TEXCOORD;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float3 n : NORMAL, float2 tc : TEXCOORD)
{
	VSOut vso;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), mWorldTransform);
    vso.worldNor = mul(n, (float3x3) mWorldTransform);
    vso.pos = mul(float4(vso.worldPos, 1.0f), mViewProj);
    vso.tc = tc;
    
	return vso;
}

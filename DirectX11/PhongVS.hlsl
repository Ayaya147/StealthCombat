cbuffer CBuf : register(b0)
{
	matrix mWorldTransform;
	matrix mViewProj;
};

struct VSOut
{
	float3 worldPos : Position;
	float3 worldNor : Normal;
    float2 tc : TexCoord;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float2 tc : TexCoord)
{
	VSOut vso;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), mWorldTransform);
    vso.worldNor = mul(n, (float3x3) mWorldTransform);
    vso.pos = mul(float4(vso.worldPos, 1.0f), mViewProj);
    vso.tc = tc;
    
	return vso;
}

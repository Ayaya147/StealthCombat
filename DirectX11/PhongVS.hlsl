cbuffer CBuf : register(b0)
{
	matrix worldTransform;
	matrix viewProj;
};

struct VSOut
{
	float3 worldPos : Position;
	float3 worldNor : Normal;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal)
{
	VSOut vso;
    vso.worldPos = (float3)mul(float4(pos, 1.0f), worldTransform);
    vso.worldNor = mul(n, (float3x3) worldTransform);
    vso.pos = mul(float4(pos, 1.0f), mul(worldTransform, viewProj));
	return vso;
}

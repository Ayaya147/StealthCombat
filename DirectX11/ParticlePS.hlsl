
struct PS_IN_PARTICLE
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN_PARTICLE In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
}

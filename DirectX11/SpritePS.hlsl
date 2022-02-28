SamplerState splr : register(s0);
Texture2D tex : register(t0);

float4 main(float4 col : COLOR, float2 tc : TEXCOORD) : SV_TARGET
{
    return col * tex.Sample(splr, tc);
}

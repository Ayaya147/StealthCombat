SamplerState splr : register(s0);
Texture2D tex : register(t0);

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
    float3 color = tex.Sample(splr, tc).rgb;
    
    return float4((float3) 1.0f - color, 1.0f);
}

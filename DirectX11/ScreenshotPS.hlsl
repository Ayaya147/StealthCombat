SamplerState splr : register(s0);
Texture2D tex : register(t0);

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
    float range = 0.01;
    
    if (tc.x <= range || tc.x >= 1.0f - range || tc.y <= range || tc.y >= 1.0f - range)
    {
        return float4(1.0f, 1.0f, 1.0f, 0.5f);
    }
    else
    {
        return float4(tex.Sample(splr, tc).rgb, 1.0f);
    }
}

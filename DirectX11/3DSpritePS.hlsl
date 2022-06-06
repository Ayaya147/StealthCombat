SamplerState splr : register(s0);
Texture2D tex : register(t0);

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
    if (tc.x <= 0.005f || tc.x >= 0.995f || tc.y <= 0.005f || tc.y >= 0.995f)
    {
        return float4(1.0f, 1.0f, 1.0f, 0.5f);
    }
    else
    {
        return float4(tex.Sample(splr, tc).rgb, 1.0f);
    }
}

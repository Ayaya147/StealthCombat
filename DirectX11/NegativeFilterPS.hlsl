SamplerState splr : register(s0);
Texture2D tex : register(t0);

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
    float4 color = (float4) 1.0f;
    float range = 0.01;
    
    if (tc.x <= range || tc.x >= 1.0f - range || tc.y <= range || tc.y >= 1.0f - range)
    {
        color = float4(0.0f, 0.0f, 0.0f, 0.5f);
    }
    else
    {
        color = float4(tex.Sample(splr, tc).rgb, 0.0f);
    }
    
    return (float4) 1.0f - color;
}

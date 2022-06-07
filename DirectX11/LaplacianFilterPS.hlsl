SamplerState splr : register(s0);
Texture2D tex : register(t0);

static const float weight[] =
{
    1.0f, 1.0f, 1.0f,
    1.0f, -8.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
};

float4 main(float2 tc : TEXCOORD, float4 pos : SV_POSITION) : SV_TARGET
{
    float w = 0.0f;
    float h = 0.0f;
    tex.GetDimensions(w, h);
    
    float invW = 1.0f / w;
    float invH = 1.0f / h;
    
    float4 c[9];
    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            c[(j + 1) * 3 + i + 1] = tex.Sample(splr, tc + float2(invW * i, invH * j));
        }
    }

    float4 color = (float4) 0.0f;
    for (int i = 0; i < 9; i++)
    {
        color += c[i] * weight[i];
    }
    color.a = 1.0f;
    
    float range = 0.01;
    if (tc.x <= range || tc.x >= 1.0f - range || tc.y <= range || tc.y >= 1.0f - range)
    {
        color = float4(1.0f, 1.0f, 1.0f, 0.5f);
    }
    
    return color;
}

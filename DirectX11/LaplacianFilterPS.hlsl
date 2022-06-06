SamplerState splr : register(s0);
Texture2D tex : register(t0);

float4 main(float2 tc : TEXCOORD, float4 pos : SV_POSITION) : SV_TARGET
{
    float invW = 0.0f;
    float invH = 0.0f;
    float w = 0.0f;
    float h = 0.0f;
    
    tex.GetDimensions(w, h);
    invW = 1.0f / w;
    invH = 1.0f / h;
    
    float4 c0 = tex.Sample(splr, tc + float2(invW *  0, invH *  0));
    float4 c1 = tex.Sample(splr, tc + float2(invW *  0, invH * -1));
    float4 c2 = tex.Sample(splr, tc + float2(invW *  0, invH *  1));
    float4 c3 = tex.Sample(splr, tc + float2(invW * -1, invH *  0));
    float4 c4 = tex.Sample(splr, tc + float2(invW *  1, invH *  0));
    float4 c5 = tex.Sample(splr, tc + float2(invW * -1, invH * -1));
    float4 c6 = tex.Sample(splr, tc + float2(invW *  1, invH * -1));
    float4 c7 = tex.Sample(splr, tc + float2(invW * -1, invH *  1));
    float4 c8 = tex.Sample(splr, tc + float2(invW *  1, invH *  1));

    float4 color = c8 + c7 + c6 + c5 + c4 + c3 + c2 + c1 + (c0 * -8.0);
    return float4(color.rgb, 1.0f);
}

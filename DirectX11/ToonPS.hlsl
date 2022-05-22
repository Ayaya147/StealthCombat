cbuffer CBuf : register(b0)
{
    float3 mCameraPos;
    float3 mDirection;
    float3 mAmbientLight;
    float3 mDiffuseColor;
    float3 mSpecColor;
};

SamplerState splr : register(s0);
Texture2D tex : register(t0);

float4 main(float3 worldPos : POSITION, float3 worldNor : NORMAL, float2 tc : TEXCOORD) : SV_TARGET
{
    float3 normal = normalize(worldNor);
    float3 lightDir = normalize(-mDirection);
    float nDotL = dot(normal, lightDir);
    
    float p = nDotL;
    p = p * 0.5f + 0.5f;
    p = p * p;
    
    float3 colorToon;
    if (p < 0.33f)
    {
        float c = 0.2f;
        colorToon = float3(c, c, c);
    }
    else if (p < 0.66f)
    {
        float c = 0.5f;
        colorToon = float3(c, c, c);
    }
    else
    {
        float c = 1.0f;
        colorToon = float3(c, c, c);
    }

    float4 color = tex.Sample(splr, tc);    
    color.rgb *= colorToon.rgb;
    
    return color;
}

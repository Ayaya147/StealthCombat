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
    float3 viewDir = normalize(mCameraPos - worldPos);
    float3 reflectDir = normalize(reflect(-lightDir, normal));
    float nDotL = dot(normal, lightDir);
    
    float3 colorToon = (float3) 0.0f;
    float3 specular = (float3) 0.0f;

    if (nDotL < 0.0f)
    {
        float c = 0.01f;
        colorToon = float3(c, c, c);
    }
    else if (nDotL < 0.33f)
    {
        float c = 0.2f;
        colorToon = float3(c, c, c);
        specular = mSpecColor * pow(max(0.0f, dot(reflectDir, viewDir)), 50.0f);
    }
    else if (nDotL < 0.66f)
    {
        float c = 0.5f;
        colorToon = float3(c, c, c);
        specular = mSpecColor * pow(max(0.0f, dot(reflectDir, viewDir)), 50.0f);
    }
    else
    {
        float c = 1.0f;
        colorToon = float3(c, c, c);
        specular = mSpecColor * pow(max(0.0f, dot(reflectDir, viewDir)), 50.0f);
    }

    float4 color = saturate(tex.Sample(splr, tc) * float4(mAmbientLight + mDiffuseColor * nDotL, 1.0f) + float4(specular, 0.0f));
    color.rgb *= colorToon.rgb;
    
    return color;
}

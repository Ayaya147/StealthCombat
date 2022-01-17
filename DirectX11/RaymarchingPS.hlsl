cbuffer CBuf : register(b0)
{
    float3 cameraPos;
    float3 ambientLight;
};

cbuffer DirectLightCBuf : register(b1)
{
    float3 direction;
    float diffuseColor;
    float specColor;
};

cbuffer ObjectCBuf : register(b2)
{
    matrix worldInverse;
};

SamplerState splr : register(s0);
Texture2D tex : register(t0);

//float noiseScale = 8.0f;
//float radius = 0.7f;
//float absorption = 43.0f;
//float opacity = 100.0f;
//float absorptionLight = 14.8f;
//float opacityLight = 84.0f;
//int loopLight = 6;
//float lightStepScale = 0.5f;
//float4 lightColor0 = float4(1.0f, 1.0f, 1.0f, 1.0f);
static const float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
static const int  loop = 32;
static const float intensity = 0.1f;
static const float value = 10.0f;

float densityFunction(float3 p)
{
    return value - length(p);
}

float4 main(float3 worldPos : Position) : SV_Target
{    
    float3 worldDir = normalize(worldPos - cameraPos);
    
    float3 localPos = (float3) mul(worldInverse, float4(worldPos, 1.0f));
    float3 localDir = (float3) mul(worldInverse, float4(worldDir, 1.0f));
    
    float step = 1.0f / loop;
    float3 localStep = localDir * step;
    
    float alpha = 0.0f;
    
    for (int i = 0; i < loop; i++)
    {
        float density = densityFunction(localPos);
        
        if (density > 0.001f)
        {
            alpha += (1.0f - alpha) * density * intensity;
        }
        
        localPos += localStep;
        
        if (!all(max(value - abs(localPos), 0.0f)))
        {
            break;
        }
    }
    
    float4 colorOut = color;
    colorOut.a *= alpha;
    return colorOut;
}

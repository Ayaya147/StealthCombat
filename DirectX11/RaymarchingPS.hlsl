//cbuffer CBuf : register(b0)
//{
//    float3 cameraPos;
//    float3 ambientLight;
//};

//cbuffer DirectLightCBuf : register(b1)
//{
//    float3 direction;
//    float diffuseColor;
//    float specColor;
//};

//cbuffer ObjectCBuf : register(b2)
//{
//    matrix worldInverse;
//};

//SamplerState splr : register(s0);
//Texture2D tex : register(t0);

//static const float4 cloudColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
//static const int  loop = 464;
//static const float size = 10.0f;
//static const float noiseScale = 1.0f;
//static const float radius = 1.0f;
//static const float absorption = 50.0f;
//static const float opacity = 100.0f;

//static const float absorptionLight = 14.8f;
//static const float opacityLight = 84.0f;
//static const int loopLight = 6;
//static const float lightStepScale = 0.5f;
//static const float4 lightColor0 = float4(1.0f, 1.0f, 1.0f, 1.0f);

//float hash(float n)
//{
//    return frac(sin(n) * 43758.5453f);
//}

//float noise(float3 x)
//{
//    float3 p = floor(x);
//    float3 f = frac(x);
//    f = f * f * (3.0f - 2.0f * f);
    
//    float n = p.x + p.y * 57.0f + p.z * 113.0f;
//    float res = lerp(lerp(lerp(hash(n +   0.0f), hash(n +   1.0f), f.x), 
//                          lerp(hash(n +  57.0f), hash(n +  58.0f), f.x), f.y),
//                     lerp(lerp(hash(n + 113.0f), hash(n + 114.0f), f.x), 
//                          lerp(hash(n + 170.0f), hash(n + 141.0f), f.x), f.y),f.z);
    
//    return res;
//}

//float fbm(float3 p)
//{
//    float3x3 m = float3x3(0.0f, 0.8f, 0.6f, -0.8f, 0.36f, -0.48f, -0.6f, -0.48f, 0.64f);    
//    float f = 0.0f;
//    f += 0.5f * noise(p);
//    p = mul(m, p) * 2.02f;
//    f += 0.3f * noise(p);
//    p = mul(m, p) * 2.03f;
//    f += 0.2f * noise(p);
    
//    return f;
//}

//float densityFunction(float3 p)
//{
//    return fbm(p * noiseScale) * size - length(p / radius);
//}

//float4 main(float3 worldPos : Position) : SV_Target
//{    
//    float step = 1.0f / loop;

//    float3 worldDir = normalize(worldPos - cameraPos);
    
//    float3 localPos = (float3) mul(worldInverse, float4(worldPos, 1.0f));
//    float3 localDir = (float3) mul(worldInverse, float4(worldDir, 1.0f));
//    //float3 localDir = normalize(mul((float3x3) worldInverse, worldDir));

//    float3 localStep = localDir * step;
//    float jitter = hash(localPos.x + localPos.y * 10.0f + localPos.z * 100.0f);
//    localPos += jitter * localStep;
    
//    //float lightStep = 1.0f / loopLight;
//    //float3 localLightDir = (float3) mul(worldInverse, float4(direction, 1.0f));
//    //float3 localLightStep = localLightDir * lightStep * lightStepScale;
    
//    float4 color = float4(cloudColor.rgb, 0.0f);
//    float transmittance = 1.0f;    
    
//    for (int i = 0; i < loop; i++)
//    {
//        float density = densityFunction(localPos);
        
//        if (density > 0.0f)
//        {
//            float d = density * step;
//            transmittance *= 1.0f - d * absorption;
            
//            if (transmittance < 0.01f)
//            {
//                break;
//            }
            
//            //float transmittanceLight = 1.0f;
//            //float3 lightPos = localPos;
            
//            //for (int j = 0; j < loopLight; j++)
//            //{
//            //    float densityLight = densityFunction(lightPos);
                
//            //    if (densityLight>0.0f)
//            //    {
//            //        float dl = densityLight * lightStep;
//            //        transmittanceLight *= 1.0f - dl * absorptionLight;
                    
//            //        if(transmittanceLight<0.01f)
//            //        {
//            //            transmittanceLight = 0.0f;
//            //            break;
//            //        }
//            //    }
                
//            //    lightPos += localLightStep;
//            //}
            
//            color.a += opacity * d * transmittance;
//            //color.rgb += (lightColor0 * (opacityLight * d * transmittance * transmittanceLight)).rgb;
//        }
        
//        color = clamp(color, 0.0f, 1.0f);
//        localPos += localStep;
        
//        if (!all(max(size - abs(localPos), 0.0f)))
//        {
//            break;
//        }
//    }
    
//    return color;
//}

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

static const float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
static const int loop = 32;
static const float intensity = 0.1f;
static const float value = 0.5f;

float densityFunction(float3 p)
{
    return value - length(p);
}

float4 main(float3 worldPos : Position) : SV_Target
{
    float3 worldDir = normalize(worldPos - cameraPos);
    
    float3 localPos = (float3) mul(worldInverse, float4(worldPos, 1.0f));
    //float3 localDir = (float3) mul(worldInverse, float4(worldDir, 1.0f));
    float3 localDir = normalize(mul((float3x3) worldInverse, worldDir));

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

cbuffer CBuf : register(b0)
{
    float3 cameraPos;
    float3 ambientLight;
};

cbuffer DirectLightCBuf : register(b1)
{
    float3 direction;
    float3 diffuseColor;
    float3 specColor;
};

cbuffer ObjectCBuf : register(b2)
{
    matrix worldInverse;
};

static const float4 cloudColor = float4(0.8f, 0.8f, 0.8f, 1.0f);
static const int loop = 32;
static const float noiseScale = 5.0f;
static const float radius = 0.6f;
static const float absorption = 50.0f;
static const float opacity = 100.0f;

static const float absorptionLight = 60.0f;
static const float opacityLight = 80.0f;
static const float lightStepScale = 0.4f;
static const int loopLight = 5;

float hash(float n)
{
    return frac(sin(n) * 43758.5453f);
}

float noise(float3 x)
{
    float3 p = floor(x);
    float3 f = frac(x);
    f = f * f * (3.0f - 2.0f * f);
    
    float n = p.x + p.y * 57.0f + p.z * 113.0f;
    float res = lerp(lerp(lerp(hash(n +   0.0f), hash(n +   1.0f), f.x),
                          lerp(hash(n +  57.0f), hash(n +  58.0f), f.x), f.y),
                     lerp(lerp(hash(n + 113.0f), hash(n + 114.0f), f.x),
                          lerp(hash(n + 170.0f), hash(n + 141.0f), f.x), f.y), f.z);
    
    return res;
}

float fbm(float3 p)
{
    float3x3 m = float3x3(
         0.00f,  0.80f,  0.60f,
        -0.80f,  0.36f, -0.48f,
        -0.60f, -0.48f,  0.64f
    );
    
    float f = 0.0f;
    f += 0.5f * noise(p); p = mul(m, p) * 2.02f;
    f += 0.3f * noise(p); p = mul(m, p) * 2.03f;
    f += 0.2f * noise(p);
    
    return f;
}

float densityFunction(float3 p)
{
    return fbm(p * noiseScale) - length(p / radius);
}

float4 main(float3 worldPos : Position) : SV_Target
{
    float step = 1.0f / loop;

    float3 worldDir = normalize(worldPos - cameraPos);
    
    float3 localPos = (float3) mul(worldInverse, float4(worldPos, 1.0f));
    float3 localDir = normalize(mul((float3x3) worldInverse, worldDir));
    float3 localStep = localDir * step;
    float jitter = hash(localPos.x + localPos.y * 10.0f + localPos.z * 100.0f);
    localPos += jitter * localStep;
    
    float lightStep = 1.0f / loopLight;
    float3 localLightDir = normalize(mul((float3x3) worldInverse, -direction));
    float3 localLightStep = localLightDir * lightStep * lightStepScale;
    
    float4 color = float4(cloudColor.rgb, 0.0f);
    float transmittance = 1.0f;
    
    for (int i = 0; i < loop; i++)
    {
        float density = densityFunction(localPos);
        
        if (density > 0.0f)
        {
            float d = density * step;
            transmittance *= 1.0f - d * absorption;
            
            if (transmittance < 0.01f)
            {
                break;
            }
            
            float transmittanceLight = 1.0f;
            float3 lightPos = localPos;
            
            for (int j = 0; j < loopLight; j++)
            {
                float densityLight = densityFunction(lightPos);
                
                if ( densityLight > 0.0f)
                {
                    float dl = densityLight * lightStep;
                    transmittanceLight *= 1.0f - dl * absorptionLight;
                    
                    if ( transmittanceLight < 0.01f)
                    {
                        transmittanceLight = 0.0f;
                        break;
                    }
                }
                
                lightPos += localLightStep;
            }
            
            color.a += cloudColor.a * (opacity * d * transmittance);
            color.rgb += diffuseColor * (opacityLight * d * transmittance * transmittanceLight);
        }
        
        color = clamp(color, 0.0f, 1.0f);
        localPos += localStep;
        
        if (!all(max(0.5f - abs(localPos), 0.0f)))
        {
            break;
        }
    }

    return color;
}

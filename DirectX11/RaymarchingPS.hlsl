cbuffer CBuf : register(b0)
{
    float3 mCameraPos;
    float3 mAmbientLight;
    float3 mDirection;
    float3 mDiffuseColor;
    float3 mSpecColor;
};

cbuffer ObjectCBuf : register(b1)
{
    matrix mWorldInverse;
};

cbuffer CloudCBuf : register(b2)
{
    float3 mCloudColor;
    int mLoop;
    float mNoiseScale;
    float mRadius;
    int mAbsorption;
    int mOpacity;
    int mAbsorptionLight;
    int mOpacityLight;
    float mLightStepScale;
    int mLoopLight;
    float mTime;
};

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

float sphere(float3 pos, float radius)
{
    return length(pos) - radius;
}

float ellipsoid(float3 pos, float3 radius)
{
    float k0 = length(pos / radius);
    float k1 = length(pos / (radius * radius));
    return k0 * (k0 - 1.0f) / k1;
}

float torus(float3 pos, float2 radius)
{
    float2 r = float2(length(pos.xz) - radius.x, pos.y);
    return length(r) - radius.y;
}

float densityFunction(float3 p)
{
    return fbm(p * mNoiseScale) - sphere(p / mRadius, 0.0f);
    //return 0.5f - length(p / mRadius);
    //return fbm(p * mNoiseScale) - ellipsoid(p / mRadius, float3(0.2f, 0.05f, 0.1f));
}

float densityFunctionAnime(float3 p)
{
    float f = fbm(p * mNoiseScale);

    float d1 = -sphere(p, mRadius) + f * 0.3f;
    float d2 = -torus(p, float2(mRadius * 1.5f, 0.1f)) + f * 0.2f;
    float blend = 0.5f + 0.5f * sin(mTime * 2.0f);
    return lerp(d1, d2, blend);
}

float4 main(float3 worldPos : Position) : SV_Target
{
    float step = 1.0f / mLoop;

    float3 worldDir = normalize(worldPos - mCameraPos);
    
    float3 localPos = (float3) mul(mWorldInverse, float4(worldPos, 1.0f));
    float3 localDir = normalize(mul((float3x3) mWorldInverse, worldDir));
    float3 localStep = localDir * step;
    float jitter = hash(localPos.x + localPos.y * 10.0f + localPos.z * 100.0f + mTime);
    localPos += jitter * localStep;
    
    float3 invLocalDir = 1.0f / localDir;
    float3 t1 = (-0.5f - localPos) * invLocalDir;
    float3 t2 = (0.5f - localPos) * invLocalDir;
    float3 tmax3 = max(t1, t2);
    float2 tmax2 = min(tmax3.xx, tmax3.yz);
    float traverseDist = min(tmax2.x, tmax2.y);
    int loop = floor(traverseDist / step);
    
    float lightStep = 1.0f / mLoopLight;
    float3 localLightDir = normalize(mul((float3x3) mWorldInverse, -mDirection));
    float3 localLightStep = localLightDir * lightStep * mLightStepScale;
    
    float4 color = float4(mCloudColor, 0.0f);
    float transmittance = 1.0f;
    
    for (int i = 0; i < loop; i++)
    {
        float density = densityFunction(localPos);
        
        if (density > 0.0f)
        {
            float d = density * step;
            transmittance *= 1.0f - d * mAbsorption;
            
            if (transmittance < 0.01f)
            {
                break;
            }
            
            float transmittanceLight = 1.0f;
            float3 lightPos = localPos;
            
            for (int j = 0; j < mLoopLight; j++)
            {
                float densityLight = densityFunction(lightPos);
                
                if ( densityLight > 0.0f)
                {
                    float dl = densityLight * lightStep;
                    transmittanceLight *= 1.0f - dl * mAbsorptionLight;
                    
                    if ( transmittanceLight < 0.01f)
                    {
                        transmittanceLight = 0.0f;
                        break;
                    }
                }
                
                lightPos += localLightStep;
            }
            
            color.a += mCloudColor.x * (mOpacity * d * transmittance);
            color.rgb += mDiffuseColor * (mOpacityLight * d * transmittance * transmittanceLight);
        }
        
        color = clamp(color, 0.0f, 1.0f);
        localPos += localStep;
        
        //if (!all(max(0.5f - abs(localPos), 0.0f)))
        //{
        //    break;
        //}
    }

    return color;
}

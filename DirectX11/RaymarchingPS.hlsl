cbuffer CBuf : register(b0)
{
    float3 mCameraPos;
    float3 mDirection;
    float3 mAmbientLight;
    float3 mDiffuseColor;
    float3 mSpecColor;
};

cbuffer CBuf1 : register(b1)
{
    matrix mWorldInverse;
    float mTime;
};

cbuffer CBuf2 : register(b2)
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
};

float Hash(float n)
{
    return frac(sin(n) * 43758.5453f);
}

float Noise(float3 x)
{
    float3 p = floor(x);
    float3 f = frac(x);
    f = f * f * (3.0f - 2.0f * f);
    
    float n = p.x + p.y * 57.0f + p.z * 113.0f;
    float res = lerp(lerp(lerp(Hash(n +   0.0f), Hash(n +   1.0f), f.x),
                          lerp(Hash(n +  57.0f), Hash(n +  58.0f), f.x), f.y),
                     lerp(lerp(Hash(n + 113.0f), Hash(n + 114.0f), f.x),
                          lerp(Hash(n + 170.0f), Hash(n + 171.0f), f.x), f.y), f.z);
    
    return res;
}

float FBM(float3 p)
{
    float3x3 m = float3x3(
         0.00f,  0.80f,  0.60f,
        -0.80f,  0.36f, -0.48f,
        -0.60f, -0.48f,  0.64f
    );
    
    float f = 0.0f;
    f += 0.500f * Noise(p); p = mul(m, p) * 2.02f;
    f += 0.250f * Noise(p); p = mul(m, p) * 2.03f;
    f += 0.125f * Noise(p);
    
    return f;
}

float Sphere(float3 pos, float radius)
{
    return length(pos) - radius;
}

float Ellipsoid(float3 pos, float3 radius)
{
    float k0 = length(pos / radius);
    float k1 = length(pos / (radius * radius));
    return k0 * (k0 - 1.0f) / k1;
}

float Torus(float3 pos, float2 radius)
{
    float2 r = float2(length(pos.xz) - radius.x, pos.y);
    return length(r) - radius.y;
}

float DensityFunction(float3 p)
{
    float f = FBM(p * mNoiseScale);
    
    return f * 0.3f - Sphere(p, mRadius);
    //return f * 0.8f - Sphere(p / mRadius,0.0f );
    //return f * 0.2f - Ellipsoid(p, float3(mRadius, mRadius / 3.0f, mRadius / 2.0f));
}

float DensityFunctionAnime(float3 p)
{
    float f = FBM(p * mNoiseScale);

    float d1 = f * 0.3f - Sphere(p, mRadius);
    float d2 = f * 0.2f - Torus(p, float2(mRadius * 1.1f, 0.1f));
    float blend = 0.5f + 0.5f * sin(mTime * 1.5f);
    return lerp(d1, d2, blend);
}

float4 main(float3 worldPos : POSITION) : SV_TARGET
{
    float step = 1.0f / mLoop;

    float3 worldDir = normalize(worldPos - mCameraPos);
    
    float3 localPos = (float3) mul(mWorldInverse, float4(worldPos, 1.0f));
    float3 localDir = normalize(mul((float3x3) mWorldInverse, worldDir));
    float3 localStep = localDir * step;
    float jitter = Hash(localPos.x + localPos.y * 10.0f + localPos.z * 100.0f + mTime);
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
    
    float4 color = float4(mCloudColor + mAmbientLight, 0.0f);
    float transmittance = 1.0f;
    
    for (int i = 0; i < loop; i++)
    {
        float density = DensityFunction(localPos);
        
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
                float densityLight = DensityFunction(lightPos);
                
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
            
            color.a += 1.0f * (mOpacity * d * transmittance);
            color.rgb += mDiffuseColor * (mOpacityLight * d * transmittance * transmittanceLight);
        }
        
        color = clamp(color, 0.0f, 1.0f);
        localPos += localStep;
    }

    return color;
}

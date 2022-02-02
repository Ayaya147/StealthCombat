cbuffer CBuf : register(b0)
{
    float3 mCameraPos;
    float3 mAmbientLight;
    float3 mDirection;
    float3 mDiffuseColor;
    float3 mSpecColor;
};

SamplerState splr : register(s0);
Texture2D tex : register(t0);

static const float specPower = 20.0f;

float4 main(float3 worldPos : POSITION, float3 worldNor : NORMAL, float2 tc : TEXCOORD) : SV_TARGET
{
    float3 n = normalize(worldNor);
    float3 l = normalize(-mDirection);
    float3 v = normalize(mCameraPos - worldPos);
    float3 r = normalize(reflect(-l, n));

    float3 diffuse = 0.0f;
    float3 specular = 0.0f;
    
    float nDotL = dot(n, l);
    if( nDotL > 0 )
    {
        diffuse = mDiffuseColor * nDotL;
        specular = mSpecColor * pow(max(0.0f, dot(r, v)), specPower);
    }

    return float4(saturate((diffuse + mAmbientLight) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}

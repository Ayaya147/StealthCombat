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

static const float specPower = 20.0f;

float4 main(float3 worldPos : POSITION, float3 worldNor : NORMAL, float2 tc : TEXCOORD) : SV_TARGET
{
    float3 normal = normalize(worldNor);
    float3 lightDir = normalize(-mDirection);
    float3 viewDir = normalize(mCameraPos - worldPos);
    float3 reflectDir = normalize(reflect(-lightDir, normal));

    float3 diffuse = (float3) 0.0f;
    float3 specular = (float3) 0.0f;
    
    float nDotL = dot(normal, lightDir);
    if( nDotL > 0 )
    {
        diffuse = mDiffuseColor * nDotL;
        specular = mSpecColor * pow(max(0.0f, dot(reflectDir, viewDir)), specPower);
    }

    return saturate(float4(diffuse + mAmbientLight, 1.0f) * tex.Sample(splr, tc) + float4(specular, 0.0f));
}

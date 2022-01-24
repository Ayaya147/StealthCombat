cbuffer CBuf : register(b0)
{
    float3 mCameraPos;
    float3 mAmbientLight;
};

cbuffer DirectLightCBuf : register(b1)
{
    float3 mDirection;
    float3 mDiffuseColor;
    float3 mSpecColor;
};

SamplerState splr : register(s0);
Texture2D nmap0 : register(t0);
Texture2D nmap1 : register(t1);

static const float4 seaColor = float4(0.0f, 0.5f, 0.9f, 1.0f);
static const float specPower = 30.0f;

float4 main(float3 worldPos : Position, float3 worldNor : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc0 : Tex0Coord, float2 tc1 : Tex1Coord) : SV_Target
{
    float3 localNormal = nmap0.Sample(splr, tc0).xyz;
    localNormal = localNormal * 2.0f - 1.0f;
    float3 normal0 = tan * localNormal.x + bitan * localNormal.y + worldNor * localNormal.z;
    
    float3 localNormal1 = nmap1.Sample(splr, tc1).xyz;
    localNormal1 = localNormal1 * 2.0f - 1.0f;
    float3 normal1 = tan * localNormal1.x + bitan * localNormal1.y + worldNor * localNormal1.z;

    float3 n = normalize(normal0 + normal1);
    float3 l = normalize(-mDirection);
    float3 v = normalize(mCameraPos - worldPos);
    float3 r = normalize(reflect(-l, n));

    float3 diffuse = 0.0f;
    float3 specular = 0.0f;
    
    float nDotL = dot(n, l);    
    if (nDotL > 0)
    {
        diffuse = mDiffuseColor * nDotL;
        specular = mSpecColor * pow(max(0.0f, dot(r, v)), specPower);
    }

    return float4(saturate((diffuse + mAmbientLight) * seaColor.rgb + specular), 1.0f);
}

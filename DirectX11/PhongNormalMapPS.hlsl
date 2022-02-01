cbuffer CBuf : register(b0)
{
    float3 mCameraPos;
    float3 mAmbientLight;
    float3 mDirection;
    float3 mDiffuseColor;
    float3 mSpecColor;
};

SamplerState splr : register(s0);
Texture2D nmap0 : register(t0);
Texture2D nmap1 : register(t1);

static const float3 seaColor = float3(0.0f, 0.4f, 0.9f);
static const float specPower = 30.0f;
static const float f0 = 0.02f;
static const float3 _SkyColor = float3(1.0f, 1.0f, 1.0f);

float3 GetSkyColor(float3 dir, float3 c)
{
    dir.y = max(0.0, dir.y);
    float et = 1.0 - dir.y;
    return (1.0 - c) * et + c;
}

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
    float fresnel = 0.0f;
    float3 water_color = 0.0f;
    
    float nDotL = dot(n, l);
    if (nDotL > 0)
    {
        diffuse = seaColor * nDotL * mDiffuseColor;
        specular = mSpecColor * pow(max(0.0f, dot(r, v)), specPower);
        fresnel = f0 + (1.0f - f0) * pow(1.0f - dot(v, n), 5.0f);
        
        float3 sea_base_color = diffuse + mAmbientLight;
        float3 sea_reflect_color = GetSkyColor(reflect(-v, n), _SkyColor);
        water_color = lerp(sea_base_color, sea_reflect_color, fresnel);
    }

    return float4(saturate(water_color + specular), 1.0f);
}

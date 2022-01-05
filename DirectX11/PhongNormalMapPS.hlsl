cbuffer CBuf : register(b0)
{
    float3 cameraPos;
    float3 ambientLight;
    float specPower;
};

cbuffer DirectLightCBuf : register(b1)
{
    float3 direction;
    float diffuseColor;
    float specColor;
};

cbuffer TransformCBuf : register(b2)
{
    matrix worldTransform;
    matrix viewProj;
};

SamplerState splr : register(s0);
Texture2D nmap0 : register(t0);
Texture2D nmap1 : register(t1);

float4 main(float3 worldPos : Position, float3 worldNor : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc0 : TexCoordf, float2 tc1 : TexCoords) : SV_Target
{
    float3 localNormal = nmap0.Sample(splr, tc0).xyz;
    localNormal = localNormal * 2.0f - 1.0f;
    float3 normal0 = tan * localNormal.x + bitan * localNormal.y + worldNor * localNormal.z;
    
    float3 localNormal1 = nmap1.Sample(splr, tc1).xyz;
    localNormal1 = localNormal1 * 2.0f - 1.0f;
    float3 normal1 = tan * localNormal1.x + bitan * localNormal1.y + worldNor * localNormal1.z;

    float3 n = normalize(normal0 + normal1);
    float3 l = normalize(-direction);
    float3 v = normalize(cameraPos - worldPos);
    float3 r = normalize(reflect(-l, n));
    
    float3 phong = ambientLight;
    
    float nDotL = dot(n, l);    
    if (nDotL > 0)
    {
        float3 diffuse = diffuseColor * nDotL;
        float3 specular = specColor * pow(max(0.0f, dot(r, v)), specPower);
        phong += diffuse + specular;
    }

    return float4(saturate(phong), 1.0f) * float4(0.1f, 0.6f, 1.0f, 1.0f);
}

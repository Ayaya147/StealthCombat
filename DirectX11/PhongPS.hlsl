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

SamplerState splr : register(s0);
Texture2D tex : register(t0);

float4 main(float3 worldPos : Position, float3 worldNor : Normal, float2 tc : TexCoord) : SV_Target
{
    float3 n = normalize(worldNor);
    float3 l = normalize(-direction);
    float3 v = normalize(cameraPos - worldPos);
    float3 r = normalize(reflect(-l, n));
    
    float3 phong = ambientLight;
    float nDotL = dot(n, l);
    if( nDotL > 0 )
    {
        float3 diffuse = diffuseColor * nDotL;
        float3 specular = specColor * pow(max(0.0f, dot(r, v)), specPower);
        phong += diffuse + specular;
    }
    //return float4(saturate(phong * float3(1.0f, 0.0f, 0.0f)), 1.0f);
    //return float4(saturate(phong * tex.Sample(splr, tc).rgb), 1.0f);
    return float4(saturate(phong), 1.0f) * tex.Sample(splr, tc);
}

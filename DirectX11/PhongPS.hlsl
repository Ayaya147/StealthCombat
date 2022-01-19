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

SamplerState splr : register(s0);
Texture2D tex : register(t0);

static const float specPower = 20.0f;

float4 main(float3 worldPos : Position, float3 worldNor : Normal, float2 tc : TexCoord) : SV_Target
{
    float3 n = normalize(worldNor);
    float3 l = normalize(-direction);
    float3 v = normalize(cameraPos - worldPos);
    float3 r = normalize(reflect(-l, n));

    float3 diffuse = 0.0f;
    float3 specular = 0.0f;
    
    float nDotL = dot(n, l);
    if( nDotL > 0 )
    {
        diffuse = diffuseColor * nDotL;
        specular = specColor * pow(max(0.0f, dot(r, v)), specPower);
    }

    return float4(saturate((diffuse + ambientLight) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}

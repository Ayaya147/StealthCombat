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

float Sphere_d(float3 pos, float size)
{
    return length(pos) - size;
}

float4 main(float3 worldPos : Position, float3 worldNor : Normal, float2 tc : TexCoord) : SV_Target
{
    float3 raydir = normalize(worldPos - cameraPos);
    float3 cur = cameraPos;
    
    float3 col = float3(0.0f, 0.0f, 0.0f);
    
    //float size = 10.0f;
    
    //for (int i = 0; i < 16; i++)
    //{
    //    float d = Sphere_d(cur, size);
        
    //    if (d < 0.0001)
    //    {
    //        col = tex.Sample(splr, tc);
    //        break;
    //    }
        
    //    cur += raydir * d;
        
    //}
    
    return float4(col, 1.0f);
}

cbuffer CBuf : register(b0)
{
    matrix mWorldTransform;
    matrix mViewProj;
};

struct VSOut
{
    float2 uv : TEXCOORD0;
    float4 vertex : SV_POSITION;
    float3 world_pos : TEXCOORD1;
    float4 proj_coord : TEXCOORD5;
};

static const int wave_number = 8;
static const int count = 4;

VSOut main(float4 vertex : Position, float2 uv : TEXCOORD)
{
    VSOut o;
    float4 vt = vertex;
    float4 world_pos = mul(mWorldTransform, vt);
    o.world_pos = world_pos.xyz;

    float time = 1.0f;

    float3 p = 0.0;
    for (int i = 0; i < count; i++)
    {
        p += GerstnerWave(amp[i], freq[i], steep[i], speed[i], noise_size[i], dir[i], world_pos.xz, time, i);
    }
    for (int j = wave_number - count; j < wave_number; j++)
    {
        p += GerstnerWave_Cross(amp[j], freq[j], steep[j], speed[j], noise_size[j], dir[j], world_pos.xz, time, j);
    }
    world_pos.xyz += p;

    vt = mul(unity_WorldToObject, world_pos);

    o.uv = TRANSFORM_TEX(v.uv, _MainTex);
    o.vertex = UnityObjectToClipPos(vt);
    o.proj_coord = ComputeScreenPos(o.vertex);

    return o;
}

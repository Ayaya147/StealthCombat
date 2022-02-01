#include "GerstnerWave.hlsli"

cbuffer CBuf : register(b0)
{
    float3 mCameraPos;
    float3 mAmbientLight;
    float3 mDirection;
    float3 mDiffuseColor;
    float3 mSpecColor;
};

static const float r = 0.02f;
static const float _BaseColorStrength = 0.9f;
static const float _ShallowColorStrength = 0.35f;
static const float _Shininess = 0.27f;
static const float _ColorHightOffset = 0.15f;
static const float3 _SeaBaseColor = float3(0.1f, 0.22f, 0.35f);
static const float3 _SeaShallowColor = float3(0.29f, 0.35f, 0.14f);
static const float3 _LightColor0 = float3(1.0f, 1.0f, 1.0f);
static const float3 _SkyColor = float3(0.0f, 0.41f, 1.0f);

float3 GetSkyColor(float3 dir, float3 c)
{
    dir.y = max(0.0, dir.y);
    float et = 1.0 - dir.y;
    return (1.0 - c) * et + c;
}

float3 OceanColor(float3 worldPos, float wave_height, float3 normal)
{
    float3 lightDir = normalize(-mDirection);
    float3 viewDir = normalize(mCameraPos - worldPos);
    float3 halfDir = normalize(lightDir + viewDir);
	
    float facing = saturate(1.0 - dot(normal, viewDir));
    float fresnel = r + (1.0 - r) * pow(facing, 5.0);
    float3 reflectDir = reflect(-viewDir, normal);
	
    float3 diff = saturate(dot(normal, lightDir)) * _LightColor0;
	//float spec = pow(max(0.0, dot(normal, halfDir)), _Shininess * 128.0) * _LightColor0;	//Blinn-Phong
	
    float dotSpec = saturate(dot(reflectDir, lightDir) * 0.5 + 0.5);
    float3 spec = (1.0 - fresnel) * saturate(lightDir.y) * pow(dotSpec, 512.0) * (_Shininess * 1.8 + 0.2);
    spec += spec * 25.0 * saturate(_Shininess - 0.05) * _LightColor0;
	
    float3 sea_reflect_color = GetSkyColor(reflectDir, _SkyColor);
    float3 sea_base_color = _SeaBaseColor * diff * _BaseColorStrength + lerp(_SeaBaseColor, _SeaShallowColor * _ShallowColorStrength, diff);
    float3 water_color = lerp(sea_base_color, sea_reflect_color, fresnel);
    float3 sea_color = water_color + _SeaShallowColor * (wave_height * 0.5 + 0.2) * _ColorHightOffset;
 
    return sea_color + spec;
}

float4 main(float3 worldPos : Position) : SV_Target
{
    float3 world_pos = worldPos;
    float3 geo_pos = world_pos;

    float time = mTime / 20.0f * _WaveSpeed;
    
    float3 p = 0.0;
    float3 pb = float3(0.05, 0.0, 0.0);
    float3 pt = float3(0.0, 0.0, 0.05);
    float3 v_bi = world_pos.xyz + float3(0.05, 0.0, 0.0);
    float3 v_tan = world_pos.xyz + float3(0.0, 0.0, 0.05);
    for (int m = 0; m < count; m++)
    {
        p += GerstnerWave(amp[m], freq[m], steep[m], speed[m], noise_size[m], dir[m], world_pos.xz, time, m);
        pb += GerstnerWave(amp[m], freq[m], steep[m], speed[m], noise_size[m], dir[m], v_bi.xz, time, m);
        pt += GerstnerWave(amp[m], freq[m], steep[m], speed[m], noise_size[m], dir[m], v_tan.xz, time, m);
    }
    for (int n = wave_number - count; n < wave_number; n++)
    {
        p += GerstnerWave_Cross(amp[n], freq[n], steep[n], speed[n], noise_size[n], dir[n], world_pos.xz, time, n);
        pb += GerstnerWave_Cross(amp[n], freq[n], steep[n], speed[n], noise_size[n], dir[n], v_bi.xz, time, n);
        pt += GerstnerWave_Cross(amp[n], freq[n], steep[n], speed[n], noise_size[n], dir[n], v_tan.xz, time, n);
    }
    world_pos += p;
    float3 normal = normalize(cross(pt - p, pb - p));

    float wave_height = world_pos.y - geo_pos.y;
    float3 result = OceanColor(world_pos, wave_height, normal);
    float4 col = float4(result, 1.0);
    
    return col;
}

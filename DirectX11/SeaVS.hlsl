cbuffer CBuf : register(b0)
{
    matrix mWorldTransform;
    matrix mViewProj;
};

cbuffer TBuf : register(b1)
{
    float mTime;
}

struct VSOut
{
    float3 world_pos : Position;
    float4 vertex : SV_POSITION;
};



static const int wave_number = 8;
static const int count = 4;
static const float _NoiseStrength = 1.26f;
static const float _NoiseSizeLerp = 0.5;
static const float _WaveSpeed = 1.0f;

static const float4 _Amplitude = float4(0.78, 0.81, 0.6, 0.27);
static const float4 _Amplitude2 = float4(0.17, 0.12, 0.21, 0.06);
static const float4 _Frequency = float4(0.16, 0.18, 0.21, 0.27);
static const float4 _Frequency2 = float4(0.7, 0.84, 0.54, 0.80);
static const float4 _Steepness = float4(1.70, 1.60, 1.20, 1.80);
static const float4 _Steepness2 = float4(1.56, 2.18, 2.80, 1.90);
static const float4 _Speed = float4(24, 40, 48, 60);
static const float4 _Speed2 = float4(32, 40, 48, 60);
static const float4 _DirectionA = float4(0.35, 0.31, 0.08, 0.60);
static const float4 _DirectionB = float4(-0.95, -0.74, 0.7, -0.5);
static const float4 _DirectionC = float4(0.7, 0.6, 0.10, 0.38);
static const float4 _DirectionD = float4(0.43, 0.07, 0.42, 0.61);
static const float4 _Noise = float4(0.39, 0.31, 0.27, 0.57);
static const float4 _Noise2 = float4(0.33, 0.81, 0.39, 0.45);

static const float amp[8] = { _Amplitude.x, _Amplitude.y, _Amplitude.z, _Amplitude.w, _Amplitude2.x, _Amplitude2.y, _Amplitude2.z, _Amplitude2.w };
static const float freq[8] = { _Frequency.x, _Frequency.y, _Frequency.z, _Frequency.w, _Frequency2.x, _Frequency2.y, _Frequency2.z, _Frequency2.w };
static const float steep[8] = { _Steepness.x, _Steepness.y, _Steepness.z, _Steepness.w, _Steepness2.x, _Steepness2.y, _Steepness2.z, _Steepness2.w };
static const float speed[8] = { _Speed.x, _Speed.y, _Speed.z, _Speed.w, _Speed2.x, _Speed2.y, _Speed2.z, _Speed2.w };
static const float2 dir[8] = { _DirectionA.xy, _DirectionA.zw, _DirectionB.xy, _DirectionB.zw, _DirectionC.xy, _DirectionC.zw, _DirectionD.xy, _DirectionD.zw };
static const float noise_size[8] = { _Noise.x, _Noise.y, _Noise.z, _Noise.w, _Noise2.x, _Noise2.y, _Noise2.z, _Noise2.w };

float2 rand2d(float2 st, int seed)
{
    float2 s = float2(dot(st, float2(127.1, 311.7)) + seed, dot(st, float2(269.5, 183.3)) + seed);
    return -1.0 + 2.0 * frac(sin(s) * 43758.5453123);
}

float noise2(float2 st, int seed)
{
    float2 p = floor(st);
    float2 f = frac(st);
 
    float w00 = dot(rand2d(p, seed), f);
    float w10 = dot(rand2d(p + float2(1.0, 0.0), seed), f - float2(1.0, 0.0));
    float w01 = dot(rand2d(p + float2(0.0, 1.0), seed), f - float2(0.0, 1.0));
    float w11 = dot(rand2d(p + float2(1.0, 1.0), seed), f - float2(1.0, 1.0));
	
    float2 u = f * f * (3.0 - 2.0 * f);
 
    return lerp(lerp(w00, w10, u.x), lerp(w01, w11, u.x), u.y);
}

float3 GerstnerWave(float amp, float freq, float steep, float speed, float noise, float2 dir, float2 v, float time, int seed)
{
    float3 p;
    float2 d = normalize(dir.xy);
    float q = steep;
 
    seed *= 3;
    v += noise2(v * noise + time, seed) * _NoiseStrength;
    float f = dot(d, v) * freq + time * speed;
    p.xz = q * amp * d.xy * cos(f);
    p.y = amp * sin(f);
 
    return p;
}

float3 GerstnerWave_Cross(float amp, float freq, float steep, float speed, float noise, float2 dir, float2 v, float time, int seed)
{
    float3 p;
    float2 d = normalize(dir.xy);
    float q = steep;
 
    float noise_strength = _NoiseStrength;
    seed *= 3;
 
    float3 p1;
    float3 p2;
    float2 d1 = normalize(dir.xy);
    float2 d2 = float2(-d.y, d.x);
 
    float2 v1 = v + noise2(v * noise + time * d * 10.0, seed) * noise_strength;
    float2 v2 = v + noise2(v * noise + time * d * 10.0, seed + 12) * noise_strength;
    float f1 = dot(d1, v1) * freq + time * speed;
    float f2 = dot(d2, v2) * freq + time * speed;
    p1.xz = q * amp * d1.xy * cos(f1);
    p1.y = amp * sin(f1);
    p2.xz = q * amp * d2.xy * cos(f2);
    p2.y = amp * sin(f2);
 
    p = lerp(p1, p2, noise2(v * _NoiseSizeLerp + time, seed) * 0.5 + 0.5);
    return p;
}

VSOut main(float3 vertex : Position)
{
    VSOut o;
    //float4 vt = float4(vertex, 1.0f);
    //float4 world_pos = mul(mWorldTransform, vt);
    //o.world_pos = (float3)world_pos;

    float4 world_pos = mul(float4(vertex, 1.0f), mWorldTransform);
    o.world_pos = (float3) mul(float4(vertex, 1.0f), mWorldTransform);

    float time = mTime/20.0f * _WaveSpeed;
    //float time = 1.0f;
    
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

    o.vertex = mul(world_pos, mViewProj);

    return o;
}

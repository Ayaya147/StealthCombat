static const float4 amplitude = float4(0.78, 0.81, 0.6, 0.27);
static const float4 amplitude2 = float4(0.17, 0.12, 0.21, 0.06);
static const float4 frequency = float4(0.16, 0.18, 0.21, 0.27);
static const float4 frequency2 = float4(0.7, 0.84, 0.54, 0.80);
static const float4 steepness = float4(1.70, 1.60, 1.20, 1.80);
static const float4 steepness2 = float4(1.56, 2.18, 2.80, 1.90);
static const float4 speed = float4(24, 40, 48, 60);
static const float4 speed2 = float4(32, 40, 48, 60);
static const float4 directionA = float4(0.35, 0.31, 0.08, 0.60);
static const float4 directionB = float4(-0.95, -0.74, 0.7, -0.5);
static const float4 directionC = float4(0.7, 0.6, 0.10, 0.38);
static const float4 directionD = float4(0.43, 0.07, 0.42, 0.61);
static const float4 noise = float4(0.39, 0.31, 0.27, 0.57);
static const float4 noise2 = float4(0.33, 0.81, 0.39, 0.45);

static const float amp[8] = { amplitude.x, amplitude.y, amplitude.z, amplitude.w, amplitude2.x, amplitude2.y, amplitude2.z, amplitude2.w };
static const float freq[8] = { frequency.x, frequency.y, frequency.z, frequency.w, frequency2.x, frequency2.y, frequency2.z, frequency2.w };
static const float steep[8] = { steepness.x, steepness.y, steepness.z, steepness.w, steepness2.x, steepness2.y, steepness2.z, steepness2.w };
static const float spd[8] = { speed.x, speed.y, speed.z, speed.w, speed2.x, speed2.y, speed2.z, speed2.w };
static const float2 dir[8] = { directionA.xy, directionA.zw, directionB.xy, directionB.zw, directionC.xy, directionC.zw, directionD.xy, directionD.zw };
static const float noiseSize[8] = { noise.x, noise.y, noise.z, noise.w, noise2.x, noise2.y, noise2.z, noise2.w };

float2 Rand2d(float2 st, int seed)
{
    float2 s = float2(dot(st, float2(127.1f, 311.7f)) + seed, dot(st, float2(269.5f, 183.3f)) + seed);
    return -1.0f + 2.0f * frac(sin(s) * 43758.5453123f);
}

float Noise2(float2 st, int seed)
{
    float2 p = floor(st);
    float2 f = frac(st);
    float2 u = f * f * (3.0f - 2.0f * f);

    float w00 = dot(Rand2d(p, seed), f);
    float w10 = dot(Rand2d(p + float2(1.0f, 0.0f), seed), f - float2(1.0f, 0.0f));
    float w01 = dot(Rand2d(p + float2(0.0f, 1.0f), seed), f - float2(0.0f, 1.0f));
    float w11 = dot(Rand2d(p + float2(1.0f, 1.0f), seed), f - float2(1.0f, 1.0f));	
 
    return lerp(lerp(w00, w10, u.x), lerp(w01, w11, u.x), u.y);
}

float3 GerstnerWave(float amp, float freq, float steep, float speed, float noise, float2 dir, float2 v, float time, int seed, float noiseStrength)
{
    float3 p = 0.0f;
    float2 d = normalize(dir);
 
    v += Noise2(v * noise + time, seed * 3) * noiseStrength;
    float f = dot(d, v) * freq + time * speed;
    p.xz = steep * amp * d * cos(f);
    p.y = amp * sin(f);
 
    return p;
}

float3 GerstnerWaveCross(float amp, float freq, float steep, float speed, float noise, float2 dir, float2 v, float time, int seed, float noiseStrength, float noiseSizeLerp)
{
    float2 d = normalize(dir);
    float2 d1 = float2(-d.y, d.x);
    float3 p1 = 0.0f;
    float3 p2 = 0.0f;
 
    float2 v1 = v + Noise2(v * noise + time * d * 10.0f, seed * 3) * noiseStrength;
    float2 v2 = v + Noise2(v * noise + time * d * 10.0f, seed * 3 + 12) * noiseStrength;
    float f1 = dot(d, v1) * freq + time * speed;
    float f2 = dot(d1, v2) * freq + time * speed;
    p1.xz = steep * amp * d * cos(f1);
    p1.y = amp * sin(f1);
    p2.xz = steep * amp * d1 * cos(f2);
    p2.y = amp * sin(f2);
 
    float3 p = lerp(p1, p2, Noise2(v * noiseSizeLerp + time, seed) * 0.5f + 0.5f);
    return p;
}

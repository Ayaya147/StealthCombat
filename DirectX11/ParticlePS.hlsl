struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
    float scale : SCALE;
};

static const float PI = 3.14159;

float4 main(PS_INPUT input) : SV_Target
{
    float4 out_col = input.col;
    float alphaX = sin(input.uv.x * 2 * PI / 2.0);
    float alphaY = sin(input.uv.y * 2 * PI / 2.0);
    float finalAlpha = alphaX * alphaY * 0.3f;
    out_col.a = finalAlpha;
    return out_col;
}

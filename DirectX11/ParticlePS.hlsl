struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 tc : TEXCOORD0;
    float scale : SCALE;
};

static const float PI = 3.14159;

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = input.col;
    float alphaX = sin(input.tc.x * 2 * PI / 2.0);
    float alphaY = sin(input.tc.y * 2 * PI / 2.0);
    float finalAlpha = alphaX * alphaY * 0.3f;
    color.a = finalAlpha;
    
    return color;
}

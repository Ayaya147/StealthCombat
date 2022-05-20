struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 tc : TEXCOORD0;
    float scale : SCALE;
};

[maxvertexcount(4)]
void main(point PS_INPUT input[1], inout TriangleStream<PS_INPUT> outputStream)
{
    float posX = 0.5f * input[0].scale;
    float posY = posX * 16.0f / 9.0f;
    
    PS_INPUT output;
    output.scale = input[0].scale;
    output.col = input[0].col;
    output.tc = float2(0.0f, 0.0f);
    output.pos = input[0].pos + float4(-posX, posY, 0.0f, 0.0f);
    outputStream.Append(output);
    output.tc = float2(1.0f, 0.0f);
    output.pos = input[0].pos + float4(posX, posY, 0.0f, 0.0f);
    outputStream.Append(output);
    output.tc = float2(0.0f, 1.0f);
    output.pos = input[0].pos + float4(-posX, -posY, 0.0f, 0.0f);
    outputStream.Append(output);
    output.tc = float2(1.0f, 1.0f);
    output.pos = input[0].pos + float4(posX, -posY, 0.0f, 0.0f);
    outputStream.Append(output);
}

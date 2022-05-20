struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
    float scale : SCALE;
};

[maxvertexcount(4)]
void main(point PS_INPUT input[1], inout TriangleStream<PS_INPUT> outputStream)
{
    float posX = 0.5 * input[0].scale;
    float posY = posX * 16.0f / 9.0f;
    
    PS_INPUT output;
    output.scale = input[0].scale;
    output.col = input[0].col;
    output.uv = float2(0, 0);
    output.pos = input[0].pos + float4(-posX, posY, 0, 0);
    outputStream.Append(output);
    output.uv = float2(1, 0);
    output.pos = input[0].pos + float4(posX, posY, 0, 0);
    outputStream.Append(output);
    output.uv = float2(0, 1);
    output.pos = input[0].pos + float4(-posX, -posY, 0, 0);
    outputStream.Append(output);
    output.uv = float2(1, 1);
    output.pos = input[0].pos + float4(posX, -posY, 0, 0);
    outputStream.Append(output);
}

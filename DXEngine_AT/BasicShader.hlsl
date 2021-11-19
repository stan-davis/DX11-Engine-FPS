cbuffer cBuffer
{
    float4x4 WVP;
};

Texture2D _texture;
SamplerState _samplerstate;

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(float4 position : POSITION, float2 texCoord : TEXCOORD)
{
    VS_OUTPUT output;

    output.position = mul(position, WVP);
    output.texCoord = texCoord;

    return output;
};

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float4 tex = _texture.Sample(_samplerstate, input.texCoord);

    clip(tex.a - 0.01f);

    return tex;
};
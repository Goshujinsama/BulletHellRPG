struct SpriteGSOut
{
	float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

Texture2D       t0 : register( t0 );
SamplerState    s0 : register( s0 );

float4 SpritePS(SpriteGSOut input) : SV_TARGET
{
	return t0.Sample( s0, input.texCoord );
}
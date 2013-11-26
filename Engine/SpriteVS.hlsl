struct SpriteVertex
{
	float3 Position			: POSITION;
	float2 Size				: SIZE;
	float Opacity			: OPACITY;
	uint Frame				: FRAME;
};

SpriteVertex SpriteVS( SpriteVertex v )
{
	return v;
}
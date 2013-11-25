struct SpriteVertex
{
	float3 Position			: POSITION;
	float2 Size				: SIZE;
	float Opacity			: OPACITY;
	float2 UVTopLeft		: UV_TOP_LEFT;
	float2 UVBottomRight	: UV_BOTTOM_RIGHT;
};

SpriteVertex SpriteVS( SpriteVertex v )
{
	return v;
}
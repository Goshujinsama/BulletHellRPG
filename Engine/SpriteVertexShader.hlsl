struct SpriteVertex
{
	float2 Position			: POSITION;
	float2 Size				: SIZE;
	float Opacity			: OPACITY;
	float2 UVTopLeft		: UV_TOP_LEFT;
	float2 UVBottomRight	: UV_BOTTOM_RIGHT;
};

SpriteVertex SpriteVertexShader( SpriteVertex v )
{
	return v;
}
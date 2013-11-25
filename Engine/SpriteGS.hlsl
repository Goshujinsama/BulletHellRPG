struct SpriteVertex
{
	float3 Position			: POSITION;
	float2 Size				: SIZE;
	float Opacity			: OPACITY;
	float2 UVTopLeft		: UV_TOP_LEFT;
	float2 UVBottomRight	: UV_BOTTOM_RIGHT;

};

struct SpriteGSOut
{
	float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

[maxvertexcount(4)]
void SpriteGS( point SpriteVertex input[1], inout TriangleStream< SpriteGSOut > output, uint primID : SV_PrimitiveID )
{
	SpriteGSOut element;
	element.position.z = input[0].Position.z;
	element.position.w = 1.0;

	element.position.x = input[0].Position.x - input[0].Size.x;
	element.position.y = input[0].Position.y + input[0].Size.y;
	element.texCoord.xy = float2( input[0].UVTopLeft.x, input[0].UVTopLeft.y );
	output.Append(element);

	element.position.x = input[0].Position.x + input[0].Size.x;
	element.position.y = input[0].Position.y + input[0].Size.y;
	element.texCoord.xy = float2( input[0].UVBottomRight.x, input[0].UVTopLeft.y );
	output.Append(element);

	element.position.x = input[0].Position.x - input[0].Size.x;
	element.position.y = input[0].Position.y - input[0].Size.y;
	element.texCoord.xy = float2( input[0].UVTopLeft.x, input[0].UVBottomRight.y );
	output.Append(element);

	element.position.x = input[0].Position.x + input[0].Size.x;
	element.position.y = input[0].Position.y - input[0].Size.y;
	element.texCoord.xy = float2( input[0].UVBottomRight.x, input[0].UVBottomRight.y );
	output.Append(element);

	output.RestartStrip();
}
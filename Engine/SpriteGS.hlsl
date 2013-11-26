cbuffer LayoutBuffer
{
	uint Columns;
	uint Rows;
};

struct SpriteVertex
{
	float3 Position			: POSITION;
	float2 Size				: SIZE;
	float Opacity			: OPACITY;
	uint Frame				: FRAME;
};

struct SpriteGSOut
{
	float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

[maxvertexcount(4)]
void SpriteGS( point SpriteVertex input[1], inout TriangleStream< SpriteGSOut > output, uint primID : SV_PrimitiveID )
{
	float UVSizeX = 1.0 / Columns;
	float UVSizeY = 1.0 / Rows;
	float UVTop = (uint)(input[0].Frame / Columns) * UVSizeY;
	float UVLeft = (uint)(input[0].Frame % Columns) * UVSizeX;


	SpriteGSOut element;
	element.position.z = input[0].Position.z;
	element.position.w = 1.0;

	element.position.x = input[0].Position.x - input[0].Size.x;
	element.position.y = input[0].Position.y + input[0].Size.y;
	element.texCoord.xy = float2( UVLeft, UVTop );
	output.Append(element);

	element.position.x = input[0].Position.x + input[0].Size.x;
	element.position.y = input[0].Position.y + input[0].Size.y;
	element.texCoord.xy = float2( UVLeft + UVSizeX, UVTop );
	output.Append(element);

	element.position.x = input[0].Position.x - input[0].Size.x;
	element.position.y = input[0].Position.y - input[0].Size.y;
	element.texCoord.xy = float2( UVLeft, UVTop + UVSizeY );
	output.Append(element);

	element.position.x = input[0].Position.x + input[0].Size.x;
	element.position.y = input[0].Position.y - input[0].Size.y;
	element.texCoord.xy = float2( UVLeft + UVSizeX, UVTop + UVSizeY );
	output.Append(element);

	output.RestartStrip();
}
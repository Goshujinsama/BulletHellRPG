#ifndef SPRITE_H
#define SPRITE_H

#include "Common.h"
#include "Graphics.h"
#include "Image.h"
#include "Queue.h"

namespace Engine
{
	struct SpriteVertex
	{
		FLOAT3 Position;
		FLOAT2 Size;
		FLOAT Opacity;
		UINT8 Frame;
	};

	struct SpriteLayout
	{
		UINT32 Columns;
		UINT32 Rows;
	};

	class Sprite
	{
	public:
		Sprite(Graphics *graphics, Image *image, UINT32 columns, UINT32 rows);
		~Sprite();

		bool Initialize();

		void Enqueue(UINT8 frame, FLOAT x, FLOAT y, FLOAT z, FLOAT width, FLOAT height);
		void Draw();

	private:
		Graphics					*m_graphics;
		Image						*m_image;
		ID3D11InputLayout			*m_inputLayout;
		ID3D11ShaderResourceView	*m_spriteSheet;
		ID3D11VertexShader			*m_vertexShader;
		ID3D11GeometryShader		*m_geometryShader;
		ID3D11PixelShader			*m_pixelShader;
		SpriteLayout				m_spriteLayout;
		Queue<SpriteVertex>			*m_vertexQueue;
	};
}

#endif // SPRITE_H
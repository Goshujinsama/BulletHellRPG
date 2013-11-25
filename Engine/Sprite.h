#ifndef SPRITE_H
#define SPRITE_H

#include "Graphics.h"
#include "Queue.h"

namespace Engine
{
	struct SpriteVertex
	{
		float PositionX;
		float PositionY;
		float SizeX;
		float SizeY;
		float Opacity;
		float UVLeft;
		float UVTop;
		float UVRight;
		float UVBottom;
	};

	class Sprite
	{
	public:
		Sprite(Graphics *graphics, wchar_t *resourceName, unsigned int columns, unsigned int rows);
		~Sprite();

		bool Initialize();

		void Enqueue(unsigned int frame, float x, float y, float width, float height);
		void Draw();

	private:
		Graphics					*m_graphics;
		ID3D11InputLayout			*m_inputLayout;
		ID3D11ShaderResourceView	*m_spriteSheet;
		ID3D11VertexShader			*m_vertexShader;
		ID3D11GeometryShader		*m_geometryShader;
		ID3D11PixelShader			*m_pixelShader;
		unsigned int				m_columns;
		unsigned int				m_rows;
		Queue<SpriteVertex>			*m_vertexQueue;
	};
}

#endif // SPRITE_H
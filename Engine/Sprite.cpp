#include "Common.h"
#include "Sprite.h"

#include "Shaders\SpriteVertexShader.h"

namespace Engine
{
	D3D11_INPUT_ELEMENT_DESC SpriteVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "OPACITY", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV_TOP_LEFT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV_BOTTOM_RIGHT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	Sprite::Sprite(Graphics *graphics, wchar_t *resourceName, unsigned int columns, unsigned int rows)
	{
		m_graphics = graphics;
		m_columns = columns;
		m_rows = rows;

		m_vertexQueue = new Queue<SpriteVertex>();
	}

	Sprite::~Sprite()
	{
		SAFE_DELETE(m_vertexQueue);

		SAFE_RELEASE(m_inputLayout);
		SAFE_RELEASE(m_vertexShader);
	}

	bool Sprite::Initialize()
	{
		RETURN_FAIL( m_graphics->GetDevice()->CreateVertexShader(g_SpriteVertexShader, sizeof(g_SpriteVertexShader), NULL, &m_vertexShader) );
		RETURN_FAIL( m_graphics->GetDevice()->CreateInputLayout(SpriteVertexDesc, 5, g_SpriteVertexShader, sizeof(g_SpriteVertexShader), &m_inputLayout) );

		return true;
	}

	void Sprite::Enqueue(unsigned int frame, float x, float y, float width, float height)
	{
		SpriteVertex vertex;
		vertex.PositionX = x;
		vertex.PositionY = y;
		vertex.SizeX = width;
		vertex.SizeY = height;
		vertex.UVTop = (int)(frame / m_columns) * 1.0f / m_rows;
		vertex.UVLeft = (int)(frame % m_columns) * 1.0f / m_columns;
		vertex.UVBottom = vertex.UVTop + 1.0f / m_rows;
		vertex.UVRight = vertex.UVLeft + 1.0f / m_columns;
		vertex.Opacity = 1.0f;
		m_vertexQueue->Push(vertex);
	}

	void Sprite::Draw()
	{
		unsigned long i;

		SpriteVertex *vertexBuffer = new SpriteVertex[m_vertexQueue->Length()];
		for(i = 0; m_vertexQueue->Length() > 0; ++i)
		{
			vertexBuffer[i] = m_vertexQueue->Pop();
		}
	}
}
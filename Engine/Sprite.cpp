#include "Common.h"
#include "Sprite.h"

#include "Shaders\SpriteVS.h"
#include "Shaders\SpriteGS.h"
#include "Shaders\SpritePS.h"

namespace Engine
{
	D3D11_INPUT_ELEMENT_DESC SpriteVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "OPACITY", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV_TOP_LEFT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV_BOTTOM_RIGHT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	Sprite::Sprite(Graphics *graphics, Image *image, unsigned int columns, unsigned int rows)
		: m_graphics( graphics ), m_image( image ), m_columns( columns ), m_rows( rows )
	{
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
		RETURN_FAIL( m_graphics->GetDevice()->CreateVertexShader(g_SpriteVS, sizeof(g_SpriteVS), NULL, &m_vertexShader) );
		RETURN_FAIL( m_graphics->GetDevice()->CreateInputLayout(SpriteVertexDesc, 5, g_SpriteVS, sizeof(g_SpriteVS), &m_inputLayout) );
		RETURN_FAIL( m_graphics->GetDevice()->CreateGeometryShader(g_SpriteGS, sizeof(g_SpriteGS), NULL, &m_geometryShader) );
		RETURN_FAIL( m_graphics->GetDevice()->CreatePixelShader(g_SpritePS, sizeof(g_SpritePS), NULL, &m_pixelShader) );

		return true;
	}

	void Sprite::Enqueue(unsigned int frame, float x, float y, float z, float width, float height)
	{
		SpriteVertex vertex;
		vertex.PositionX = x;
		vertex.PositionY = y;
		vertex.PositionZ = z;
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
		ID3D11DeviceContext *context = m_graphics->GetContext();
		ID3D11SamplerState *sampler = m_graphics->GetSampler();
		ID3D11ShaderResourceView *srv = m_image->GetView();
		ID3D11Buffer *vertexBuffer = nullptr;

		unsigned long i, numVert;

		numVert = m_vertexQueue->Length();

		SpriteVertex *vertices = new SpriteVertex[numVert];
		for(i = 0; m_vertexQueue->Length() > 0; ++i)
		{
			vertices[i] = m_vertexQueue->Pop();
		}
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth		= sizeof(SpriteVertex) * numVert;
		bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags	= 0;
		bufferDesc.MiscFlags		= 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = vertices;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		m_graphics->GetDevice()->CreateBuffer( &bufferDesc, &initData, &vertexBuffer );

		context->VSSetShader( m_vertexShader, nullptr, 0 );
		context->GSSetShader( m_geometryShader, nullptr, 0 );
		//context->HSSetShader( nullptr, nullptr, 0 );
		//context->DSSetShader( nullptr, nullptr, 0 );
		context->PSSetShader( m_pixelShader, nullptr, 0 );

		context->PSSetShaderResources( 0, 1, &srv );
		context->PSSetSamplers( 0, 1, &sampler );

		context->IASetInputLayout( m_inputLayout );

		UINT stride = sizeof(SpriteVertex);
		UINT offset = 0;
		context->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
		//context->IASetIndexBuffer( nullptr, DXGI_FORMAT_R32_UINT, 0 );   
		context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
		context->Draw( numVert, 0 );

		SAFE_RELEASE(vertexBuffer);
	}
}
#ifndef IMAGE_H
#define IMAGE_H

#include "Graphics.h"

namespace Engine
{
	class Image
	{
	public:
		~Image();

		bool Initialize(wchar_t *fileName, Graphics *graphics);
		ID3D11Texture2D *GetTexture();
		ID3D11ShaderResourceView *GetView();
	private:
		unsigned char				*m_bytes;
		size_t						m_size;
		ID3D11Texture2D				*m_texture2D;
		ID3D11ShaderResourceView	*m_shaderResourceView;
	};
}

#endif
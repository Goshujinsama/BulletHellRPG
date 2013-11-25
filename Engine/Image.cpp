#include <iostream>
#include <fstream>
#include <memory>
#include <wincodec.h>

#include "Common.h"
#include "Image.h"

namespace Engine
{
	struct WICtoDXGI
	{
		GUID                wic;
		DXGI_FORMAT         dxgi;
	};

	static WICtoDXGI g_WICFormats[] = 
	{
		{ GUID_WICPixelFormat128bppRGBAFloat,       DXGI_FORMAT_R32G32B32A32_FLOAT },

		{ GUID_WICPixelFormat64bppRGBAHalf,         DXGI_FORMAT_R16G16B16A16_FLOAT },
		{ GUID_WICPixelFormat64bppRGBA,             DXGI_FORMAT_R16G16B16A16_UNORM },

		{ GUID_WICPixelFormat32bppRGBA,             DXGI_FORMAT_R8G8B8A8_UNORM },
		{ GUID_WICPixelFormat32bppBGRA,             DXGI_FORMAT_B8G8R8A8_UNORM },
		{ GUID_WICPixelFormat32bppBGR,              DXGI_FORMAT_B8G8R8X8_UNORM },

		{ GUID_WICPixelFormat32bppRGBA1010102XR,    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM },
		{ GUID_WICPixelFormat32bppRGBA1010102,      DXGI_FORMAT_R10G10B10A2_UNORM },

		{ GUID_WICPixelFormat32bppGrayFloat,        DXGI_FORMAT_R32_FLOAT },
		{ GUID_WICPixelFormat16bppGrayHalf,         DXGI_FORMAT_R16_FLOAT },
		{ GUID_WICPixelFormat16bppGray,             DXGI_FORMAT_R16_UNORM },
		{ GUID_WICPixelFormat8bppGray,              DXGI_FORMAT_R8_UNORM },

		{ GUID_WICPixelFormat8bppAlpha,             DXGI_FORMAT_A8_UNORM },
	};

	Image::~Image()
	{
		SAFE_DELETE_ARRAY(m_bytes);
		SAFE_RELEASE(m_shaderResourceView);
		SAFE_RELEASE(m_texture2D);
	}

	bool Image::Initialize(wchar_t *filename, Graphics *graphics)
	{
		std::ifstream file;
		IWICImagingFactory *pWIC;
		IWICStream *stream;
		IWICBitmapDecoder *decoder;
		IWICBitmapFrameDecode *frame;
		unsigned int width, height, twidth, theight, maxsize, bpp, i, support, rowPitch, imageSize;
		float ratio;
		WICPixelFormatGUID pixelFormat;
		DXGI_FORMAT format;
		IWICComponentInfo *cinfo;
		IWICPixelFormatInfo *pfinfo;
		IWICBitmapScaler *scaler;
		bool autogen = false;
		D3D11_TEXTURE2D_DESC desc;
		D3D11_SUBRESOURCE_DATA initData;
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

		file.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
		if(!file.is_open()) return false;

		m_size = file.tellg();
		file.seekg(0, std::ios::beg);

		m_bytes = new unsigned char [m_size];
		file.read((char *)m_bytes, m_size);

		file.close();

		RETURN_FAIL( CoCreateInstance( CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (LPVOID*)&pWIC ) );
		RETURN_FAIL( pWIC->CreateStream( &stream ) );
		RETURN_FAIL( stream->InitializeFromMemory( m_bytes, m_size ) );
		RETURN_FAIL( pWIC->CreateDecoderFromStream( stream, 0, WICDecodeMetadataCacheOnDemand, &decoder ) );
		RETURN_FAIL( decoder->GetFrame( 0, &frame ) );
		RETURN_FAIL( frame->GetSize( &width, &height ) );

		SAFE_RELEASE( stream );
		SAFE_RELEASE( decoder );

		if( width <= 0 || height <= 0 ) return false;

		maxsize = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;

		if( width > maxsize || height > maxsize )
		{
			ratio = static_cast<float>(height) / static_cast<float>(width);
			if(width > height)
			{
				twidth = maxsize;
				theight = static_cast<unsigned int>( static_cast<float>(maxsize) * ratio );
			}
			else
			{
				theight = maxsize;
				twidth = static_cast<unsigned int>( static_cast<float>(maxsize) / ratio );
			}
		}
		else
		{
			twidth = width;
			theight = height;
		}

		RETURN_FAIL( frame->GetPixelFormat( &pixelFormat ) );

		format = DXGI_FORMAT_UNKNOWN;
		for( i = 0; i < _countof(g_WICFormats); ++i )
		{
			if( memcmp( &g_WICFormats[i].wic, &pixelFormat, sizeof(GUID) )  == 0 )
			{
				format = g_WICFormats[i].dxgi;
				break;
			}
		}

		RETURN_FAIL( pWIC->CreateComponentInfo( pixelFormat, &cinfo ) );
		RETURN_FAIL( cinfo->QueryInterface( __uuidof(IWICPixelFormatInfo), (LPVOID*)&pfinfo ) );
		RETURN_FAIL( pfinfo->GetBitsPerPixel( &bpp ) );

		SAFE_RELEASE( cinfo );
		SAFE_RELEASE( pfinfo );

		RETURN_FAIL( graphics->GetDevice()->CheckFormatSupport( format, &support ) );
		if( !(support & D3D11_FORMAT_SUPPORT_TEXTURE2D) ) return false;

		rowPitch = ( twidth * bpp + 7 ) / 8;
		imageSize = rowPitch * theight;

		std::unique_ptr<uint8_t[]> temp( new (std::nothrow) uint8_t[ imageSize ] );
		if( !temp ) return false;

		if( twidth == width && theight == height )
		{
			RETURN_FAIL( frame->CopyPixels( 0, rowPitch, imageSize, temp.get() ) );
		}
		else
		{
			RETURN_FAIL( pWIC->CreateBitmapScaler( &scaler ) );
			RETURN_FAIL( scaler->Initialize( frame, twidth, theight, WICBitmapInterpolationModeFant ) );
			RETURN_FAIL( scaler->CopyPixels( 0, rowPitch, imageSize, temp.get() ) );
			SAFE_RELEASE( scaler );
		}

		SAFE_RELEASE( frame );

		if( support & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN )
		{
			autogen = true;
		}

		desc.Width = twidth;
		desc.Height = theight;
		desc.MipLevels = (autogen) ? 0 : 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.BindFlags = (autogen) ? D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET : D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = (autogen) ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		initData.pSysMem = temp.get();
		initData.SysMemPitch = rowPitch;
		initData.SysMemSlicePitch = imageSize;

		RETURN_FAIL( graphics->GetDevice()->CreateTexture2D( &desc, (autogen) ? nullptr : &initData, &m_texture2D ) );

		//ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
		memset( &SRVDesc, 0, sizeof( SRVDesc ) );
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = (autogen) ? -1 : 1;

		RETURN_FAIL( graphics->GetDevice()->CreateShaderResourceView( m_texture2D, &SRVDesc, &m_shaderResourceView ) );
		
		if( autogen )
		{
			graphics->GetContext()->UpdateSubresource( m_texture2D, 0, nullptr, temp.get(), rowPitch, imageSize );
			graphics->GetContext()->GenerateMips( m_shaderResourceView );
		}

		SAFE_RELEASE(pWIC);
	}

	ID3D11Texture2D *Image::GetTexture()
	{
		return m_texture2D;
	}

	ID3D11ShaderResourceView *Image::GetView()
	{
		return m_shaderResourceView;
	}
}
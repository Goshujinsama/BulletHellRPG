#include "Common.h"
#include "Graphics.h"
#include "Window.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace Engine
{
	Graphics::Graphics(Window* window) : m_window(window)
	{
		m_vsync_enabled = true;
		m_fullscreen = false;
		m_isReady = false;

		m_window->SetGraphics(this);

		return;
	}

	bool Graphics::Initialize()
	{
		IDXGIFactory					*factory;
		IDXGIAdapter					*adapter;
		IDXGIOutput						*adapterOutput;
		DXGI_MODE_DESC					*displayModeList;
		DXGI_ADAPTER_DESC				adapterDesc;
		DXGI_SWAP_CHAIN_DESC			swapChainDesc;
		D3D_FEATURE_LEVEL				featureLevel;
		D3D11_BLEND_DESC				blendDesc;
		D3D11_DEPTH_STENCIL_DESC		depthStencilDesc;
		D3D11_SAMPLER_DESC				samplerDesc;
		unsigned int numModes, i, numerator, denominator, stringLength;
		int error;

		m_screenSize.cx = m_window->GetSize().cx;
		m_screenSize.cy = m_window->GetSize().cy;

		RETURN_FAIL( CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory) );
		RETURN_FAIL( factory->EnumAdapters(0, &adapter) );
		RETURN_FAIL( adapter->EnumOutputs(0, &adapterOutput) );
		RETURN_FAIL( adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL) );

		displayModeList = new DXGI_MODE_DESC[numModes];

		RETURN_FAIL( adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList) );

		for(i = 0; i < numModes; ++i)
		{
			if(displayModeList[i].Width == m_screenSize.cx)
			{
				if(displayModeList[i].Height == m_screenSize.cy)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		RETURN_FAIL( adapter->GetDesc(&adapterDesc) );

		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		RETURN_ERROR( wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128) );

		SAFE_DELETE_ARRAY( displayModeList );
		SAFE_RELEASE(adapterOutput);
		SAFE_RELEASE(adapter);
		SAFE_RELEASE(factory);

		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 2;
		swapChainDesc.Windowed = true;
		swapChainDesc.BufferDesc.Width = m_screenSize.cx;
		swapChainDesc.BufferDesc.Height = m_screenSize.cy;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		if(m_vsync_enabled)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = m_window->GetHWnd();
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		featureLevel = D3D_FEATURE_LEVEL_11_0;
		RETURN_FAIL( D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext) );
		RETURN_FAIL( m_swapChain->GetParent(__uuidof(IDXGIFactory), (LPVOID*)&factory) );
		RETURN_FAIL( factory->MakeWindowAssociation(m_window->GetHWnd(), DXGI_MWA_NO_WINDOW_CHANGES) );

		ZeroMemory( &blendDesc, sizeof( blendDesc ) );
		blendDesc.AlphaToCoverageEnable = true;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		RETURN_FAIL( m_device->CreateBlendState( &blendDesc, &m_blendAlpha ) );

		m_deviceContext->OMSetBlendState( m_blendAlpha, NULL, 0xFFFFFF );

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		RETURN_FAIL( m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState) );

		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

		ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
		samplerDesc.AddressU        = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV        = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW        = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.ComparisonFunc  = D3D11_COMPARISON_NEVER;
		samplerDesc.Filter          = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.MaxLOD          = D3D11_FLOAT32_MAX;
		RETURN_FAIL( m_device->CreateSamplerState( &samplerDesc, &m_samplerLinear ) );

		return CreateTargets();
	}

	bool Graphics::CreateTargets()
	{
		ID3D11Texture2D					*backBufferPtr;
		D3D11_TEXTURE2D_DESC			depthBufferDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;

		SAFE_RELEASE(m_depthStencilView);
		SAFE_RELEASE(m_depthStencilState);
		SAFE_RELEASE(m_depthStencilBuffer);
		SAFE_RELEASE(m_renderTargetView);

		RETURN_FAIL( m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr) );
		RETURN_FAIL( m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView) );

		SAFE_RELEASE(backBufferPtr);

		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.Width = m_window->GetSize().cx;
		depthBufferDesc.Height = m_window->GetSize().cy;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		RETURN_FAIL( m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer) );

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		RETURN_FAIL( m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView) );

		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

		m_isReady = true;

		return true;
	}

	void Graphics::Shutdown()
	{
		m_swapChain->SetFullscreenState(false, NULL);

		SAFE_RELEASE(m_depthStencilView);
		SAFE_RELEASE(m_depthStencilState);
		SAFE_RELEASE(m_depthStencilBuffer);
		SAFE_RELEASE(m_renderTargetView);
		SAFE_RELEASE(m_deviceContext);
		SAFE_RELEASE(m_device);
		SAFE_RELEASE(m_swapChain);

		return;
	}

	void Graphics::Begin()
	{
		float color[4];
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		color[3] = 0.0f;

		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		return;
	}

	void Graphics::End()
	{
		if(m_vsync_enabled)
		{
			m_swapChain->Present(1, 0);
		}
		else
		{
			m_swapChain->Present(0, 0);
		}

		return;
	}

	void Graphics::SetViewPort( float x, float y, float width, float height, float minDepth, float maxDepth )
	{
		D3D11_VIEWPORT viewport = { x * m_screenSize.cx, y * m_screenSize.cy, width * m_screenSize.cx, height * m_screenSize.cy, minDepth, maxDepth };
		m_deviceContext->RSSetViewports( 1, &viewport );
	}

	bool Graphics::IsFullscreen()
	{
		return m_fullscreen;
	}

	void Graphics::SetFullscreen( bool fullscreen )
	{
		if(IsFullscreen() != fullscreen)
		{
			if(m_swapChain != NULL)
			{
				if(fullscreen)
				{
					SetResolution(1680, 1050);
					m_fullscreen = fullscreen;
					m_swapChain->SetFullscreenState(fullscreen, NULL);
				}
				else
				{
					m_fullscreen = fullscreen;
					m_swapChain->SetFullscreenState(fullscreen, NULL);
					SetResolution(800, 600);
				}
			}
		}

		return;
	}

	SIZE Graphics::GetResolution()
	{
		return m_screenSize;
	}

	void Graphics::SetResolution(unsigned int width, unsigned int height)
	{
		DXGI_MODE_DESC modeDesc;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		m_screenSize.cx = width;
		m_screenSize.cy = height;

		if(m_swapChain != NULL)
		{
			m_swapChain->GetDesc(&swapChainDesc);
			if(IsFullscreen() && swapChainDesc.Windowed)
			{
				SetResolution(800, 600);
			}
			else if(swapChainDesc.BufferDesc.Width != width || swapChainDesc.BufferDesc.Height != height)
			{
				m_isReady = false;
				SAFE_RELEASE(m_depthStencilView);
				SAFE_RELEASE(m_depthStencilState);
				SAFE_RELEASE(m_depthStencilBuffer);
				SAFE_RELEASE(m_renderTargetView);
				
				ZeroMemory(&modeDesc, sizeof(modeDesc));
				modeDesc.Format = DXGI_FORMAT_UNKNOWN;
				modeDesc.Width = width;
				modeDesc.Height = height;
				
				m_swapChain->ResizeTarget( &modeDesc );
			}
		}
	}

	void Graphics::ResizeBuffers(unsigned int width, unsigned int height)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		if(m_swapChain != NULL)
		{
			if(width == m_screenSize.cx && height == m_screenSize.cy)
			{
				m_deviceContext->OMSetRenderTargets(0, 0, 0);
				SAFE_RELEASE(m_renderTargetView);

				m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH );

				CreateTargets();
			}
		}
	}

	ID3D11Device *Graphics::GetDevice()
	{
		return m_device;
	}

	ID3D11DeviceContext	*Graphics::GetContext()
	{
		return m_deviceContext;
	}

	ID3D11RenderTargetView *Graphics::GetRenderTarget()
	{
		return m_renderTargetView;
	}
	
	ID3D11SamplerState *Graphics::GetSampler()
	{
		return m_samplerLinear;
	}

	bool Graphics::IsReady()
	{
		return m_isReady;
	}
}
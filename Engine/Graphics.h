#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

namespace Engine
{
	class Window;

	class Graphics
	{
	public:
		Graphics(Window *window);

		bool Initialize();
		void Shutdown();
		void Begin();
		void End();
		void SetViewPort( float x, float y, float width, float height, float minDepth, float maxDepth );

		bool IsFullscreen();
		void SetFullscreen( bool fullscreen );

		SIZE GetResolution();
		void SetResolution(unsigned int width, unsigned int height);
		void ResizeBuffers(unsigned int width, unsigned int height);

		ID3D11Device			*GetDevice();
		ID3D11DeviceContext		*GetContext();
		ID3D11RenderTargetView	*GetRenderTarget();
		ID3D11SamplerState		*GetSampler();

	private:
		bool				m_vsync_enabled;
		bool				m_fullscreen;
		SIZE				m_screenSize;

		Window				*m_window;

		int m_videoCardMemory;
		char m_videoCardDescription[128];
		IDXGISwapChain			*m_swapChain;
		ID3D11Device			*m_device;
		ID3D11DeviceContext		*m_deviceContext;
		ID3D11RenderTargetView	*m_renderTargetView;
		ID3D11Texture2D			*m_depthStencilBuffer;
		ID3D11DepthStencilState	*m_depthStencilState;
		ID3D11DepthStencilView	*m_depthStencilView;
		ID3D11BlendState		*m_blendAlpha;
		ID3D11SamplerState		*m_samplerLinear;

		bool CreateTargets();
	};
}

#endif // GRAPHICS_H
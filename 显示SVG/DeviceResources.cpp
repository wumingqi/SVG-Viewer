#include "Pch.h"
#include "DeviceResources.h"


DeviceResources::DeviceResources(HWND hWnd):
	m_hWnd(hWnd)
{
}

void DeviceResources::CreateDeviceIndependentResources()
{
	// D2D工厂
	ThrowIfFailed(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, 
		IID_PPV_ARGS(&m_d2dFactory)
	));
	m_d2dFactory->GetDesktopDpi(&m_dpi, &m_dpi);

	// DWrite工厂
	ThrowIfFailed(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dwriteFactory),
		&m_dwriteFactory
	));

	// WIC工厂
	ThrowIfFailed(CoCreateInstance(
		CLSID_WICImagingFactory2,
		nullptr,CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_wicFactory)
	));
}

void DeviceResources::CreateDeviceResources()
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	ThrowIfFailed(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&m_d3dDevice,
		&m_featureLevel,
		&m_d3dContext
	));

	ThrowIfFailed(m_d3dDevice.As(&m_dxgiDevice));

	ComPtr<ID2D1Device> d2dDevice;
	ThrowIfFailed(D2D1CreateDevice(
		m_dxgiDevice.Get(),
		CreationProperties(
			D2D1_THREADING_MODE_SINGLE_THREADED,
			D2D1_DEBUG_LEVEL_WARNING,
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE
		),
		&d2dDevice
	));
	ThrowIfFailed(d2dDevice.As(&m_d2dDevice));
	ThrowIfFailed(
		m_d2dDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_d2dContext
		)
	);
}

void DeviceResources::CreateWindowSizeDependentResources()
{
	if (m_swapChain != nullptr)
	{
		if (m_targetSize.width&&m_targetSize.height)
		{
			ID3D11RenderTargetView* nullViews[] = { nullptr };
			m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
			m_d2dContext->SetTarget(nullptr);
			m_d2dTargetBitmap = nullptr;
			m_d3dContext->Flush();

			ThrowIfFailed(m_swapChain->ResizeBuffers(
				2,
				m_targetSize.width,
				m_targetSize.height,
				DXGI_FORMAT_B8G8R8A8_UNORM, 0));
		}
	}
	else
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

		swapChainDesc.Width = m_targetSize.width; // Match the size of the window.
		swapChainDesc.Height = m_targetSize.height;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Runtime apps must use this SwapEffect.
		swapChainDesc.Flags = 0;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE; // When using XAML interop, this value cannot be DXGI_ALPHA_MODE_PREMULTIPLIED.
		swapChainDesc.Scaling = DXGI_SCALING_NONE;

#ifdef _DEBUG
		UINT dxgiFlags = DXGI_CREATE_FACTORY_DEBUG;
#else
		UINT dxgiFlags = 0;
#endif
		ComPtr<IDXGIFactory7> dxgiFactory;
		ThrowIfFailed(CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(&dxgiFactory)));
		ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
			m_d3dDevice.Get(),
			m_hWnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&m_swapChain
		));
	}

	ComPtr<IDXGISurface2> dxgiBackBuffer;
	ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)));
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED),
		m_dpi,
		m_dpi);
	ThrowIfFailed(m_d2dContext->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer.Get(),
		&bitmapProperties,
		&m_d2dTargetBitmap));
	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
	//m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	ThrowIfFailed(m_swapChain->SetBackgroundColor(&ColorF(0, 0)));
}

void DeviceResources::SetTargetSize(UINT width, UINT height)
{
	m_targetSize.width = width;
	m_targetSize.height = height;
}


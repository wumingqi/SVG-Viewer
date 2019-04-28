#pragma once
class DeviceResources
{
	// 窗口资源
	HWND								m_hWnd;
	D2D1_SIZE_U							m_targetSize;
	float								m_dpi;

	// Direct2D设备资源
	ComPtr<ID2D1Factory7>				m_d2dFactory;
	ComPtr<ID2D1Device6>				m_d2dDevice;
	ComPtr<ID2D1DeviceContext6>			m_d2dContext;
	ComPtr<ID2D1Bitmap1>				m_d2dTargetBitmap;

	// DirectWrite资源
	ComPtr<IDWriteFactory7>				m_dwriteFactory;

	// WIC资源
	ComPtr<IWICImagingFactory2>			m_wicFactory;

	// D3D资源
	ComPtr<ID3D11Device>				m_d3dDevice;
	ComPtr<ID3D11DeviceContext>			m_d3dContext;
	D3D_FEATURE_LEVEL					m_featureLevel;
	ComPtr<IDXGIDevice4>				m_dxgiDevice;
	ComPtr<IDXGISwapChain1>				m_swapChain;

public:
	DeviceResources(HWND hWnd);
	void CreateDeviceIndependentResources();
	void CreateDeviceResources();
	void CreateWindowSizeDependentResources();
	void SetTargetSize(UINT width, UINT height);

public:
	ID2D1Factory7*			GetD2DFactory()			const	{ return m_d2dFactory.Get(); }
	ID2D1Device6*			GetD2DDevice()			const	{ return m_d2dDevice.Get(); }
	ID2D1DeviceContext6*	GetD2DDeviceContext()	const	{ return m_d2dContext.Get(); }
	IDWriteFactory7*		GetDWriteFactory()		const	{ return m_dwriteFactory.Get(); }
	IWICImagingFactory2*	GetWicImagingFactory()	const	{ return m_wicFactory.Get(); }
	IDXGISwapChain1*		GetSwapChain()			const	{ return m_swapChain.Get(); }
};


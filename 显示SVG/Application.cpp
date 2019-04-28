#include "Pch.h"
#include "Application.h"

void Application::InitializeResources()
{
	m_deviceResources = new DeviceResources(m_hWnd);
	m_deviceResources->CreateDeviceIndependentResources();
	m_deviceResources->CreateDeviceResources();
	m_d2dContext = m_deviceResources->GetD2DDeviceContext();
	m_d2dContext->CreateSolidColorBrush(ColorF(0xFF2244), &m_brush);

	//加载资源文件中的SVG矢量图
	MemoryData svgData = Helper::LocateResource(MAKEINTRESOURCE(IDR_SVG_Strawberry), L"SVG");
	ComPtr<IStream> xmlInputStream = Helper::CreateStream(svgData);
	m_d2dContext->CreateSvgDocument(xmlInputStream.Get(), { 256,256 }, &m_svg);

	//加载资源文件中的位图
	MemoryData bmpData = Helper::LocateResource(MAKEINTRESOURCE(IDR_PNG_VisualStudio), L"PNG");
	ComPtr<IStream> bmpInputStream = Helper::CreateStream(bmpData);
	m_bmp = Helper::LoadBitmapSource(bmpInputStream.Get(), m_deviceResources->GetWicImagingFactory(), m_deviceResources->GetD2DDeviceContext());

	HorizontalAlignment hAligns[] = { HorizontalAlignment::left,HorizontalAlignment::center,HorizontalAlignment::right };
	VerticalAlignment vAligns[] = { VerticalAlignment::top,VerticalAlignment::center,VerticalAlignment::bottom };

	for (int i = 0; i < _countof(hAligns); i++)
	{
		for (int j = 0; j < _countof(vAligns); j++)
		{
			Image image(m_bmp.Get());
			image.horizontalAlignment = hAligns[i];
			image.verticalAlignment = vAligns[j];
			m_children.push_back(image);
		}
	}
}

void Application::Update()
{
	m_d2dContext = m_deviceResources->GetD2DDeviceContext();
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(ColorF(0xFFFFFF));
	
	for(auto child:m_children)
	{
		child.Render(m_d2dContext.Get());
	}

	float radius = 128.f;
	m_svg->SetViewportSize({ radius * 2,radius * 2 });
	m_d2dContext->SetTransform(Matrix3x2F::Translation(
		static_cast<float>(m_width / 2 - radius),
		static_cast<float>(m_height / 2 - radius)));
	m_d2dContext->DrawSvgDocument(m_svg.Get());
	m_d2dContext->EndDraw();
	m_deviceResources->GetSwapChain()->Present(1, 0);
}

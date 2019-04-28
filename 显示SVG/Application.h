#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include "DeviceResources.h"

class Application
{
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	UINT m_width, m_height;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT Handle_WM_CREATE(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_PAINT(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_SIZE(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_RBUTTONDOWN(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_RBUTTONUP(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_MOUSEWHEEL(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_KEYDOWN(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_DESTROY(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_DROPFILES(WPARAM wParam, LPARAM lParam);
	LRESULT Handle_WM_COMMAND(WPARAM wParam, LPARAM lParam);

	DeviceResources*				m_deviceResources;
	ComPtr<ID2D1DeviceContext6>		m_d2dContext;
	ComPtr<ID2D1SolidColorBrush>	m_brush;
	ComPtr<ID2D1SvgDocument>		m_svg;
	ComPtr<ID2D1Bitmap>				m_bmp;

	std::list<Image>			m_children;

	void InitializeResources();
	void Update();

public:Application(UINT width, UINT height, HINSTANCE hInstance) :
			m_width(width),
			m_height(height),
			m_hInstance(hInstance)
		{}
	   ~Application()
	   {
		   delete m_deviceResources;
	   }

	   int Run(int nCmdShow)
	   {
		   WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
		   wc.style = CS_VREDRAW | CS_HREDRAW;
		   wc.lpfnWndProc = WndProc;
		   wc.cbClsExtra = 0;
		   wc.cbWndExtra = 0;
		   wc.hInstance = m_hInstance;
		   wc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
		   wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		   wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		   wc.lpszMenuName = nullptr;
		   wc.lpszClassName = L"HelloWindows";
		   wc.hIconSm = nullptr;

		   RegisterClassEx(&wc);

		   DWORD style = WS_OVERLAPPEDWINDOW; DWORD styleEx = 0;
		   RECT rc = { 0,0,(LONG)m_width,(LONG)m_height };
		   AdjustWindowRectEx(&rc, style, false, styleEx);
		   auto cx = GetSystemMetrics(SM_CXSCREEN);
		   auto cy = GetSystemMetrics(SM_CYFULLSCREEN);
		   auto w = rc.right - rc.left;
		   auto h = rc.bottom - rc.top;
		   auto x = (cx - w) / 2;
		   auto y = (cy - h) / 2;

		   m_hWnd = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP, wc.lpszClassName, L"Windows", WS_OVERLAPPEDWINDOW,
			   x, y, w, h, nullptr, nullptr, m_hInstance, nullptr);

		   InitializeResources();
		   SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		   ShowWindow(m_hWnd, nCmdShow);

		   MSG msg = {};
		   while (GetMessage(&msg, nullptr, 0, 0))
		   {
			   TranslateMessage(&msg);
			   DispatchMessage(&msg);
		   }
		   return static_cast<int>(msg.wParam);
	   }
};

#endif // !APPLICATION_H

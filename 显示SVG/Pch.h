#pragma once
#ifndef PCH_H
#define PCH_H

#include <wrl.h>
using namespace Microsoft::WRL;

#include <d2d1_3.h>
#pragma comment(lib, "d2d1.lib")
using namespace D2D1;

#include <d3d11_4.h>
#pragma comment(lib, "d3d11.lib")

#include <dwrite_3.h>
#pragma comment(lib, "dwrite.lib")

#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

#include <dcomp.h>
#pragma comment(lib, "dcomp.lib")

#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <stdexcept>
#include <list>
#include <errors.h>
#pragma comment(lib, "Quartz.lib")

#include "resource.h"

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		char msg[MAX_ERROR_TEXT_LEN];
		AMGetErrorTextA(hr, msg, MAX_ERROR_TEXT_LEN);
		throw new std::runtime_error(msg);
	}
}

struct MemoryData
{
	LPBYTE pData;
	UINT cbData;
};


enum class HorizontalAlignment :DWORD
{
	left = 0,
	center = 1,
	right = 2,
	stretch = 4
};

enum class VerticalAlignment :DWORD
{
	top = 0,
	center = 1,
	bottom = 2,
	stretch = 4
};

enum class Visibility :DWORD
{
	Collapsed = 1,
	Visible = 0
};


struct Helper
{
private:
	static ComPtr<IWICFormatConverter> ConvertBitmapFormat(
		IWICImagingFactory* wicFactory,
		IWICBitmapDecoder* bitmapDecoder
	);
public:
	/********************************************************************************
	*					锁定资源的位置												*
	********************************************************************************/
	static MemoryData LocateResource(LPCTSTR lpName, LPCTSTR lpType);
	/********************************************************************************
	*					创建流的函数。IStream接口很多方法都会用到。						*
	********************************************************************************/
	static ComPtr<IStream> CreateStream(
		LPCTSTR filename
	);
	static ComPtr<IStream> CreateStream(
		const BYTE* pData,
		UINT cbData
	);
	static ComPtr<IStream> CreateStream(
		MemoryData dataDesc
	);
	/********************************************************************************
	*					加载位图的函数。ID2D1Bitmap接口用来显示图片。					*
	********************************************************************************/
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		LPCTSTR filename,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		ULONG_PTR hFile,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		IStream* stream,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		HBITMAP hBitmap,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);
	static ComPtr<ID2D1Bitmap> LoadBitmapSource(
		HICON hIcon,
		IWICImagingFactory* wicFactory,
		ID2D1RenderTarget* renderTarget
	);

	static D2D1_SIZE_F ComputeElementOffset(
		D2D1_SIZE_F targetSize,
		D2D1_SIZE_F elementSize,
		HorizontalAlignment hAlign,
		VerticalAlignment vAlign
	);
	
	template<typename T>
	static float DPI(T p, float dpi)
	{
		return p / (144 / dpi);
	}
};


class IUIElement
{
public:
	HorizontalAlignment		horizontalAlignment;
	VerticalAlignment		verticalAlignment;
	Visibility				visibility;

	D2D1_SIZE_F				size;

	virtual void Render(ID2D1RenderTarget* renderTarget) = 0;
};

class Image :public IUIElement
{
	ComPtr<ID2D1Bitmap>		bitmap;
public:
	Image(ID2D1Bitmap* bitmap)
	{
		this->bitmap = bitmap;
		this->size = bitmap->GetSize();
	}


	// 通过 IUIElement 继承
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

};

#endif // !PCH_H

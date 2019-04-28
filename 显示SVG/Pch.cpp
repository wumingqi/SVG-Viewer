#include "Pch.h"

ComPtr<IWICFormatConverter> Helper::ConvertBitmapFormat(IWICImagingFactory* wicFactory, IWICBitmapDecoder* bitmapDecoder)
{
	ComPtr<IWICBitmapFrameDecode>	bitmapFrame;
	ComPtr<IWICFormatConverter>		formatConverter;
	bitmapDecoder->GetFrame(0, &bitmapFrame);

	wicFactory->CreateFormatConverter(&formatConverter);
	formatConverter->Initialize(
		bitmapFrame.Get(),
		GUID_WICPixelFormat32bppPRGBA,
		WICBitmapDitherTypeNone,
		nullptr,
		1.0,
		WICBitmapPaletteTypeCustom);
	return formatConverter;
}

MemoryData Helper::LocateResource(LPCTSTR lpName, LPCTSTR lpType)
{
	MemoryData data = {};
	HRSRC resHandle = FindResource(nullptr, lpName, lpType);
	HGLOBAL resDataHandle = LoadResource(nullptr, resHandle);
	data.pData = (LPBYTE)LockResource(resDataHandle);
	data.cbData = SizeofResource(nullptr, resHandle);
	return data;
}


/********************************************************************************
*					创建流的函数。IStream接口很多方法都会用到。						*
********************************************************************************/

ComPtr<IStream> Helper::CreateStream(LPCTSTR filename)
{
	ComPtr<IStream> stream;
	SHCreateStreamOnFile(filename, STGM_READ, &stream);
	return stream;
}

ComPtr<IStream> Helper::CreateStream(const BYTE* pData, UINT cbData)
{
	return SHCreateMemStream(pData, cbData);
}

ComPtr<IStream> Helper::CreateStream(MemoryData dataDesc)
{
	return CreateStream(dataDesc.pData, dataDesc.cbData);
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(LPCTSTR filename, IWICImagingFactory * wicFactory, ID2D1RenderTarget * renderTarget)
{
	ComPtr<IWICBitmapDecoder>		bitmapDecoder;

	wicFactory->CreateDecoderFromFilename(
		filename, 
		nullptr, 
		GENERIC_READ, 
		WICDecodeMetadataCacheOnDemand, 
		&bitmapDecoder);
	
	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		ConvertBitmapFormat(wicFactory, bitmapDecoder.Get()).Get(), 
		&bitmap);
	return bitmap;
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(ULONG_PTR hFile, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	ComPtr<IWICBitmapDecoder>		bitmapDecoder;

	wicFactory->CreateDecoderFromFileHandle(
		hFile,
		nullptr,
		WICDecodeMetadataCacheOnDemand,
		&bitmapDecoder);

	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		ConvertBitmapFormat(wicFactory, bitmapDecoder.Get()).Get(),
		&bitmap);
	return bitmap;
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(IStream* stream, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	ComPtr<IWICBitmapDecoder>		bitmapDecoder;

	wicFactory->CreateDecoderFromStream(
		stream,
		nullptr,
		WICDecodeMetadataCacheOnDemand,
		&bitmapDecoder);

	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		ConvertBitmapFormat(wicFactory, bitmapDecoder.Get()).Get(),
		&bitmap);
	return bitmap;
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(HBITMAP hBitmap, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	ComPtr<IWICBitmap>		wicBitmap;
	wicFactory->CreateBitmapFromHBITMAP(
		hBitmap,
		nullptr, WICBitmapAlphaChannelOption::WICBitmapUsePremultipliedAlpha,
		&wicBitmap
	);

	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		wicBitmap.Get(),
		&bitmap);
	return bitmap;
}

ComPtr<ID2D1Bitmap> Helper::LoadBitmapSource(HICON hIcon, IWICImagingFactory* wicFactory, ID2D1RenderTarget* renderTarget)
{
	ComPtr<IWICBitmap>		wicBitmap;
	wicFactory->CreateBitmapFromHICON(
		hIcon,
		&wicBitmap
	);

	ComPtr<ID2D1Bitmap> bitmap;
	renderTarget->CreateBitmapFromWicBitmap(
		wicBitmap.Get(),
		&bitmap);
	return bitmap;
}

D2D1_SIZE_F Helper::ComputeElementOffset(D2D1_SIZE_F targetSize, D2D1_SIZE_F elementSize, HorizontalAlignment hAlign, VerticalAlignment vAlign)
{
	D2D1_SIZE_F offset = {};
	switch (hAlign)
	{
	case HorizontalAlignment::left:		offset.width = 0; break;
	case HorizontalAlignment::center:	offset.width = (targetSize.width - elementSize.width) / 2; break;
	case HorizontalAlignment::right:	offset.width = targetSize.width - elementSize.width; break;
	case HorizontalAlignment::stretch:	break;
	}
	switch (vAlign)
	{
	case VerticalAlignment::top:		offset.height = 0; break;
	case VerticalAlignment::center:		offset.height = (targetSize.height - elementSize.height) / 2; break;
	case VerticalAlignment::bottom:		offset.height = targetSize.height - elementSize.height; break;
	case VerticalAlignment::stretch:	break;
	}

	return offset;
}

void Image::Render(ID2D1RenderTarget* renderTarget)
{
	if (visibility == Visibility::Visible)
	{
		D2D1_SIZE_F offset = Helper::ComputeElementOffset(renderTarget->GetSize(), size, horizontalAlignment, verticalAlignment);

		renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(offset));
		renderTarget->DrawBitmap(bitmap.Get(), {0,0,size.width,size.height});
	}
}

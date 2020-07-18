#include <iostream>
#include <atlimage.h>
#include <GdiPlus.h>
#include <map>
#pragma comment( lib, "gdiplus" )


using namespace std;
using namespace Gdiplus;

int Height = GetSystemMetrics(SM_CYSCREEN);
int Width = GetSystemMetrics(SM_CXSCREEN);

map<int, int> allPixels = {};

map<map<int, int>, Color> img1;
map<map<int, int>, Color> img2;

bool change = false;
bool compare = false;

//Sets all the pixels in the screen on allPixels variable and returning a map of map<int , int>
map<int, int> getAllPixels(map<int, int> allPixels)
{
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			allPixels.insert({i, j});
		}
	}
	return allPixels;
}

//Saving all the pixels with a color value and returning a map of map<map<int, int>, Color>
void getPixelsColor(Bitmap& bitmap, map<map<int, int>, Color> pixelsColor, map<int, int> allPixels)
{
	Color color;
	for (auto itr = allPixels.begin(); itr != allPixels.end(); ++itr)
	{
		bitmap.GetPixel(itr->first, itr->second, &color);
		pixelsColor.insert({{itr->first, itr->second}, color});
	}
}


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT num = 0; // number of image encoders
	UINT size = 0; // size of the image encoder array in bytes
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1; // Failure
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1; // Failure
	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j; // Success
		}
	}
	free(pImageCodecInfo);
	return 0;
}


void gdiscreen(const wchar_t* path)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{
		HDC scrdc, memdc;
		HBITMAP membit;
		scrdc = ::GetDC(0);
		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, Width, Height);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
		BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
		Bitmap bitmap(membit, NULL);
		CLSID clsid;
		GetEncoderClsid(L"image/jpeg", &clsid);

		bitmap.Save(path, &clsid);


		if (!change)
		{
			getPixelsColor(bitmap, img1, allPixels);
			change = true;
		}else{
			getPixelsColor(bitmap, img2, allPixels);
			change = false;
		}


		
		SelectObject(memdc, hOldBitmap);
		DeleteObject(memdc);
		DeleteObject(membit);
		::ReleaseDC(0, scrdc);
	}
	GdiplusShutdown(gdiplusToken);
}

//Comaparing two images and returning a map<map<int , int> , Color>
map<map<int, int>, Color> compare_maps(map<map<int, int>, Color> img1Pixels, map<map<int, int>, Color> img2Pixels)
{
	if (!compare)
		return img1;

	//compare between img1 to img2 and change img2
	if(change)
	{


		

		
	}
	
	
	
	
}



int main()
{
	getAllPixels(allPixels);


	return 1;
}

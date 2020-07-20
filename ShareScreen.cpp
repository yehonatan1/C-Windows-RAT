#include <iostream>
#include <atlimage.h>
#include <GdiPlus.h>
#include <WS2tcpip.h>
#include <map>

#pragma comment( lib, "gdiplus" )


using namespace std;
using namespace Gdiplus;

int Height = GetSystemMetrics(SM_CYSCREEN);
int Width = GetSystemMetrics(SM_CXSCREEN);

map<int, int> allPixels = {};

map <map<int, int>, Color> img1;
map <map<int, int>, Color> img2;

bool change = false;
bool compare = false;

//Sets all the pixels in the screen on allPixels variable and returning a map of map<int , int>
map<int, int> getAllPixels(map<int, int> allPixels) {
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            allPixels.insert({i, j});
        }
    }
    return allPixels;
}

//Saving all the pixels with a color value and returning a map of map<map<int, int>, Color>
void getPixelsColor(Bitmap &bitmap, map <map<int, int>, Color> pixelsColor, map<int, int> allPixels) {
    Color color;
    for (auto itr = allPixels.begin(); itr != allPixels.end(); ++itr) {
        bitmap.GetPixel(itr->first, itr->second, &color);
        pixelsColor.insert({{itr->first, itr->second}, color});
    }
}


int GetEncoderClsid(const WCHAR *format, CLSID *pClsid) {
    using namespace Gdiplus;
    UINT num = 0; // number of image encoders
    UINT size = 0; // size of the image encoder array in bytes
    ImageCodecInfo *pImageCodecInfo = NULL;
    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1; // Failure
    pImageCodecInfo = (ImageCodecInfo * )(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1; // Failure
    GetImageEncoders(num, size, pImageCodecInfo);
    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j; // Success
        }
    }
    free(pImageCodecInfo);
    return 0;
}

void gdiscreen(const wchar_t *path) {
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    {
        HDC scrdc, memdc;
        HBITMAP membit;
        scrdc = ::GetDC(0);
        memdc = CreateCompatibleDC(scrdc);
        membit = CreateCompatibleBitmap(scrdc, Width, Height);
        HBITMAP hOldBitmap = (HBITMAP) SelectObject(memdc, membit);
        BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
        Bitmap bitmap(membit, NULL);
        CLSID clsid;
        GetEncoderClsid(L"image/jpeg", &clsid);

        bitmap.Save(path, &clsid);


        if (!change) {
            getPixelsColor(bitmap, img1, allPixels);
            change = true;
        } else {
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
map <map<int, int>, Color>
compare_maps(map <map<int, int>, Color> &img1Pixels, map <map<int, int>, Color> &img2Pixels) {
    if (!compare)
        return img1;


    map <map<int, int>, Color> imgToSend = {};
    //compare between img1 to img2 and change img2
    if (change) {
        for (auto itr = img1Pixels.begin(); itr != img1Pixels.end(); ++itr) {
            if (img1Pixels[itr->first].GetValue() != img2Pixels[itr->first].GetValue());
            {
                imgToSend[itr->first] = img2Pixels[itr->first];
            }
        }
        return imgToSend;
    }

    for (auto itr = img2Pixels.begin(); itr != img2Pixels.end(); ++itr) {
        if (img2Pixels[itr->first].GetValue() != img1Pixels[itr->first].GetValue());
        {
            imgToSend[itr->first] = img1Pixels[itr->first];
        }
    }
    return imgToSend;
}


void sendData(SOCKET socket, map <map<int, int>, Color> data) {

    send(socket, static_cast<const string>((const char *const) &data).c_str(), sizeof(data), 0);
}

void client(string ip, int port) {
    WSADATA data;
    WORD ver = MAKEWORD(ver, &data);
    int resualt = WSAStartup(ver, &data);

    if (resualt != 1) {
        cerr << "Cant start Winsock" << endl;
        return;
    }
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Cant create socket" << WSAGetLastError << endl;
        WSACleanup();
        return;
    }
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

    int connResult = connect(sock, (sockaddr * ) & hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        cerr << "Cant connect to server" << WSAGetLastError << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }
    string userInput;

    while (userInput != "exit") {
        cout << "Please enter a command" << endl;
        cin >> userInput;

        if (userInput == "send") {
            gdiscreen(L"C:\\C projects\\a.jpg");
            sendData(sock, compare_maps(img1, img2));
        }
    }
}


int main() {
    getAllPixels(allPixels);
    client("127.0.0.1", 7613);
    return 1;
}

testdfsafdsafdsa
fdsafdsa
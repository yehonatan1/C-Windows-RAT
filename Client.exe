#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <inaddr.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;


void Client() {
    string ipAdress = "127.0.0.1"; //IP Address pf the server
    int port = 7943; //Listtening port # on server


    // Initialize Winsock

    WSADATA data;
    WORD ver = MAKEWORD(2, 2);
    int wsResualt = WSAStartup(ver, &data);

    if (wsResualt != 0) {
        cerr << "Cant start Winsock";
        return;
    }

    // Create socket

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Cant create socket" << WSAGetLastError << endl;
        WSACleanup();
        return;
    }
    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        cerr << "Cant connect to server" << WSAGetLastError << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    //Do-while loop to send and receive data
    char buf[4096];
    string userInput;

    do {
        //Prompt the user for some text
        cout << "> " << endl;
        getline(cin, userInput);

        //Send the text
        if (userInput.size() > 0) {
            int sendResault = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResault != SOCKET_ERROR) {
                // Wait fir response
                ZeroMemory(buf, 4096);
                int bytesRecived = recv(sock, buf, 4096, 0);
                if (bytesRecived > 0) {
                    // Echo  response to console
                    cout << "Server " << string(buf, 0, bytesRecived) << endl;
                }
            }
        }

        //Wait for response
    } while (userInput.size() > 0);

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();
}


int main() {
    cout << "Hello" << endl;
    Client();
}

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;

int main(){
    // WSAStartup
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wserr = WSAStartup(wVersionRequested, &wsaData);
    if(wserr != 0){
        cout << "The winsock dll not found" << endl;
        return 0;
    } else {
        cout << "The Winsock dll found" << endl;
        cout << "The status: " << wsaData.szSystemStatus << endl;
    }

    // Socket creation
    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket == INVALID_SOCKET){
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    } else {
        cout << "socket is OK!" << endl;
    }

    // Connection to server
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("3.8.190.125"); // Server IP address
    clientService.sin_port = htons(13000); // Update this to match the new server port
    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR){
        cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    } else {
        cout << "Client: Connect() is OK!" << endl;
        cout << "Client: Can start sending and receiving data..." << endl;
    }

    // Sending data
    char buffer[200];
    printf("Enter the message: ");
    cin.getline(buffer,200);
    int sbyteCount = send(clientSocket, buffer, strlen(buffer), 0); // Use strlen(buffer) instead of 200 to send the actual message length
    if(sbyteCount == SOCKET_ERROR){
        cout << "Server send error: " << WSAGetLastError() << endl;
        return -1;
    } else {
        cout << "Server: sent " << sbyteCount << " bytes" << endl;
    }

    // Properly close the socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}

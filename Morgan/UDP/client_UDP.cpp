#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

int main(){
    // WSAStartup
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wserr = WSAStartup(wVersionRequested, &wsaData);
    if(wserr != 0){
        std::cout << "The winsock dll not found" << std::endl;
        return 0;
    } else {
        std::cout << "The Winsock dll found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
    }

    // Socket creation
    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(clientSocket == INVALID_SOCKET){
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    } else {
        std::cout << "socket is OK!" << std::endl;
    }

    // Connection to server
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(12000); // Ensure the port is correct
    inet_pton(AF_INET, "3.8.190.125", &clientService.sin_addr); // Ensure the IP is correct

    

    // Sending data
    char buffer[200];
    printf("Enter the message: ");
    std::cin.getline(buffer,200);
int sbyteCount = sendto(clientSocket, buffer, strlen(buffer), 0, (SOCKADDR*)&clientService, sizeof(clientService));
    if(sbyteCount == SOCKET_ERROR){
        std::cout << "Server send error: " << WSAGetLastError() << std::endl;
        return -1;
    } else {
        std::cout << "Server: sent " << sbyteCount << " bytes" << std::endl;
    }

    // Properly close the socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}

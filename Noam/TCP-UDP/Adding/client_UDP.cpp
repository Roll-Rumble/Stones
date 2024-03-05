#include <cstdint>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

struct data_to_send {
    int16_t x;
    int16_t y;
};

void WSAStartup() {
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wserr = WSAStartup(wVersionRequested, &wsaData);
    if(wserr != 0){
        std::cout << "The winsock dll not found" << std::endl;
        return;
    } else {
        std::cout << "The Winsock dll found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
    }
}

void SocketCreation(SOCKET &clientSocket) { // Pass clientSocket by reference
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(clientSocket == INVALID_SOCKET){
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    } else {
        std::cout << "socket is OK!" << std::endl;
    }
}

void send_data(SOCKET clientSocket,sockaddr_in clientService, std::pair<int16_t,int16_t> input_vals) { //data will be a pair of integers

    data_to_send data_being_sent;
    data_being_sent.x = htons(input_vals.first);
    data_being_sent.y = htons(input_vals.second);

    int sbyteCount = sendto(clientSocket, (const char*) &data_being_sent, sizeof(data_being_sent), 0, (SOCKADDR*)&clientService, sizeof(clientService));
    if(sbyteCount == SOCKET_ERROR){
        std::cout << "Server send error: " << WSAGetLastError() << std::endl;
        return;
    } else {
        std::cout << "Server: sent " << sbyteCount << " bytes" << std::endl;
    }
}

int main(){
    // WSAStartup
    WSAStartup();

    // Socket creation
    SOCKET clientSocket;
    SocketCreation(clientSocket); // Pass clientSocket by reference

    // Connection to server
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(12000); // Ensure the port is correct
    inet_pton(AF_INET, "35.178.225.231", &clientService.sin_addr); // Ensure the IP is correct

    // Sending data
    send_data(clientSocket,clientService);

    // Properly close the socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}

#include "winsock_start.hpp"

#include "netutils.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

void start_WSA() {
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wserr = WSAStartup(wVersionRequested, &wsaData);
    if (wserr) {
        throw NetworkException("winsock dll not found");
    }
}

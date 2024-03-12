#ifndef CLIENT_TCP_HPP
#define CLIENT_TCP_HPP

#include <string>
#include <winsock2.h>


// TODO: MOVE THIS TO NETUTIL
#define SERVER_TCP_PORT 13000


#define SEND_BUF_SIZE
#define RECEIVE_BUF_SIZE

class TCPClient {
public:
    TCPClient();
    ~TCPClient();

    void send_data(char buffer[SEND_BUF_SIZE]);
    void receive(char buffer[RECEIVE_BUF_SIZE]);

private:
    SOCKET socket_;
};

#endif

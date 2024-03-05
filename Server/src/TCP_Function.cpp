#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

std::string wait_for_tcp() {
    std::cout << "We're in TCP server..." << std::endl;

    // Select server port
    int server_port = 13000;

    // Create welcome socket
    int welcome_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcome_socket == -1) { // If socket creation fails, print error message and exit
        std::cerr << "Socket creation failed" << std::endl;
        throw "Socket creation failed";
    }

    // Bind server to local host at port server_port (12000)
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address; INADDR_ANY allows to have multiple clients connected
    server_addr.sin_port = htons(server_port); 

    if (bind(welcome_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        // If bind fails, print error message and exit
        std::cerr << "Bind failed" << std::endl;
        throw "Bind failed";
    }

    listen(welcome_socket, 1);

    // Message to indicate that server is running
    std::cout << "Server running on port " << server_port << std::endl;

    sockaddr_in client_addr; // Client address
    socklen_t client_len = sizeof(client_addr);

    int connection_socket = accept(welcome_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len); // Accept the connection

    if (connection_socket == -1) {
        std::cerr << "Error accepting connection" << std::endl;
        throw "Error accepting connection";
    }



    
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	close(connection_socket);
    close(welcome_socket);

	std::cout << std::string(client_ip) << std::endl;
	return std::string(client_ip);
}

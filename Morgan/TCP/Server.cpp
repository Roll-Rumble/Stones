#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    std::cout << "We're in TCP server..." << std::endl;

    // Select server port
    int server_port = 13000;

    // Create welcome socket
    int welcome_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcome_socket == -1) { // If socket creation fails, print error message and exit
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
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
        return 1;
    }

    listen(welcome_socket, 1);

    // Message to indicate that server is running
    std::cout << "Server running on port " << server_port << std::endl;

    sockaddr_in client_addr; // Client address
    socklen_t client_len = sizeof(client_addr);

    int connection_socket = accept(welcome_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len); // Accept the connection

    if (connection_socket == -1) {
        std::cerr << "Error accepting connection" << std::endl;
        return 1;
    }

    char buffer[1024]; // Used to hold data from the client; array of characters
    int bytes_received;

    while (true) {
    // Receive data from the client
    bytes_received = recv(connection_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) { // Error receiving data or client closed connection
        break;
    }

    std::string received_data; // Used to hold the received data

    // Null-terminate the received data
    buffer[bytes_received] = 0;

    // Append received data to a string
    received_data.append(buffer);

    // Check if the received data contains a complete message
    int pos;
    while ((pos = received_data.find(" ")) != std::string::npos) {
        // Extract a token from the received data
        std::string token = received_data.substr(0, pos);

        // Process the token
        std::string processed_data = token;
        send(connection_socket, processed_data.c_str(), processed_data.length(), 0);

        // Remove the token from the received data
        received_data.erase(0, pos + 1);
    }
}



    std::cout << buffer << std::endl; 

    close(connection_socket);
    close(welcome_socket);

    return 0;
}
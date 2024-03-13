#include <iostream>

#include "networking.hpp"
#include "ball.hpp"
#include "map.hpp"

#define NUM_CLIENTS 2

int main()
{
	TCPServ tcp_serv;
	Map map;
	std::vector<std::pair<std::string, int>> client_addrs = tcp_serv.get_connections(NUM_CLIENTS);

	std::vector<UDPServ *> udp_handlers;
	std::vector<Ball> balls;

    for (int i = 0; i < NUM_CLIENTS; i++) {
        tcp_serv.send_xy(i, (float) i, (float) i);
        std::cout << "About to do emplace number " << i << "\n";
        udp_handlers.emplace_back(new UDPServ(client_addrs[i].first, i));
        std::cout << "Finished emplace number " << i << "\n";
        balls.emplace_back(map, 0);
    }

    std::cout << "Size of udp handlers is " << udp_handlers.size() << "\n";

    // DEBUG PRINTS
    std::cout << "Client 0 has address " << client_addrs[0].first << " and port number "
              <<  client_addrs[0].second << "\n";
    std::cout << "Client 1 has address " << client_addrs[1].first << " and port number "
              <<  client_addrs[1].second << "\n";



    // for (int i = 0; i < NUM_CLIENTS; i++) {
    //     udp_handlers.emplace_back(client_addrs[i], i);

    // }

	// for (std::pair<std::string, int> &addr_and_port : client_addrs) {
	// 	udp_handlers.emplace_back(addr_and_port.first, addr_and_port.second);
	// 	balls.emplace_back(map);
	// }

	std::vector<std::pair<int16_t, int16_t>> input = {{0,0}, {0,0}};
	while (true) {
        // std::cout << "Number of balls is " << udp_handlers.size() << "\n";
		for (int i = 0; i < udp_handlers.size(); i++) {
		// 	int prev_input = input.first;
            // std::cout << "udp handlers size is " << udp_handlers.size() << "\n";
            std::pair<int16_t,int16_t> prev_input = input[i];
            // std::cout << "About to try to receive on UDP from client " << i << "\n";
			input[i] = udp_handlers[i]->recv_xy(input[i]);
            // std::cout << "Successfully executed receive function from client" << i << "\n";
            if (input[i] != prev_input && udp_handlers.size() == 2) {
                std::cout << "Sending data to client " << !i << "\n";
                std::cout << "Data being sent is x: " << input[i].first << ", y: " << input[i].second << "\n";
                udp_handlers[!i]->send_xy((float) input[i].first, (float) input[i].second);
            }

            // if (input != prev_input) {
            //     std::cout << "new input,  x: " << input.first << ", y: " << input.second << "\n";
            // }

		// 	if (input.first != prev_input) {
		// 		udp_handlers[i].send_xy(input.first, input.second);
		// 		std::cout << "Received packet " << (int) input.first << std::endl;
		// 		if (input.first < prev_input) {
		// 			std::cout << "PACKET SWAP" << std::endl;
		// 		} else if (input.first != prev_input + 1) {
		// 			std::cout << "PACKET LOST" << std::endl;
		// 		}
		// 	}
			// balls[i].set_velocity(
			// 	normalize_accel({
			// 		.x=input.first,
			// 		.y=input.second,
			// 	}));
			// balls[i].update_position();
			// balls[i].resolve_wall_collisions(map);
			// XYPairFloat pos = balls[i].get_position();
			// udp_handlers[i].send_xy(pos.x, pos.y);
		}
	}

    for (auto udp_handler : udp_handlers) {
        delete udp_handler;
    }

	return 0;
}

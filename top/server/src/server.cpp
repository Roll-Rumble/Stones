#include <iostream>
#include <thread>
#include <climits>
#include <functional>
#include <chrono>

#include "networking.hpp"
#include "ball.hpp"
#include "map.hpp"
#include "db.hpp"
#include "netutils.hpp"

#define NUM_CLIENTS 2
#define BUF_SIZE 1024

#define WIN_CODE 2048
#define LOSE_CODE 4096
#define RESTART_CODE 3000

void db_thread(int client_id, TCPServ &serv)
{
	Logger db(0);
	// wait for 'g' (get)

	char buf[1];


	// send no of games

	unsigned char buf_out[BUF_SIZE];

	while (true) {

		buf[0] = 'i';
		while (buf[0] == 'i') {
			serv.recv_buffer(client_id, (unsigned char *)buf, 1);
		}
		if(buf[0] == 'g'){
			serv.send_int(client_id, Logger::GetLatestGame());
		}
		else
		{
			// wait for game id
			int game_id = serv.recv_int(client_id);
			// send data for game id

			std::vector<std::vector<XYPairInt16>> game_data = db.Parse(game_id);
			serv.send_int(client_id, game_data.size());

			unsigned char *buf_ptr = buf_out;
			for (int i = 0; i < game_data.size(); i++) {
				pack::packu16(buf_ptr, game_data[i][0].x);
				pack::packu16(buf_ptr+2, game_data[i][0].y);
				pack::packu16(buf_ptr+4, game_data[i][1].x);
				pack::packu16(buf_ptr+6, game_data[i][1].y);
				buf_ptr += 8;
				if (buf_ptr - buf_out == BUF_SIZE) {
                    std::cout << client_id << " sending 1024 bytes" << std::endl;
					serv.send_buffer(client_id, buf_out, BUF_SIZE);
					buf_ptr = buf_out;
				}
			}
            std::cout << client_id << " sending remainder" << std::endl;
			serv.send_buffer(client_id, buf_out, buf_ptr - buf_out);
		}
	}
}

int main()
{
	TCPServ tcp_serv;
	Map map;
	std::vector<std::pair<std::string, int>> client_addrs = tcp_serv.get_connections(NUM_CLIENTS);

	std::thread client_thread_0(db_thread, 0, std::ref(tcp_serv));
	std::thread client_thread_1(db_thread, 1, std::ref(tcp_serv));

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
	Logger db(Logger::GetLatestGame());
    db.Open(Logger::GetLatestGame());
	std::vector<std::pair<uint16_t, uint16_t>> input = {{0,0}, {0,0}};
    int wait_for_client_num = -1;
    bool stopped = false;
	while (true) {
        // std::cout << "Number of balls is " << udp_handlers.size() << "\n";
		for (int i = 0; i < udp_handlers.size(); i++) {
		// 	int prev_input = input.first;
            // std::cout << "udp handlers size is " << udp_handlers.size() << "\n";
            std::pair<uint16_t,uint16_t> prev_input = input[i];
            // std::cout << "About to try to receive on UDP from client " << i << "\n";
			input[i] = udp_handlers[i]->recv_xy(input[i]);
            if (i == wait_for_client_num) {
                if (input[i].first == RESTART_CODE && input[i].second == RESTART_CODE) {
                    wait_for_client_num = -1;
                    db.Open(Logger::GetLatestGame());
                    for (int j = 0; j < 100; j++) {
                        udp_handlers[i]->recv_xy(input[i]);
                        udp_handlers[!i]->recv_xy(input[i]);
                    }
                } else {
                    continue;
                }
            }
            else if (input[i].first == RESTART_CODE && input[i].second == RESTART_CODE) {
                wait_for_client_num = !i;
            }

            if (stopped) {
                continue;
            }
            // std::cout << "Successfully executed receive function from client" << i << "\n";
            if (input[i] != prev_input && udp_handlers.size() == 2) {
                if (map.is_exit({(float) input[i].first, (float) input[i].second})) {
                    // Send win code to winner
                    udp_handlers[i]->send_xy((float)WIN_CODE, (float)WIN_CODE);
                    // Send lose code to winner
                    udp_handlers[!i]->send_xy((float)LOSE_CODE, (float)LOSE_CODE);
                    XYPairFloat start_pos = map.get_start_position();

                    db.Put(input);
                    db.Close();
                    input = {{(uint16_t)start_pos.x, (uint16_t)start_pos.y},
                        {(uint16_t)start_pos.x, (uint16_t)start_pos.y}};
                    prev_input = {(uint16_t)start_pos.x, (uint16_t)start_pos.y};
                    stopped = true;
                } else {
                    std::cout << "Sending data to client " << !i << "\n";
                    std::cout << "Data being sent is x: " << input[i].first << ", y: " << input[i].second << "\n";
                    udp_handlers[!i]->send_xy((float) input[i].first, (float) input[i].second);

                    db.Put(input);
                }



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

	client_thread_0.join();
	client_thread_1.join();

	return 0;
}

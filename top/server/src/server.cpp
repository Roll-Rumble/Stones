#include <iostream>

#include "networking.hpp"
#include "ball.hpp"
#include "map.hpp"


int main()
{
	TCPServ tcp_serv;
	Map map;
	std::vector<std::string> client_addrs = tcp_serv.get_connections(1);
	std::vector<UDPServ> udp_handlers;
	std::vector<Ball> balls;
	for (std::string &addr : client_addrs) {
		udp_handlers.emplace_back(addr);
		balls.emplace_back(map);
	}
	std::pair<int16_t, int16_t> input = {0, 0};
	while (true) {
		for (int i = 0; i < udp_handlers.size(); i++) {
			input = udp_handlers[i].recv_xy(input);
			balls[i].set_velocity(
				normalize_accel({
					.x=input.first,
					.y=input.second,
				}));
			balls[i].update_position();
			balls[i].resolve_wall_collisions(map);
			XYPairFloat pos = balls[i].get_position();
			udp_handlers[i].send_xy(pos.x, pos.y);
		}
	}
	return 0;
}
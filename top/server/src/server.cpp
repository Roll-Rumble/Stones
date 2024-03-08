#include "networking.hpp"
#include "Ball.hpp"
#include "Wall.hpp"
#include "Collision_detect.hpp"


int main()
{
	std::cout << "starting server..." << std::endl;
	auto [addr, port] = wait_for_tcp();
	std::cout << "TCP handshake successfull" << std::endl;
	UDPServ udp_serv(addr);
	// int wall_num =1;
	Ball ball(0, 0, 0.1);
	// std::vector<Wall> walls;
	// Wall wall1(-1, -1, 1, 1);
	// walls.push_back(wall1);

	while (true) {
		auto [input_x, input_y] = udp_serv.recv_xy();

		std::cout << "x_in: " << input_x << " y_in: " << input_y << std::endl;
		ball.Update(input_x, input_y);

		
		udp_serv.send_xy(ball.Get_location_x(), ball.Get_location_y());

		//int_fast8_t decision = Collision_detect(ball, wall1);
		//std::cout << decision << std::endl;
	}
	return 0;
}
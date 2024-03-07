#include "networking.hpp"
#include "Ball.hpp"
#include "Wall.hpp"
#include "Collision_detect.hpp"


int main()
{
	auto [addr, port] = wait_for_tcp();
	UDPServ udp_serv(addr);
	// int wall_num =1;
	Ball ball(0, 0, 0.1);
	// std::vector<Wall> walls;
	// Wall wall1(-1, -1, 1, 1);
	// walls.push_back(wall1);

	while (true) {
		auto [input_x, input_y] = udp_serv.recv_xy();


		ball.Update(input_x, input_y);

		
		udp_serv.send_xy(ball.Get_location_x(), ball.Get_location_y());

		//int_fast8_t decision = Collision_detect(ball, wall1);
		//std::cout << decision << std::endl;
	}
	return 0;
}
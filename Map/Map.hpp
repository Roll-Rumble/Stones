#include "array"

#define WIDTH = 32
#define HEIGHT = 18

class Map {
	private:

		enum Tile {W = '1', F = '0', I = '*', E = '^'};
		std::array<std::array<int,2>, 2> start = {{{{1,1}}, {{1,1}}}};
		std::array<std::array<Tile, 32>, 18> map = {{
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,F,F,F, W,W,W,F, F,W,W,F, F,W,W,F, F,W,W,F, F,F,W,W, W,W,W,W, W,W,W,W}},
			{{W,F,I,F, W,E,F,F, F,F,F,F, F,F,F,F, F,F,F,F, F,F,F,F, W,W,W,W, W,W,W,W}},
			{{W,F,F,F, W,W,W,W, W,F,F,W, W,F,F,W, W,F,F,W, W,W,F,F, F,W,W,W, W,W,W,W}},

			{{W,F,F,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,F,F, F,W,W,W, W,W,W,W}},
			{{W,F,F,F, F,F,F,F, F,F,F,F, F,F,F,W, W,W,W,W, W,W,F,F, F,W,W,W, W,W,W,W}},
			{{W,F,F,F, F,F,F,F, F,F,F,F, F,F,F,W, W,W,W,W, F,F,F,F, F,F,W,W, W,W,W,W}},
			{{W,W,W,W, W,F,F,W, W,W,W,W, W,W,W,W, W,W,W,F, F,F,W,W, F,F,F,W, W,W,W,W}},

			{{W,W,W,W, W,F,F,F, F,F,F,F, W,W,W,W, W,W,W,F, F,F,W,W, W,F,F,F, W,W,W,W}},
			{{W,W,W,W, F,F,F,W, W,W,F,F, W,W,W,W, W,F,F,F, W,W,W,W, W,W,F,F, F,W,W,W}},
			{{W,W,W,F, F,F,W,W, W,W,W,F, F,W,W,W, W,F,F,F, W,W,W,W, W,W,W,F, F,W,W,W}},
			{{W,W,F,F, F,W,W,W, W,W,W,W, F,F,W,W, W,W,W,F, F,F,F,F, W,W,W,F, F,W,W,W}},

			{{W,W,F,F, W,W,W,W, W,W,W,W, F,F,F,W, W,W,W,F, F,F,F,F, W,W,W,F, F,W,W,W}},
			{{W,W,W,F, F,F,F,F, F,F,F,F, F,F,F,W, W,W,W,F, F,F,F,F, F,F,F,F, F,F,F,W}},
			{{W,W,W,W, F,F,F,F, F,F,F,F, W,W,W,W, W,W,W,F, F,F,F,F, F,F,F,F, F,F,F,W}},
			{{W,W,W,W, W,W,W,F, F,W,F,F, W,F,F,F, F,W,W,F, F,F,F,F, W,W,W,W, W,W,W,W}},

			{{W,W,W,W, W,W,W,F, F,W,F,F, F,F,W,W, F,F,F,F, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
		}}; 

	public:

};


/*
{{
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},

			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},

			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},

			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},

			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
			{{W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W, W,W,W,W}},
		}}; 
*/
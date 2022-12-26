#ifndef PARTICLE_H
#define PARTICLE_H

#include<vector>
#include<SFML/Graphics.hpp>

using namespace std;
using namespace sf;
namespace runner {
	class Particle {
	public:
		Particle( RectangleShape shape,int xVel, int yVel);
		int xV;
		int yV;
		RectangleShape s;
	};
}
#endif;

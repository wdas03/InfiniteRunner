#include "Particle.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

namespace runner {
	int xV;
	int yV;
	RectangleShape s;
	Particle::Particle(RectangleShape shape, int xVel, int yVel) {
		xV = xVel;
		yV = yVel;
		s = shape;
	}
}
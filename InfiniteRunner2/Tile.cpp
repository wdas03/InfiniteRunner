#include "Tile.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include<string>
using namespace sf;
using namespace std;

namespace runner {
	bool hasGravity = false;
	int dataID=0;
	float yVel = 0;
	string name="";
	bool xFlipped = false;
	bool isCollidable = true;
	Tile::Tile() {}
	Tile::Tile(Texture texture, IntRect rect, int data) {
		setTexture(texture);
		setTextureRect(rect);
		dataID = data;
	}
}
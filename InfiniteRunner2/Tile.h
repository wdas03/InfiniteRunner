#ifndef TILE_H
#define TILE_H

#include<vector>
#include<SFML/Graphics.hpp>
#include<string>

using namespace std;
using namespace sf;
namespace runner {
	class Tile : public sf::Sprite {
	public:
		using Sprite::Sprite;
		bool hasGravity;
		float yVel;
		int dataID;
		string name;
		bool xFlipped;
		bool isCollidable;
		Tile();
		Tile(Texture texture, IntRect rect, int data);
	private:
	};
}
#endif;

#ifndef TERRAIN_H
#define TERRAIN_H

#include<vector>
#include<SFML/Graphics.hpp>
#include "Tile.h"
#include <map>
#include <string>
using namespace std;
using namespace sf;
namespace runner {
	class Terrain {
	public:
		Terrain(int height, int width,Texture sp,Texture ts);
		vector<Tile> getTerrain(float highestX);
	private:
		Texture spritesheet;
		int screenH;
		int screenW;
		vector<vector<Tile>> structures;
		vector<Tile> stringToStructure(string s, map<char, Tile> key);
	};
}
#endif;
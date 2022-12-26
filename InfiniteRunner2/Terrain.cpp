#include "Terrain.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Tile.h"
#include <map>
#include <string>
#include<sstream>
using namespace sf;
using namespace std;

namespace runner {
	Texture Tilesheet;
	Texture tileSheet;
	int screenH;
	int screenW;
	//A list of defined obstacles
	vector<vector<Tile>> structures;

	Terrain::Terrain(int height, int width,Texture sp,Texture ts) {
		Tilesheet = sp;
		screenW = width;
		screenH = height;
		tileSheet = ts;
		map<char, Tile> keys;

		Tile grass(Tilesheet, IntRect(-128, 0, 128, 128));
		grass.scale(Vector2f(0.2, 0.2));
		grass.xFlipped = false;
		grass.hasGravity = false;
		Tile dirt(Tilesheet, IntRect(5 * 128, 8 * 128, 128, 128));
		dirt.scale(Vector2f(0.2, 0.2));
		dirt.xFlipped = false;
		dirt.hasGravity = false;
		Tile spring(tileSheet, IntRect(128, 14 * 128, 128, 128));
		spring.dataID = 1;
		spring.name = "Spring";
		spring.setScale(0.2, 0.2);
		spring.isCollidable = false;
		spring.xFlipped = false;
		spring.hasGravity = false;
		Tile stone(Tilesheet, IntRect(4 * 128, 6 * 128, 128, 128));
		stone.scale(0.2, 0.2);
		stone.xFlipped = false;
		stone.hasGravity = false;
		Tile snow(Tilesheet, IntRect(3 * 128, 8 * 128, 128, 128));
		snow.scale(0.2, 0.2);
		snow.xFlipped = false;
		snow.hasGravity = false;
		Tile platform(Tilesheet, IntRect(2 * 128, 10 * 128, 128, 128));
		platform.scale(0.2, 0.2);
		platform.xFlipped = false;
		platform.hasGravity = false;
		Tile platformEdgeLeft(Tilesheet, IntRect(2 * 128, 9 * 128, 128, 128));
		platformEdgeLeft.scale(0.2, 0.2);
		platformEdgeLeft.xFlipped = true;
		platformEdgeLeft.hasGravity = false;
		Tile platformEdgeRight(Tilesheet, IntRect(2 * 128, 9 * 128, 128, 128));
		platformEdgeRight.scale(0.2, 0.2);
		platformEdgeRight.xFlipped = false;
		platformEdgeRight.hasGravity = false;
		Tile fallingIron(tileSheet, IntRect(0, 13 * 128, 128, 128));
		fallingIron.scale(0.2, 0.2);
		fallingIron.dataID = 2;
		fallingIron.xFlipped = false;
		fallingIron.hasGravity = false;
		Tile ironButton(tileSheet, IntRect(128, 8 * 128, 128, 128));
		ironButton.scale(0.2, 0.2);
		ironButton.isCollidable = false;
		ironButton.dataID = 3;
		ironButton.xFlipped = false;
		ironButton.hasGravity = false;
		keys['G'] = grass;
		keys['D'] = dirt;
		keys['S'] = spring;
		keys['T'] = stone;
		keys['N'] = snow;
		keys['P'] = platform;
		keys['l'] = platformEdgeLeft;
		keys['r'] = platformEdgeRight;
		keys['I'] = fallingIron;
		keys['B'] = ironButton;
		vector<Tile> smallPlain;
		{
			string structure =
				string("GGGGGGGGGG\n") +
				string("DDDDDDDDDD\n");
			smallPlain = stringToStructure(structure, keys);
		}
		vector<Tile> holeSmall;
		{
			string structure =
				string("GGGG      GGGG\n") +
				string("DDDD      DDDD\n");
			holeSmall = stringToStructure(structure,keys);
		}
		vector<Tile> floatingPlatform;
		{
			string structure =
				string("     lPPPPPPPPPPPPr     \n") + 
				string("                        \n") +
				string("GG                    GG\n") +
				string("DD                    DD\n");
			floatingPlatform = stringToStructure(structure, keys);
		}
		vector<Tile> plateau;
		{
			string structure =
				string("  NNNNNNNNNNNNNNNNNNNNNN  \n") +
				string("  TTTTTTTTTTTTTTTTTTTTTT  \n") +
				string("GGTTTTTTTTTTTTTTTTTTTTTTGG\n") +
				string("DDTTTTTTTTTTTTTTTTTTTTTTDD\n");
			plateau = stringToStructure(structure, keys);
		}
		vector<Tile> springPlatform;
		{
			string structure =
				string("                lPPPPPr                \n") +
				string("                                       \n") +
				string("                                       \n") +
				string("                                       \n") +
				string("                                       \n") +
				string("                                       \n") +
				string("                                       \n") +
				string("                                       \n") +
				string(" S                                     \n") +
				string("GG                                  GGG\n") +
				string("DD                                  DDD\n") ;
			springPlatform = stringToStructure(structure, keys);

		}
		vector<Tile> trap;
		{
			string structure =
				string("           T   \n") +
				string("           I   \n") +
				string("               \n") +
				string("               \n") +
				string("               \n") +
				string("               \n") +
				string("               \n") +
				string("               \n") +
				string("               \n") +
				string("               \n") +
				string("               \n") +
				string(" B             \n") +
				string("GGGGGGGGGGGGGGG\n") +
				string("DDDDDDDDDDDDDDD\n");
			trap = stringToStructure(structure, keys);
		}
		/*
		vector<Tile> Mountain;
		{
			string structure =
				string("                                                                   NNNNNNNNN                                                                   \n") +
				string("                                                               NNNNTTTTTTTTTNNNNN                                                              \n") +
				string("                                                          NNNNNTTTTTTTTTTTTTTTTTTNNNN                                                          \n") +
				string("                                                       NNNTTTTTTTTTTTTTTTTTTTTTTTTTTTNNN                                                       \n") +
				string("                                                  NNNNNTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTNNNNN                                                    \n") +
				string("                                                NNTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTNN                                                  \n") +
				string("                                              NNTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT                                                          \n") +
				string("                                            NNTTTTTTTTTTTTTTTTTTTTTTT                                                                          \n") +
				string("                                          NNTTTTTTTTTTTTTTTTTTTTT                                                                              \n") +
				string("                                        NNTTTTTTTTTTTTTTTTTTTTTT                                                                               \n") +
				string("                                      NNTTTTTTTTTTTTTTTTT                                   TTTTTTTTTNN                                        \n") +
				string("                                  NNNNTTTTTTTTTTTTT                                   TTTTTTTTTTTTTTTTTNNNN                                    \n") +
				string("                               NNNTTTTTTTTTTT                           TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTNNN                                 \n") +
				string("                             NNTTTTTTTTT                            TTTTTTTT                                                                   \n") +
				string("                            NTTTTTTTTTT                        TTTTTTTTTTTTT                                                                   \n") +
				string("                           NTTTTTTTTTT                   TTTTTTTTTTTTTT                                                                        \n") +
				string("                         NNTTTTTTTTT           TTTTTTTTTTTTTTTT                                                                                \n") +
				string("                    NNNNNTTTTTTTTT          TTTT                                          TTTTTTTTTTTTTTTTTTTTTTTTTTNNNNN                      \n") +
				string("                NNNNTTTTTT                                                      TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTNNNN                  \n") +
				string("           NNNNNTTTTT                                                    TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTNNNNN             \n") +
				string("     TTTTTTTTTTTT                                          S                                                         TTTTTTTTTTTTTTTTTTTT      \n") +
				string("                                    S       TTTTTTTTTTTTTTTT                                                                                   \n") +
				string("                           TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT                                                                  \n") +
				string("             TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT                                                 \n") +
				string("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n") +
				string("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
			Mountain = stringToStructure(structure, keys);
		}*/
		vector<Tile> Mountain;
		{
			string structure = 
				string("              NNNNNNN             \n") +
				string("          NNNTTTTTTTTNNN          \n") +
				string("       NNTT            TTNN       \n") +
				string("      TTT                TTT      \n") +
				string("     TTT                  TTT     \n") +
				string("    TTT                    TTT    \n") +
				string("   TTT                      TTT   \n") +
				string("                                  \n") +
				string("                                  \n") +
				string("GGGGGGGGGGGGG        GGGGGGGGGGGGG\n") +
				string("DDDDDDDDDDDDD        DDDDDDDDDDDDD\n");
			Mountain = stringToStructure(structure, keys);
		}
		vector<Tile> Springplatforms;
		{
			string s = 
				string("                                               lPPPr      \n")+
				string("                            S                             \n") + 
				string("                         lPPPr                            \n") +
				string("                                                          \n") +
				string("  S                                                       \n") +
				string("GGG                                                       \n") + 
				string("DDD                                                       \n");
			Springplatforms = stringToStructure(s, keys);
		}
		vector<Tile> upAndDown;
		{
			string s =
				string("                lPPr                            lPPr                                   \n") +
				string("                                                                                       \n") +
				string("                                                                                       \n") +
				string("                                                                                 lPPr  \n") +
				string("                                                                                       \n") +
				string("GGG    S                            S                          S                     GG\n") +
				string("DDD lPPr                         lPPr                       lPPr                     DD\n");
			upAndDown = stringToStructure(s, keys);
		}
		structures.push_back(upAndDown);
		structures.push_back(Springplatforms);
		structures.push_back(Mountain);
		structures.push_back(trap);
		structures.push_back(springPlatform);
		structures.push_back(plateau);
		structures.push_back(floatingPlatform);
		structures.push_back(holeSmall);
		structures.push_back(smallPlain);
		
	}

	vector<Tile> Terrain::getTerrain(float highestX) {
		int i = rand() % structures.size();
		vector<Tile> terr;
		for (Tile t : structures[i]) {
			t.setPosition(t.getPosition().x - screenW + highestX, t.getPosition().y);
			terr.push_back(t);
		}
		return terr;
	}
	vector<Tile> Terrain::stringToStructure(string s, map<char, Tile> key) {
		vector<Tile> tiles;
		vector<string> linesList;
		int lines = 0;
		int longestLine = 0;
		int lineLength = 0;
		for (int i = 0; i < s.length(); i++) {
			if (s[i] == '\n') {
				lines++;
				if (lineLength > longestLine) {
					longestLine = lineLength;
				}
				lineLength = 0;
			}
			else {
				lineLength++;
			}
		}
		lineLength = 0;
		ostringstream oss;
		for (int i = 0; i < s.length(); i++) {
			if (s[i] == '\n') {
				if (lineLength < longestLine) {
					for (int i = lineLength; i < longestLine; i++) {
						oss << ' ';
					}
				}
				oss << endl;
				linesList.push_back(oss.str());
				oss.clear();
				oss.str("");
			}
			else {
				oss << s[i];
				lineLength++;
			}
		}
		// pppppppppp
		// pppp   ppp
		for (int i = 0; i < longestLine;i++) {
			for (int j = 0; j < linesList.size();j++) {
				Tile t(Tilesheet, IntRect(6 * 128, 12 * 128, 128, 128));
				char c = linesList[j][i];
				if (c == ' ') {
					t.scale(0.2, 0.2);
					t.isCollidable = false;
					t.hasGravity = false;
				}
				else if (c == '\n') {
					continue;
				}
				else {
					t = key[c];
				}
				if (t.xFlipped) {
					t.scale(-1, 1);
					t.setPosition(Vector2f(screenW + ((i + 1) * 26), screenH - ((linesList.size() - j) * 26)));
				}
				else {
					t.setPosition(Vector2f(screenW + (i * 26), screenH - ((linesList.size() - j) * 26)));
				}
				tiles.push_back(t);
				if (t.dataID == 1 && t.name != "Spring") {
					t.dataID = 0;
				}
			}
		}
		return tiles;
	}
}
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include "AnimatedSprite.hpp"
#include "Terrain.h"
#include "Particle.h"
#include "Tile.h"
#include <math.h>

#define SCREENW 1920
#define SCREENH 1080
using namespace std;
using namespace sf;
using namespace runner;

void offset(vector<Tile> &orig, float amount, bool destroy);
void offset(vector<Sprite> &orig, float amount, bool destroy);
void append(vector<Tile> &v1, vector<Tile> v2);
void append(vector<Sprite> &v1, vector<Sprite> v2);

Texture players;
Texture ground;
Texture background;
Texture tileEntities;

Font font;
Text title;
Text highscore;
Text score;
Text gameOver;
float overVel = 0;

float curScore=0;
float curHighscore=0;

float secondsPerSpeedIncrease = 0.1;
float speedIncrease = 0.005;
float currentTimer = 0;
float startSpeed = 6;

vector<Tile> tileMap;
vector<Sprite> bgSprites;
vector<Particle> deathParticles; 

Sound jump;
SoundBuffer buffer;
Sound death;
SoundBuffer deathBuffer;
Music music;

float yVel = 0;
float xVel = 0;

float speed = startSpeed;

float jumpSpeed = 10;
bool canJump = true;

int main() {
	srand(time(0));
	RenderWindow window(VideoMode(SCREENW, SCREENH), "Infinite Runner", Style::None);	
	window.setVerticalSyncEnabled(true);
	ifstream infile("highscore.sav");
	if (!infile) {
		cout << "Highscore not able to be loaded" << endl;
	}
	int readHigh;
	while (infile >> readHigh) {
		curHighscore = readHigh;
	}
	if (!players.loadFromFile("Images/spritesheet_players.png")) {
		cout << "Loading of player tilesheet failed!" << endl;
		return 1;
	}if (!ground.loadFromFile("Images/spritesheet_ground.png")) {
		cout << "Loading of ground tilesheet failed!" << endl;
		return 1;
	}if (!background.loadFromFile("Images/background.png")) {
		cout << "Loading of background PNG failed!" << endl;
		return 1;
	}if (!font.loadFromFile("Font.ttf")) {
		cout << "Loading of Font failed!" << endl;
		return 1;
	}
	if (!buffer.loadFromFile("Sounds/Jump.wav")) {
		return 1;
	}
	if (!deathBuffer.loadFromFile("Sounds/death.wav")) {
		return 1;
	}
	if (!music.openFromFile("Sounds/background.wav")) {
		return 1;
	}

	if (!tileEntities.loadFromFile("Images/spritesheet_tiles.png")) {
		cout << "Loading of Tilesheet PNG failed!" << endl;
		return 1;
	}

	jump.setBuffer(buffer);
	death.setBuffer(deathBuffer);
	title.setFont(font);
	title.setString("INFINITE RUNNER");
	title.setCharacterSize(48);
	title.setColor(Color(0,240,255));
	title.setOutlineColor(Color(0, 0, 0));
	title.setOutlineThickness(4);

	ostringstream os;
	os << "HIGHSCORE: " << round(curHighscore);
	highscore.setFont(font);
	highscore.setString(os.str());
	highscore.setCharacterSize(24);
	highscore.setColor(Color(0, 240, 255));
	highscore.setOutlineColor(Color(0, 0, 0));
	highscore.setOutlineThickness(4);


	score.setFont(font);
	score.setString("SCORE: ");
	score.setCharacterSize(24);
	score.setColor(Color(0, 240, 255));
	score.setOutlineColor(Color(0, 0, 0));
	score.setOutlineThickness(4);

	gameOver.setFont(font);
	gameOver.setString("GAME OVER");
	gameOver.setCharacterSize(64);
	gameOver.setColor(Color(0, 240, 255));
	gameOver.setOutlineColor(Color(0, 0, 0));
	gameOver.setOutlineThickness(4);

	title.setPosition(SCREENW,SCREENH - (SCREENH / 2));
	highscore.setPosition(SCREENW, SCREENH - (SCREENH / 3));
	score.setPosition(SCREENW/16, SCREENH/16);
	gameOver.setPosition(SCREENW / 2 - (gameOver.getLocalBounds().width/2), SCREENH);
	Sprite bgSprite(background,IntRect(0,0,1024,1024));
	float scaleAmt = SCREENH/1024.0;
	bgSprite.scale(scaleAmt, scaleAmt);
	for (int i = 0; i < SCREENW; i += 1024*scaleAmt) {
		Sprite c = bgSprite;
		if (i + (1024 * scaleAmt) >= SCREENW) {
			c.setPosition(i + (1024 * scaleAmt), 0);
			bgSprites.push_back(c);
		}
		c.setPosition(i, 0);
		bgSprites.push_back(c);
	}
	for (int i = 0; i < SCREENW; i += 26) {
		Tile s(ground, IntRect(-128, 0, 128, 128));
		s.scale(Vector2f(0.2, 0.2));
		s.setPosition(Vector2f(i,SCREENH-52));
		Tile s1(ground, IntRect(5*128, 8*128, 128, 128));
		s1.scale(Vector2f(0.2, 0.2));
		s1.setPosition(Vector2f(i, SCREENH - 26));
		s.hasGravity = false;
		s1.hasGravity = false;
		tileMap.push_back(s);
		tileMap.push_back(s1);
	}


	Animation running;
	running.setSpriteSheet(players);
	running.addFrame(IntRect(512, 256, 128, 256));
	running.addFrame(IntRect(512, 512, 128, 256));

	AnimatedSprite playerSprite(seconds(0.1), true, true);
	playerSprite.scale(Vector2f(0.2, 0.2));
	playerSprite.setPosition(Vector2f(0, SCREENH/2));
	playerSprite.setAnimation(running);
	Terrain terrain(SCREENH, SCREENW,ground,tileEntities);

	Clock frameClock;
	bool isDead = false;
	int deadFrames = 0;
	bool playAgain = false;
	bool showDeadScore = false;
	music.play();
	music.setLoop(true);
	music.setPitch(1.25);
	while (window.isOpen()) {
		Time frameTime = frameClock.restart();
		if (playerSprite.getPosition().x < -26 || playerSprite.getPosition().y > SCREENH || isDead) {
			speed = 0;
			if (deathParticles.size() == 0&& deadFrames==0) {
				death.play();
				deadFrames = 0;
				for (int i = 0; i < 50; i++) {
					int size = (rand() % 50) + 25;
					RectangleShape shape(Vector2f(size, size));
					shape.setFillColor(Color(rand() % 155 + 100,0,0));
					shape.setOutlineColor(Color(0, 0, 0));
					shape.setOutlineThickness(2);
					shape.setPosition(playerSprite.getPosition());
					Particle p(shape, (rand() % 50) - 25, (rand() % 50) - 25);
					deathParticles.push_back(p);
				}
				//Lose game
				isDead = true;
				
				if (curScore > curHighscore) {
					curHighscore = curScore;
				}
				os.clear();
				os.str("");
				os << "HIGHSCORE: " << round(curHighscore);
				highscore.setString(os.str());
				curScore = 0;
				ofstream out("highscore.sav");
				out << curHighscore;
				out.close();
			}
			else {
				if (gameOver.getPosition().y > SCREENH / 2) {
					overVel -= 1;
				}
				else {
					overVel += 1;
				}
				overVel = overVel*0.99;
				if (abs(gameOver.getPosition().y - (SCREENH/2))<10) {
					if (overVel < 1) {
						overVel = 0;
						gameOver.setPosition(gameOver.getPosition().x, SCREENH / 2);
						score.setPosition(SCREENW / 2 - (score.getLocalBounds().width/2), gameOver.getPosition().y + (SCREENH / 16));
						showDeadScore = true;
					}
				}

				gameOver.move(0, overVel);
				deadFrames++;
				for (int i = 0; i < deathParticles.size(); i++) {
					RectangleShape s = deathParticles[i].s;
					s.move(deathParticles[i].xV, deathParticles[i].yV);
					s.rotate(rand() % 20 - 10);
					s.scale(pow(.999,deadFrames),pow(0.999,deadFrames));
					deathParticles[i] = Particle(s,deathParticles[i].xV,deathParticles[i].yV);
					if (s.getScale().x <0.05|| s.getScale().y<0.05) {
						deathParticles.erase(deathParticles.begin() + i);
					}
				}
				if (deathParticles.size() == 0) {
					if (playAgain) {
						playAgain = false;
						deathParticles.clear();
						tileMap.clear();

						for (int i = 0; i < SCREENW; i += 26) {
							Tile s(ground, IntRect(-128, 0, 128, 128));
							s.scale(Vector2f(0.2, 0.2));
							s.setPosition(Vector2f(i, SCREENH - 52));
							Tile s1(ground, IntRect(5 * 128, 8 * 128, 128, 128));
							s1.scale(Vector2f(0.2, 0.2));
							s1.setPosition(Vector2f(i, SCREENH - 26));
							s.hasGravity = false;
							s1.hasGravity = false;
							tileMap.push_back(s);
							tileMap.push_back(s1);
						}
						speed = startSpeed;
						playerSprite.setPosition(Vector2f(0, SCREENH / 2));
						title.setPosition(SCREENW, SCREENH - (SCREENH / 2));
						highscore.setPosition(SCREENW, SCREENH - (SCREENH / 3));
						score.setPosition(SCREENW / 16, SCREENH / 16);
						gameOver.setPosition(SCREENW / 2 - (gameOver.getLocalBounds().width / 2), SCREENH);
						overVel = 0;
						deadFrames = 0;
						isDead = false;
						showDeadScore = false;
					}
					else {

					}
				}
			}
		}
		else {

			currentTimer += frameTime.asSeconds();
			os.clear();
			os.str("");
			os << "SCORE: ";
			os << round(curScore);
			score.setString(os.str());
			if (playerSprite.getPosition().x < 256) {
				xVel = abs(playerSprite.getPosition().x - 256) / 64;
			}
			yVel -= 0.5 * (frameTime.asSeconds()*60);
		}if (!isDead) {
			for (size_t i = 0; i < abs(xVel); ++i) {
				playerSprite.move(xVel * (frameTime.asSeconds() * 60), 0);
			}
			for (size_t i = 0; i < abs(yVel); ++i) {
				for (int i = 10; i < (200 > tileMap.size() ? tileMap.size() : 200); i++) {
					if (playerSprite.getGlobalBounds().contains(tileMap[i].getPosition())) {
						if (tileMap[i].isCollidable) {
							canJump = true;
							if (abs((playerSprite.getPosition().y + 128) - tileMap[i].getPosition().y) > 90) {
								isDead = true;
							}
							else {
								yVel = 0;
								playerSprite.move(0, -1 * (frameTime.asSeconds() * 60));
							}
						}

						switch (tileMap[i].dataID) {
						case 1:
							yVel = (jumpSpeed*1.8)/(speed/(speed*1.1));
							tileMap[i].setTextureRect(IntRect(128, 13 * 128, 128, 128));
							playerSprite.move(0, -5);
							canJump = true;
							break;
						case 3:
							tileMap[i +(9*14+4)].hasGravity = true;
							tileMap[i].setTextureRect(IntRect(128,7*128,128,128));
							break;
						}
					}
				}

				int dir = 0;
				if (yVel > 0) {
					dir = -1;
				}
				else if (yVel < 0) {
					dir = 1;
				}
				playerSprite.move(0, 1 * dir* (frameTime.asSeconds() * 60));

			}
		}
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
			if (event.type == Event::KeyPressed) {
				if (isDead) {
					playAgain = true;
				}
				if (event.key.code == Keyboard::Space) {
					if (canJump) {
						canJump = false;
						if (abs(yVel) <= 1) {
							canJump = true;
						}
						playerSprite.move(0, -5);
						yVel = jumpSpeed;
						jump.play();
					}
				}
			}
		}
		if (currentTimer >= secondsPerSpeedIncrease) {
			currentTimer = 0;
			speed += speedIncrease;
		}
		offset(tileMap, speed * (frameTime.asSeconds() * 60), true);
		offset(bgSprites, (speed / 4) * (frameTime.asSeconds() * 60), false);
		playerSprite.play(running);
		playerSprite.update(frameTime);
		title.move((-speed / 3)* (frameTime.asSeconds() * 60), 0);
		highscore.move((-speed / 3)* (frameTime.asSeconds() * 60), 0);
		curScore += (speed / 100)* (frameTime.asSeconds() * 60);
		window.clear();
		for (int i = 0; i < bgSprites.size(); i++) {
			if (bgSprites[i].getPosition().x <= -(1024 * scaleAmt)) {
				bgSprites[i].setPosition(Vector2f((bgSprites.size() - 1)*SCREENH, 0));
			}
			Sprite bg = bgSprites[i];
			float bgSize = 1024 * bg.getScale().x;
			int thing = bgSprites[i].getPosition().x - (bgSprites[i == 0 ? bgSprites.size() - 1 : i - 1].getPosition().x + (bgSize * 2));
			bgSprites[i].setPosition(floor(bg.getPosition().x)-(thing > -SCREENH?thing:0), bg.getPosition().y);
			window.draw(bgSprites[i]);
		}
		bool shouldGenerate = true;
		float highestX = 0;
		for (int i = 0; i < tileMap.size(); i++) {
			if (tileMap[i].hasGravity) {
				tileMap[i].yVel = speed/1;
				tileMap[i].move(0, tileMap[i].yVel);
			}
			Tile s = tileMap[i];
			highestX = tileMap[i].getPosition().x > highestX ? tileMap[i].getPosition().x : highestX;
			if (tileMap[i].getPosition().x > SCREENW) {
				shouldGenerate = false;
			}
			if (s.dataID == 1) {
			}
			window.draw(tileMap[i]);
		}

		if (shouldGenerate) {
			append(tileMap, terrain.getTerrain(highestX));
		}
		for (int i = 0; i < deathParticles.size(); i++) {
			RectangleShape s = deathParticles[i].s;
			window.draw(s);
		}
		window.draw(title);
		window.draw(highscore);
		if (isDead) {
			window.draw(gameOver);
			if (showDeadScore) {
				window.draw(score);
			}
		}
		else {
			window.draw(score);
			window.draw(playerSprite);
		}
		window.display();
	}
	return 0;
}

void offset(vector<Tile> &orig, float amount,bool destroy) {
	vector<Tile> offsetArr;
	for (int i = 0; i < orig.size(); i++) {
		Tile x = orig[i];
		x.move(Vector2f(ceil(-amount), 0));
		if (x.getPosition().x > -52 || !destroy) {
			offsetArr.push_back(x);
		}
	}
	orig.clear();
	append(orig, offsetArr);
}
void offset(vector<Sprite> &orig, float amount, bool destroy) {
	vector<Sprite> offsetArr;
	for (int i = 0; i < orig.size(); i++) {
		Sprite x = orig[i];
		x.move(Vector2f(-amount, 0));
		if (x.getPosition().x > -52 || !destroy) {
			offsetArr.push_back(x);
		}
	}
	orig.clear();
	append(orig, offsetArr);
}
void append(vector<Sprite> &v1, vector<Sprite> v2) {
	for (int i = 0; i < v2.size(); i++) {
		v1.push_back(v2[i]);
	}
}
void append(vector<Tile> &v1, vector<Tile> v2) {
	for (int i = 0; i < v2.size(); i++) {
		v1.push_back(v2[i]);
	}
}
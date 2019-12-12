#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <functional>
#include <vector>
#include <memory>
#include "Circle.h"
#include <iostream>
#include "Client.h"
#undef main
class Client;
class Game
{
public:
	Game(Client* session);
	~Game();

	bool init();

	bool loadMedia();
	SDL_Texture* loadTexture(std::string path);

	void loop();

	void close();
	void draw();
	void update();

	void setPlayer(Circle newPlayer);
	void setEnemy(Circle enemy);
	void setEnemyPos(int x, int y);

	bool isEnemyInit() { return m_enemyInit; }
	void setEnemyInit(bool state) { m_enemyInit = state; }
	Circle getPlayer() { return m_player; }
	void setEndGame(bool state) { m_gameEnd = state; }
	bool getEndGame() { return m_gameEnd; }

private:
	//Screen dimension constants
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The surface contained by the window
	SDL_Surface* gScreenSurface = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Current displayed texture
	SDL_Texture* gTexture = NULL;

	SDL_Texture* gTexture2 = NULL;

	SDL_Texture* textureRedCircle = NULL;
	SDL_Texture* textureBlueCircle = NULL;

	std::vector<SDL_Texture*> m_wallTextures;

	Circle m_player;
	Circle m_enemy;

	Client* session;
	int m_playerSpeed = 8;
	bool m_enemyInit = false;
	bool m_loadedMedia = false;

	bool m_gameEnd = false;
};


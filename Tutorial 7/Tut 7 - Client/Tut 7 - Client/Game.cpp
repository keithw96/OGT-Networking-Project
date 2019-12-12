#include "Game.h"
Game::Game(Client* socket)
	: session(socket)
{
	m_wallTextures.push_back(NULL);
	m_wallTextures.push_back(NULL);
}

Game::~Game()
{

}

bool Game::init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0,0,0,255);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool Game::loadMedia()
{
	m_loadedMedia = true;
	bool success = true;

	textureBlueCircle = loadTexture("blue.png");
	if (textureBlueCircle == NULL)
	{
		printf("Failed to load texture image blue.png!\n");
		success = false;
	}
	textureRedCircle = loadTexture("red.png");
	if (textureRedCircle == NULL)
	{
		printf("Failed to load texture image red.png!\n");
		success = false;
	}
	switch (m_player.m_color)
	{
	case Color::RED:
		m_player.setTexture(textureRedCircle);
		break;
	case Color::BLUE:
		m_player.setTexture(textureBlueCircle);
		break;
	default:
		break;
	}
	return success;
}

void Game::close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

void Game::draw()
{
	if (m_enemy.texture == NULL)
	{
		switch (m_enemy.m_color)
		{
		case Color::RED:
			m_enemy.setTexture(textureRedCircle);
			break;
		case Color::BLUE:
			m_enemy.setTexture(textureBlueCircle);
			break;
		default:
			break;
		}
	}
	SDL_RenderClear(gRenderer);

	m_player.draw(gRenderer);
	m_enemy.draw(gRenderer);

	SDL_RenderPresent(gRenderer);
}

void Game::update()
{
	if (!m_gameEnd)
	{
		m_player.update();
		m_enemy.update();
	
		float vectPlayerToEnemyX = m_enemy.m_xPos - m_player.m_xPos;
		float vectPlayerToEnemyY = m_enemy.m_yPos - m_player.m_yPos;
		float lenPlayerToEnemy = sqrt((vectPlayerToEnemyX * vectPlayerToEnemyX) + (vectPlayerToEnemyY * vectPlayerToEnemyY)); 
		float lenRadii = m_player.m_radius + m_enemy.m_radius;
		if (lenPlayerToEnemy <= lenRadii)
		{
			m_gameEnd = true;
			session->sendEndGame(m_gameEnd);
		}
		
	}
}

void Game::setPlayer(Circle newPlayer)
{
	m_player = newPlayer;
}

void Game::setEnemy(Circle enemy)
{
	m_enemy.m_xPos = enemy.m_xPos;
	m_enemy.m_yPos = enemy.m_yPos;
	m_enemy.playerRect = enemy.playerRect;
	m_enemy.m_color = enemy.m_color;
	switch (m_enemy.m_color)
	{
	case Color::RED:
		m_enemy.setTexture(textureRedCircle);
		break;
	case Color::BLUE:
		m_enemy.setTexture(textureBlueCircle);
		break;
	default:
		break;
	}
}

void Game::setEnemyPos(int x, int y)
{
	m_enemy.m_xPos = x;
	m_enemy.m_yPos = y;
}

SDL_Texture* Game::loadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void Game::loop()
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					switch (e.type)
					{
					case SDL_KEYDOWN:
						if (e.key.keysym.sym == SDLK_DOWN)
						{

						}
						if (e.key.keysym.sym == SDLK_ESCAPE) 
						{
							quit = true;
						}
						if (e.key.keysym.sym == SDLK_d)
						{
							m_player.m_xPos+= m_playerSpeed;
							session->SendCircle(m_player);
						}
						if (e.key.keysym.sym == SDLK_a)
						{
							m_player.m_xPos-= m_playerSpeed;
							session->SendCircle(m_player);
						}
						if (e.key.keysym.sym == SDLK_w)
						{
							m_player.m_yPos-= m_playerSpeed;
							session->SendCircle(m_player);
						}
						if (e.key.keysym.sym == SDLK_s)
						{
							m_player.m_yPos+= m_playerSpeed;
							session->SendCircle(m_player);
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						break;
					default:
						break;
					}
				}
				update();
				draw();
			}
		}
	}
	close();
}

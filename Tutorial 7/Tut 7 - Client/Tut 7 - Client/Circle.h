#pragma once
#include <SDL.h>

enum class Color { RED, BLUE };
class Circle
{
public:
	Circle();
	Circle(int rad, int x, int y, Color _col);
	~Circle();
	void draw(SDL_Renderer* renderer);
	void update();
	void setTexture(SDL_Texture* _tex);
	int m_radius;
	int m_xPos;
	int m_yPos;
	Color m_color;
	SDL_Texture* texture;
	SDL_Rect playerRect;
};


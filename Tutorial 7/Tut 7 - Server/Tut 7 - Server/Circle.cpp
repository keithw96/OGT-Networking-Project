#include "Circle.h"

Circle::Circle()
{
}

Circle::Circle(int rad, int x, int y, Color _col)
	: m_radius(rad)
	, m_xPos(x)
	, m_yPos(y)
	, m_color(_col)
{
	playerRect.w = rad;
	playerRect.h = rad;
	playerRect.x = x;
	playerRect.y = y;
}


Circle::~Circle()
{
}

void Circle::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, m_texture, NULL, &playerRect);
}

void Circle::setTexture(SDL_Texture * texture)
{
	m_texture = texture;
}

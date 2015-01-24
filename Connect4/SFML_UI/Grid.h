#pragma once
#include <SFML/Graphics.hpp>

class Grid
{
public:
	virtual sf::Vector2f toPixels(sf::Vector2i square) = 0;
	virtual sf::Vector2i toSquares(sf::Vector2f pos) = 0;
	virtual bool isOnGrid(sf::Vector2i square) = 0;
};


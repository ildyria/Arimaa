#pragma once
#include "Grid.h"
#include "ConfigOptions.h"
#include "SFMLOperations.h"

#define SQUARE_SIZE_VECT sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE)
#define HALF_SQUARE_SIZE_VECT sf::Vector2f((float)m_squareSize/2, (float)m_squareSize/2)

#define HALF_SIZE_IN_SQUARES sf::Vector2f((float)m_sizeInSquares.x/2, (float)m_sizeInSquares.y/2)

class CenteredGrid :
	public Grid
{
public:
	CenteredGrid(int m_squareSize, sf::Vector2i m_sizeInSquares);
	CenteredGrid(int m_squareSize, sf::Vector2i m_sizeInSquares, sf::Vector2f center);
	~CenteredGrid();

	sf::Vector2f toPixels(sf::Vector2i square);
	sf::Vector2i toSquares(sf::Vector2f pos);
	bool isOnGrid(sf::Vector2i square);

private:
	int m_squareSize;
	sf::Vector2i m_sizeInSquares;
	sf::Vector2f m_center;
};


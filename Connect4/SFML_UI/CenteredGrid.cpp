#include "CenteredGrid.h"

CenteredGrid::CenteredGrid(int squareSize, sf::Vector2i sizeInSquares) : CenteredGrid(squareSize, sizeInSquares, ConfigOptions::getNativeCenter())
{
}

CenteredGrid::CenteredGrid(int squareSize, sf::Vector2i sizeInSquares, sf::Vector2f center) : Grid(), m_squareSize(squareSize), m_sizeInSquares(sizeInSquares), m_center(center)
{
}

CenteredGrid::~CenteredGrid()
{
}

sf::Vector2f CenteredGrid::toPixels(sf::Vector2i square) 
{
	return m_center + HALF_SQUARE_SIZE_VECT + sfmlop::multiply(sfmlop::toFloatVect(square) - HALF_SIZE_IN_SQUARES, (float)m_squareSize);
}

sf::Vector2i CenteredGrid::toSquares(sf::Vector2f pos)
{
	sf::Vector2f v = (sfmlop::divide(pos - m_center, (float)m_squareSize) + HALF_SIZE_IN_SQUARES);	return sf::Vector2i((int)floor(v.x), (int)floor(v.y));
}

bool CenteredGrid::isOnGrid(sf::Vector2i square)
{
	return square.x >= 0 && square.y >= 0 && square.x < m_sizeInSquares.x && square.y < m_sizeInSquares.y;
}
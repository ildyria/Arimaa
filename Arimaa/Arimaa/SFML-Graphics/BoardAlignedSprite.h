#pragma once
#include <SFML/Graphics.hpp>
#include "SFMLOperations.h"
#include "ResourceManager.h"

#define BOARD_ORIGIN sf::Vector2f(420, 0)
#define BOARD_ORIGIN_CENTERED sf::Vector2f(487.5, 67.5) //BOARD_ROIGIN + SQUARE_SIZE/2
#define SQUARE_SIZE 135

class BoardAlignedSprite :
	public sf::Sprite
{
public:
	BoardAlignedSprite();
	BoardAlignedSprite(std::string image);
	~BoardAlignedSprite(void);

	inline virtual void SetImage(const sf::Image& img) { Sprite::SetImage(img); SetCenter(sfmlop::divide(GetSize(), 2)); }

	inline void moveOnSquare(sf::Vector2i square) { if(isOnBoard(square)) SetPosition( toPixels(square) ); }

	static inline sf::Vector2f toPixels(sf::Vector2i square) { return BOARD_ORIGIN_CENTERED + sfmlop::toFloatVect(sfmlop::multiply(square, SQUARE_SIZE)); }
	static inline sf::Vector2i toSquares(sf::Vector2f pos) { return sfmlop::toIntVect(sfmlop::divide(pos - BOARD_ORIGIN, SQUARE_SIZE)); }
	static inline bool isOnBoard(sf::Vector2i square) { return square.x >= 0 && square.y >= 0 && square.x < 8 && square.y < 8; }
};

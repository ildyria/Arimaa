#include "PieceSprite.h"

PieceSprite::PieceSprite() : BoardAlignedSprite()
{
}

PieceSprite::PieceSprite(Piece* p, std::string image) : BoardAlignedSprite(image), m_piece(p)
{
	int x = m_piece->getType() * SQUARE_SIZE;
	int y = m_piece->getColor() * SQUARE_SIZE;
	SetSubRect(sf::IntRect( x, y, x + SQUARE_SIZE, y + SQUARE_SIZE ));
	SetCenter(SQUARE_SIZE/2, SQUARE_SIZE/2);
}

PieceSprite::~PieceSprite(void)
{
}

void PieceSprite::SetImage(const sf::Image& img)
{
	BoardAlignedSprite::SetImage(img);
	int x = m_piece->getType() * SQUARE_SIZE;
	int y = m_piece->getColor() * SQUARE_SIZE;
	SetSubRect(sf::IntRect( x, y, x + SQUARE_SIZE, y + SQUARE_SIZE ));
	SetCenter(SQUARE_SIZE/2, SQUARE_SIZE/2);
}

void PieceSprite::update(float elpasedTime)
{
}
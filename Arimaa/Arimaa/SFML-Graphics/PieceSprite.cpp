#include "PieceSprite.h"

#define FREEZE_COLOR sf::Color(100,100,255)
#define APPEARANCE_SPEED 10

PieceSprite::PieceSprite() : BoardAlignedSprite(), m_piece(NULL), m_disappearing(false)
{
}

PieceSprite::PieceSprite(Piece* p, std::string image) : BoardAlignedSprite(image), m_piece(p), m_disappearing(false)
{
	int x = m_piece->getType() * SQUARE_SIZE;
	int y = m_piece->getColor() * SQUARE_SIZE;
	SetSubRect(sf::IntRect( x, y, x + SQUARE_SIZE, y + SQUARE_SIZE ));
	SetCenter(SQUARE_SIZE/2, SQUARE_SIZE/2);
	
	//Prepare piece pretty appearance
	SetScale(.01f, .01f);
	SetColor(sf::Color(255, 255, 255, (int) (255*.01f) ));
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

void PieceSprite::setPiece(Piece* p)
{
	m_piece = p;
}

void PieceSprite::update(float elapsedTime)
{
	BoardAlignedSprite::update(elapsedTime);
	if(m_disappearing)
	{
		float ratio = GetScale().x;
		ratio -= APPEARANCE_SPEED*elapsedTime;
		if(ratio < 0)
			ratio = 0;
		SetScale(ratio, ratio);
		SetColor(sf::Color(255, 255, 255, (int) (255*ratio)));
	}
	else
	{
		float ratio = GetScale().x;
		if(ratio < 1) //if appearing
		{
			ratio += APPEARANCE_SPEED*elapsedTime;
			if(ratio > 1)
				ratio = 1;
			SetScale(ratio, ratio);
			SetColor(sf::Color(255, 255, 255, (int) (255*ratio)));
		}
	}
}

void PieceSprite::freeze(bool frozen)
{
	if(frozen)
		SetColor(FREEZE_COLOR);
	else
		SetColor(sf::Color::White);
}

void startDisappearing()
{

}
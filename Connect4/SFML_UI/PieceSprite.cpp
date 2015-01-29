#include "PieceSprite.h"
#include <iostream>

#define FREEZE_COLOR sf::Color(100,100,255)


PieceSprite::PieceSprite(int playerID, Grid* g, std::string image, int appSpeed) : BoardAlignedSprite(g, image), m_appSpeed(appSpeed)
{
	spriteSetup(playerID);

	//Prepare piece pretty appearance
	SetScale(.01f, .01f);
	SetColor(sf::Color(255, 255, 255, (int) (255*.01f) ));
}

PieceSprite::~PieceSprite(void)
{
}

void PieceSprite::spriteSetup(int playerID)
{
	int size = (int) GetSize().x;
	int x = 0;
	int y = playerID * size;
	SetSubRect(sf::IntRect(x, y, x + size, y + size));
	SetCenter((float)size / 2, (float)size / 2);
}

void PieceSprite::update(float elapsedTime)
{
	BoardAlignedSprite::update(elapsedTime);
	float ratio = GetScale().x;
	if (ratio < 1) //if appearing
	{
		ratio += m_appSpeed*elapsedTime;
		if (ratio > 1)
			ratio = 1;
		SetScale(ratio, ratio);
		sf::Color c = GetColor(); //saving current color in case the game is loading and the piece is frozen
		SetColor(sf::Color(c.r, c.g, c.b, (int)(255 * ratio)));
	}
}
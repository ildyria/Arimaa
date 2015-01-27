#include "PieceSprite.h"
#include <iostream>

#define PIECE_SIZE 135
#define FREEZE_COLOR sf::Color(100,100,255)
#define APPEARANCE_SPEED 25


PieceSprite::PieceSprite(int playerID, Grid* g, std::string image) : BoardAlignedSprite(g, image)
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
	int x = 0;
	int y = playerID * PIECE_SIZE;
	SetSubRect(sf::IntRect(x, y, x + PIECE_SIZE, y + PIECE_SIZE));
	SetCenter(PIECE_SIZE / 2, PIECE_SIZE / 2);
}

void PieceSprite::update(float elapsedTime)
{
	BoardAlignedSprite::update(elapsedTime);
	float ratio = GetScale().x;
	if (ratio < 1) //if appearing
	{
		ratio += APPEARANCE_SPEED*elapsedTime;
		if (ratio > 1)
			ratio = 1;
		SetScale(ratio, ratio);
		sf::Color c = GetColor(); //saving current color in case the game is loading and the piece is frozen
		SetColor(sf::Color(c.r, c.g, c.b, (int)(255 * ratio)));
	}
}
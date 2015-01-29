#pragma once
#include "BoardAlignedSprite.h"

class PieceSprite :
	public BoardAlignedSprite
{
public:
	PieceSprite(int playerID, Grid* g, std::string image = "Pieces.png", int appSpeed = 25);
	~PieceSprite(void);


	void update(float elapsedTime);

private:
	int m_appSpeed;
	void spriteSetup(int playerID = 0);
};

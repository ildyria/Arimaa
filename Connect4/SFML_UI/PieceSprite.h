#pragma once
#include "BoardAlignedSprite.h"

class PieceSprite :
	public BoardAlignedSprite
{
public:
	PieceSprite(int playerID, Grid* g, std::string image = "Connect4/Pieces.png");
	~PieceSprite(void);


	void update(float elapsedTime);

private:
	void spriteSetup(int playerID);
};

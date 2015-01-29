#pragma once
#include "BoardAlignedSprite.h"

class PieceSprite :
	public BoardAlignedSprite
{
public:
	PieceSprite(int playerID, Grid* g, std::string image = "Pieces.png");
	~PieceSprite(void);


	void update(float elapsedTime);

private:
	void spriteSetup(int playerID);
};

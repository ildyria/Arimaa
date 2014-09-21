#pragma once
#include "BoardAlignedSprite.h"
#include "../Model/Piece.h"

class PieceSprite :
	public BoardAlignedSprite
{
public:
	PieceSprite();
	PieceSprite(Piece* p, std::string image = "Pieces.png");
	~PieceSprite(void);

	void SetImage(const sf::Image& img);

	void freeze(bool frozen);

private:
	Piece* m_piece;
};

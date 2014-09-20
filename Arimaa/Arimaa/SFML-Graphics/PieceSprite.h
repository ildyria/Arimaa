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

	//inline bool moveIfSame(Piece* p, sf::Vector2i s) { if(p == m_piece) moveOnSquare(s); return (p == m_piece);  }
	void update(float elpasedTime);

private:
	Piece* m_piece;
};

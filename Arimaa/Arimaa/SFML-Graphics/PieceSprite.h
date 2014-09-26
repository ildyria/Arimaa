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
	void setPiece(Piece* p);

	void update(float elapsedTime);

	void freeze(bool frozen);
	inline void startDisappearing() { m_disappearing = true; }
	inline bool hasDisappeared() { return GetScale().x == 0; }

private:
	Piece* m_piece;
	bool m_disappearing;
};

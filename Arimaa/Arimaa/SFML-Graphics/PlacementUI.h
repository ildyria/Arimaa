#pragma once
#include "PieceSprite.h"
#include "ConfigOptions.h"

#define PIECES_GAP 135
#define PUI_X_POS (ConfigOptions::screenWidth() - 200)

class PlacementUI
{
public:
	PlacementUI(void);
	~PlacementUI(void);

	PieceType click(sf::Vector2f pos); //returns the selected piece or NB_PIECES if end of turn
	PieceType goUp(); //returns the selected piece
	PieceType goDown(); //returns the selected piece
	void select(PieceType t);
	void setColor(Color c);

	void loadAssets();
	void unloadAssets();
	void draw(sf::RenderWindow& app, bool canEndPlacement = true);

private:
	PieceSprite m_piecesSprites[NB_PIECES];
	PieceType m_selectedPiece;
	sf::Sprite m_endTurnButton;
};

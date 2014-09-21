#pragma once
#include <iostream>
#include <sstream>
#include "screen.h"
#include "InputHandler.h"
#include "ResourceManager.h"
#include "ConfigOptions.h"
#include "PieceSprite.h"
#include "../Model/Game.h"
#include "TurnSign.h"

#define NULL_SQUARE sf::Vector2i(-1,-1)

class GameScreen :
	public Screen
{
public:
	GameScreen(unsigned int myID);
	int update (sf::RenderWindow &app);
	void draw (sf::RenderWindow &app);
	void initialize ();
	void uninitialize ();
	~GameScreen(void);

private:
	InputHandler* m_iHandler;
	sf::Sprite m_background;
	BoardAlignedSprite m_cursor;
	BoardAlignedSprite m_selectionSprite;
	BoardAlignedSprite m_targettingSprite;
	std::map<Piece*, PieceSprite> m_pieces;
	sf::Sprite m_goldTurnIndicator;
	sf::Sprite m_silverTurnIndicator;
	TurnSign m_turnSign;
	sf::Sprite m_nbMovesSprite;

	PieceType m_selectedType;
	sf::Vector2i m_selectedPiece;
	sf::Vector2i m_selectedTarget;

	Game m_game;

	bool playerHasHand() const;

	void place(sf::Vector2i s);
	void remove(sf::Vector2i s);
	bool select(sf::Vector2i s); //returns true if a move is made
	void unselect();

	void selectPiece(sf::Vector2i s); //call only if there is a piece there
	void selectTarget(sf::Vector2i s); //call only if there is a piece there
	void updatePositionsAndDeath();

	void updateNbMoves();

	static inline sf::Vector2i toVector(const Square& s) { return sf::Vector2i(s.x, s.y); }
	static inline Square toSquare(const sf::Vector2i& v) { return Square(v.x, v.y); }
	
	static sf::Vector2i m_cardinals[];
	static bool areAdjacent(sf::Vector2i p1, sf::Vector2i p2);
};

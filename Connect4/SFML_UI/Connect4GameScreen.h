#pragma once
#include <iostream>
#include <sstream>
#include <thread>
#include "Screen.h"
#include "InputHandler.h"
#include "ResourceManager.h"
#include "ConfigOptions.h"
#include "PieceSprite.h"
#include "CenteredGrid.h"
#include "../API/Game.h"
#include "../API/Ai.h"
#include "WinSign.h"

class Connect4GameScreen :
	public Screen
{
public:
	Connect4GameScreen(unsigned int myID);
	int update (sf::RenderWindow &app);
	void draw (sf::RenderWindow &app);
	void initialize ();
	void uninitialize ();
	void clickOn(sf::Vector2i square);
	~Connect4GameScreen(void);

private:
	//Game
	api::Game m_game;
	bool m_p1AI;
	bool m_p2AI;
	api::Ai m_ai;
	bool m_AIThinking;

	//UI
	InputHandler* m_iHandler;
	Grid* m_grid;
	WinSign m_winSign;

	sf::Sprite m_background;
	std::list<PieceSprite*> m_pieces;
	sf::Sprite m_verticalHighlight;
	bool m_verticalHighlightVisible;

	void placePiece(int col);
	bool currPlayerHuman();
	void makeAIMove();
	void checkForWin();

	inline bool gameOver() { return m_game.getWinner() != 0; };
};

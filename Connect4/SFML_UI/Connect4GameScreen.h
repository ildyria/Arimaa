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
#include "DialogBubble.h"

class Connect4GameScreen :
	public Screen
{
public:
	Connect4GameScreen(unsigned int myID);
	int update (sf::RenderWindow &app);
	void draw (sf::RenderWindow &app);
	void initialize ();
	void uninitialize ();
	~Connect4GameScreen(void);

private:
	//Game
	api::Game* m_game;
	bool m_p1AI;
	bool m_p2AI;
	api::Ai m_ai;
	bool m_AIThinking;

	//UI
	InputHandler* m_iHandler;
	CenteredGrid m_grid;
	int m_cursor;

	sf::Sprite m_background;
	std::list<PieceSprite*> m_pieces;
	std::list<PieceSprite*> m_highlight;
	sf::Sprite m_verticalHighlight;
	bool m_verticalHighlightVisible;
	WinSign m_winSign;
	DialogBubble m_bubble;

	void clickOn(sf::Vector2i square);
	void moveCursor(int col);
	void moveCursorRel(int offset);
	void updateCursorPos();
	void updateCursorCol();
	void setCursorCol(int col);
	void placePiece(int col);
	void placeHighlight(sf::Vector2i pos);
	bool currPlayerHuman();
	void makeAIMove();
	void checkForWin();
	void refreshDialogText();
	void resetGame();

	inline bool gameOver() { return m_game->getWinner() != 0; };
};

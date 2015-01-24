#pragma once
#include <iostream>
#include <sstream>
#include "Screen.h"
#include "InputHandler.h"
#include "ResourceManager.h"
#include "ConfigOptions.h"
#include "BoardAlignedSprite.h"
#include "CenteredGrid.h"

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
	InputHandler* m_iHandler;
	Grid* m_grid;

	sf::Sprite m_background;
	std::list<BoardAlignedSprite*> m_pieces;
	sf::Sprite m_verticalHighlight;
	bool m_verticalHighlightVisible;

	BoardAlignedSprite* makePiece(bool blue);
};

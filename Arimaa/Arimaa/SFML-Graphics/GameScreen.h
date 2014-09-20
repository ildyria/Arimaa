#pragma once
#include <iostream>
#include "screen.h"
#include "InputHandler.h"
#include "ResourceManager.h"
#include "ConfigOptions.h"
#include "BoardAlignedSprite.h"

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
};

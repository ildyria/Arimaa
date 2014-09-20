#include "../StdAfx.h"
#include "GameScreen.h"

using namespace sf;

GameScreen::GameScreen(unsigned int myID) : Screen(myID), m_iHandler(ConfigOptions::getIHandler())
{
}

GameScreen::~GameScreen(void)
{
}

int GameScreen::update (sf::RenderWindow &app)
{
	int nextScreen = myID;
	float elapsedTime = app.GetFrameTime();

	Event event;
	while (app.GetEvent(event)) // Boucle des évènements en attente
	{
		if (event.Type == Event::Closed)
		{
			return EXIT;
		}
		else if(m_iHandler->testEvent(event, "Quit"))
		{
			return EXIT;
		}
		else if (event.Type == Event::MouseMoved)
		{
			m_cursor.moveOnSquare(BoardAlignedSprite::toSquares(sf::Vector2f( (float) event.MouseMove.X, (float) event.MouseMove.Y)));
		}
		else if (m_iHandler->testEvent(event, "LClick"))
		{

		}
	}

	return nextScreen;
}

void GameScreen::draw (sf::RenderWindow &app)
{
	app.Clear();
	
	app.Draw(m_background);
	app.Draw(m_cursor);

	app.Display();
}

void GameScreen::initialize ()
{
	if(m_background.GetImage() == NULL)
		m_background.SetImage(*ResourceManager::getImage("Board.png"));
	if(m_cursor.GetImage() == NULL)
		m_cursor.SetImage(*ResourceManager::getImage("Cursor.png"));
}

void GameScreen::uninitialize ()
{
	ResourceManager::unloadImage("Board.png");
	ResourceManager::unloadImage("Cursor.png");
}
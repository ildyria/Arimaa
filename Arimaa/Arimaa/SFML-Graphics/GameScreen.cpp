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
	}

	return nextScreen;
}

void GameScreen::draw (sf::RenderWindow &app)
{
	app.Clear();
	
	app.Draw(m_background);

	app.Display();
}

void GameScreen::initialize ()
{
	if(m_background.GetImage() == NULL)
		m_background.SetImage(*ResourceManager::getImage("Assets/Graphics/Board.png"));
}

void GameScreen::uninitialize ()
{
	ResourceManager::unloadImage("Assets/Graphics/Board.png");
}
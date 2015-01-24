#include "Connect4GameScreen.h"

Connect4GameScreen::Connect4GameScreen(unsigned int myID) : Screen(myID), m_iHandler(ConfigOptions::getIHandler()), m_verticalHighlightVisible(false), m_grid(new CenteredGrid(135, sf::Vector2i(7, 6)))
{
}

Connect4GameScreen::~Connect4GameScreen(void)
{
}

int Connect4GameScreen::update (sf::RenderWindow &app)
{
	int nextScreen = myID;
	float elapsedTime = app.GetFrameTime();

	sf::Event event;
	while (app.GetEvent(event)) //waiting events' loop
	{
		if (event.Type == sf::Event::Closed)
		{
			return EXIT;
		}
		else if(m_iHandler->testEvent(event, "Quit"))
		{
			return EXIT;
		}
		else if (event.Type == sf::Event::MouseMoved)
		{
			sf::Vector2f pos = app.ConvertCoords(event.MouseMove.X, event.MouseMove.Y, &ConfigOptions::getView());
			sf::Vector2i squ = m_grid->toSquares(pos);
			squ.y = 0; //only considers the x value
			if (m_grid->isOnGrid(squ))
			{
				m_verticalHighlight.SetPosition(m_grid->toPixels(squ).x, 0);
				m_verticalHighlightVisible = true;
			}
			else
				m_verticalHighlightVisible = false;
		}
	} //end of event loop

	return nextScreen;
}

void Connect4GameScreen::draw (sf::RenderWindow &app)
{
	app.Clear();
	app.SetView(ConfigOptions::getView()); //switching to custom view for easy resizing of the screen

	//START DRAWING
	app.Draw(m_background);

	for (BoardAlignedSprite* p : m_pieces)
		app.Draw(*p);

	if (m_verticalHighlightVisible)
		app.Draw(m_verticalHighlight);
	//STOP DRAWING

	app.Display();
}

void Connect4GameScreen::initialize ()
{
	if(m_background.GetImage() == nullptr)
		m_background.SetImage(*ResourceManager::getImage("Connect4/Board.png"));

	if (m_verticalHighlight.GetImage() == nullptr)
	{
		m_verticalHighlight.SetImage(*ResourceManager::getImage("Connect4/VerticalHighlight.png"));
		m_verticalHighlight.SetCenter(m_verticalHighlight.GetSize().x / 2, 0);
	}

	ResourceManager::getImage("Connect4/Pieces.png");
}

void Connect4GameScreen::uninitialize ()
{
	ResourceManager::unloadImage("Connect4/Board.png");
	ResourceManager::unloadImage("Connect4/Pieces.png");
	ResourceManager::unloadImage("Connect4/VerticalHighlight.png");
}

BoardAlignedSprite* Connect4GameScreen::makePiece(bool blue)
{
	BoardAlignedSprite* b = new BoardAlignedSprite(m_grid, "Connect4/Pieces.png");
	float delta = 0;
	if (blue)
		delta = b->GetSize().y;
	b->SetSubRect(sf::IntRect(0, (int) delta, (int) b->GetSize().x, (int) (b->GetSize().y + delta)));
	return b;
}
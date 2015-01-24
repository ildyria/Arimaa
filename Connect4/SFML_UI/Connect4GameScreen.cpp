#include "Connect4GameScreen.h"

#define NB_COL 7
#define NB_ROW 6
#define CENTER_OFFSET sf::Vector2f(0, 67.5)

Connect4GameScreen::Connect4GameScreen(unsigned int myID) : Screen(myID), m_iHandler(ConfigOptions::getIHandler()), m_verticalHighlightVisible(false), m_grid(new CenteredGrid(135, sf::Vector2i(NB_COL, NB_ROW), ConfigOptions::getNativeCenter() + CENTER_OFFSET)), m_game()
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
		else if (m_iHandler->testEvent(event, "LClick"))
		{
			sf::Vector2f mouseCoords = app.ConvertCoords(event.MouseButton.X, event.MouseButton.Y, &ConfigOptions::getView());
			sf::Vector2i s = m_grid->toSquares(mouseCoords);
			if (m_grid->isOnGrid(s))
				clickOn(s);
		}
	} //end of event loop

	
	for (PieceSprite* p : m_pieces)
		p->update(elapsedTime);

	return nextScreen;
}

void Connect4GameScreen::draw (sf::RenderWindow &app)
{
	app.Clear();
	app.SetView(ConfigOptions::getView()); //switching to custom view for easy resizing of the screen

	//START DRAWING
	app.Draw(m_background);

	for (PieceSprite* p : m_pieces)
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

void Connect4GameScreen::clickOn(sf::Vector2i square)
{
	int col = square.x;// +1;
	if (m_game.makeMove(col)) //tries to make a move
	{
		sf::Vector2i piecePos(col, NB_ROW - m_game.colHeight(col));
		m_pieces.push_back(new PieceSprite(m_game.activePlayer() - 1, m_grid));
		m_pieces.back()->moveOnSquare(piecePos);
	}
}
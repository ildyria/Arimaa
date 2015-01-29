#include "Connect4GameScreen.h"

#define NB_COL 7
#define NB_ROW 7
#define CENTER_OFFSET sf::Vector2f(0, 0)//67.5)

Connect4GameScreen::Connect4GameScreen(unsigned int myID) : Screen(myID), m_iHandler(ConfigOptions::getIHandler()), m_verticalHighlightVisible(false),
m_grid(new CenteredGrid(135, sf::Vector2i(NB_COL, NB_ROW), ConfigOptions::getNativeCenter() + CENTER_OFFSET)), m_game(), m_p1AI(ConfigOptions::getP1AI()), m_p2AI(ConfigOptions::getP2AI()),
m_ai(ConfigOptions::getAiThinkingTime()), m_AIThinking(false), m_cursor(-1)
{
}

Connect4GameScreen::~Connect4GameScreen(void)
{
	for (PieceSprite* p : m_pieces)
		delete p;

	for (PieceSprite* h : m_highlight)
		delete h;
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
			moveCursor(m_grid->toSquares(pos).x);
		}
		else if (m_iHandler->testEvent(event, "Left"))
		{
			moveCursorRel(-1);
		}
		else if (m_iHandler->testEvent(event, "Right"))
		{
			moveCursorRel(1);
		}
		else if (m_iHandler->testEvent(event, "LClick"))
		{
			sf::Vector2f mouseCoords = app.ConvertCoords(event.MouseButton.X, event.MouseButton.Y, &ConfigOptions::getView());
			sf::Vector2i s = m_grid->toSquares(mouseCoords);
			if (m_grid->isOnGrid(s))
				clickOn(s);
		}
		else if (m_iHandler->testEvent(event, "Select"))
		{
			if (m_cursor != -1)
				clickOn(sf::Vector2i(m_cursor,0));
		}
	} //end of event loop

	if (!gameOver() && !currPlayerHuman() && !m_AIThinking) //if the AI plays this turn and the AI is not thinking and the game is not over
	{
		m_AIThinking = true;
		std::thread (&Connect4GameScreen::makeAIMove, this).detach();
	}
	
	for (PieceSprite* p : m_pieces)
		p->update(elapsedTime);

	for (PieceSprite* h : m_highlight)
		h->update(elapsedTime);

	m_winSign.update(elapsedTime);

	return nextScreen;
}

void Connect4GameScreen::draw (sf::RenderWindow &app)
{
	app.Clear();
	app.SetView(ConfigOptions::getView()); //switching to custom view for easy resizing of the screen

	//START DRAWING
	app.Draw(m_background);

	for (PieceSprite* h : m_highlight)
		app.Draw(*h);

	for (PieceSprite* p : m_pieces)
		app.Draw(*p);

	if (m_verticalHighlightVisible)
		app.Draw(m_verticalHighlight);

	m_winSign.draw(app);
	//STOP DRAWING

	app.Display();
}

void Connect4GameScreen::initialize ()
{
	if(m_background.GetImage() == nullptr)
		m_background.SetImage(*ResourceManager::getImage("Board.png"));

	if (m_verticalHighlight.GetImage() == nullptr)
	{
		m_verticalHighlight.SetImage(*ResourceManager::getImage("VerticalHighlight.png"));
		m_verticalHighlight.SetCenter(m_verticalHighlight.GetSize().x / 2, 0);
	}

	ResourceManager::getImage("Pieces.png");
	m_winSign.loadAssets();

	if (m_p1AI || m_p2AI) //if there is an AI, load the AI
		m_ai.init(&m_game);
}

void Connect4GameScreen::uninitialize ()
{
	ResourceManager::unloadImage("Board.png");
	ResourceManager::unloadImage("Pieces.png");
	ResourceManager::unloadImage("VerticalHighlight.png");
	m_winSign.unloadAssets();
}

void Connect4GameScreen::clickOn(sf::Vector2i square)
{
	if (!gameOver() && currPlayerHuman())  //if a human plays this turn and the game is still on
	{
		int col = square.x;
		if (m_game.makeMove(col+1)) //tries to make a move
		{
			placePiece(col);
			checkForWin();
		}
	}
}

void Connect4GameScreen::moveCursor(int col)
{
	sf::Vector2i v = sf::Vector2i(col, 0); //only considers the x value

	if (m_grid->isOnGrid(v))
		m_cursor = col;
	else
		m_cursor = -1;

	updateCursorSprite();
}

void Connect4GameScreen::moveCursorRel(int offset)
{
	if (m_cursor == -1)
		offset = (offset + 1) / 2; //going right from nothing places you in col 0, going left in col -1

	m_cursor += offset;
	m_cursor = (m_cursor + NB_COL) % NB_COL;
	updateCursorSprite();
}

void Connect4GameScreen::updateCursorSprite()
{
	if (m_cursor == -1)
		m_verticalHighlightVisible = false;
	else
	{
		m_verticalHighlightVisible = true;
		m_verticalHighlight.SetPosition(m_grid->toPixels(sf::Vector2i(m_cursor, 0)).x, 0);
	}
}

void Connect4GameScreen::placePiece(int col)
{
	sf::Vector2i piecePos(col, NB_ROW - m_game.colHeight(col + 1));
	sf::Vector2i origPiecePos(col, 0);
	m_pieces.push_back(new PieceSprite(m_game.activePlayer() - 1, m_grid));
	m_pieces.back()->moveOnSquare(origPiecePos);
	m_pieces.back()->moveOnSquare(piecePos, false);
}

void Connect4GameScreen::placeHighlight(sf::Vector2i pos)
{
	sf::Vector2i piecePos(NB_COL - pos.x - 1, NB_ROW - pos.y - 1);
	m_highlight.push_back(new PieceSprite(0, m_grid, "Highlight.png", 5));
	m_highlight.back()->moveOnSquare(piecePos);
}

bool Connect4GameScreen::currPlayerHuman()
{
	if (m_game.activePlayer() == 1)
		return !m_p1AI;
	//else
	return !m_p2AI;
}


void Connect4GameScreen::makeAIMove()
{
	int col = m_ai.makeMove(!(m_p1AI && m_p2AI)) - 1;
	m_game.makeMove(col + 1);
	placePiece(col);
	checkForWin();
	m_AIThinking = false;
}

void Connect4GameScreen::checkForWin()
{
	if (gameOver())
	{
		//win sign
		m_winSign.activate(m_game.getWinner()-1);

		//highlight
		auto highlightPos = m_game.getWinningLine();
		for (std::pair<int, int> p : highlightPos)
		{
			placeHighlight(sfmlop::toVect2(p));
		}
	}
}
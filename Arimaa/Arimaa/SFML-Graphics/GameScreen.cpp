#include "../StdAfx.h"
#include "GameScreen.h"

using namespace sf;

sf::Vector2i GameScreen::m_cardinals[] = {sf::Vector2i(1,0), sf::Vector2i(0,1), sf::Vector2i(-1,0), sf::Vector2i(0,-1)};

GameScreen::GameScreen(unsigned int myID) : Screen(myID), m_iHandler(ConfigOptions::getIHandler()), m_selectedType(RABBIT)
{
	unselect();
	m_playerText.SetPosition(0,0);
	m_movesLeftText.SetPosition(0,100);
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
			sf::Vector2i s = BoardAlignedSprite::toSquares(sf::Vector2f( (float) event.MouseButton.X, (float) event.MouseButton.Y));
			if(m_game.hasStarted())
			{
				if(select(s)) //a move was made
				{
					updatePositionsAndDeath();
					updateTexts();
				}
			}
			else
				place(s);
		}
		else if (m_iHandler->testEvent(event, "RClick"))
		{
			sf::Vector2i s = BoardAlignedSprite::toSquares(sf::Vector2f( (float) event.MouseButton.X, (float) event.MouseButton.Y));
			if(!m_game.hasStarted())
				remove(s);
		}
		else if (m_iHandler->testEvent(event, "Confirm"))
		{
			if(m_game.endPlacement())
			{
				updateTexts();
			}
		}
		else if (m_iHandler->testEvent(event, "Rabbit"))
		{
			m_selectedType = RABBIT;
		}
		else if (m_iHandler->testEvent(event, "Cat"))
		{
			m_selectedType = CAT;
		}
		else if (m_iHandler->testEvent(event, "Dog"))
		{
			m_selectedType = DOG;
		}
		else if (m_iHandler->testEvent(event, "Horse"))
		{
			m_selectedType = HORSE;
		}
		else if (m_iHandler->testEvent(event, "Camel"))
		{
			m_selectedType = CAMEL;
		}
		else if (m_iHandler->testEvent(event, "Elephant"))
		{
			m_selectedType = ELEPHANT;
		}
	}

	return nextScreen;
}

void GameScreen::draw (sf::RenderWindow &app)
{
	app.Clear();
	
	app.Draw(m_background);
	if(m_selectedPiece != NULL_SQUARE)
		app.Draw(m_selectionSprite);
	if(m_selectedTarget != NULL_SQUARE)
		app.Draw(m_targettingSprite);
	app.Draw(m_cursor);
	for(std::map<Piece*, PieceSprite>::iterator it = m_pieces.begin(); it != m_pieces.end(); ++it)
		app.Draw(it->second);
	app.Draw(m_playerText);
	app.Draw(m_movesLeftText);

	app.Display();
}

void GameScreen::initialize ()
{
	if(m_background.GetImage() == NULL)
		m_background.SetImage(*ResourceManager::getImage("Board.png"));
	if(m_cursor.GetImage() == NULL)
		m_cursor.SetImage(*ResourceManager::getImage("Cursor.png"));
	if(m_selectionSprite.GetImage() == NULL)
		m_selectionSprite.SetImage(*ResourceManager::getImage("Selection.png"));
	if(m_targettingSprite.GetImage() == NULL)
		m_targettingSprite.SetImage(*ResourceManager::getImage("Targetting.png"));

	m_movesLeftText.SetFont(*ResourceManager::getFont());
	m_playerText.SetFont(*ResourceManager::getFont());
	updateTexts();
}

void GameScreen::uninitialize ()
{
	ResourceManager::unloadImage("Board.png");
	ResourceManager::unloadImage("Cursor.png");
	ResourceManager::unloadFont();
}

void GameScreen::place(sf::Vector2i s)
{
	if(BoardAlignedSprite::isOnBoard(s)) //there is a piece to be placed, and the square to place it in is valid
	{
		Piece* p = m_game[toSquare(s)];
		PieceType oldPieceType = NB_PIECES;
		if(p != NULL)
			oldPieceType = p->getType();
		remove(s); //removing any piece that would happen to be here
		if(m_game.place(m_selectedType, toSquare(s))) //if the placement was a success
		{
			m_pieces[m_game[toSquare(s)]] = PieceSprite(m_game[toSquare(s)]);
			m_pieces[m_game[toSquare(s)]].moveOnSquare(s);
		}
		else if (oldPieceType != NB_PIECES) //the placement has failed : we put back the piece that was there before
		{
			m_game.place(oldPieceType, toSquare(s));
			m_pieces[m_game[toSquare(s)]] = PieceSprite(m_game[toSquare(s)]);
			m_pieces[m_game[toSquare(s)]].moveOnSquare(s);
		}
	}
}

void GameScreen::remove(sf::Vector2i s)
{
	if(BoardAlignedSprite::isOnBoard(s) && m_game[toSquare(s)] != NULL) //there is a piece to be removed
	{
		if(m_game.remove(toSquare(s))) //removing the actual piece
			m_pieces.erase(m_game[toSquare(s)]); //removing piece sprite
	}
}

bool GameScreen::select(sf::Vector2i s)
{
	if(m_selectedPiece != NULL_SQUARE) //if a piece is selected
	{
		if(m_selectedTarget != NULL_SQUARE) //if an enemy piece is selected
		{
			if(!areAdjacent(s, m_selectedTarget)) //squares aren't adjacent, unselect
				unselect();
			else //squares are adjacent
			{
				bool res = m_game.displace(toSquare(m_selectedPiece), toSquare(s), toSquare(m_selectedTarget), false);
				unselect();
				return res;
			}
		}
		else //no enemy piece selected
		{
			if(!areAdjacent(s, m_selectedPiece)) //squares aren't adjacent, unselect
				unselect();
			else //squares are adjacent
			{
				Piece* p = m_game[toSquare(s)];
				if(p == NULL) //no piece here
				{
					bool res = m_game.move(toSquare(m_selectedPiece), toSquare(s), false);
					unselect();
					return res;
				}
				else if(p->getColor() != m_game.getActivePlayer() && p < m_game[toSquare(m_selectedPiece)]) //the piece is an enemy one, and it can be displaced
					selectTarget(s);
				else
					unselect();
			}
		}
	}
	else //no piece is selected
	{
		Piece* p = m_game[toSquare(s)];
		if(p != NULL && p->getColor() == m_game.getActivePlayer())  //there is a piece and it can be selected
			selectPiece(s);
	}
	return false;
}

void GameScreen::unselect()
{
	m_selectedPiece = NULL_SQUARE;
	m_selectedTarget = NULL_SQUARE;
}

bool GameScreen::areAdjacent(sf::Vector2i p1, sf::Vector2i p2)
{
	for(int i = 0; i < 4; ++i) //checks the 4 directions
	{
		if(p2 == p1 + m_cardinals[i])
			return true;
	}
	return false;
}

void GameScreen::selectPiece(sf::Vector2i s)
{
	m_selectedPiece = s;
	m_selectionSprite.moveOnSquare(s);
}

void GameScreen::selectTarget(sf::Vector2i s)
{
	m_selectedTarget = s;
	m_targettingSprite.moveOnSquare(s);
}

void GameScreen::updatePositionsAndDeath()
{
	std::vector<Piece*> endangeredPieces;
	for(int i = 0; i < BOARD_SIZE; ++i)
	{
		for(int j = 0; j < BOARD_SIZE; ++j)
		{
			Piece* p = m_game[Square(i,j)];
			if(m_game.isTrap(Square(i,j)))
				endangeredPieces.push_back(p);
			if(p != NULL)
			{
				m_pieces[p].moveOnSquare(sf::Vector2i(i,j));
			}
		}
	}

	m_game.applyDeaths();

	std::vector<Piece*> survivingPieces;
	for(int i = 0; i < BOARD_SIZE; ++i)
	{
		for(int j = 0; j < BOARD_SIZE; ++j)
		{
			if(m_game.isTrap(Square(i,j)))
				survivingPieces.push_back(m_game[Square(i,j)]);
		}
	}

	for(int i = 0; i < NB_TRAPS; ++i)
	{
		if(endangeredPieces[i] != survivingPieces[i]) //the piece has died (replaced by NULL)
			m_pieces.erase(endangeredPieces[i]);
	}

}
void GameScreen::updateTexts()
{
	std::stringstream s1;
	s1 << m_game.getMovesLeft();
	m_movesLeftText.SetText( s1.str() );

	std::stringstream s2;
	s2 << m_game.getActivePlayer();
	m_playerText.SetText( s2.str() );
}
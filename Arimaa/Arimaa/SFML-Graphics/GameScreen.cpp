#include "../StdAfx.h"
#include "GameScreen.h"

#define TURN_INDICATOR_HEIGHT 150
#define NB_MOVES_X 120
#define NB_BG_X 20
#define VICT_APP_SPD 1

#define BACKUP_FILE "Backup.arm"

sf::Vector2i GameScreen::m_cardinals[] = {sf::Vector2i(1,0), sf::Vector2i(0,1), sf::Vector2i(-1,0), sf::Vector2i(0,-1)};

GameScreen::GameScreen(unsigned int myID) : Screen(myID), m_iHandler(ConfigOptions::getIHandler()), m_selectedType(RABBIT), m_victor(NB_PLAYERS)
{
	unselect();
	m_goldTurnIndicator.SetPosition(0, TURN_INDICATOR_HEIGHT);
	m_silverTurnIndicator.SetPosition(0, (float) ConfigOptions::nativeHeight() - TURN_INDICATOR_HEIGHT);
	m_nbMovesSprite.SetPosition(NB_MOVES_X, (float) ConfigOptions::nativeHeight() /2);
	m_movesBackgroundSprite.SetPosition(NB_BG_X, (float) ConfigOptions::nativeHeight() /2);
	m_cursor.moveOnSquare(sf::Vector2i(0,0));
	m_victorySign.SetPosition(ConfigOptions::getNativeCenter());
	m_victorySign.SetColor(sf::Color(255,255,255,1));
}

GameScreen::~GameScreen(void)
{
}

int GameScreen::update (sf::RenderWindow &app)
{
	int nextScreen = myID;
	float elapsedTime = app.GetFrameTime();

	sf::Event event;
	while (app.GetEvent(event)) // Boucle des évènements en attente
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
			sf::Vector2f mouseCoords = app.ConvertCoords(event.MouseMove.X, event.MouseMove.Y, &ConfigOptions::getView());
			m_cursor.moveOnSquare(BoardAlignedSprite::toSquares(mouseCoords), false);
		}
		else if(m_iHandler->testEvent(event, "Up"))
		{
			sf::Vector2i s = BoardAlignedSprite::toSquares(m_cursor.GetPosition()) + sf::Vector2i(0, -1);
			if(isValid(s))
				m_cursor.moveOnSquare(s, false);
		}
		else if(m_iHandler->testEvent(event, "Left"))
		{
			sf::Vector2i s = BoardAlignedSprite::toSquares(m_cursor.GetPosition()) + sf::Vector2i(-1, 0);
			if(isValid(s))
				m_cursor.moveOnSquare(s, false);
		}
		else if(m_iHandler->testEvent(event, "Down"))
		{
			sf::Vector2i s = BoardAlignedSprite::toSquares(m_cursor.GetPosition()) + sf::Vector2i(0, 1);
			if(isValid(s))
				m_cursor.moveOnSquare(s, false);
		}
		else if(m_iHandler->testEvent(event, "Right"))
		{
			sf::Vector2i s = BoardAlignedSprite::toSquares(m_cursor.GetPosition()) + sf::Vector2i(1, 0);
			if(isValid(s))
				m_cursor.moveOnSquare(s, false);
		}
		else if(playerHasHand())
		{
			if(m_iHandler->testEvent(event, "Save"))
			{
				m_game.saveToFile(BACKUP_FILE);
			}
			else if(m_iHandler->testEvent(event, "Load"))
			{
				m_game.loadFromFile(BACKUP_FILE);
				refreshAll();
			}
			else if (m_iHandler->testEvent(event, "LClick"))
			{
				sf::Vector2f mouseCoords = app.ConvertCoords(event.MouseButton.X, event.MouseButton.Y, &ConfigOptions::getView());
				sf::Vector2i s = BoardAlignedSprite::toSquares(mouseCoords);
				if(isValid(s))
					clickOn(s);
				else if(!m_game.hasStarted())
				{
					PieceType type = m_placementUI.click(mouseCoords);
					if(type == NB_PIECES) //end of turn
						tryAndEndTurn();
					else
						m_selectedType = type;
				}
			}
			if (m_iHandler->testEvent(event, "Choose"))
			{
				sf::Vector2i s = BoardAlignedSprite::toSquares(m_cursor.GetPosition());
				if(isValid(s))
					clickOn(s);
			}
			else if (m_iHandler->testEvent(event, "RClick"))
			{
				sf::Vector2f mouseCoords = app.ConvertCoords(event.MouseButton.X, event.MouseButton.Y, &ConfigOptions::getView());
				sf::Vector2i s = BoardAlignedSprite::toSquares(mouseCoords);
				if(!m_game.hasStarted())
					remove(s);
			}
			else if (m_iHandler->testEvent(event, "EndTurn"))
			{
				tryAndEndTurn();
			}
			else if (event.Type == sf::Event::MouseWheelMoved)
			{
				if(event.MouseWheel.Delta > 0)
				{
					m_selectedType = m_placementUI.goUp();
				}
				else
				{
					m_selectedType = m_placementUI.goDown();
				}
			}
			else if (m_iHandler->testEvent(event, "Rabbit"))
			{
				selectPieceType(RABBIT);
			}
			else if (m_iHandler->testEvent(event, "Cat"))
			{
				selectPieceType(CAT);
			}
			else if (m_iHandler->testEvent(event, "Dog"))
			{
				selectPieceType(DOG);
			}
			else if (m_iHandler->testEvent(event, "Horse"))
			{
				selectPieceType(HORSE);
			}
			else if (m_iHandler->testEvent(event, "Camel"))
			{
				selectPieceType(CAMEL);
			}
			else if (m_iHandler->testEvent(event, "Elephant"))
			{
				selectPieceType(ELEPHANT);
			}
		} //end of if(playerHasHand)
		else if(isOver() && m_victorySign.GetColor().a == 255 && (m_iHandler->testEvent(event, "EndTurn") || m_iHandler->testEvent(event, "Choose") || m_iHandler->testEvent(event, "LClick")))
		{
			//if we do something after the match ends, remove the victory sign
			m_victorySign.SetColor(sf::Color(255,255,255,0));
		}

	} //end of event loop

	m_turnSign.update(elapsedTime);
	m_cursor.update(elapsedTime);
	for(std::map<PiecePtr, PieceSprite>::iterator it = m_pieces.begin(); it != m_pieces.end(); ++it)
		it->second.update(elapsedTime);

	//removing pieces that have finished disappearing
	for (auto it = m_disappearingPieces.begin(); it != m_disappearingPieces.end(); ) //++it done below if necessary
	{
		PiecePtr p = *it;
		if (m_pieces[p].hasDisappeared())
		{
			m_pieces.erase(p);
			it = m_disappearingPieces.erase(it); //gives it the value of the next element, thus no need to increase it again
		}
		else
			++it;
	}

	if(isOver()) //manages the appearance of the victory sign
	{
		int alpha = m_victorySign.GetColor().a;
		if(alpha > 0 && alpha < 255) //0 means it's disappeared already
		{
			alpha += (int) (elapsedTime * VICT_APP_SPD * 255);
			if(alpha > 255)
				alpha = 255;
			m_victorySign.SetColor(sf::Color(255,255,255,alpha));
		}
	}

	return nextScreen;
}

void GameScreen::draw (sf::RenderWindow &app)
{
	app.Clear();
	app.SetView(ConfigOptions::getView()); //switching to custom view for easy resizing of the screen
	
	app.Draw(m_background);
	m_highlighter.draw(app);
	if(m_selectedPiece != NULL_SQUARE)
		app.Draw(m_selectionSprite);
	if(m_selectedTarget != NULL_SQUARE)
		app.Draw(m_targettingSprite);
	app.Draw(m_cursor);
	for(std::map<PiecePtr, PieceSprite>::iterator it = m_pieces.begin(); it != m_pieces.end(); ++it)
		app.Draw(it->second);
	if(!isOver())
	{
		if(m_game.getActivePlayer() == GOLD)
			app.Draw(m_goldTurnIndicator);
		else //SILVER
			app.Draw(m_silverTurnIndicator);
		if(m_game.hasStarted())
		{
			app.Draw(m_movesBackgroundSprite);
			app.Draw(m_nbMovesSprite);
		}
		else //game has not started
		{
			m_placementUI.draw(app, m_game.canEndPlacement());
		}
		m_turnSign.draw(app);
	}
	else //the game is over
		app.Draw(m_victorySign);

	app.SetView(app.GetDefaultView()); //switching back to default view
	app.Display();
}

void GameScreen::initialize ()
{
	if(m_background.GetImage() == nullptr)
		m_background.SetImage(*ResourceManager::getImage("Board.png"));
	ResourceManager::getImage("Pieces.png");
	if (m_cursor.GetImage() == nullptr)
		m_cursor.SetImage(*ResourceManager::getImage("Cursor.png"));
	if (m_selectionSprite.GetImage() == nullptr)
		m_selectionSprite.SetImage(*ResourceManager::getImage("Selection.png"));
	if (m_targettingSprite.GetImage() == nullptr)
		m_targettingSprite.SetImage(*ResourceManager::getImage("Targetting.png"));
	if (m_goldTurnIndicator.GetImage() == nullptr)
	{
		m_goldTurnIndicator.SetImage(*ResourceManager::getImage("Gold_Turn.png"));
		m_goldTurnIndicator.SetCenter(0, m_goldTurnIndicator.GetSize().y/2);
	}
	if (m_silverTurnIndicator.GetImage() == nullptr)
	{
		m_silverTurnIndicator.SetImage(*ResourceManager::getImage("Silver_Turn.png"));
		m_silverTurnIndicator.SetCenter(0, m_silverTurnIndicator.GetSize().y/2);
	}
	if (m_nbMovesSprite.GetImage() == nullptr)
	{
		m_nbMovesSprite.SetImage(*ResourceManager::getImage("Nb_Moves.png"));
		m_nbMovesSprite.SetCenter(0, (float) m_nbMovesSprite.GetImage()->GetHeight()/8); // height/8 because 4 sprites
	}
	if (m_movesBackgroundSprite.GetImage() == nullptr)
	{
		m_movesBackgroundSprite.SetImage(*ResourceManager::getImage("Moves_Back.png"));
		m_movesBackgroundSprite.SetCenter(0, m_movesBackgroundSprite.GetSize().y/2);
	}
	if (m_victorySign.GetImage() == nullptr)
	{
		m_victorySign.SetImage(*ResourceManager::getImage("Victory_Sign.png"));
		m_victorySign.SetCenter(m_victorySign.GetSize().x/2, m_victorySign.GetSize().y/4);
	}
	
	m_turnSign.loadAssets();
	m_highlighter.loadAssets();
	m_placementUI.loadAssets();
	updateNbMoves();
}

void GameScreen::uninitialize ()
{
	ResourceManager::unloadImage("Board.png");
	ResourceManager::unloadImage("Cursor.png");
	ResourceManager::unloadImage("Selection.png");
	ResourceManager::unloadImage("Targetting.png");
	ResourceManager::unloadImage("Gold_Turn.png");
	ResourceManager::unloadImage("Silver_Turn.png");
	ResourceManager::unloadImage("Nb_Moves.png");
	ResourceManager::unloadImage("Moves_Back.png");
	ResourceManager::unloadImage("Victory_Sign.png");
	m_turnSign.unloadAssets();
	m_highlighter.unloadAssets();
	m_placementUI.unloadAssets();
}

bool GameScreen::playerHasHand() const
{
	return !(m_turnSign.isActive() || isOver());
}

void GameScreen::clickOn(sf::Vector2i s)
{
	if(m_game.hasStarted())
	{
		Color oldPlayer = m_game.getActivePlayer();
		if(select(s)) //a move was made
		{
			updatePositionsAndDeath();
			updateNbMoves();
			//checks if the game is over
			setVictor(m_game.getWinner());
			
			if(!isOver())
			{
				Color newPlayer = m_game.getActivePlayer();
				if(oldPlayer != newPlayer) //turn is over
					m_turnSign.activate(newPlayer == GOLD);
			}
		}
	}
	else
		place(s);
}

void GameScreen::place(sf::Vector2i s)
{
	if(BoardAlignedSprite::isOnBoard(s)) //there is a piece to be placed, and the square to place it in is valid
	{
		PiecePtr p = m_game[toSquare(s)];
		PieceType oldPieceType = NB_PIECES;
		if (p != nullptr)
			oldPieceType = p->getType();
		remove(s); //removing any piece that would happen to be here
		if(m_game.place(m_selectedType, toSquare(s))) //if the placement was a success
		{
			m_pieces[m_game[toSquare(s)]] = PieceSprite(m_game[toSquare(s)]);
			m_pieces[m_game[toSquare(s)]].moveOnSquare(s);
			updatePieceAvailability(m_selectedType);//updating the availability in placement UI
			if(oldPieceType != NB_PIECES)
				updatePieceAvailability(oldPieceType); //id a piece was replaced, update its availability as well
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
	if(BoardAlignedSprite::isOnBoard(s)) //there is a piece to be removed
	{
		PiecePtr p = m_game[toSquare(s)];
		if (p != nullptr && m_game.remove(toSquare(s))) //removing the actual piece
		{
			killPieceSprite(p); //removing piece sprite
			updatePieceAvailability(m_selectedType);//updating the availability in placement UI
		}
	}
}

bool GameScreen::select(sf::Vector2i s)
{
	if(m_selectedPiece != NULL_SQUARE) //if a piece is selected
	{
		if(m_selectedTarget != NULL_SQUARE) //if an enemy piece is selected
		{
			if(!m_highlighter.isHighlighted(s)) //square isn't selectable
				unselect();
			else //square is selectable
			{
				bool res = m_game.displace(toSquare(m_selectedPiece), toSquare(s), toSquare(m_selectedTarget), false);
				unselect();
				return res;
			}
		}
		else //no enemy piece selected
		{
			if(!m_highlighter.isHighlighted(s)) //square isn't selectable
				unselect();
			else //square is selectable
			{
				PiecePtr p = m_game[toSquare(s)];
				if (p == nullptr) //no piece here
				{
					bool res = m_game.move(toSquare(m_selectedPiece), toSquare(s), false);
					unselect();
					return res;
				}
				else
					selectTarget(s);
			}
		}
	}
	else //no piece is selected
	{
		PiecePtr p = m_game[toSquare(s)];
		if (p != nullptr && p->getColor() == m_game.getActivePlayer())  //there is a piece and it can be selected
			selectPiece(s);
	}
	return false;
}

void GameScreen::unselect()
{
	m_selectedPiece = NULL_SQUARE;
	m_selectedTarget = NULL_SQUARE;
	m_highlighter.removeHighlights();
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

	std::vector<Square> possibleMoves = m_game.getPossibleMoves(toSquare(s));
	std::vector<sf::Vector2i> possibleMoves2;
	for(unsigned int i = 0; i < possibleMoves.size(); ++i)
	{
		possibleMoves2.push_back(toVector(possibleMoves[i]));
	}
	m_highlighter.setHighlights(possibleMoves2);
}

void GameScreen::selectTarget(sf::Vector2i s)
{
	m_selectedTarget = s;
	m_targettingSprite.moveOnSquare(s);

	std::vector<Square> possibleMoves = m_game.getPossibleDisplacements(toSquare(m_selectedPiece), toSquare(s));
	std::vector<sf::Vector2i> possibleMoves2;
	for(unsigned int i = 0; i < possibleMoves.size(); ++i)
	{
		possibleMoves2.push_back(toVector(possibleMoves[i]));
	}
	m_highlighter.setHighlights(possibleMoves2);
}

bool GameScreen::tryAndEndTurn()
{
	if(m_game.endPlacement())
	{
		m_turnSign.activate(m_game.getActivePlayer() == GOLD);
		m_placementUI.setPlayer(m_game.getActivePlayer());
		m_placementUI.resetAvailability();
		selectPieceType(RABBIT);
		return true;
	}
	return false;
}

void GameScreen::updatePositionsAndDeath()
{
	//updates movement
	std::vector<PiecePtr> endangeredPieces;
	for(int i = 0; i < BOARD_SIZE; ++i)
	{
		for(int j = 0; j < BOARD_SIZE; ++j)
		{
			PiecePtr p = m_game[Square(i,j)];
			if(m_game.isTrap(Square(i,j)))
				endangeredPieces.push_back(p);
			if (p != nullptr)
			{
				m_pieces[p].moveOnSquare(sf::Vector2i(i,j), false);
			}
		}
	}

	//let the pieces die in the model
	m_game.applyDeaths();

	//update deaths (has to be done after movement update)
	std::vector<PiecePtr> survivingPieces;
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
		if(endangeredPieces[i] != survivingPieces[i]) //the piece has died (replaced by nullptr)
			killPieceSprite(endangeredPieces[i]);
	}

	//update freezing (has to be done after death update)
	for(int i = 0; i < BOARD_SIZE; ++i)
	{
		for(int j = 0; j < BOARD_SIZE; ++j)
		{
			PiecePtr p = m_game[Square(i,j)];
			if (p != nullptr)
			{
				m_pieces[p].freeze(m_game.isFrozen(Square(i,j))); //if the piece is frozen, show it
			}
		}
	}
}

void GameScreen::updateNbMoves()
{
	int nbMoves = m_game.getMovesLeft();

	int width = m_nbMovesSprite.GetImage()->GetWidth();
	int height = m_nbMovesSprite.GetImage()->GetHeight()/4; // /4 beacause 4 sprites
	m_nbMovesSprite.SetSubRect(sf::IntRect(0, (nbMoves-1)*height, width, nbMoves*height));
}

void GameScreen::clearAll()
{
	for(std::map<PiecePtr, PieceSprite>::iterator it = m_pieces.begin(); it != m_pieces.end(); ++it)
		killPieceSprite(it->first);
}

void GameScreen::refreshAll()
{
	clearAll();
	if(!m_game.hasStarted())
		m_placementUI.resetAvailability();
	for(int i = 0; i < BOARD_SIZE; ++i)
	{
		for(int j = 0; j < BOARD_SIZE; ++j)
		{
			sf::Vector2i s(i,j);
			PiecePtr p = m_game[toSquare(s)];
			if (p != nullptr)
			{
				m_pieces[p] = PieceSprite(p);
				m_pieces[p].moveOnSquare(s);
				m_pieces[p].freeze(m_game.isFrozen(toSquare(s))); //if the piece is frozen, show it
				
				if(!m_game.hasStarted() && (p->getColor() == m_game.getActivePlayer())) //if in placement phase, and if the piece belongs to the current player
					updatePieceAvailability(p->getType());//updating the availability in placement UI
			}
		}
	}
	updateNbMoves();
	if(!m_game.hasStarted())
		m_placementUI.setPlayer(m_game.getActivePlayer());
}

void GameScreen::setVictor(Color victor)
{
	m_victor = victor;
	if(isOver())
	{
		int dx = m_victorySign.GetImage()->GetWidth();
		int dy = m_victorySign.GetImage()->GetHeight()/2;
		int y = ((int) victor) * dy;
		m_victorySign.SetSubRect(sf::IntRect(0,y,dx,y+dy));
	}
}
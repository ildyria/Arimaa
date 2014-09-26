#include "Game.h"

int Game::m_maxPieceCount[NB_PIECES] = { 8, 2, 2, 2, 1, 1 };

Game::Game(void) : m_activePlayer(STARTING_PLAYER), m_movesLeft(4), m_hasStarted(0)
{
	for(int i = 0; i < NB_PIECES; ++i)
		m_pieceCount[i] = 0;
}

Game::~Game(void)
{
}

bool Game::place(Piece* p, Square s)
{
	if(!m_hasStarted && m_pieceCount[p->getType()] < m_maxPieceCount[p->getType()]) // if we are not started yet and all pieces of this type haven't been plaed
		if(m_board.inStartingZone(p->getColor(), s) && m_board.placePiece(p, s))
		{
			m_pieceCount[p->getType()]++;
			return true;
		}
	return false;
}

bool Game::remove(Square s)
{
	if(!m_hasStarted) // if we are not started yet
	{
		bool res = !m_board.isFree(s) && m_board.getPiece(s)->getColor() == m_activePlayer; //if there is a piece here and it belongs to the right player
		if(res)
		{
			m_pieceCount[m_board.getPiece(s)->getType()]--;
			m_board.removePiece(s);
		}
		return res;
	}
	return false;
}

bool Game::canEndPlacement() const
{
	if(m_hasStarted) //if the placement is already over
		return false;

	for(int i = 0; i < NB_PIECES; ++i)
		if(m_pieceCount[i] < m_maxPieceCount[i]) //all pieces haven't been placed yet 
			return false;

	//if all pieces have been placed :
	return true;
}

bool Game::play(Move& m, bool applyDeaths)
{
	if(m_hasStarted && m.execute(m_board, m_activePlayer, m_movesLeft)) //the move could be performed
	{
		if(applyDeaths)
			m_board.applyDeaths();
		if(m_movesLeft == 0)
			nextTurn();
		return true;
	}
	return false;
}

void Game::nextTurn()
{
	if(m_activePlayer == GOLD)
		m_activePlayer = SILVER;
	else
		m_activePlayer = GOLD;
	m_movesLeft = NB_MOVES_PER_TURN;

	if(!m_hasStarted)
	{
		if(m_activePlayer == STARTING_PLAYER) //both players placed their pieces
			m_hasStarted = true;
		else //olny the starting player placed his pawns
		{
			for(int i = 0; i < NB_PIECES; ++i)
				m_pieceCount[i] = 0;
		}
	}
}

bool Game::isWon()
{
	//Checks winning by reaching the goal
	int goalY;
	if(m_activePlayer == GOLD)
		goalY = 7;
	else //SILVER
		goalY = 0;
	for(int i = 0; i < BOARD_SIZE; ++i) //going through each square of the goal
	{
		Piece* p = m_board.getPiece(Square(i, goalY));
		if(p != NULL && p->getColor() == m_activePlayer && p->getType() == RABBIT) //if there is a piece, it belongs to the player, and it's a rabbit 
			return true;
	}

	return false;
}

std::vector<Square> Game::getPossibleMoves(Square pieceToMove)
{
	std::vector<Square> res;

	for(int i = 0; i < 4; ++i) //checks the 4 directions
	{
		Square s = pieceToMove + Board::m_cardinals[i];
		Move m(pieceToMove, s);
		if((m.isPossible(m_board, m_activePlayer, m_movesLeft) || getPossibleDisplacements(pieceToMove, s).size() > 0)) //if we can move here or displace the piece here, add it to the list
			res.push_back(s);
	}

	return res;
}

std::vector<Square> Game::getPossibleDisplacements(Square pieceToMove, Square pieceToDisplace)
{
	std::vector<Square> res;
	
	//pushes
	for(int i = 0; i < 4; ++i) //checks the 4 directions
	{
		Square s = pieceToDisplace + Board::m_cardinals[i];
		if(s != pieceToMove) //takes out the obvious case of wanting to push the target on the pushing piece
		{
			Displace m(pieceToMove, s, pieceToDisplace);
			if(m.isPossible(m_board, m_activePlayer, m_movesLeft)) //if we can displace it here add this square to the list
				res.push_back(s);
		}
	}
	//pulls
	for(int i = 0; i < 4; ++i) //checks the 4 directions
	{
		Square s = pieceToMove + Board::m_cardinals[i];
		if(s != pieceToDisplace) //takes out the obvious case of wanting to pull the target and take its place
		{
			Displace m(pieceToMove, s, pieceToDisplace);
			if(m.isPossible(m_board, m_activePlayer, m_movesLeft)) //if we can displace it here add this square to the list
				res.push_back(s);
		}
	}

	return res;
}

bool Game::saveToFile(std::string fileName) const
{
	//opens the file and checks if it opened correctly
	std::ofstream stream(fileName.c_str());
	if (!stream.is_open())
	{
		return false;
	}

	//writing active player and nb moves
	int movesLeft = 0;
	if(m_hasStarted)
		movesLeft = m_movesLeft;
	stream << m_activePlayer << ";" << movesLeft << std::endl;

	//writing the board
	for(int i = 0; i < BOARD_SIZE; ++i)
	{
		for(int j = 0; j < BOARD_SIZE; ++j)
		{
			Square s(j,i);
			if(m_board.isFree(s))
				stream << ' ';
			else
				stream << m_board.getPiece(s)->toChar();
		}
		stream << std::endl;
	}
	return true;
}

bool Game::loadFromFile(std::string fileName)
{
	//opens the file and checks if it opened correctly
	std::ifstream stream(fileName.c_str());
	if (!stream.is_open())
	{
		return false;
	}

	//reading active player and nb moves
	std::string line;
	char c;
	std::getline(stream, line);
	if(line.size() < 3) //if the line is not long enough to contain the info, fail
		return false;
	m_movesLeft = (int) (line[2]-'0');
	m_activePlayer = (Color) (line[0]-'0');
	if(m_movesLeft == 0) //game not started yet
	{
		m_movesLeft = 4; //precaution, probably unnecessary
		m_hasStarted = false;
	}
	else
	{
		m_hasStarted = true;
	}

	//resetting nb of pieces placed if needed
	if(!m_hasStarted)
		for(int i = 0; i < NB_PIECES; ++i)
			m_pieceCount[i] = 0;
	
	//reading the board
	m_board.clear();
	for(int i = 0; i < BOARD_SIZE; ++i)
	{
		std::getline(stream, line);
		if(line.size() < BOARD_SIZE) //if the line is not long enough to contain the info, fail
			return false;
		for(int j = 0; j < BOARD_SIZE; ++j)
		{
			c = line[j];
			if(c != ' ') //a piece is here
			{
				Piece* p = new Piece(Piece::fromChar(c));
				m_board.placePiece(p, Square(j,i));
				if(!m_hasStarted && p->getColor() == m_activePlayer) //increments nb of pieces placed if needed
					m_pieceCount[(int) p->getType()]++;
			}
		}
	}
	return true;
}
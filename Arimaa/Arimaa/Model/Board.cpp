#include "Board.h"

Square Board::m_trapList[] = {Square(2,2), Square(2,5), Square(5,2), Square(5,5)};
Square Board::m_cardinals[] = {Square(1,0), Square(0,1), Square(-1,0), Square(0,-1)};

Board::Board(void)
{
	//Builing an empty board
	for(int i = 0; i < BOARD_SIZE; ++i)
	{
		m_board.push_back(std::vector<Piece*>());
		for(int j = 0; j < BOARD_SIZE; ++j)
		{
			m_board.back().push_back(NULL);
		}
	}
}

Board::~Board(void)
{
	for(int i = 0; i < BOARD_SIZE; ++i)
	{
		for(int j = 0; j < BOARD_SIZE; ++j)
		{
			delete m_board[i][j];
			m_board[i][j] = NULL;
		}
	}
}

bool Board::placePiece(Piece* p, Square s)
{
	if(!isFree(s))
		return false;
	m_board[s.y][s.x] = p;
	return true;
}

void Board::removePiece(Square s)
{
	delete m_board[s.y][s.x];
	m_board[s.y][s.x] = NULL;
}

bool Board::movePiece(Square pos, Square dest)
{
	if(isFree(dest))
	{
		m_board[dest.y][dest.x] = m_board[pos.y][pos.x];
		m_board[pos.y][pos.x] = NULL;
		checkDeath(dest);
		return true;
	}
	return false;
}

bool Board::isFrozen(Square s) const
{
	if(isFree(s))
		return true; //a blank square is considered frozen
	Color color = getPiece(s)->getColor();
	Square adjSquare;
	bool frozen = false;
	for(int i = 0; i < 4; ++i) //checks the 4 directions
	{
		adjSquare = s + m_cardinals[i];
		if(isValid(adjSquare) && !isFree(adjSquare))
		{
			if(getPiece(adjSquare)->getColor() == color) //an ally is around : the piece can't be frozen
				return false;
			else //an ennemy is around : frozen if weaker
				frozen = (*getPiece(s) < *getPiece(adjSquare));
		}
	}
	return frozen;
}

bool Board::isTrap(Square s)
{
	for(int i = 0; i < NB_TRAPS; ++i)
		if(m_trapList[i] == s)
			return true;
	return false;
}

bool Board::isValid(Square s)
{
	return (s.x >= 0) && (s.y >= 0) && (s.x < BOARD_SIZE) && (s.y < BOARD_SIZE);
}

bool Board::areAdjacent(Square s1, Square s2)
{
	for(int i = 0; i < 4; ++i) //checks the 4 directions
	{
		if(s2 == s1 + m_cardinals[i])
			return true;
	}
	return false;
}

bool Board::checkDeath(Square s)
{
	if(!isTrap(s))
		return false;

	Color color = getPiece(s)->getColor();
	Square adjSquare;
	for(int i = 0; i < 4; ++i) //checks the 4 directions
	{
		adjSquare = s + m_cardinals[i];
		if(isValid(adjSquare) && !isFree(adjSquare))
		{
			if(getPiece(adjSquare)->getColor() == color) //an ally is around : the piece can't die
				return false;
		}
	}
	removePiece(s);
	return true;
}

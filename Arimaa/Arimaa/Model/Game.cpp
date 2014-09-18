#include "Game.h"

int Game::m_maxPieceCount[NB_PIECES] = { 8, 2, 2, 2, 1, 1 };

Game::Game(void) : m_activePlayer(GOLD), m_movesLeft(4), m_hasStarted(0)
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
		if(m_board.placePiece(p, s))
		{
			m_pieceCount[p->getType()]++;
			return true;
		}
	return false;
}

bool Game::remove(Square s)
{
	if(!m_hasStarted)
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

bool Game::play(Move& m)
{
	if(m_hasStarted && m.execute(m_board, m_activePlayer, m_movesLeft)) //the move could be performed
	{
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
		for(int i = 0; i < NB_PIECES; ++i)
			m_pieceCount[i] = 0;
	}
}
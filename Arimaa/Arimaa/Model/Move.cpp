#include "Move.h"

Move::Move(Square position, Square destination, int cost) : m_position(position), m_destination(destination), m_cost(cost)
{
}

Move::~Move(void)
{
}

bool Move::isPossible(const Board& b, Color player, int movesLeft) const
{
	if(movesLeft < m_cost) //not enough moves left
		return false;
	if(b.isFrozen(m_position) || b.getPiece(m_position)->getColor() != player) //there is no piece to move (or it is frozen) or it is the wrong color
		return false;
	if(b.isFree(m_destination)) //the destination is free
		return true;
	return false;
}

bool Move::execute(Board& b, Color player, int& movesLeft) const
{
	if(!isPossible(b, player, movesLeft))
		return false;

	b.movePiece(m_position, m_destination);
	movesLeft -= m_cost;

	return true;
}

bool Move::isValid()
{
	return Board::isValid(m_position) && Board::isValid(m_destination) && Board::areAdjacent(m_position, m_destination);
}
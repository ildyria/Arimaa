#include "Displace.h"

Displace::Displace(Square position, Square destination, Square target, int cost) : Move(position, destination, cost), m_target(target)
{
}

Displace::~Displace(void)
{
}

bool Displace::isPossible(const Board& b, Color player, int movesLeft) const
{
	if(movesLeft < m_cost) //not enough moves left
		return false;
	if(b.isFrozen(m_position) || b.getPiece(m_position)->getColor() != player) //there is no piece to move (or it is frozen) or it is the wrong color
		return false;
	if(b.isFree(m_target) || b.getPiece(m_target)->getColor() == player) //there is no piece to displace or it is the wrong color
		return false;
	if(b.getPiece(m_position) <= b.getPiece(m_target)) //the target piece is too strong to be pushed
		return false;
	if(b.isFree(m_destination) || (m_destination == m_position && b.isFree(2*m_position - m_target))) //the destination is free or is the same as the starting position (case of pull), in which case the square behind position is free
		return true;
	return false;
}

bool Displace::execute(Board& b, Color player, int& movesLeft) const
{
	if(!isPossible(b, player, movesLeft))
		return false;

	if(m_position == m_destination) //pull
	{
		b.movePiece(m_position, 2*m_position - m_target); //moving the piece backwards
		b.movePiece(m_target, m_position); //moving the target towards the piece
	}
	else //push
	{
		b.movePiece(m_target, m_destination); //moving the target
		b.movePiece(m_position, m_target); //moving the piece onwards
	}
	movesLeft -= m_cost;

	return true;
}

bool Displace::isValid()
{
	return Board::isValid(m_position) && Board::isValid(m_destination) && Board::isValid(m_target)
		&& Board::areAdjacent(m_position, m_target) && Board::areAdjacent(m_target, m_destination);
}
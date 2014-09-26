#include "Displace.h"

Displace::Displace(Square position, Square destination, Square target, int cost) : Move(position, destination, cost), m_target(target)
{
}

Displace::~Displace(void)
{
}

bool Displace::isPossible(const Board& b, Color player, int movesLeft) const
{
	if(!isValid())
		return false;
	if(movesLeft < m_cost) //not enough moves left
		return false;
	if(b.isFrozen(m_position) || b.getPiece(m_position)->getColor() != player) //there is no piece to move (or it is frozen) or it is the wrong color
		return false;
	if(b.isFree(m_target) || b.getPiece(m_target)->getColor() == player) //there is no piece to displace or it is the wrong color
		return false;
	if(*b.getPiece(m_position) <= *b.getPiece(m_target)) //the target piece is too strong to be pushed
		return false;
	return b.isFree(m_destination); //rturns true if the destination is free
}

bool Displace::execute(Board& b, Color player, int& movesLeft) const
{
	if(!isPossible(b, player, movesLeft))
		return false;

	if(isAPull())
	{
		b.movePiece(m_position, m_destination); //moving the piece backwards
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

bool Displace::isValid() const
{
	return Board::isValid(m_position) && Board::isValid(m_destination) && Board::isValid(m_target) //the 3 positions are on the board
		&& Board::areAdjacent(m_position, m_target) && (Board::areAdjacent(m_target, m_destination) || Board::areAdjacent(m_position, m_destination));
	//the piece to displace has to be next to the moving piece, and the destination has to be next to one of the two (depending on if it's a push or a pull)
}
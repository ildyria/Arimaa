#pragma once
#include "move.h"

class Displace :
	public Move
{
public:
	Displace(Square position, Square destination, Square target, int cost = 2);
	~Displace(void);

	inline bool isAPull() const { return Board::areAdjacent(m_position, m_destination); }
	bool isPossible(const Board& b, Color player, int movesLeft) const;
	bool execute(Board& b, Color player, int& movesLeft) const;

private:
	Square m_target;

	bool isValid() const;
};

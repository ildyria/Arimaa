#pragma once
#include "Board.h"

class Move
{
public:
	Move(Square position, Square destination, int cost = 1);
	~Move(void);

	inline Square getPosition() const { return m_position; }
	inline Square getdestinationition() const { return m_destination; }
	inline int getCost() const { return m_cost; }
	inline void setPosition(Square pos) { m_position = pos; }
	inline void setdestinationition(Square pos) { m_destination = pos; }
	inline void setCost(int cost) { m_cost = cost; }

	virtual bool isPossible(const Board& b, Color player, int movesLeft) const;
	virtual bool execute(Board& b, Color player, int& movesLeft) const;

protected:
	Square m_position;
	Square m_destination;
	int m_cost;

	virtual bool isValid();
};

#pragma once
#include "Board.h"

/**
 *  \brief A class representing a move.
 */
class Move
{
public:
	/**
	 *  \brief Basic constructor for Move.
	 *  
	 *  \param position The position of the piece to move.
	 *  \param destination The place where the piece is to be moved.
	 *  \param cost The number of moves necessary to perform the move (1 by default).
	 *  
	 *  \details For displacements, see Displace.
	 */
	Move(Square position, Square destination, int cost = 1);
	~Move(void);

	/**
	 *  \brief Returns the position of the piece to move.
	 */
	inline Square getPosition() const { return m_position; }
	/**
	 *  \brief Returns the place where the piece is to be moved.
	 */
	inline Square getDestinationition() const { return m_destination; }
	/**
	 *  \brief Returns number of moves necessary to perform the move.
	 */
	inline int getCost() const { return m_cost; }
	/**
	 *  \brief Sets the position of the piece to move.
	 */
	inline void setPosition(Square pos) { m_position = pos; }
	/**
	 *  \brief Sets the place where the piece is to be moved.
	 */
	inline void setdestinationition(Square pos) { m_destination = pos; }
	/**
	 *  \brief Sets the number of moves necessary to perform the move.
	 */
	inline void setCost(int cost) { m_cost = cost; }

	/**
	 *  \brief Checks if the move is possible.
	 *  
	 *  \param b The board where the move will be applied.
	 *  \param player The current player.
	 *  \param movesLeft How many moves the current player can do before the end of his turn.
	 *  \return True if the move is possible.
	 */
	virtual bool isPossible(const Board& b, Color player, int movesLeft) const;
	/**
	 *  \brief Tries to execute the move.
	 *  
	 *  \param b The board where the move will be applied.
	 *  \param player The current player.
	 *  \param movesLeft How many moves the current player can do before the end of his turn.
	 *  \return True if the operation was a success.
	 */
	virtual bool execute(Board& b, Color player, int& movesLeft) const;

protected:
	Square m_position;
	Square m_destination;
	int m_cost;

	virtual bool isValid() const;
};

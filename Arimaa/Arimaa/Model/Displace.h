#pragma once
#include "move.h"

/**
 *  \brief A class representing a displacement (a push or a pull).
 */
class Displace :
	public Move
{
public:
	/**
	 *  \brief Basic constructor for Displace.
	 *  
	 *  \param position The position of the ally piece.
	 *  \param destination The place where the ally (in case of a pull) or enemy (in case of a push) piece is to be moved.
	 *  \param target The position of the enemy piece.
	 *  \param cost The number of moves necessary to perform the move (2 by default).
	 *  
	 *  \details For basic moves, see Move.
	 */
	Displace(Square position, Square destination, Square target, int cost = 2);
	~Displace(void);

	/**
	 *  \brief Checks if the displacement is a pull or a push.
	 *  
	 *  \return True if the displacement is a pull, false if it is a push.
	 */
	inline bool isAPull() const { return Board::areAdjacent(m_position, m_destination); }
	/**
	 *  \brief Checks if the move is possible.
	 *  
	 *  \param b The board where the move will be applied.
	 *  \param player The current player.
	 *  \param movesLeft How many moves the current player can do before the end of his turn.
	 *  \return True if the move is possible.
	 */
	bool isPossible(const Board& b, Color player, int movesLeft) const;
	/**
	 *  \brief Tries to execute the move.
	 *  
	 *  \param b The board where the move will be applied.
	 *  \param player The current player.
	 *  \param movesLeft How many moves the current player can do before the end of his turn.
	 *  \return True if the operation was a success.
	 */
	bool execute(Board& b, Color player, int& movesLeft) const;

private:
	Square m_target;

	bool isValid() const;
};

#pragma once
#include <string>
#include <fstream>
#include "Board.h"
#include "Displace.h"

#define NB_MOVES_PER_TURN 4

#define MAX_ELEPHANT 1
#define MAX_CAMEL 1
#define MAX_HORSE 2
#define MAX_DOG 2
#define MAX_CAT 2
#define MAX_RABBIT 8

#define STARTING_PLAYER GOLD

/**
 *  \brief A class representing a game. It is the class players interact with.
 */
class Game
{
public:
	Game(void);
	~Game(void);

	/**
	 *  \brief Returns the Color of the player whose turn it is.
	 *  
	 *  \return The Color of the player whose turn it is.
	 */
	inline Color getActivePlayer() const { return m_activePlayer; };
	/**
	 *  \brief Returns how many moves the player can make before the end of his turn.
	 *  
	 *  \return How many moves the player can make before the end of his turn.
	 */
	inline int getMovesLeft() const { return m_movesLeft; };
	/**
	 *  \brief Returns true if the unit placement is over.
	 *  
	 *  \return True if the unit placement is over.
	 */
	inline bool hasStarted() const { return m_hasStarted; };
	/**
	 *  \brief Returns the number of pieces of a given PieceType that haven't been placed.
	 *  
	 *  \param  t The PieceType to consider.
	 *  \return The number of pieces of type t that haven't been placed.
	 *  
	 *  \details This function is useful during the placement phase only.
	 */
	int getNbRemaining(PieceType t) { return m_maxPieceCount[t] - m_pieceCount[t]; }

	/**
	 *  \brief Tries to place a piece in a given square.
	 *  
	 *  \param  p The piece to place.
	 *  \param  s The square to place it in.
	 *  \return True if the operation was successful.
	 *  
	 *  \details This function doesn't allow its user to place a piece out of his starting zone,
	 *  to place a piece of the wrong color, or to exceed the maximum number of pieces for each player.
	 */
	bool place(Piece* p, Square s);
	/**
	 *  \brief Tries to place a piece in a given square. The piece owner will be the current player.
	 *  
	 *  \param  t The type of piece to place.
	 *  \param  s The square to place it in.
	 *  \return True if the operation was successful.
	 *  
	 *  \details This function doesn't allow its user to place a piece out of his starting zone,
	 *  to place a piece of the wrong color, or to exceed the maximum number of pieces for each player.
	 */
	inline bool place(PieceType t, Square s) { return place(new Piece(t, m_activePlayer), s);  }
	/**
	 *  \brief Tries to remove a piece on a given square.
	 *  
	 *  \param  s The square where the piece to remove is.
	 *  \return True if the operation was successful.
	 *  
	 *  \details This function only allows the removal of pieces owned by the current player.
	 */
	bool remove(Square s);
	/**
	 *  \brief Ends the placement phase of the current player.
	 *  
	 *  \return True if the operation was successful.
	 *  
	 *  \details This will only work if all pieces of the current player have been placed.
	 */
	inline bool endPlacement() { if(canEndPlacement()) { nextTurn(); return true; } return false; }
	/**
	 *  \brief Returns true if the current player can end his placement phase.
	 *  
	 *  \return True if the current player can end his placement phase.
	 */
	bool canEndPlacement() const;
	/**
	 *  \brief Tries to perform a given move.
	 *  
	 *  \param  m The move to make.
	 *  \param  applyDeaths Set to true if you want pieces to die immediately.
	 *  If set to false, make sure to call applyDeaths() before the next move.
	 *  \return True if the operation was successful.
	 */
	bool play(Move& m, bool applyDeaths = true); //set to false if you want to apply deaths manually later
	/**
	 *  \brief Tries to move a piece.
	 *  
	 *  \param  pos The position of the piece to move.
	 *  \param  dest The place where the piece is to be moved.
	 *  \param  applyDeaths Set to true if you want pieces to die immediately.
	 *  If set to false, make sure to call applyDeaths() before the next move.
	 *  \return True if the operation was successful.
	 *  
	 *  \details To displace an enemy unit, see displace(Square, Square, Square, bool).
	 */
	inline bool move(Square pos, Square dest, bool applyDeaths = true) { return play(Move(pos, dest), applyDeaths); }
	/**
	 *  \brief Tries to displace an enemy piece.
	 *  
	 *  \param  pos The position of the ally piece.
	 *  \param  dest The place where the ally (in case of a pull) or enemy (in case of a push) piece is to be moved.
	 *  \param  target The position of the enemy piece.
	 *  \param  applyDeaths Set to true if you want pieces to die immediately.
	 *  If set to false, make sure to call applyDeaths() before the next move.
	 *  \return True if the operation was successful.
	 *  
	 *  \details To move an enemy unit, see move(Square, Square, bool).
	 */
	inline bool displace(Square pos, Square dest, Square target, bool applyDeaths = true) { return play(Displace(pos, dest, target), applyDeaths); }

	/**
	 *  \brief Resolves piece deaths.
	 *  
	 *  \return True if a piece died in the process.
	 *  
	 *  \details Use it only if you made a move with applyDeaths set to false.
	 */
	inline bool applyDeaths() { return m_board.applyDeaths(); }
	/**
	 *  \brief Checks if the given square is a trap.
	 *  
	 *  \param  s The square to check.
	 *  \return True if s is a trap.
	 */
	inline bool isTrap(Square s) const { return m_board.isTrap(s); }
	/**
	 *  \brief Checks if the unit stationed on the given square is frozen.
	 *  
	 *  \param  s The square to check.
	 *  \return True if the unit stationed on s is frozen.
	 */
	inline bool isFrozen(Square s) const { return m_board.isFrozen(s); }
	/**
	 *  \brief Checks if any of the players has won the game.
	 *  
	 *  \return The color of the winning player, or NB_PLAYERS by default.
	 *  
	 *  \details Victory by immobilisation has not been implemented yet.
	 */
	Color getWinner(); 

	/**
	 *  \brief Makes the list of every move the piece can make with it's player's remaining moves.
	 *  
	 *  \param  pieceToMove The piece to check.
	 *  \return The list of every square the piece can move to (or displace a unit stationed there).
	 */
	std::vector<Square> getPossibleMoves(Square pieceToMove);
	/**
	 *  \brief Makes the list of every displacement possible with the given two pieces.
	 *  
	 *  \param  pieceToMove The piece that is performing the displacement.
	 *  \param  pieceToDisplace The enemy piece to displace.
	 *  \return The list of every square available for displacement.
	 *  
	 *  \details The list contains squares where the enemy piece can move to,
	 *  and squares where the ally piece can move to while pulling the enemy piece.
	 */
	std::vector<Square> getPossibleDisplacements(Square pieceToMove, Square pieceToDisplace);
	
	/**
	 *  \brief Finds the Piece occupying a given square.
	 *  
	 *  \param  s The square to check.
	 *  \return A pointer to the piece, or nullptr if no piece is on this square.
	 */
	inline Piece* operator[] (const Square& s) const { return m_board.getPiece(s); }

	/**
	 *  \brief Save the current game to a file.
	 *  
	 *  \param  fileName The file to save the game in.
	 *  \return True if the operation was successful.
	 */
	bool saveToFile(std::string fileName) const;
	/**
	 *  \brief Loads a game from a file.
	 *  
	 *  \param  fileName The file to load the game from.
	 *  \return True if the operation was successful.
	 */
	bool loadFromFile(std::string fileName);

private:
	Board m_board;
	Color m_activePlayer;
	int m_movesLeft;
	bool m_hasStarted;

	int m_pieceCount[NB_PIECES];
	static int m_maxPieceCount[NB_PIECES];

	void nextTurn();
};

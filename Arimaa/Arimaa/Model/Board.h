#pragma once
#include <vector>
#include "Piece.h"

#define BOARD_SIZE 8
#define NB_TRAPS 4

//----------------Square-------------------//
/**
 *  \brief A class representing a square.
 */
typedef struct Square
{
	Square(int a = 0, int b = 0) : x(a), y(b) {}
	int x;
	int y;
} Square;

inline bool operator== (const Square& s1, const Square& s2) { return (s1.x == s2.x) && (s1.y == s2.y); }
inline bool operator!= (const Square& s1, const Square& s2) { return (s1.x != s2.x) || (s1.y != s2.y); }
inline Square operator+ (const Square& s1, const Square& s2) { return Square(s1.x + s2.x, s1.y + s2.y); }
inline Square operator- (const Square& s1, const Square& s2) { return Square(s1.x - s2.x, s1.y - s2.y); }
inline Square operator* (const Square& s, const int& i) { return Square(s.x*i, s.y*i); }
inline Square operator* (const int& i, const Square& s) { return s*i; }
inline Square operator/ (const Square& s, const int& i) { return Square(s.x/i, s.y/i); }

//----------------Board-------------------//
/**
 *  \brief A class representing the game board.
 */
class Board
{
public:
	Board(void);
	~Board(void);

	/**
	 *  \brief Checks if the given square is free.
	 *  
	 *  \param s The Square to check.
	 *  \return True if s is free.
	 */
	inline bool isFree(Square s) const { return m_board[s.y][s.x] == nullptr; }
	/**
	 *  \brief Finds the Piece occupying a given square.
	 *  
	 *  \param  s The square to check.
	 *  \return A pointer to the piece, or nullptr if no piece is on this square.
	 */
	inline Piece* getPiece(Square s) const { return m_board[s.y][s.x]; }
	/**
	 *  \brief Tries to place the given Piece on the given Square.
	 *  
	 *  \param p The Piece to place.
	 *  \param s The Square to place the Piece on.
	 *  \return True if the Piece could be placed.
	 *  
	 *  \details The only restriction for piece placement is that the square is valid and free.
	 *  See isFree(Square) and isValid(Square).
	 */
	bool placePiece(Piece* p, Square s);
	/**
	 *  \brief Tries to remove a Piece from the Board.
	 *  
	 *  \param s The Square where the piece to remove is.
	 *  \return True if the Piece could be removed.
	 *  
	 *  \details The only restriction for piece removal is that there is indeed a Piece on the given Square.
	 */
	void removePiece(Square s);
	/**
	 *  \brief Tries to move a Piece.
	 *  
	 *  \param pos The Square where the piece to move currently is.
	 *  \param dest The place where the Piece is to be moved.
	 *  \return True if the Piece could be moved.
	 *  
	 *  \details The only restriction for piece movement is that there is indeed a Piece on the given Square,
	 *  and that the destination is valid and free. See isFree(Square) and isValid(Square).
	 */
	bool movePiece(Square pos, Square dest);
	/**
	 *  \brief Checks if the unit stationed on the given square is frozen.
	 *  
	 *  \param  s The square to check.
	 *  \return True if the unit stationed on the given square is frozen.
	 */
	bool isFrozen(Square s) const;
	/**
	 *  \brief Resolves piece deaths.
	 *  
	 *  \return True if a piece died in the process.
	 */
	bool applyDeaths();
	/**
	 *  \brief Removes all Pieces from the board.
	 */
	void clear();
	/**
	 *  \brief Checks if the given square is a trap.
	 *  
	 *  \param  s The square to check.
	 *  \return True s is a trap.
	 */
	static bool isTrap(Square s);
	/**
	 *  \brief Checks if the given square is in the Board.
	 *  
	 *  \param  s The square to check.
	 *  \return True if s is in the Board.
	 */
	static bool isValid(Square s);
	/**
	 *  \brief Checks if the given square is in the given player's starting zone.
	 *  
	 *  \param c The player's Color.
	 *  \param s The square to check.
	 *  \return True if s is in the player c's starting zone.
	 */
	static bool inStartingZone(Color c, Square s);
	/**
	 *  \brief Checks if two given squares are adjacent
	 *  
	 *  \param s1 The first square to consider.
	 *  \param s2 The second square to consider.
	 *  \return True if s1 and s2 are adjacent
	 */
	static bool areAdjacent(Square s1, Square s2);

	static const Square m_cardinals[];

private:
	std::vector<std::vector<Piece*>> m_board;

	bool checkDeath(Square s);

	static Square m_trapList[];
};

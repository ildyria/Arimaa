#pragma once
#include <vector>
#include "Piece.h"

#define BOARD_SIZE 8
#define NB_TRAPS 4

//----------------Square-------------------//
typedef struct Square
{
	Square(int a = 0, int b = 0) : x(a), y(b) {}
	int x;
	int y;
} Square;

inline bool operator== (const Square& s1, const Square& s2) { return (s1.x == s2.x) && (s1.y == s2.y); }
inline Square operator+ (const Square& s1, const Square& s2) { return Square(s1.x + s2.x, s1.y + s2.y); }
inline Square operator- (const Square& s1, const Square& s2) { return Square(s1.x - s2.x, s1.y - s2.y); }
inline Square operator* (const Square& s, const int& i) { return Square(s.x*i, s.y*i); }
inline Square operator* (const int& i, const Square& s) { return s*i; }
inline Square operator/ (const Square& s, const int& i) { return Square(s.x/i, s.y/i); }

//----------------Board-------------------//
class Board
{
public:
	Board(void);
	~Board(void);

	inline bool isFree(Square s) const { return m_board[s.y][s.x] == NULL; }
	inline Piece* getPiece(Square s) const { return m_board[s.y][s.x]; }
	bool placePiece(Piece* p, Square s);
	void removePiece(Square s);
	bool movePiece(Square pos, Square dest);
	bool isFrozen(Square s) const;
	bool applyDeaths();

	static bool isTrap(Square s);
	static bool isValid(Square s);
	static bool inStartingZone(Color c, Square s);
	static bool areAdjacent(Square s1, Square s2);

	static const Square m_cardinals[];

private:
	std::vector<std::vector<Piece*>> m_board;

	bool checkDeath(Square s);

	static Square m_trapList[];
};

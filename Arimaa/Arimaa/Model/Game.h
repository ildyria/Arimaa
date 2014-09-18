#pragma once
#include "Board.h"
#include "Displace.h"

#define NB_MOVES_PER_TURN 4

#define MAX_ELEPHANT 1
#define MAX_CAMEL 1
#define MAX_HORSE 2
#define MAX_DOG 2
#define MAX_CAT 2
#define MAX_RABBIT 8

class Game
{
public:
	Game(void);
	~Game(void);

	bool place(Piece* p, Square s);
	inline bool place(PieceType t, Square s) { return place(new Piece(t, m_activePlayer), s);  }
	bool remove(Square s);
	bool play(Move& m);
	inline bool move(Square pos, Square dest) { return play(Move(pos, dest)); }
	inline bool displace(Square pos, Square dest, Square target) { return play(Displace(pos, dest, target)); }

private:
	Board m_board;
	Color m_activePlayer;
	int m_movesLeft;
	bool m_hasStarted;

	int m_pieceCount[NB_PIECES];
	static int m_maxPieceCount[NB_PIECES];

	void nextTurn();
};

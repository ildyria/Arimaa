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

#define STARTING_PLAYER GOLD

class Game
{
public:
	Game(void);
	~Game(void);

	inline Color getActivePlayer() const { return m_activePlayer; };
	inline int getMovesLeft() const { return m_movesLeft; };
	inline bool hasStarted() const { return m_hasStarted; };
	int getNbRemaining(PieceType t) { return m_maxPieceCount[t] - m_pieceCount[t]; }

	bool place(Piece* p, Square s);
	inline bool place(PieceType t, Square s) { return place(new Piece(t, m_activePlayer), s);  }
	bool remove(Square s);
	bool endPlacement();
	bool play(Move& m, bool applyDeaths = true); //set to false if you want to apply deaths manually later
	inline bool move(Square pos, Square dest, bool applyDeaths = true) { return play(Move(pos, dest), applyDeaths); }
	inline bool displace(Square pos, Square dest, Square target, bool applyDeaths = true) { return play(Displace(pos, dest, target), applyDeaths); }
	inline bool applyDeaths() { return m_board.applyDeaths(); } //use only if you moved with applyDeaths set to false
	inline bool isTrap(Square s) { return m_board.isTrap(s); }
	bool isWon(); //checks if the current player won the game. For now, only checks victory by reaching the goal
	
	inline Piece* operator[] (const Square& s) const { return m_board.getPiece(s); }

private:
	Board m_board;
	Color m_activePlayer;
	int m_movesLeft;
	bool m_hasStarted;

	int m_pieceCount[NB_PIECES];
	static int m_maxPieceCount[NB_PIECES];

	void nextTurn();
};

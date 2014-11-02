#include "TheGame.h"

int TheGame::_game = 0;

TheGame::TheGame()
{
}


TheGame::~TheGame()
{
}

void TheGame::setGame(int game)
{
	_game = game;
}

//int TheGame::end(const Bitboard& board)

//void TheGame::play(string position, Bitboard& board)

/*void TheGame::diplayBoard(const Bitboard& board)
{
	if (TheGame::_game == 0)
	{
		TicTacToe::diplayBoard(board);
	}
	throw("game not chosen");
}

list<string> TheGame::listPossibleMoves(const Bitboard& board)
{
	if (TheGame::_game == 0)
	{
		return TicTacToe::listPossibleMoves(board);
	}
	throw("game not chosen");
}*/
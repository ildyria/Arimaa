/**
 * \file TheGame.h
 * \brief Define class skelleton for an abstract class (WIP)
 * \author Benoit Viguier
 * \version 0.1
 * \date october 30th 2014
 *
 */
#pragma once
#include "Bitboard.h"
#include <string>
#include <list>
#include "Tictactoe.h"

using std::string;
using std::list;

class TheGame
{
	TheGame();
	~TheGame();
public:
	static int _game;
	static void setGame(int game = 0);

	static inline int end(const Bitboard& board)
	{
		if (TheGame::_game == 0)
		{
			return TicTacToe::end(board);
		}
		throw("game not chosen");
	};


	static inline void play(string position, Bitboard* board)
	{
		if (TheGame::_game == 0)
		{
			TicTacToe::play(position, *board);
		}
		throw("game not chosen");
	};

	static inline void diplayBoard(const Bitboard& board)
	{
		if (TheGame::_game == 0)
		{
			TicTacToe::diplayBoard(board);
		}
		throw("game not chosen");
	};

	static inline list<string> listPossibleMoves(const Bitboard& board){
		if (TheGame::_game == 0)
		{
			return TicTacToe::listPossibleMoves(board);
		}
		throw("game not chosen");
	};
};


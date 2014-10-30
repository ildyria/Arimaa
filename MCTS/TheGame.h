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

using std::string;
using std::list;

class TheGame
{

	TheGame();
	~TheGame();
public:

	static int end(const Bitboard& board);

	static void play(string position, Bitboard& board);

	static void diplayBoard(const Bitboard& board);

	static list<string> listPossibleMoves(const Bitboard& board);
};


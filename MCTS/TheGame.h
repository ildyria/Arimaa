/**
 * \file TheGame.h
 * \brief Define class skelleton for an abstract class (WIP)
 * \author Benoit Viguier
 * \version 0.1
 * \date october 30th 2014
 *
 */
#pragma once
#include "Move.h"
#include "Bitboard.h"
#include <list>

using std::string;
using std::list;

class TheGame
{
public:
	virtual ~TheGame() {}

	virtual int end(const Bitboard& board) = 0;

	virtual void play(Move& position, Bitboard& board) = 0;

	virtual void diplayBoard(const Bitboard& board) = 0;

	virtual list<Move> listPossibleMoves(const Bitboard& board) = 0;

	virtual int playRandomMoves(Bitboard& board) = 0;
};
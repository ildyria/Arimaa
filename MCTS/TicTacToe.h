/**
 * \file TicTacToe.h
 * \brief Define class TicTacToe
 * \author Benoit Viguier
 * \version 1.0
 * \date October 30th 2014
 *
 */
#pragma once
#include "Bitboard.h"
#include <string>
#include <list>
#include <algorithm>

using std::string;
using std::list;

class TicTacToe
{

	// NOT USED
	TicTacToe();
	~TicTacToe();

public:

	/**
	 * \fn end
	 * \brief check if the game end
	 * \details returns 1 if player 1 wins
	 * returns 2 if player 2 wins
	 * returns 3 if the game is a tie
	 * returns 0 if the game hasn't ended yet
	 * \return integer corresponding to the state of the Board
	 */
	static int end(const Bitboard& board);

	/**
	* \fn play
	* \brief simulate the play of the "position" move and returns the bitboard after the move
	*
	* \param position move to be play
	* \param board board to be used to play
	* \return the Bitboard after the play
	*/
	static void play(string position, Bitboard& board);

	/**
	 * \fn diplayBoard
	 * \brief display the board of the given Bitboard
	 * 
	 * \param board Bitboard to be displayed
	 */
	static void diplayBoard(const Bitboard& board);

	/**
	* \fn listPossibleMoves
	* \brief return a list of all possible moves given a board
	*
	* \param board Bitboard to be looked at
	* \return the list of the possible moves
	*/
	static list<string> listPossibleMoves(const Bitboard& board);

};


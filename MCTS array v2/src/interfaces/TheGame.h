/**
 * \file TheGame.h
 * \brief Define class skelleton for an abstract class
 * \author Benoit Viguier
 * \version 0.1
 * \date october 30th 2014
 */
#pragma once
#include "Move.h"
#include "Bitboard.h"
#include <list>

/**
 * \brief TheGame class
 * \details Abstract class which aim to provide a skeleton for the functions used by the MCTS algorithm.
 */
class TheGame
{
public:
	virtual ~TheGame() {}

	/**
	* \fn end(const Bitboard* board)
	* \brief check if the game end
	* \details returns 1 if player 1 wins
	* returns 2 if player 2 wins
	* returns 3 if the game is a tie
	* returns 0 if the game hasn't ended yet
	* \return integer corresponding to the state of the Board
	*/
	virtual int end(const Bitboard* board) = 0;

	/**
	* \fn play(Move& position, Bitboard* board)
	* \brief simulate the play of the "position" move and returns the bitboard after the move
	*
	* \param position : move to be play
	* \param board : board to be used to play
	* \return the Bitboard after the play
	*/
	virtual void play(Move& position, Bitboard* board) = 0;

	/**
	* \fn diplayBoard(const Bitboard* board)
	* \brief display the board of the given Bitboard
	*
	* \param board : Bitboard to be displayed
	*/
	virtual void diplayBoard(const Bitboard* board) = 0;

	/**
	* \fn listPossibleMoves(Bitboard* board)
	* \brief return a list of all possible moves given a board
	*
	* \param board Bitboard to be looked at
	* \return the list of the possible moves
	*/
	virtual std::list<Move> listPossibleMoves(Bitboard* board) = 0;

	/**
	* \fn playRandomMoves(Bitboard* board)
	* \brief play random moves until the game reach an end and return the int corresponding to the winner (1 2 or 3)
	*
	* \param board Bitboard to be looked at
	* \return winner of the current game
	*/
	virtual int playRandomMoves(Bitboard* board) = 0;
};
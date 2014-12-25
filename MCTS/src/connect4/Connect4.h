/**
* \file Connect4.h
* \brief Define class Connect4
* \author Benoit Viguier, Pascal Garcia
* \version 1.0
* \date October 30th 2014
*
*/
#pragma once
#include <algorithm>
#include "BitBoardConnect4.h"
#include "TheGame.h"
#include "Random.h"

class Connect4 : public TheGame
{

public:
	Connect4();

	virtual ~Connect4() {}

	/**
	* \fn end
	* \brief check if the game end
	* \details returns 1 if player 1 wins
	* returns 2 if player 2 wins
	* returns 3 if the game is a tie
	* returns 0 if the game hasn't ended yet
	* \return integer corresponding to the state of the Board
	*/
	virtual int end(const Bitboard* board) override;

	int checkHorizontal(const Bitboard* board);
	int checkVertical(const Bitboard* board);
	int checkDiag1(const Bitboard* board);
	int checkDiag2(const Bitboard* board);
	int checkNull(const Bitboard* board);

	/**
	* \fn play
	* \brief simulate the play of the "position" move and returns the bitboard after the move
	*
	* \param position move to be play
	* \param board board to be used to play
	* \return the Bitboard after the play
	*/
	virtual void play(Move& position, Bitboard* board) override;

	/**
	* \fn diplayBoard
	* \brief display the board of the given Bitboard
	*
	* \param board Bitboard to be displayed
	*/
	virtual void diplayBoard(const Bitboard* board) override;

	/**
	* \fn listPossibleMoves
	* \brief return a list of all possible moves given a board
	*
	* \param board Bitboard to be looked at
	* \return the list of the possible moves
	*/
	virtual std::list<Move> listPossibleMoves(Bitboard* board) override;

	/**
	* \fn playRandomMoves
	* \brief play random moves until the game reach an end and return the int corresponding to the winner (1 2 or 3)
	*
	* \param board Bitboard to be looked at
	* \return winner of the current game
	*/
	virtual int playRandomMoves(Bitboard* board) override;
};



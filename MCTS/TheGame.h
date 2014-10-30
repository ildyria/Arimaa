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
#include <map>
class TheGame
{
	Bitboard board;
	int toplay;

public:
	/**
	* \fn TheGame
	* \brief create a game TheGame
	*/
	TheGame();

	/**
	* \fn TheGame
	* \brief create a game
	* \details create a game starting from a board and a payer to play
	*
	* \param board board to play
	* \param toplay players turn
	*/
	TheGame(Bitboard board, int toplay);

	/**
	* \fn Destructor
	* \brief Destroy the game
	*/
	~TheGame();

	/**
	* \fn end
	* \brief check if the game end
	* \details returns 1 if player 1 wins
	* returns 2 if player 2 wins
	* returns 3 if the game is a tie
	* returns 0 if the game hasn't ended yet
	*/
	int end();

	/**
	* \fn play
	* \brief play the "position" move to play and record the move and return the current state of the game
	*
	* \param position move to be play
	* \return the Bitboard after the play
	*/
	Bitboard play(string position);

	/**
	* \fn play
	* \brief simulate the play of the "position" move and returns the bitboard after the move
	*
	* \param position move to be play
	* \param board board to be used to play
	* \return the Bitboard after the play
	*/
	//	Bitboard play(string position, Bitboard board);

	/**
	* \fn displayBoard
	* \brief display the board of the current game
	*/
	void diplayBoard();

	/**
	* \fn displayBoard
	* \brief display the board of the given Bitboard
	*
	* \param board Bitboard to be displayed
	*/
	void diplayBoard(Bitboard board);

	/**
	* \fn displayBoard
	* \brief display the board of the given Bitboard
	*
	* \param board Bitboard to be displayed
	* \return the list of the possible moves
	*/
	map<string, Bitboard> listPossibleMoves(Bitboard board);

};


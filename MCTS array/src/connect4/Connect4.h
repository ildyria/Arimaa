/**
* \file Connect4.h
* \brief Define class Connect4
* \author Benoit Viguier
* \version 1.0
* \date December 25th 2014
*
*/
#pragma once
#include "../interfaces/TheGame.h"
#include <sstream>
#include "../tools/Random.h"

/**
 * \brief Connect4 class
 * \details Class that contains the functions required to play the game.
 */
class Connect4 : public TheGame
{

public:
	/**
	 * \fn Connect4()
	 * \brief constructor of the Game Connect4.
	 */
	Connect4();

	/**
	 * \fn ~Connect4()
	 * \brief basic destructor
	 */
	virtual ~Connect4() {}

	/**
	 * \fn end
	 * \brief check if the game end
	 * \details returns 1 if player 1 wins
	 * returns 2 if player 2 wins
	 * returns 4 if the game is a tie
	 * returns 0 if the game hasn't ended yet
	 * \return integer corresponding to the state of the Board
	 */
	virtual int end(const Bitboard* board) override;

	/**
	 * \fn check_horizontal
	 * \brief check on each lines if there are 4 of a kind.
	 * 
	 * \param board Bitboard
	 * \return 0 1 or 2 depending of the winner
	 */
	int check_horizontal(const Bitboard* board);

	/**
	 * \fn check_vertical
	 * \brief check on each columns if there are 4 of a kind.
	 * 
	 * \param board Bitboard
	 * \return 0 1 or 2 depending of the winner
	 */
	int check_vertical(const Bitboard* board);

	/**
	 * \fn check_fst_diag
	 * \brief check on each diagonals if there are 4 of a kind.
	 * 
	 * \param board Bitboard
	 * \return 0 1 or 2 depending of the winner
	 */
	int check_fst_diag(const Bitboard* board);

	/**
	 * \fn check_snd_diag
	 * \brief check on each diagonals if there are 4 of a kind.
	 * 
	 * \param board Bitboard
	 * \return 0 1 or 2 depending of the winner
	 */
	int check_snd_diag(const Bitboard* board);

	/**
	 * \fn check_null
	 * \brief check if the board is full.
	 * 
	 * \param board Bitboard
	 * \return 0 or 4 if board is full
	 */
	int check_null(const Bitboard* board);

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
	* \fn diplay_board
	* \brief display the board of the given Bitboard
	*
	* \param board Bitboard to be displayed
	*/
	virtual void diplay_board(const Bitboard* board) override;

	/**
	* \fn list_possible_moves
	* \brief return a list of all possible moves given a board
	*
	* \param board Bitboard to be looked at
	* \return the list of the possible moves
	*/
	virtual std::list<Move> list_possible_moves(Bitboard* board) override;

	/**
	* \fn play_random_moves
	* \brief play random moves until the game reach an end and return the int corresponding to the winner (1 2 or 3)
	*
	* \param board Bitboard to be looked at
	* \return winner of the current game
	*/
	virtual int play_random_moves(Bitboard* board) override;
};



/**
* \file Arimaa.h
* \brief Define class Arimaa
* \author Benoit Viguier
* \version 1.0
* \date March 4rf 2015
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
class Arimaa : public TheGame
{
	/*
	use a short (16 bit) to check environement :
	4 bits : position nsew friend ?
	4 bits : position nsew smaller enemy => possible push / pull
	4 bits : position of same threat enemy => no push / no frozen
	4 bits : position nsew greater enemy => possible frozen
	
	
	
	*/






	void move_n(Bitboard* board, int n, int pos);
	void move_s(Bitboard* board, int n, int pos);
	void move_e(Bitboard* board, int n, int pos);
	void move_w(Bitboard* board, int n, int pos);

	inline bool close_player(Bitboard* board, int pos, int player) { return close_piece(board, pos, (NB_PIECE + 1)*player - 1); }

	bool close_piece(Bitboard* board, int pos, int boardnum);

	int is_free_n(Bitboard* board, int pos)
	{
		
	};
	int is_free_s(Bitboard* board, int pos)
	{
		
	};
	int is_free_e(Bitboard* board, int pos)
	{
		
	};
	int is_free_w(Bitboard* board, int pos)
	{
		
	};

	bool maybe_frozen(Bitboard* board, int pos, int player_to_play, int piece) // check if stronger enemy close to the position
	{
		bool frozen = false;
		int start = (NB_PIECE+1)*(2 - player_to_play) + piece + 1;
		for (int i = start; i < (NB_PIECE+1); ++i)
		{
			frozen |= close_piece(board, pos, i); // possible optimisation : add !frozen in the control structure...
		}
		return frozen;
	}



public:
	/**
	 * \fn Connect4()
	 * \brief constructor of the Game Connect4.
	 */
	Arimaa();

	/**
	 * \fn ~Connect4()
	 * \brief basic destructor
	 */
	virtual ~Arimaa() {}

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
	Move convert_move(std::string move);
};



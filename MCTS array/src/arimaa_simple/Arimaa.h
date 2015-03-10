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
 * \brief Arimaa class
 * \details Class that contains the functions required to play the game.
 */
class Arimaa : public TheGame
{
	/*

	order of checks :
	check border
	check friend
	check enemy same
	check enemy light
	check enemy strong

	4 bits : position nesw greater enemy => possible frozen
	4 bits : position nesw friend ?
	4 bits : position of same threat enemy (OR PIECE ON BORDER) => no push / no frozen
	4 bits : position nesw smaller enemy => maybe possible push / pull
	*/
	u_long get_situation(const int& pos, Bitboard* board);

	u_long check_neighbour(const Bitboard* board, const u_long& mask, const int& piece_rank, const int& player); // check if stronger enemy close to the position

	u_long checkBorder(const u_long& pos)
	{
		return (((pos & TOP_BORDER) != 0) << 3) | (((pos & RIGHT_BORDER) != 0) << 2) | (((pos & BOTTOM_BORDER) != 0) << 1) | ((pos & LEFT_BORDER) != 0);
	}

	void move(Bitboard* board, int n, int pos, int type);

	bool Arimaa::close_piece(const Bitboard* board, const u_long& mask, const int& boardnum);

	u_long get_mask(const int& pos);
	u_long get_mask_n(const u_long& mask);
	u_long get_mask_e(const u_long& mask);
	u_long get_mask_s(const u_long& mask);
	u_long get_mask_w(const u_long& mask);

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



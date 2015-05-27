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
#include "arimaa_tools.h"

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
public:
	u_short get_piece_board_num(const u_short& pos, Bitboard* board);
	u_short get_piece_board_num(const u_long& mask, Bitboard* board);

	u_short get_piece_rank(const u_short& pos, Bitboard* board);
	u_short get_piece_rank(const u_long& mask, Bitboard* board);

	u_long get_situation(const u_short& pos, Bitboard* board);

	u_long check_neighbour(const Bitboard* board, const u_long& mask, const u_short& piece_rank, const int& player); // check if stronger enemy close to the position

	u_long checkBorder(const u_long& mask);

	void move(Bitboard* board, int n, u_short pos, int type);

	bool close_piece(const Bitboard* board, const u_long& mask, const int& boardnum);

	// to pass to inline would be a great idea
	u_long get_mask(const u_short& pos);
	u_long get_mask_n(const u_long& mask);
	u_long get_mask_e(const u_long& mask);
	u_long get_mask_s(const u_long& mask);
	u_long get_mask_w(const u_long& mask);

	u_long is_rabbit(const u_short& piece_rank, const int player);

	bool is_frozen(const u_long& situation);

	void apply_traps(Bitboard* board);

	u_short possible_move(const u_long& situation);

	u_long possible_push(const u_long& mask, const u_long& situation, const Bitboard* board);

	std::vector<std::list<int>> get_pieces(Bitboard* board);

	std::list<u_long> generate_move_simple(const u_long& situation, const u_short& pos, const u_short& board_num, Bitboard* board);
	std::list<u_long> generate_move_double(const u_long& situation, const u_short& pos, const u_short& board_num, Bitboard* board);

	template<int num> std::list<u_long> list_moves_available(Bitboard* board);

	void play_simple_move(const u_long& );


//public:
	/**
	 * \fn Arimaa()
	 * \brief constructor of the Game Arimaa.
	 */
	Arimaa();

	/**
	 * \fn ~Arimaa()
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
};

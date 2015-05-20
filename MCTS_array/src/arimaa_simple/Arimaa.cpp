#include "Arimaa.h"
#include <map>

using std::string;
using std::list;

// TODO :
// 
//	rabbits should not move backward => DONE
// 
//	traps => DONE
//
//	end condition :
// 		> X turns => to be sure that random simulation happend to finish => to be implemented in the play random
// 		rabbit to an end => DONE
// 		every rabbits are dead => DONE
// 		no piece can move
//
//	generate 4 moves => DONE
//	
//	play single move => DONE
//
//	play multiple move => DONE
//
//	play random move
//		chose random piece
//		play a random generated move
//	
//	start board ???


Arimaa::Arimaa()
{
}

std::vector<std::list<int>> Arimaa::get_pieces(Bitboard* board)
{
	std::vector<std::list<int>> result = std::vector<std::list<int>>(NB_PIECE,std::list<int>());
	int j = 0;
	for (int i = (board->get_player() - 1)*(NB_PIECE + 1); i < (board->get_player() - 1)*(NB_PIECE + 1) + NB_PIECE; ++i)
	{
		result[j] = board->get_occupied(i);
		j++;
	}
	return result;
};

u_short Arimaa::get_piece_board_num(const u_short& pos, Bitboard* board)
{
	u_long mask = get_mask(pos);
	for (u_short i = 0; i < 2*(NB_PIECE + 1); ++i)
	{
		if((mask & board->get_board(i)) > 0)
			return i;
	}

	printf("SHOULD NEVER HAPPEN\n");
	// should never be the case	
	return 0;
};

u_short Arimaa::get_piece_board_num(const u_long& mask, Bitboard* board)
{
	for (u_short i = 0; i < 2*(NB_PIECE + 1); ++i)
	{
		if((mask & board->get_board(i)) > 0)
			return i;
	}

	printf("SHOULD NEVER HAPPEN\n");
	// should never be the case
	return 0;
};

u_short Arimaa::get_piece_rank(const u_short& pos, Bitboard* board)
{
	return get_piece_board_num(pos, board) % (NB_PIECE + 1);
};

u_short Arimaa::get_piece_rank(const u_long& mask, Bitboard* board)
{
	return get_piece_board_num(mask, board) % (NB_PIECE + 1);
};

u_long Arimaa::get_mask(const u_short& pos)
{
	return (static_cast<u_long>(1) << pos);
}

u_long Arimaa::get_mask_n(const u_long& mask)
{
	return mask << SIZEX;
}

u_long Arimaa::get_mask_e(const u_long& mask)
{
	return mask >> 1;
}

u_long Arimaa::get_mask_s(const u_long& mask)
{
	return mask >> SIZEX;
}

u_long Arimaa::get_mask_w(const u_long& mask)
{
	return mask << 1;
}

u_long Arimaa::checkBorder(const u_long& mask)
{
	return (((mask & TOP_BORDER) != 0) << 3) | (((mask & RIGHT_BORDER) != 0) << 2) | (((mask & BOTTOM_BORDER) != 0) << 1) | ((mask & LEFT_BORDER) != 0);
}


int Arimaa::end(const Bitboard* board)
{
	// win by no rabit
	if(board->get_board(0) == 0) 			return 2; // player 1 has no rabits => player 2 win
	if(board->get_board(NB_PIECE + 1) == 0) return 1; // player 2 has no rabits => player 1 win


	// win by rabit to border
	if(TOP_BORDER & board->get_board(0))				return 2;
	if(BOTTOM_BORDER & board->get_board(NB_PIECE + 1))	return 1;

	// win by no move possible
	// DO I REALLY HAVE TO CHECK THAT ONE ? ='(
	return 0;
}

void Arimaa::play(Move& position, Bitboard* board)
{
	constexpr u_long move_mask = (static_cast<u_long>(1) << 16) - 1;
	constexpr u_long num_mask = (static_cast<u_long>(1) << 8) - 1;
	constexpr u_long pos_mask = (static_cast<u_long>(1) << 6) - 1;
	constexpr u_long nsew_mask = (static_cast<u_long>(1) << 2) - 1;
	int num_board;
	u_short pos;
	int nsew;

	u_long moves = position.get_move();

	for(int i = 0 ; i < 4; ++i)
	{
		nsew = static_cast<int>((moves & move_mask) & nsew_mask);
		pos = static_cast<u_short>(((moves & move_mask) >> 2) & pos_mask);
		num_board = static_cast<int>(((moves & move_mask) >> 8) & num_mask);
		move(board, num_board, pos, nsew);
		moves >>= 16;
	}

	board->play();
}

void Arimaa::diplay_board(const Bitboard* board)
{
	std::vector<u_long> boards = std::vector<u_long>((NB_PIECE+1)*2, 0);
	for (u_int i = 0; i < boards.size(); ++i)
	{
		boards[i] = board->get_board(i);
	}
	u_long check = static_cast<u_long>(1);
	check = check << (SIZEX * SIZEY - 1);

	std::stringbuf buffer;
	std::ostream os(&buffer);
	os << std::endl;
	for (int y = (SIZEY - 1); y >= 0; --y)
	{
		os << "   ";
		for (int k = 0; k < SIZEX; ++k) os << "+---";
		os << "+" << std::endl;
		// 0 : rabbit 
		// 1 : cat
		// 2 : dog
		// 3 : horse
		// 4 : camel
		// 5 : elephant
		// 6 : all gold
		os << " " << (y + 1) << " ";
		for (int x = 0; x < SIZEX; ++x)
		{
			os << "| ";
			if ((boards[NB_PIECE] & check) == check)
			{
				if ((boards[0] & check) == check)
				{
					os << "R";
				}
				elseif ((boards[1] & check) == check)
				{
					os << "C";
				}
				elseif((boards[2] & check) == check)
				{
					os << "D";
				}
				elseif((boards[3] & check) == check)
				{
					os << "H";
				}
				elseif((boards[4] & check) == check)
				{
					os << "M"; // because of cat
				}
				elseif((boards[5] & check) == check)
				{
					os << "E";
				}
			}
			elseif((boards[2*NB_PIECE + 1] & check) == check)
			{
				if ((boards[NB_PIECE + 1] & check) == check)
				{
					os << "r";
				}
				elseif((boards[NB_PIECE + 2] & check) == check)
				{
					os << "c";
				}
				elseif((boards[NB_PIECE + 3] & check) == check)
				{
					os << "d";
				}
				elseif((boards[NB_PIECE + 4] & check) == check)
				{
					os << "h";
				}
				elseif((boards[NB_PIECE + 5] & check) == check)
				{
					os << "m"; // because of cat
				}
				elseif((boards[NB_PIECE + 6] & check) == check)
				{
					os << "e";
				}
			}
			else
			{
				os << " ";
			}
			os << " ";
			check >>= 1;
		}
		os << "|" << std::endl;
	}
	os << "   ";
	for (int k = 0; k < SIZEX; ++k) os << "+---";
	os << "+" << std::endl;
	os << "   ";
	for (int k = 0; k < SIZEX; ++k) os << "  " << static_cast<char>(k + 'A') << " ";
	os << std::endl;
	std::cout << buffer.str() << std::endl;
}

u_long Arimaa::is_rabbit(const u_short& piece_rank, const int player)
{
	if (!piece_rank == 0) return static_cast<u_long>(0);

	// n e s w : 8 4 2 1
	if (player == 0) return static_cast<u_long>(8); // player 1 is north so rabbit can't go north => define him as being on the north border
	return static_cast<u_long>(4); // idem mutatis mutandis
}


u_long Arimaa::get_situation(const u_short& pos, Bitboard* board)
{
	u_long mask = get_mask(pos);
	u_long result = checkBorder(mask); // n e s w : 8 4 2 1 
	u_long returnval = 0;

	u_short piece_rank = get_piece_rank(mask, board);
	int player = board->get_player(); // 1 or 2 // 0 or 1 ???
	u_long rabbit = is_rabbit(piece_rank,player);
	/*in theory there is no need to do the top and bottom check. Only left and Right are mandatory : slight optimisation possible later ? */

	// /!\ HERE WE ASSUME a strictly positive number is equivalent to bool(true)
	if (!(result & 8)) // NOT ON BORDER TOP
	{
		u_long mask_n = get_mask_n(mask);

		returnval |= (check_neighbour(board, mask_n, piece_rank, player)) << 3;
	}

	if (!(result & 4)) // NOT ON BORDER RIGHT
	{
		u_long mask_e = get_mask_e(mask);

		returnval |= (check_neighbour(board, mask_e, piece_rank, player)) << 2;
	}

	if (!(result & 2)) // NOT ON BORDER BOTTOM
	{
		u_long mask_s = get_mask_s(mask);

		returnval |= (check_neighbour(board, mask_s, piece_rank, player)) << 1;
	}

	if (!(result & 1)) // NOT ON BORDER LEFT
	{
		u_long mask_w = get_mask_w(mask);

		returnval |= (check_neighbour(board, mask_w, piece_rank, player));
	}

	returnval |= ((result|rabbit) << 4);
	return returnval;
}

bool Arimaa::is_frozen(const u_long& situation)
{
	// 1111 = 2³ + 2² + 2¹ + 2⁰ = 8 + 4 + 2 + 1 = 15
	static const u_long friends = static_cast<u_long>(15) << 8;
	static const u_long enemies = static_cast<u_long>(15) << 12;
	return ((situation & friends) == 0) && ((situation & enemies) > 0);
}

u_short Arimaa::possible_move(const u_long& situation)
{
	//	not the prettiest code but code that work, the simpler written, the easier to debug
	u_long mini_mask_impossible = static_cast<u_long>(1) | static_cast<u_long>(1) << 4 | static_cast<u_long>(1) << 8 | static_cast<u_long>(1) << 12;
	// should be calculated and given in "hard" in the code
	// compiler should optimize it so no need to worry about such improvements.

	u_short w = static_cast<u_short>(Arimaa_tools::compare_0_u(mini_mask_impossible,situation));
	mini_mask_impossible <<= 1;
	u_short s = static_cast<u_short>(Arimaa_tools::compare_0_u(mini_mask_impossible,situation));
	mini_mask_impossible <<= 1;
	u_short e = static_cast<u_short>(Arimaa_tools::compare_0_u(mini_mask_impossible,situation));
	mini_mask_impossible <<= 1;
	u_short n = static_cast<u_short>(Arimaa_tools::compare_0_u(mini_mask_impossible,situation));
	return ((n << 3) | (e << 2) | (s << 1) | w);
}

u_long Arimaa::possible_push(const u_long& mask, const u_long& situation, const Bitboard* board)
{

	u_long temp = 0;
	u_long result = 0;
	u_long buffer;
	u_long mini_mask_possible = static_cast<u_long>(8);
	u_long full_board =  board->get_board(NB_PIECE) | board->get_board(2*(NB_PIECE+1)-1);

	if(Arimaa_tools::compare_1_b(mini_mask_possible,situation)) // North
	{
		buffer = mask << SIZEX;
		temp = checkBorder(buffer);
		u_long n = get_mask_n(buffer);
		u_long e = get_mask_e(buffer); 
		u_long s = get_mask_s(buffer); // not needed but... you never know...
		u_long w = get_mask_w(buffer);

		// all impossible moves (push for W)
		temp |= (Arimaa_tools::compare_1_u(n,full_board) << 3) | (Arimaa_tools::compare_1_u(e,full_board) << 2) | (Arimaa_tools::compare_1_u(s,full_board) << 1) | Arimaa_tools::compare_1_u(w,full_board);
		result |= temp ^ 15;
	}
	mini_mask_possible >>= 1;
	result <<= 4;

	if(Arimaa_tools::compare_1_b(mini_mask_possible,situation)) // East
	{
		buffer = mask >> 1;
		temp = checkBorder(buffer);
		u_long n = get_mask_n(buffer);
		u_long e = get_mask_e(buffer); 
		u_long s = get_mask_s(buffer);
		u_long w = get_mask_w(buffer); // not needed but... you never know...

		// all impossible moves (push for W)
		temp |= (Arimaa_tools::compare_1_u(n,full_board) << 3) | (Arimaa_tools::compare_1_u(e,full_board) << 2) | (Arimaa_tools::compare_1_u(s,full_board) << 1) | Arimaa_tools::compare_1_u(w,full_board);
		result |= temp ^ 15;
	}
	mini_mask_possible >>= 1;
	result <<= 4;

	if(Arimaa_tools::compare_1_b(mini_mask_possible,situation)) // South
	{
		buffer = mask >> SIZEX;
		temp = checkBorder(buffer);
		u_long n = get_mask_n(buffer); // not needed but... you never know...
		u_long e = get_mask_e(buffer); 
		u_long s = get_mask_s(buffer);
		u_long w = get_mask_w(buffer);

		// all impossible moves (push for W)
		temp |= (Arimaa_tools::compare_1_u(n,full_board) << 3) | (Arimaa_tools::compare_1_u(e,full_board) << 2) | (Arimaa_tools::compare_1_u(s,full_board) << 1) | Arimaa_tools::compare_1_u(w,full_board);
		result |= temp ^ 15;
	}
	mini_mask_possible >>= 1;
	result <<= 4;

	if(Arimaa_tools::compare_1_b(mini_mask_possible,situation)) // West
	{
		buffer = mask << 1;
		temp = checkBorder(buffer);
		u_long n = get_mask_n(buffer);
		u_long e = get_mask_e(buffer); // not needed but... you never know... 
		u_long s = get_mask_s(buffer);
		u_long w = get_mask_w(buffer);

		// all impossible moves (push for W)
		temp |= (Arimaa_tools::compare_1_u(n,full_board) << 3) | (Arimaa_tools::compare_1_u(e,full_board) << 2) | (Arimaa_tools::compare_1_u(s,full_board) << 1) | Arimaa_tools::compare_1_u(w,full_board);
		result |= temp ^ 15;
	}

	return result;
}

std::list<u_long> Arimaa::generate_move_simple(const u_long& situation, const u_short& pos, const u_short& board_num, Bitboard* board)
{
	std::list<u_long> res = std::list<u_long>();
	// generate pull and normal move
	u_short possible_move_res = possible_move(situation);
	u_short nsew = 0;
	u_long mask_push = static_cast<u_long>(1);
	while (possible_move_res != 0)
	{
		if((possible_move_res & mask_push) > 0)
		{
			// normal move
			u_long move = static_cast<u_long>(board_num);
			move = (move << 6) | static_cast<u_long>(pos);
			move = (move << 2) | static_cast<u_long>(nsew);
			res.push_back(move);
		}
		possible_move_res ^= mask_push;
		mask_push<<=1;
		nsew++;
	}
	return res;
}

std::list<u_long> Arimaa::generate_move_double(const u_long& situation, const u_short& pos, const u_short& board_num, Bitboard* board)
{
	u_long mask = get_mask(pos);
	u_long possible_push_result = possible_push(mask, situation, board);

	std::list<u_long> res = std::list<u_long>();
	// generate push
	u_long mask_push = static_cast<u_long>(1);
	u_short nsew = 0;
	while (possible_push_result != 0)
	{
		if((possible_push_result & mask_push) > 0)
		{
			u_short pos_to_move = pos + Arimaa_tools::get_pos_next(nsew/4);
			u_short board_num_prey = get_piece_board_num(pos_to_move, board);

			// prey move
			u_long move = static_cast<u_long>(board_num_prey);
			move = (move << 6) | static_cast<u_long>(pos_to_move);
			move = (move << 2) | static_cast<u_long>(nsew % 4);

			// piece move
			u_long move2 = static_cast<u_long>(board_num);
			move = (move << 6) | static_cast<u_long>(pos);
			move = (move << 2) | static_cast<u_long>(nsew/4);
			res.push_back(((move << 16) | move2));
		}
		possible_push_result^=mask_push;
		mask_push<<=1;
		nsew++;
	}

	// generate pull and normal move
	u_short possible_move_res = possible_move(situation);
	nsew = 0;
	mask_push = static_cast<u_long>(1);
	while (possible_move_res != 0)
	{
		if((possible_move_res & mask_push) > 0)
		{

			// normal move
			u_long move = static_cast<u_long>(board_num);
			move = (move << 6) | static_cast<u_long>(pos);
			move = (move << 2) | static_cast<u_long>(nsew);
			// res.push_back(move);

			u_long mask_pull = static_cast<u_long>(1);
			// add pull now (prey)
			for (int i = 0; i < 4; ++i)
			{
				if((situation & mask_pull) > 0)
				{
					u_short pos_to_move = pos + Arimaa_tools::get_pos_next(nsew/4);
					u_short board_num_prey = get_piece_board_num(pos_to_move, board);

					u_long move2 = static_cast<u_long>(board_num_prey);
					move2 = (move2 << 6) | static_cast<u_long>(pos_to_move);
					move2 = (move2 << 2) | static_cast<u_long>((i+2)%4);

					res.push_back(((move << 16) | move2));
				}
			}
		}
		possible_move_res ^= mask_push;
		mask_push<<=1;
		nsew++;
	}

	return res;
}

u_long Arimaa::check_neighbour(const Bitboard* board, const u_long& mask, const u_short& piece_rank, const int& player)
{
	int enemy_player = 2 - player;
	// look for friends
	bool close = close_piece(board, mask, (NB_PIECE + 1)*player - 1);

	bool prey = false;
	bool same = false;
	bool frozen = false;

	// look for weaker enemy
	int start = (NB_PIECE + 1)*enemy_player;
	int end = (NB_PIECE + 1)*enemy_player + piece_rank;
	for (int i = start; i < end; ++i)
	{
		prey |= close_piece(board, mask, i); // possible optimisation : add !frozen in the control structure...
	}

	// look for same enemy
	start = (NB_PIECE + 1)*enemy_player + piece_rank;
	same |= close_piece(board, mask, start); // possible optimisation : add !frozen in the control structure...

	// look for stronger enemy
	start = (NB_PIECE + 1)*enemy_player + piece_rank + 1;
	for (int i = start; i < (NB_PIECE + 1); ++i)
	{
		frozen |= close_piece(board, mask, i); // possible optimisation : add !frozen in the control structure...
	}

	return static_cast<u_long>(frozen) << 12 | static_cast<u_long>(close) << 8 | static_cast<u_long>(same) << 4 | static_cast<u_long>(prey);
}

void Arimaa::apply_traps(Bitboard* board)
{
	std::vector<u_long> boards = std::vector<u_long>((NB_PIECE+1)*2, 0);
	for (u_int i = 0; i < boards.size(); ++i)
	{
		boards[i] = board->get_board(i);
	}
	#if SIZEX == 6
		constexpr int pos_trap1 = 7;
		constexpr int pos_trap2 = 10;
		constexpr int pos_trap3 = 25;
		constexpr int pos_trap4 = 28;
		constexpr u_long trap1 = static_cast<u_long>(1) << pos_trap1;
		constexpr u_long trap2 = static_cast<u_long>(1) << pos_trap2;
		constexpr u_long trap3 = static_cast<u_long>(1) << pos_trap3;
		constexpr u_long trap4 = static_cast<u_long>(1) << pos_trap4;
		constexpr u_long friends1 = (static_cast<u_long>(1) << 1) | (static_cast<u_long>(1) << 6) | (static_cast<u_long>(1) << 8) | (static_cast<u_long>(1) << 13); 
		constexpr u_long friends2 = friends1 << 2; 
		constexpr u_long friends3 = friends1 << 18;
		constexpr u_long friends4 = friends3 << 4; 
	#else
		constexpr int pos_trap1 = 18;
		constexpr int pos_trap2 = 21;
		constexpr int pos_trap3 = 42;
		constexpr int pos_trap4 = 45;
		constexpr u_long trap1 = static_cast<u_long>(1) << pos_trap1;
		constexpr u_long trap2 = static_cast<u_long>(1) << pos_trap2;
		constexpr u_long trap3 = static_cast<u_long>(1) << pos_trap3;
		constexpr u_long trap4 = static_cast<u_long>(1) << pos_trap4;
		constexpr u_long friends1 = (static_cast<u_long>(1) << 10) | (static_cast<u_long>(1) << 17) | (static_cast<u_long>(1) << 19) | (static_cast<u_long>(1) << 26); 
		constexpr u_long friends2 = friends1 << 4;
		constexpr u_long friends3 = friends1 << 24;
		constexpr u_long friends4 = friends3 << 4;
	#endif

	// player 1
	if((boards[NB_PIECE] & trap1) && !(boards[NB_PIECE] & friends1))
	{
		for (int i = 0; i <= NB_PIECE; ++i)
		{
			board->clearBit(i, pos_trap1);
		}
	}
	if((boards[NB_PIECE] & trap2) && !(boards[NB_PIECE] & friends2))
	{
		for (int i = 0; i <= NB_PIECE; ++i)
		{
			board->clearBit(i, pos_trap2);
		}
	}
	if((boards[NB_PIECE] & trap3) && !(boards[NB_PIECE] & friends3))
	{
		for (int i = 0; i <= NB_PIECE; ++i)
		{
			board->clearBit(i, pos_trap3);
		}
	}
	if((boards[NB_PIECE] & trap4) && !(boards[NB_PIECE] & friends4))
	{
		for (int i = 0; i <= NB_PIECE; ++i)
		{
			board->clearBit(i, pos_trap4);
		}
	}

	// player 2
	if((boards[2*NB_PIECE+1] & trap1) && !(boards[2*NB_PIECE+1] & friends1))
	{
		for (int i = NB_PIECE+1; i <= 2*NB_PIECE+1; ++i)
		{
			board->clearBit(i, pos_trap1);
		}
	}
	if((boards[2*NB_PIECE+1] & trap2) && !(boards[2*NB_PIECE+1] & friends2))
	{
		for (int i = NB_PIECE+1; i <= 2*NB_PIECE+1; ++i)
		{
			board->clearBit(i, pos_trap2);
		}
	}
	if((boards[2*NB_PIECE+1] & trap3) && !(boards[2*NB_PIECE+1] & friends3))
	{
		for (int i = NB_PIECE+1; i <= 2*NB_PIECE+1; ++i)
		{
			board->clearBit(i, pos_trap3);
		}
	}
	if((boards[2*NB_PIECE+1] & trap4) && !(boards[2*NB_PIECE+1] & friends4))
	{
		for (int i = NB_PIECE+1; i <= 2*NB_PIECE+1; ++i)
		{
			board->clearBit(i, pos_trap4);
		}
	}
}

void Arimaa::move(Bitboard* board, int n, u_short pos, int type)
{
	board->clearBit(n, static_cast<int>(pos));
	board->clearBit(NB_PIECE + static_cast<int>(n/(NB_PIECE+1))*(NB_PIECE + 1), static_cast<int>(pos));

	board->setBit(n, pos + Arimaa_tools::get_pos_next(type));
	board->setBit(NB_PIECE + static_cast<int>(n/(NB_PIECE+1))*(NB_PIECE + 1), pos + Arimaa_tools::get_pos_next(type));

	apply_traps(board);
}

bool Arimaa::close_piece(const Bitboard* board, const u_long& mask, const int& boardnum)
{
	u_long boards = board->get_board(boardnum);
	// compacted : 
	return (boards & mask) > 0;
}

template<int num> std::list<u_long> Arimaa::list_moves_available(Bitboard* board)
{
	std::list<u_long> moves_available;
	std::list<u_long> moves_available_simple;
	std::list<u_long> moves_available_double;
	std::list<u_long>::iterator it;

	std::vector<std::list<int>> pieces = get_pieces(board);
	for (auto piece_rank = pieces.begin() ; piece_rank != pieces.end(); ++piece_rank)
	{
		for (auto pos = (*piece_rank).begin(); pos != (*piece_rank).end(); ++pos)
		{
			u_short position = static_cast<u_short>(*pos);
			u_long situation = get_situation(position, board);
			u_short board_num = get_piece_board_num(position, board);
			if(!is_frozen(situation))
			{
				if(num > 1)
				{
					moves_available_double = generate_move_double(situation, position, board_num, board);
					it = moves_available.begin();
					moves_available.splice(it,moves_available_double);
				}
				moves_available_simple = generate_move_simple(situation, position, board_num, board);
				it = moves_available.begin();
				moves_available.splice(it,moves_available_simple);
			}
		}
	}

	return moves_available;
}


list<Move> Arimaa::list_possible_moves(Bitboard* board)
{
	Bitboard* board_temp;
	Bitboard* board_temp2;
	Bitboard* board_temp3;
	std::list<u_long> first_moves_available;
	std::list<u_long> moves_available1;
	std::list<u_long> moves_available2;
	std::list<u_long> moves_available3;
	list<Move> moves = list<Move>();
	
	constexpr u_long move_mask_1 = (static_cast<u_long>(1) << 16) - 1;
	constexpr u_long move_mask_2 = move_mask_1 << 16;
	//	constexpr u_long move_mask_3 = move_mask_2 << 16;
	//	constexpr u_long move_mask_4 = move_mask_3 << 16;

	constexpr u_long num_mask = (static_cast<u_long>(1) << 8) - 1;
	constexpr u_long pos_mask = (static_cast<u_long>(1) << 6) - 1;
	constexpr u_long nsew_mask = (static_cast<u_long>(1) << 2) - 1;

	// move is : num | pos | nsew

	// 1 2 1
	// 1 1 2
	// 1 1 1 1
	// 2   1 1
	// 2   2
	u_long temp_move;
	u_long temp_move2;
	u_long temp_move3;

	first_moves_available = list_moves_available<4>(board);
	for (auto iter_moves = first_moves_available.begin(), iter_end = first_moves_available.end(); iter_moves != iter_end; ++iter_moves)
	{
		temp_move = *iter_moves;

		int num_board = static_cast<int>((temp_move >> 8) & num_mask);
		u_short pos = static_cast<u_short>(((temp_move & move_mask_1) >> 2) & pos_mask);
		int nsew = static_cast<int>((temp_move & move_mask_1) & nsew_mask);
		board_temp = board->clone();
		move(board_temp, num_board, pos, nsew);

		if(temp_move & move_mask_2) // 2 moves at once
		{
			int num_board = static_cast<int>(((temp_move >> 16) >> 8) & num_mask);
			u_short pos = static_cast<u_short>(((temp_move >> 16) >> 2) & pos_mask);
			int nsew = static_cast<int>((temp_move >> 16) & nsew_mask);
			board_temp2 = board->clone();
			move(board_temp2, num_board, pos, nsew);

			// 2 moves are applied now we can start the search... for the next 2 moves... 
			moves_available1 = Arimaa::list_moves_available<2>(board_temp2);
			for(auto iter_move2 = moves_available1.begin(), iter_end2 = moves_available1.end(); iter_move2 != iter_end2  ; ++iter_move2)
			{
				temp_move2 = *iter_move2;
				if(temp_move2 & move_mask_2) // 2 moves at once
				{
					u_long result = temp_move2 << 32 | temp_move;
					moves.push_back(Move(result)); // 2 2
				}
				else
				{
					int num_board = static_cast<int>(((temp_move2 & move_mask_1) >> 8) & num_mask);
					u_short pos = static_cast<u_short>(((temp_move2 & move_mask_1) >> 2) & pos_mask);
					int nsew = static_cast<int>((temp_move2 & move_mask_1) & nsew_mask);
					board_temp3 = board_temp2->clone();
					move(board_temp3, num_board, pos, nsew);
					moves_available2 = Arimaa::list_moves_available<1>(board_temp3);
					
					for(auto iter_move3 = moves_available2.begin(), iter_end3 = moves_available2.end(); iter_move3 != iter_end3  ; ++iter_move3)
					{
						u_long result = (*iter_move3) << 48 | temp_move2 << 32 | temp_move;
						moves.push_back(Move(result)); // 2 1 1
					}
					delete board_temp3;
				}
			}
			delete board_temp2; // MANDATORY ELSE MEMORY LEAK
		}
		else
		{
			// here we have still 3 moves lefts
			moves_available1 = Arimaa::list_moves_available<3>(board_temp);
			for(auto iter_move2 = moves_available1.begin(), iter_end2 = moves_available1.end(); iter_move2 != iter_end2  ; ++iter_move2)
			{
				temp_move2 = *iter_move2;
				int num_board = static_cast<int>((temp_move2 >> 8) & num_mask);
				u_short pos = static_cast<u_short>(((temp_move2 & move_mask_1) >> 2) & pos_mask);
				int nsew = static_cast<int>((temp_move2 & move_mask_1) & nsew_mask);
				board_temp2 = board_temp->clone();
				move(board_temp2, num_board, pos, nsew);

				if(temp_move2 & move_mask_2) // 1 2
				{
					int num_board = static_cast<int>(((temp_move2 >> 16) >> 8) & num_mask);
					u_short pos = static_cast<u_short>(((temp_move2 >> 16) >> 2) & pos_mask);
					int nsew = static_cast<int>((temp_move2 >> 16) & nsew_mask);
					board_temp3 = board_temp2->clone();
					move(board_temp3, num_board, pos, nsew);

					moves_available2 = Arimaa::list_moves_available<1>(board_temp3);
					for(auto iter_move3 = moves_available2.begin(), iter_end3 = moves_available2.end(); iter_move3 != iter_end3  ; ++iter_move3)
					{
						u_long result = (*iter_move3) << 48 | temp_move2 << 16 | temp_move;
						moves.push_back(Move(result)); // 1 2 1
					}
					delete board_temp3;
				}
				else // 1 1 
				{
					// 2 moves are applied now we can start the search... for the next 2 moves... 
					moves_available2 = Arimaa::list_moves_available<2>(board_temp2);
					for(auto iter_move3 = moves_available2.begin(), iter_end2 = moves_available2.end(); iter_move3 != iter_end2  ; ++iter_move3)
					{
						temp_move3 = *iter_move3;
						if(temp_move3 & move_mask_2) // 2 moves at once
						{
							u_long result = temp_move3 << 32 | temp_move2 << 16 | temp_move;
							moves.push_back(Move(result)); // 1 1 2
						}
						else
						{
							int num_board = static_cast<int>(((temp_move3 & move_mask_1) >> 8) & num_mask);
							u_short pos = static_cast<u_short>(((temp_move3 & move_mask_1) >> 2) & pos_mask);
							int nsew = static_cast<int>((temp_move3 & move_mask_1) & nsew_mask);
							board_temp3 = board_temp2->clone();
							move(board_temp3, num_board, pos, nsew);
							moves_available3 = Arimaa::list_moves_available<1>(board_temp3);
							
							for(auto iter_move3 = moves_available3.begin(), iter_end3 = moves_available3.end(); iter_move3 != iter_end3  ; ++iter_move3)
							{
								u_long result = (*iter_move3) << 48 | temp_move3 << 32 | temp_move2 << 16 | temp_move;
								moves.push_back(Move(result)); // 1 1 1 1
							}
							delete board_temp3;
						}
					}
				}
				delete board_temp2;
			}
		}
		delete board_temp; // MANDATORY ELSE MEMORY LEAK
	}
	return moves;
}

int Arimaa::play_random_moves(Bitboard* board)
{
	int iterations = 0;
	while(iterations < 120)
	{











		auto nodet = end(board);
		if(nodet > 0) return nodet;
		iterations ++;
	}
	return 0;
}


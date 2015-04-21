#include "Arimaa.h"
#include <map>

using std::string;
using std::list;
// TODO :
// 
//	rabbits should not move backward
// 
//	traps
//
//	end condition :
// 		> X turns => to be sure that random simulation happend to finish
// 		rabbit to an end
// 		no piece can move	
//
//	generate 4 moves
//	
//	play move
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
	return (static_cast<numtyp>(1) << pos);
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
	throw std::logic_error("move not implemented.");

	return 0;
}

void Arimaa::play(Move& position, Bitboard* board)
{
	numtyp boardused = board->get_board(0) | board->get_board(1); // get what places are used.
	boardused >>= (SIZEX - position.get_move());
	int i = 0;
	while( (boardused & 1) == 1)
	{
		i++;
		boardused >>= SIZEX;
	}
	
	board->setBit(board->get_player() - 1, SIZEX - static_cast<int>(position.get_move()), i); // to update
	board->play();
}

void Arimaa::diplay_board(const Bitboard* board)
{
	std::vector<numtyp> boards = std::vector<numtyp>((NB_PIECE+1)*2, 0);
	for (u_int i = 0; i < boards.size(); ++i)
	{
		boards[i] = board->get_board(i);
	}
	numtyp check = static_cast<numtyp>(1);
	check = check << (SIZEX * SIZEY - 1);

	std::stringbuf buffer;
	std::ostream os(&buffer);
	os << std::endl;
	static const std::vector<char> colChar = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
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
	for (int k = 0; k < SIZEX; ++k) os << "  " << (k + 'A') << " ";
	// for (int k = 0; k < SIZEX; ++k) os << "  " << colChar[k] << " ";
	os << std::endl;
	std::cout << buffer.str() << std::endl;
}

u_long Arimaa::get_situation(const u_short& pos, Bitboard* board)
{
	u_long mask = get_mask(pos);
	u_long result = checkBorder(mask); // n e s w : 8 4 2 1 
	u_long returnval = 0;

	u_short piece_rank = get_piece_rank(mask, board);
	int player = board->get_player(); // 1 or 2 // 0 or 1 ???

	/*in theory there is no need to do the top and bottom check. Only left and Right are mandatory : slight optimisation possible later */

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

	returnval |= (result << 4);
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

std::list<u_long> Arimaa::generate_move(const u_long& situation, const u_short& pos, const u_short& board_num, Bitboard* board)
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
			u_short pos_to_move = pos + Arimaa_tools::get_pos_push(nsew/4);
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
			res.push_back(move);

			u_long mask_pull = static_cast<u_long>(1);
			// add pull now (prey)
			for (int i = 0; i < 4; ++i)
			{
				if((situation & mask_pull) > 0)
				{
					u_short pos_to_move = pos + Arimaa_tools::get_pos_push(nsew/4);
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

void Arimaa::move(Bitboard* board, int n, u_short pos, int type)
{
	throw std::logic_error("move not implemented.");
	// TO BE COMPLETED !!!!!

	board->clearBit(n, static_cast<int>(pos));
	board->setBit(n, (pos + Arimaa_tools::get_pos_push(type)));
}

bool Arimaa::close_piece(const Bitboard* board, const u_long& mask, const int& boardnum)
{
	numtyp boards = board->get_board(boardnum);
	// compacted : 
	return (boards & mask) > 0;
}

list<Move> Arimaa::list_possible_moves(Bitboard* board)
{
	std::vector<std::list<int>> pieces = get_pieces(board);
	for (auto piece_rank = pieces.begin() ; piece_rank != pieces.end(); ++piece_rank)
	{
		for (auto pos = (*piece_rank).begin(); pos != (*piece_rank).end(); ++pos)
		{
			u_short position = static_cast<u_short>(*pos);
			u_long situation = get_situation(position, board);
			u_short board_num = get_piece_board_num(pos_to_move, board);
			if(!is_frozen(situation))
			{
				std::list<u_long> moves_available = generate_move(situation, position, board_num, board);
			}
		}
	}

	list<Move> moves;
	return moves;
}

int Arimaa::play_random_moves(Bitboard* board)
{
	throw std::logic_error("move not implemented.");

	auto nodet = end(board);
	return nodet;
}

Move Arimaa::convert_move(string move)
{
	auto transform = std::map<char, int>();
	transform.insert(std::pair<char, int>('n', 3)); // north
	transform.insert(std::pair<char, int>('e', 2)); // east
	transform.insert(std::pair<char, int>('w', 1)); // west
	transform.insert(std::pair<char, int>('s', 0)); // south
	transform.insert(std::pair<char, int>('A', SIZEX - 1)); // first col
	transform.insert(std::pair<char, int>('B', SIZEX - 2)); // second col...
	transform.insert(std::pair<char, int>('C', SIZEX - 3));
	transform.insert(std::pair<char, int>('D', SIZEX - 4));
	transform.insert(std::pair<char, int>('E', SIZEX - 5));
	transform.insert(std::pair<char, int>('F', SIZEX - 6));
	transform.insert(std::pair<char, int>('G', SIZEX - 7));
	transform.insert(std::pair<char, int>('H', SIZEX - 8));
	transform.insert(std::pair<char, int>('1', 1)); // first row
	transform.insert(std::pair<char, int>('2', 2)); // second row...
	transform.insert(std::pair<char, int>('3', 3));
	transform.insert(std::pair<char, int>('4', 4));
	transform.insert(std::pair<char, int>('5', 5));
	transform.insert(std::pair<char, int>('6', 6));
	transform.insert(std::pair<char, int>('7', 7));
	transform.insert(std::pair<char, int>('8', 8));

	numtyp futurmove;
	// futurmove = NESW | pos  
	futurmove = transform[move[0]] + (transform[move[1]] - 1)*SIZEX + 64 * transform[move[2]];
	std::cout << futurmove << std::endl;
	//	std::cout << (futurmove >> 6) << std::endl;
	return Move(futurmove);
}
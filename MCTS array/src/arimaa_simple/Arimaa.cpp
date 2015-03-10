#include "Arimaa.h"
#include <map>

//#define DISPLAY_C4
using std::string;
using std::list;

Arimaa::Arimaa()
{
}


u_long Arimaa::get_mask(const int& pos)
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

u_long Arimaa::checkBorder(const u_long& pos)
{
	return (((pos & TOP_BORDER) != 0) << 3) | (((pos & RIGHT_BORDER) != 0) << 2) | (((pos & BOTTOM_BORDER) != 0) << 1) | ((pos & LEFT_BORDER) != 0);
}


int Arimaa::end(const Bitboard* board)
{
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
	
	board->setBit(board->get_player() - 1, SIZEX - static_cast<int>(position.get_move()), i);
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
	for (int k = 0; k < SIZEX; ++k) os << "  " << colChar[k] << " ";
	os << std::endl;
	std::cout << buffer.str() << std::endl;
}

u_long Arimaa::get_situation(const int& pos, Bitboard* board)
{
	u_long result = checkBorder(pos); // n e s w : 8 4 2 1 
	u_long mask = get_mask(pos);
	u_long returnval = 0;

	int piece_rank = 0;
	int player = board->get_player();

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

u_long Arimaa::check_neighbour(const Bitboard* board, const u_long& mask, const int& piece_rank, const int& player)
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

void Arimaa::move(Bitboard* board, int n, int pos, int type)
{
	static const int move_type[] = {SIZEX, -1, -SIZEX, 1}; // n : 0, e : 1, s : 2, w : 3
	board->clearBit(n, pos);
	board->setBit(n, (pos + move_type[type]));
}

bool Arimaa::close_piece(const Bitboard* board, const u_long& mask, const int& boardnum)
{
	numtyp boards = board->get_board(boardnum);

	// compacted : 
	return (boards & mask) > 0;
}

list<Move> Arimaa::list_possible_moves(Bitboard* board)
{
	list<Move> moves;
	return moves;
}

int Arimaa::play_random_moves(Bitboard* board)
{
	nodet = end(board);
	return nodet;
}

Move Arimaa::convert_move(string move)
{
	auto transform = std::map<char, int>();
	transform.insert(std::pair<char, int>('n', 0)); // north
	transform.insert(std::pair<char, int>('e', 1)); // east
	transform.insert(std::pair<char, int>('w', 2)); // west
	transform.insert(std::pair<char, int>('s', 3)); // south
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
	futurmove = transform[move[0]] + (transform[move[1]] - 1)*SIZEX + 64 * transform[move[2]];
	std::cout << futurmove << std::endl;
//	std::cout << (futurmove >> 6) << std::endl;
	return Move(futurmove);
}
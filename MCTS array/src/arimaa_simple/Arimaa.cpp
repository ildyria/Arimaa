#include "Arimaa.h"
#include <map>

//#define DISPLAY_C4
using std::string;
using std::list;

Arimaa::Arimaa()
{
}

int Arimaa::end(const Bitboard* board)
{
	return check_null(board);
}

int Arimaa::check_null(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(0);
	boardused = ~boardused;
	boardused >>= 22; // full board
	if (((board->get_board(0) | board->get_board(1)) & boardused) == boardused)
	{
		return 4;
	}
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
	for (int i = 0; i < boards.size(); ++i)
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

void Arimaa::move(Bitboard* board, int n, int pos, int type)
{
	static const int move_type[] = {SIZEX, -1, -SIZEX, 1};
	board->clearBit(n, pos);
	board->setBit(n, (pos + move_type[type]));
}

bool Arimaa::close_piece(Bitboard* board, int pos, int boardnum)
{
	numtyp boards = board->get_board(boardnum);
	numtyp mask = static_cast<numtyp>(1) << pos;
	mask = mask << 1 | mask >> 1 | mask << SIZEX | mask >> SIZEX; // warning if pos in close to the side edges... WILL NEED CHECK
	return (boards & mask) > 0;
}

list<Move> Arimaa::list_possible_moves(Bitboard* board)
{
	list<Move> moves;
	list<int> listmoves = board->get_empty(0);
	list<int>::iterator iter;
	for (iter = listmoves.begin(); iter != listmoves.end(); ++iter)
	{
		moves.push_front(Move(*iter));
	}
	listmoves.clear();
	return moves;
}

int Arimaa::play_random_moves(Bitboard* board)
{
	list<int> ListOfMoves;
	list<int>::iterator iter;
	int chosen, nodet;

	nodet = end(board);

	while (nodet < 1)
	{
		ListOfMoves = board->get_empty(0);
		chosen = Random::I()->get_min_max(0, static_cast<int>(ListOfMoves.size()) - 1);
		for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); ++iter)
		{
			if (chosen == 0)
			{
#ifdef DISPLAY_C4
				std::cout << " > " << *iter ;
#endif // DISPLAY_C4
				numtyp boardused = board->get_board(0) | board->get_board(1); // get what places are used.
				boardused >>= (SIZEX - *iter);
				int i = 0;
				while ((boardused & 1) == 1)
				{
					i++;
					boardused >>= SIZEX;
				}

				board->setBit(board->get_player() - 1, SIZEX - *iter, i);
				board->play();
				break;
			}
			--chosen;
		}
		nodet = end(board);
		ListOfMoves.clear();
	}

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
	std::cout << (futurmove >> 6) << std::endl;
	return Move(futurmove);
}
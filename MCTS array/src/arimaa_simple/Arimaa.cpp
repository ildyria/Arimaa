#include "Arimaa.h"

//#define DISPLAY_C4
using std::string;
using std::list;

Arimaa::Arimaa()
{
}

int Arimaa::end(const Bitboard* board)
{
	int res = check_horizontal(board);
	if (res > 0)
	{
		return res;
	}

	res = check_vertical(board);
	if (res > 0)
	{
		return res;
	}

	res = check_fst_diag(board);
	if (res > 0)
	{
		return res;
	}

	res = check_snd_diag(board);
	if (res > 0)
	{
		return res;
	}

	return check_null(board);
}

int Arimaa::check_horizontal(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(15); // 0001111 in binary
	numtyp board1 = board->get_board(0);
	numtyp board2 = board->get_board(1);
	for (int y = 0; y < SIZEY; y++)
	{
		for (int x = 0; x <= SIZEX - 4; x++)
		{
			if ((boardused & board1) == boardused)
			{
				return 1;
			}
			elseif((boardused & board2) == boardused)
			{
				return 2;
			}
			boardused <<= 1;
		}
		boardused <<= 3;
	}
	return 0;
}

int Arimaa::check_vertical(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(1);
	boardused += (boardused << SIZEX) + (boardused << (SIZEX * 2)) + (boardused << (SIZEX * 3));

	numtyp board1 = board->get_board(0);
	numtyp board2 = board->get_board(1);
	for (int y = 0; y <= SIZEY - 4; y++)
	{
		for (int x = 0; x < SIZEX; x++)
		{
			if ((boardused & board1) == boardused)
			{
				return 1;
			}
			elseif((boardused & board2) == boardused)
			{
				return 2;
			}
			boardused <<= 1;
		}
	}
	return 0;
}

int Arimaa::check_fst_diag(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(1);
	boardused += (boardused << (SIZEX + 1)) + (boardused << ((SIZEX + 1) * 2)) + (boardused << ((SIZEX + 1) * 3));

	numtyp board1 = board->get_board(0);
	numtyp board2 = board->get_board(1);
	for (int y = 0; y <= SIZEY - 4; y++)
	{
		for (int x = 0; x <= SIZEX - 4; x++)
		{
			if ((boardused & board1) == boardused)
			{
				return 1;
			}
			elseif((boardused & board2) == boardused)
			{
				return 2;
			}
			boardused <<= 1;
		}
		boardused <<= 3;
	}
	return 0;
}

int Arimaa::check_snd_diag(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(1);
	boardused <<= 3; // 1000
	boardused += (boardused << (SIZEX - 1)) + (boardused << ((SIZEX - 1) * 2)) + (boardused << ((SIZEX - 1) * 3));

	numtyp board1 = board->get_board(0);
	numtyp board2 = board->get_board(1);
	for (int y = 0; y <= SIZEY - 4; y++)
	{
		for (int x = 0; x <= SIZEX - 4; x++)
		{
			if ((boardused & board1) == boardused)
			{
				return 1;
			}
			elseif((boardused & board2) == boardused)
			{
				return 2;
			}
			boardused <<= 1;
		}
		boardused <<= 3;
	}
	return 0;
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
	numtyp board0 = board->get_board(0); // get what places are used.
	numtyp board1 = board->get_board(1);
	numtyp check = static_cast<numtyp>(1);
	check = check << (SIZEX * SIZEY - 1);

	std::stringbuf buffer;
	std::ostream os(&buffer);
	os << std::endl;
	for (int y = (SIZEY - 1); y >= 0; --y)
	{
		for (int k = 0; k < SIZEX; ++k) os << "+---";
		os << "+" << std::endl;
		for (int x = 0; x < SIZEX; ++x)
		{
			os << "| ";
			if ((board0 & check) == check)
			{
				os << "O";
			}
			elseif((board1 & check) == check)
			{
				os << "X";
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
	for (int k = 0; k < SIZEX; ++k) os << "+---";
	os << "+" << std::endl;
	for (int k = 1; k <= SIZEX; ++k) os << "  " << k << " ";
	os << std::endl;
	std::cout << buffer.str() << std::endl;

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
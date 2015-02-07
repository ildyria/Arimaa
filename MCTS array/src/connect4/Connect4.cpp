#include "Connect4.h"
#include <sstream>
#define elseif else if
#include "../tools/Random.h"

//#define DISPLAY_C4
using std::string;
using std::list;

Connect4::Connect4()
{
}

int Connect4::end(const Bitboard* board)
{
	int res = checkHorizontal(board);
	if (res > 0)
	{
		return res;
	}

	res = checkVertical(board);
	if (res > 0)
	{
		return res;
	}

	res = checkDiag1(board);
	if (res > 0)
	{
		return res;
	}

	res = checkDiag2(board);
	if (res > 0)
	{
		return res;
	}

	return checkNull(board);
}

int Connect4::checkHorizontal(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(15); // 0001111 in binary
	numtyp board1 = board->getBoard(0);
	numtyp board2 = board->getBoard(1);
	for (int y = 0; y < board->getSizeY(); y++)
	{
		for (int x = 0; x <= board->getSizeX() - 4; x++)
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

int Connect4::checkVertical(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(1);
	boardused += (boardused << board->getSizeX()) + (boardused << (board->getSizeX() * 2)) + (boardused << (board->getSizeX() * 3));

	numtyp board1 = board->getBoard(0);
	numtyp board2 = board->getBoard(1);
	for (int y = 0; y <= board->getSizeY() - 4; y++)
	{
		for (int x = 0; x < board->getSizeX(); x++)
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

int Connect4::checkDiag1(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(1);
	boardused += (boardused << (board->getSizeX() + 1)) + (boardused << ((board->getSizeX() + 1) * 2)) + (boardused << ((board->getSizeX() + 1) * 3));

	numtyp board1 = board->getBoard(0);
	numtyp board2 = board->getBoard(1);
	for (int y = 0; y <= board->getSizeY() - 4; y++)
	{
		for (int x = 0; x <= board->getSizeX() - 4; x++)
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

int Connect4::checkDiag2(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(1);
	boardused <<= 3; // 1000
	boardused += (boardused << (board->getSizeX() - 1)) + (boardused << ((board->getSizeX() - 1) * 2)) + (boardused << ((board->getSizeX() - 1) * 3));

	numtyp board1 = board->getBoard(0);
	numtyp board2 = board->getBoard(1);
	for (int y = 0; y <= board->getSizeY() - 4; y++)
	{
		for (int x = 0; x <= board->getSizeX() - 4; x++)
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

int Connect4::checkNull(const Bitboard* board)
{
	numtyp boardused = static_cast<numtyp>(0);
	boardused = ~boardused;
	boardused >>= 22; // full board
	if (((board->getBoard(0) | board->getBoard(1)) & boardused) == boardused)
	{
		return 3;
	}
	return 0;
}

void Connect4::play(Move& position, Bitboard* board)
{
	numtyp boardused = board->getBoard(0) | board->getBoard(1); // get what places are used.
	boardused >>= (7 - position.getMove());
	int i = 0;
	while( (boardused & 1) == 1)
	{
		i++;
		boardused >>= board->getSizeX();
	}
	
	board->setBit(board->getPlayer() - 1, board->getSizeX() - static_cast<int>(position.getMove()), i);
	board->play();
}

void Connect4::diplayBoard(const Bitboard* board)
{
	numtyp board0 = board->getBoard(0); // get what places are used.
	numtyp board1 = board->getBoard(1);
	numtyp check = static_cast<numtyp>(1);
	check = check << 42;

	std::stringbuf buffer;
	std::ostream os(&buffer);
	os << std::endl;
	for (int y = 5; y >= 0; --y)
	{
		for (int k = 0; k < 7; ++k) os << "+---";
		os << "+" << std::endl;
		for (int x = 0; x < 7; ++x)
		{
			check >>= 1;
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
		}
		os << "|" << std::endl;
	}
	for (int k = 0; k < 7; ++k) os << "+---";
	os << "+" << std::endl;
	for (int k = 1; k <= 7; ++k) os << "  " << k << " ";
	os << std::endl;
	std::cout << buffer.str() << std::endl;

}

list<Move> Connect4::listPossibleMoves(Bitboard* board)
{
	list<Move> moves;
	list<int> listmoves = board->getEmpty(0);
	list<int>::iterator iter;
	for (iter = listmoves.begin(); iter != listmoves.end(); ++iter)
	{
		moves.push_front(Move(*iter));
	}
	listmoves.clear();
	return moves;
}

int Connect4::playRandomMoves(Bitboard* board)
{
	list<int> ListOfMoves;
	list<int>::iterator iter;
	int chosen, nodet;

	nodet = end(board);

	while (nodet < 1)
	{
		ListOfMoves = board->getEmpty(0);
		chosen = Random::I()->getNum(0, static_cast<int>(ListOfMoves.size()) - 1);
		for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); ++iter)
		{
			if (chosen == 0)
			{
#ifdef DISPLAY_C4
				std::cout << " > " << *iter ;
#endif // DISPLAY_C4
				numtyp boardused = board->getBoard(0) | board->getBoard(1); // get what places are used.
				boardused >>= (7 - *iter);
				int i = 0;
				while ((boardused & 1) == 1)
				{
					i++;
					boardused >>= board->getSizeX();
				}

				board->setBit(board->getPlayer() - 1, board->getSizeX() - *iter, i);
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
#include "TicTacToe.h"
#define elseif else if

using std::to_string;

void TicTacToe::diplayBoard(const Bitboard& board){
	cout << "player to play : " << board.getPlayer() << endl;
	int i = 0;
	for (int i = 0; i < 9; i++)
	{
		if (board.getBit(0, i) == 1)
		{
			cout << " x ";
		}
		else if (board.getBit(1, i) == 1)
		{
			cout << " o ";
		}
		else
		{
			cout << " _ ";
		}
		
		if (i % 3 == 2)
		{
			cout << endl;
		}
		else
		{
			cout << "|";
		}
	}
}

int TicTacToe::end(const Bitboard& board){
	unsigned long board1 = board.getBoard(0);
	unsigned long board2 = board.getBoard(1);
	unsigned long pos1 = (unsigned long)448;	// 111000000
	unsigned long pos2 = (unsigned long)56;		// 000111000
	unsigned long pos3 = (unsigned long)7;		// 000000111
	unsigned long pos4 = (unsigned long)292;	// 100100100
	unsigned long pos5 = (unsigned long)146;	// 010010010
	unsigned long pos6 = (unsigned long)73;		// 001001001
	unsigned long pos7 = (unsigned long)273;	// 100010001
	unsigned long pos8 = (unsigned long)84;		// 001010100
	if ((board1 & pos1) == pos1 ||
		(board1 & pos2) == pos2 ||
		(board1 & pos3) == pos3 ||
		(board1 & pos4) == pos4 ||
		(board1 & pos5) == pos5 ||
		(board1 & pos6) == pos6 ||
		(board1 & pos7) == pos7 ||
		(board1 & pos8) == pos8)
	{
		cout << "player 1 wins detected." << endl;
		return 1;
	}
	if ((board2 & pos1) == pos1 ||
		(board2 & pos2) == pos2 ||
		(board2 & pos3) == pos3 ||
		(board2 & pos4) == pos4 ||
		(board2 & pos5) == pos5 ||
		(board2 & pos6) == pos6 ||
		(board2 & pos7) == pos7 ||
		(board2 & pos8) == pos8)
	{
		cout << "player 2 wins detected." << endl;
		return 2;
	}
	if ((board1 | board2) == (unsigned long)511)
	{
		cout << "Board full detected : TIE." << endl;
		return 3;
	}
	cout << "No Winner Yet" << endl;
	return 0;
}

void TicTacToe::play(string position, Bitboard& board)
{

	int pos = strtol(position.c_str(), nullptr, 10);
	int player = board.getPlayer() - 1;
	board.setBit(player, pos);
	board.play();
}

list<string> TicTacToe::listPossibleMoves(const Bitboard& board)
{
	list<string> moves;
	list<int> free1 = board.getEmpty(0);
	list<int> free2 = board.getEmpty(1);
	list<int>::iterator iterl1, iterl2, iter;

#ifdef DEBUG
	cout << "possible moves free1 : ";
	for (iter = free1.begin(); iter != free1.end(); iter++){
		cout << *iter << " ";
	}
	cout << endl;

	cout << "possible moves free2 : ";
	for (iter = free2.begin(); iter != free2.end(); iter++){
		cout << *iter << " ";
	}
	cout << endl;
#endif // DEBUG


	// this algorithm works because list are already sorted !!!
	iterl1 = free1.begin();
	iterl2 = free2.begin();
	while (iterl2 != free2.end() && iterl1 != free1.end())
	{
		if (*iterl1 == *iterl2) // check if list item are the same => keep it
		{
			moves.push_back(to_string(*iterl1));
			
			if (iterl2 == free2.end() || iterl1 == free1.end())
			{
				break;
			}
			else	// if we still have more items to look at
			{
				iterl1++;
				iterl2++;
			}
		}
		elseif(*iterl1 > *iterl2) // if item list1 is greater than item list2
		{
			if (iterl2 != free2.end()) // if we still have more items to look at
			{
				iterl2++;
			}
			else // we don't => get out
			{
				break;
			}
		}
		else // if item list2 is greater than item list1
		{
			if (iterl1 != free1.end()) // if we still have more items to look at
			{
				iterl1++;
			}
			else // we don't => get out
			{
				break;
			}
		}
	}

	return moves;
}


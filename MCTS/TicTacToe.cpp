#include "TicTacToe.h"
#define elseif else if
//#define DISPLAY_TTT
//#define DEBUG_TTT

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
	unsigned long pos1 = static_cast<unsigned long>(448);	// 111000000
	unsigned long pos2 = static_cast<unsigned long>(56);	// 000111000
	unsigned long pos3 = static_cast<unsigned long>(7);		// 000000111
	unsigned long pos4 = static_cast<unsigned long>(292);	// 100100100
	unsigned long pos5 = static_cast<unsigned long>(146);	// 010010010
	unsigned long pos6 = static_cast<unsigned long>(73);	// 001001001
	unsigned long pos7 = static_cast<unsigned long>(273);	// 100010001
	unsigned long pos8 = static_cast<unsigned long>(84);	// 001010100
	if ((board1 & pos1) == pos1 ||
		(board1 & pos2) == pos2 ||
		(board1 & pos3) == pos3 ||
		(board1 & pos4) == pos4 ||
		(board1 & pos5) == pos5 ||
		(board1 & pos6) == pos6 ||
		(board1 & pos7) == pos7 ||
		(board1 & pos8) == pos8)
	{
#ifdef DISPLAY_TTT
		cout << "player 1 wins detected." << endl;
#endif // DISPLAY_TTT
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
#ifdef DISPLAY_TTT
		cout << "player 2 wins detected." << endl;
#endif // DISPLAY_TTT
		return 2;
	}
	if ((board1 | board2) == static_cast<unsigned long>(511))
	{
#ifdef DISPLAY_TTT
		cout << "Board full detected : TIE." << endl;
#endif // DISPLAY_TTT
		return 3;
	}
//	cout << "No Winner Yet" << endl;
	return 0;
}

void TicTacToe::play(Move& position, Bitboard& board)
{
	int pos = position.getInt();
	int player = board.getPlayer() - 1;
	board.setBit(player, pos);
	board.play();
}

list<Move> TicTacToe::listPossibleMoves(const Bitboard& board)
{
	list<Move> moves;
	list<int> free1 = board.getEmpty(0);
	list<int> free2 = board.getEmpty(1);
	list<int>::iterator iterl1, iterl2;

#ifdef DEBUG_TTT
	cout << "possible moves free1 : ";
	for (iter1 = free1.begin(); iter1 != free1.end(); ++iter1){
		cout << *iter1 << " ";
	}
	cout << endl;

	cout << "possible moves free2 : ";
	for (iter2 = free2.begin(); iter2 != free2.end(); ++iter2){
		cout << *iter2 << " ";
	}
	cout << endl;
#endif // DEBUG

	for (iterl1 = free1.begin(); iterl1 != free1.end(); ++iterl1)
	{
		iterl2 = find(free2.begin(), free2.end(), *iterl1);
		if (iterl2 != free2.end())
		{
			moves.push_back(Move(*iterl1));
		}
	}

	return moves;
}

int TicTacToe::playRandomMoves(Bitboard& board)
{
	list<Move> ListOfMoves;
	list<Move>::iterator iter;
	int chosen, nodet;

	nodet = end(board);

	while (nodet < 1)
	{
		ListOfMoves = listPossibleMoves(board);
		chosen = Random::I()->getNum(0, ListOfMoves.size() - 1);
		for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); ++iter)
		{
			if (chosen == 0){
#ifdef DISPLAY_TTT
				cout << " > " << *iter << endl;
#endif // DISPLAY_TTT
				play(*iter, board);
				break;
			}
			--chosen;
		}
		nodet = end(board);
	}

	return nodet;
}
#include "TicTacToe.h"
#include "Mcts.h"
#define elseif else if

using namespace mcts;

int main(int argc, char const *argv[])
{
	cout << endl << "\t\t    If it compiles then it works ! " << endl;
	cout << "\tBut remember, all code is guilty until proven inocent !" << endl << endl;

	Bitboard Bb = Bitboard(3,2,1);
	Bitboard bitboard = Bitboard(3, 2, 1);
	int result = 0, moveok;
	string move;
	list<string> Listtoprint;
	list<string>::iterator iter;

	Mcts mcts = Mcts(bitboard, 5, 1, 1000, 10);
	move = mcts.GetBestMove();
	mcts.movePlayed(move);
	cout << "chosen move by MCTS : " << move << endl;
	cout << endl;
	mcts.print_tree(1);

	TicTacToe::play(move, Bb);


	while (result == 0)
	{
		TicTacToe::diplayBoard(Bb);
		Listtoprint = TicTacToe::listPossibleMoves(Bb);
		cout << "possible moves : ";
		for (iter = Listtoprint.begin(); iter != Listtoprint.end(); ++iter){
			cout << *iter << " ";
		}
		cout << endl;

		if (Bb.getPlayer() == 2)
		{
			moveok = 0;
			while (moveok == 0)
			{
				cin >> move;
				iter = find(Listtoprint.begin(), Listtoprint.end(), move);
				if (iter != Listtoprint.end())
				{
					moveok = 1;
				}
			}
		}
		else
		{
			move = mcts.GetBestMove();
			mcts.print_tree(1);
		}
		mcts.movePlayed(move);
		TicTacToe::play(move, Bb);
		result = TicTacToe::end(Bb);
	}
	cout << endl;
	TicTacToe::diplayBoard(Bb);

	if (result == 1)
	{
		cout << "player 1 wins detected." << endl;
	}
	elseif(result == 2)
	{
		cout << "player 2 wins detected." << endl;
	}
	else
	{
		cout << "Board full detected : TIE." << endl;
	}
	_sleep(4000);
}
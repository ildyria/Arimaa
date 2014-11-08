#include "Mcts.h"
#include "TicTacToe.h"
#include <time.h>
#include <thread>
#define elseif else if

using namespace mcts;

int main(int argc, char const *argv[])
{
	cout << endl << "\t\t    If it compiles then it works ! " << endl;
	cout << "\tBut remember, all code is guilty until proven inocent !" << endl << endl;

	Bitboard Bb = Bitboard(3,2,1);
	Bitboard bitboard = Bitboard(3, 2, 1);
	int result = 0, moveok;
	Move move;
	list<Move> Listtoprint;
	list<Move>::iterator iter;

	TicTacToe* game = new TicTacToe();

	Mcts mcts = Mcts(game,		// game
					bitboard,	// board to start with
					4,			// depth of the maximum search
					500,		// time limit for the simulations
					1000,		// number of root simulations
					10);		// number of leaf simulations

	int IA = 2;
	while (result == 0)
	{
		game->diplayBoard(Bb);
		Listtoprint = game->listPossibleMoves(Bb);
		cout << "possible moves : ";
		for (iter = Listtoprint.begin(); iter != Listtoprint.end(); ++iter){
			cout << *iter << " ";
		}
		cout << "or -1 to pass."<< endl;
		//mcts.UpdateRoot();

		if (Bb.getPlayer() != IA)
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
				elseif(move.getMove() == "exit")
				{
					exit(0);
				}
				elseif(move.getInt() == -1)
				{
					IA = (IA == 2) ? 1 : 2;
					moveok = 1;
				}
			}
		}

		if (Bb.getPlayer() == IA)
		{
			move = mcts.GetBestMove();
			mcts.print_tree(2);
		}
		mcts.movePlayed(move);
		game->play(move, Bb);
		result = game->end(Bb);
	}
	cout << endl;
	game->diplayBoard(Bb);

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
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));;
}
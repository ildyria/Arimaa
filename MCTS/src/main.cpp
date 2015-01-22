#define CONNECT4
//#define TICTACTOE
//#define DISPLAY_TREE

#include "./tools/Count.h"

#include "./mcts/Mcts.h"

#ifdef TICTACTOE
#include "./tictactoe/TicTacToe.h"
#endif

#ifdef CONNECT4
#include "./connect4/Connect4.h"
#include "./connect4/BitboardConnect4.h"
#endif

#include <thread>
#define elseif else if

using namespace mcts;
using std::cout;
using std::cin;
using std::endl;
using std::list;

int main(int argc, char const *argv[])
{
#ifdef TICTACTOE
	TicTacToe* game = new TicTacToe();
	Bitboard* Bb = new Bitboard(3, 3, 2, 1);
#endif

#ifdef CONNECT4
	Connect4* game = new Connect4();
	BitboardConnect4* Bb = new BitboardConnect4();
#endif

	cout << endl << "\t\t    If it compiles then it works ! " << endl;
	cout << "\tBut remember, all code is guilty until proven innocent !" << endl << endl;

	int result = 0, moveok;
	Move move;

	MctsArgs* args = new MctsArgs();

	list<Move> Listtoprint;
	list<Move>::iterator iter;


	Mcts mcts = Mcts(game,		// game
					Bb,	// board to start with
					args);

	int IA = 2;
	while (result == 0)
	{
		game->diplayBoard(Bb);
		mcts.UpdateRoot();

		if (Bb->getPlayer() != IA)
		{
			Listtoprint = game->listPossibleMoves(Bb);
			cout << endl << "possible moves : ";
			for (iter = Listtoprint.begin(); iter != Listtoprint.end(); ++iter){
				cout << *iter << " ";
			}
			cout << "or -1 to pass.";

			moveok = 0;
			while (moveok == 0)
			{
				cout << endl << "Your move ?" << endl;
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

		if (Bb->getPlayer() == IA)
		{
			cout << endl << "AI turn... please wait.";
			move = mcts.GetBestMove();
#ifdef DISPLAY_TREE
			mcts.print_tree(2);
#endif // DISPLAY_TREE
			mcts.get_Number_Leaves();
			cout << endl << Count::I();
			cout << endl << "chosen move : " << move << endl;
		}
#ifdef CONNECT4
		Bb = static_cast<BitboardConnect4*>(mcts.movePlayed(move));
#endif
#ifndef CONNECT4
		Bb = mcts.movePlayed(move);
#endif
		if (mcts.winning_Strategy() == 10)
		{
			cout << "You don't know it yet, but you lost ! =D" << endl;
		};
		mcts.get_Number_Leaves();
		cout << endl << Count::I();

		result = game->end(Bb);
	}
	cout << endl;
	game->diplayBoard(Bb);

	if (result == 1)
	{
		cout << endl << "player 1 wins detected." << endl;
	}
	elseif(result == 2)
	{
		cout << endl << "player 2 wins detected." << endl;
	}
	else
	{
		cout << endl << "Board full detected : TIE." << endl;
	}
	delete game;
	mcts.kill_tree();
	int s;
	cin >> s;
}
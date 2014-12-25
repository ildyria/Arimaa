#define CONNECT4
//#define TICTACTOE

#include "Mcts.h"
#ifdef TICTACTOE
#include "TicTacToe.h"
#endif
#ifdef CONNECT4
#include "Connect4.h"
#endif
#include <time.h>
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
	cout << "\tBut remember, all code is guilty until proven inocent !" << endl << endl;

	int result = 0, moveok;
	Move move;

	MctsArgs args = MctsArgs();

	list<Move> Listtoprint;
	list<Move>::iterator iter;


	Mcts mcts = Mcts(game,		// game
					Bb,	// board to start with
					args);

	int IA = 2;
	while (result == 0)
	{
		game->diplayBoard(Bb);
		Listtoprint = game->listPossibleMoves(Bb);
		cout << endl << "possible moves : ";
		for (iter = Listtoprint.begin(); iter != Listtoprint.end(); ++iter){
			cout << *iter << " ";
		}
		cout << "or -1 to pass."<< endl;
		mcts.UpdateRoot();

		if (Bb->getPlayer() != IA)
		{
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
			move = mcts.GetBestMove();
			mcts.print_tree(2);
			cout << endl << "chosen move : " << move;
		}
		mcts.movePlayed(move);
		game->play(move, Bb);
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
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}
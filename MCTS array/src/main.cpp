//#include <vld.h>
//#define TEST_API

#include "tools/typedef.h"

#if defined(TEST_API)
	#include "api/Game.h"
	#include "api/Ai.h"
	#include "api/test.h"
	using namespace api;
#else
	#define DISPLAY_TREE
	#include "tools/Memory.h"

	#include "./tools/Count.h"

	#include "./mcts/Mcts.h"

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
	cout << endl << "\t\t    If it compiles then it works ! " << endl;
	cout << "\tBut remember, all code is guilty until proven innocent !" << endl << endl;
#ifdef TEST_API
	test_api();
#else
	Connect4* game = new Connect4();
	BitboardConnect4* Bb = new BitboardConnect4();
	int result = 0;
	int moveok;
	Move move;

	MctsArgs* args = new MctsArgs();

	list<Move> Listtoprint;
	list<Move>::iterator iter;


	Mcts mcts = Mcts(game,		// game
					Bb,	// board to start with
					args);

	Memory::getfreememory();

	int IA = 2;
	while (result == 0)
	{
		mcts.UpdateRoot();
		game->diplayBoard(Bb);

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
				std::string tmp;
				cout << endl << "Your move ?" << endl;
				cin >> tmp;
				move = Move(tmp);
				iter = find(Listtoprint.begin(), Listtoprint.end(), move);
				if (iter != Listtoprint.end())
				{
					moveok = 1;
				}
				elseif(tmp == "exit")
				{
					exit(0);
				}
				elseif(tmp == "-1")
				{
					IA = (IA == 2) ? 1 : 2;
					moveok = 1;
				}
			}
		}

		if (Bb->getPlayer() == IA)
		{
			cout << endl << "AI turn... please wait." << endl;
			move = mcts.GetBestMove();
#ifdef DISPLAY_TREE
			mcts.print_tree(3);
#endif // DISPLAY_TREE
			mcts.get_Number_Leaves();
			cout << endl << Count::I();
			cout << endl << "chosen move : " << move << endl;
		}
		Bb = static_cast<BitboardConnect4*>(mcts.movePlayed(move));

		if (mcts.winning_Strategy() == 42)
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
#endif

	int s;
	cin >> s;
}
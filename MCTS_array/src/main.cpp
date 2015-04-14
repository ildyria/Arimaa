//#define TEST_API

#include "tools/typedef.h"

#if defined(TEST_API)
	#include "api/Game.h"
	#include "api/Ai.h"
	#include "api/test.h"
	using namespace api;
#else
//	#define DISPLAY_TREE
	#include "./tools/Memory.h"

	#include "./tools/ChillPill.h"
	#include "./tools/Count.h"

	#include "./mcts/Mcts.h"

	#if defined(ARIMAA)
		#include "./arimaa_simple/Arimaa.h"
//		#include "./arimaa_simple/BitboardConnect4.h"
	#else
		#include "./connect4/Connect4.h"
		#include "./connect4/BitboardConnect4.h"
	#endif
#endif

#include <thread>
#include "tools/Random.h"
#include "tools/bench.h"
#include "tools/read_args.h"

using namespace mcts;
using std::cout;
using std::cin;
using std::endl;
using std::list;




int main(int argc, char const *argv[])
{
	cout << endl << "\t\t    If it compiles then it works ! " << endl;
	cout << "\tBut remember, all code is guilty until proven innocent !" << endl << endl;

	prog_options options = read_args(argc, argv);

	int think_while_waiting = options.think_while_waiting;


#ifdef TEST_API
	test_api();
#else
#if defined(ARIMAA)
	Arimaa* game = new Arimaa();
	Bitboard* Bb = new Bitboard((NB_PIECE+1)*2,1);
	std::string t = "";
	game->diplay_board(Bb);
	while (t != "exit")
	{
		cin >> t;
		game->convert_move(t);
	}
	exit(1);
#else
	Connect4* game = new Connect4();
	BitboardConnect4* Bb = new BitboardConnect4();
#endif
	int result = 0;
	int moveok;
	u_long more;
	Move move;
	Random::I();

	MctsArgs* args;
	if(options.updated)
	{
		args = new MctsArgs(options);
	}
	else
	{
		args = new MctsArgs();
	}

	if(options.bench)
	{
		Bench bench = Bench(game,Bb,args);
		bench.run();
		exit(1);
	}

	list<Move> Listtoprint;
	list<Move>::iterator iter;


	Mcts mcts = Mcts(game,		// game
					Bb,	// board to start with
					args);

	Memory::get_free_memory();

	int IA = 2;
	while (result == 0)
	{
		mcts.update_root();
		game->diplay_board(Bb);

		if (Bb->get_player() != IA)
		{
			moveok = 0;
			more = 0;
			#pragma omp parallel shared(moveok,think_while_waiting,more)
			{
				if (omp_get_thread_num() == 0)
				{
					Listtoprint = game->list_possible_moves(Bb);
					cout << endl << "possible moves : ";
					for (iter = Listtoprint.begin(); iter != Listtoprint.end(); ++iter){
						cout << *iter << " ";
					}
					cout << "or -1 to pass.";
					while (moveok == 0)
					{
						std::string tmp;
						cout << endl << "Your move ?" << endl;
						cin >> tmp;
						move = Move(tmp);
						iter = find(Listtoprint.begin(), Listtoprint.end(), move);
						if (iter != Listtoprint.end())
						{
							#pragma omp atomic
							moveok += 1;
						}
						elseif(tmp == "exit")
						{
							exit(0);
						}
						elseif(tmp == "-1")
						{
							IA = (IA == 2) ? 1 : 2;
							#pragma omp atomic
							moveok += 1;
						}
					}
				}
				elseif(think_while_waiting)
				{
					while (moveok == 0)
					{
						#pragma omp atomic
						more += 1;
						
						mcts.explore();
					}
				}
			}
			if (more > 0)
			{
				cout << endl << "Thank you for taking your time." << endl;
				cout << "you allowed me to do " << Count::format(more) << " more simulations. <3" << endl;
			}
		}

		if (Bb->get_player() == IA)
		{
			cout << endl << "AI turn... please wait." << endl;
			move = mcts.get_best_move();
#ifdef DISPLAY_TREE
			mcts.print_tree(3);
#endif // DISPLAY_TREE
			mcts.get_number_leaves();
			cout << endl << Count::I();
			cout << endl << "chosen move : " << move << endl;
		}

#if defined(ARIMAA)
		Bb = mcts.move_played(move);
#else
		Bb = static_cast<BitboardConnect4*>(mcts.move_played(move));
#endif

#ifdef DISPLAY_TREE
		mcts.print_tree(3);
#endif // DISPLAY_TREE

		if (mcts.winning_chances() == 42)
		{
			cout << "You don't know it yet, but you lost ! =D" << endl;
		}
		mcts.get_number_leaves();
		cout << endl << Count::I();

		result = game->end(Bb);
	}
	cout << endl;
	game->diplay_board(Bb);

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
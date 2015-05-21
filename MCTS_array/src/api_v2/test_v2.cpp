#include "test_v2.h"

using std::cout;
using std::endl;

namespace api_v2 {
	void test::run(prog_options& options)
	{
		Game* game = new Game();
		Ai* ai = new Ai(options);
		ai->setThinkingTime(2);

		game->displayASCII();

		game->makeMove(4);
		ai->makeMove(4);

		game->displayASCII();

		ai->explore();

		v_stat dump = ai->getMovesStatistics();
		sort(dump.begin(), dump.end(), second);
		u_long move_chosen = (*(dump.begin())).first;

		ai->makeMove(move_chosen);
		game->makeMove(4);

		game->displayASCII();

		game->makeMove(4);
		ai->makeMove(4);

		game->displayASCII();

		game->makeMove(4);
		game->displayASCII();

		game->makeMove(4);
		game->displayASCII();

		cout << game->canMakeMove(4) << endl;
		cout << game->colHeight(4) << endl;

		cout << "#####################" << endl;

		auto serialized_bitboard = game->getState();

		ai->setState(serialized_bitboard);

		ai->explore();

		dump = ai->getMovesStatistics();
		sort(dump.begin(), dump.end(), second);
		move_chosen = (*(dump.begin())).first;
		std::cout << "move - win / full" << std::endl;
		for(auto t = dump.begin(); t != dump.end(); ++t)
		{
			std::cout << (*t).first << " - " << (*t).second.first << "/" << (*t).second.second << std::endl;
		}

		ai->makeMove(move_chosen);
		serialized_bitboard = ai->getState();
		ai->displayASCII();


		game->setState(serialized_bitboard);
		game->displayASCII();

		delete game;

		cout << "test 1 passed" << endl;

		game = new Game();

		serialized_bitboard = game->getState();
		ai->setState(serialized_bitboard);
		ai->displayASCII();
		ai->_ai->print_tree(2);

		ai->makeMove(7);
		ai->explore();

		dump = ai->getMovesStatistics();
		sort(dump.begin(), dump.end(), second);
		move_chosen = (*(dump.begin())).first;

		ai->makeMove(move_chosen);
		serialized_bitboard = ai->getState();
		ai->displayASCII();


		game->setState(serialized_bitboard);
		game->displayASCII();

		game->makeMove(4);
		game->makeMove(3);

		game->makeMove(3);
		game->makeMove(3);

		game->makeMove(1);
		game->makeMove(2);

		game->makeMove(1);
		game->makeMove(2);

		game->makeMove(1);
		game->makeMove(4);

		game->makeMove(2);
		game->makeMove(4);

		game->makeMove(4);
		game->makeMove(4);

		game->makeMove(3);
		game->makeMove(3);

		game->makeMove(2);
		game->makeMove(2);

		game->makeMove(1);

		game->displayASCII();
		cout << game->getWinner() << endl;
		cout << "test 2 passed" << endl;
		auto wl = game->getWinningLine();
		for (auto it = wl.begin(); it != wl.end(); ++it)
		{	
			cout << (*it).first << ";" << (*it).second << endl;
		}
	}
}
#include "test_api.h"

void api::test::run(prog_options& options)
{
	Game* game = new Game();
	Ai* ai = new Ai(2);
	TheGame* g = game->getGame();

	ai->init(game);

	g->diplay_board(game->getBitboard());

	game->makeMove(4);

	g->diplay_board(game->getBitboard());

	cout << game->makeMove(ai->makeMove(true)) << endl;
	g->diplay_board(game->getBitboard());

	game->makeMove(4);
	cout << game->makeMove(ai->makeMove(true)) << endl;
	g->diplay_board(game->getBitboard());

	game->makeMove(4);
	cout << game->makeMove(ai->makeMove(true)) << endl;
	g->diplay_board(game->getBitboard());

	game->makeMove(4);
	g->diplay_board(game->getBitboard());

	cout << game->canMakeMove(4) << endl;
	cout << game->colHeight(4) << endl;

	v_stat dump = ai->getMovesStatistics();
	std::cout << "move - win / full" << std::endl;
	for(auto t = dump.begin(); t != dump.end(); ++t)
	{
		std::cout << (*t).first << " - " << (*t).second.first << "/" << (*t).second.second << std::endl;
	}

	delete game;
	delete ai;
	
	cout << "test 1 passed" << endl;
	game = new Game();
	ai = new Ai(2);
	g = game->getGame();

	ai->init(game);
	g->diplay_board(game->getBitboard());
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

	g->diplay_board(game->getBitboard());
	cout << game->getWinner() << endl;
	cout << "test 2 passed" << endl;
	auto wl = game->getWinningLine();
	for (auto it = wl.begin(); it != wl.end(); ++it)
	{	
		cout << (*it).first << ";" << (*it).second << endl;
	}
}
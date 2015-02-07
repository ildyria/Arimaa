#include "Ai.h"
#include "../tools/Count.h"

#ifdef _WIN64
#define CLOCK 1000
#else
#define CLOCK 10000
#endif

using namespace mcts;

namespace api {
	Ai::Ai(int t) :_param(new MctsArgs(40, t*CLOCK)), _ai(nullptr), _game(nullptr)
	{
	}

	void Ai::setThinkingTime(int t)
	{
		_param->setTimeLimitSimulationPerRoot(t * CLOCK);
	}

	double Ai::getThinkingTime()
	{
		return _param->getTimeLimitSimulationPerRoot() / CLOCK;
	}

	void Ai::init(Game* g)
	{
		if (_ai != nullptr)
		{
			_ai->kill_tree();
		}
		_game = g;
		_ai = new Mcts(_game->getGame(), _game->getBitboard()->clone(), _param);
		// clone of the BitBoard because it will be deleted once the root is destroyed
	}

	int Ai::makeMove(bool player)
	{
		if(player){
			auto m = Move(_game->getLastMove());
			_ai->UpdateRoot();
			_ai->movePlayed(m);
		}
		auto m = _ai->GetBestMove();
		_ai->print_tree(2);
		_ai->movePlayed(m);
		Count::I()->clear();
		return static_cast<int>(m.getMove());
	}

	double Ai::estimateWinChances()
	{
		return _ai->winning_Strategy();
	}
}
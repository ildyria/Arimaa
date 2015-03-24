#include "Ai.h"
#include "../tools/Count.h"

#define CLOCKTOCK 1000

using namespace mcts;

namespace api {
	Ai::Ai(int t) :_param(new MctsArgs(40, t*CLOCKTOCK)), _ai(nullptr), _game(nullptr)
	{
	}

	void Ai::setThinkingTime(int t)
	{
		_param->set_time_limit_simulation_per_root(t * CLOCKTOCK);
	}

	double Ai::getThinkingTime()
	{
		return _param->get_time_limit_simulation_per_root() / CLOCKTOCK;
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
			_ai->update_root();
			_ai->move_played(m);
		}
		auto m = _ai->get_best_move();
		_ai->print_tree(2);
		_ai->move_played(m);
		Count::I()->clear();
		return static_cast<int>(m.get_move());
	}

	double Ai::estimateWinChances()
	{
		return _ai->winning_chances();
	}

	v_stat Ai::getMovesStatistics()
	{
		return _ai->get_moves_statistics();
	}

	void Ai::explore(){
		_ai->get_best_move();
	}

	void Ai::exploreSingle()
	{
		_ai->explore();
	}
}
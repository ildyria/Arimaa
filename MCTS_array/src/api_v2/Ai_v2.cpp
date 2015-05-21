#include "Ai.h"

using namespace mcts;

namespace api {
/*
	mcts::MctsArgs* _param;
	mcts::Mcts* _ai;
	Bitboard* _board;
	TheGame* _game;

*/
	Ai::Ai() : _param(new MctsArgs()), _ai(nullptr), _game(new Connect4()), _board(new BitboardConnect4())
	{
	}

	Ai::Ai(prog_options& options) : _param(new MctsArgs(options)), _ai(nullptr), _game(new Connect4()), _board(new BitboardConnect4())
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


	void Ai::makeMove(u_long move)
	{
		auto m = Move(move);
		_ai->update_root();
		_ai->move_played(m);
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
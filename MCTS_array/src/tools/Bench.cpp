#include "Bench.h"


Bench::Bench(TheGame* game, Bitboard* Bb, mcts::MctsArgs* args) :
	_game(game),
	_param(args),
	_bb(Bb),
	_num_cpu(omp_get_num_procs()),
	_results(std::vector<u_long>(omp_get_num_procs(),0))
{
}

Bench::~Bench()
{
}

u_long Bench::stress(mcts::Mcts* mcts)
{
	u_long i = 0;
	auto start_time = high_resolution_clock::now();
	auto end_time = start_time + milliseconds(_param->get_time_limit_simulation_per_root());
	#pragma omp parallel shared(i,end_time)
	{
		while (high_resolution_clock::now() < end_time && i < _param->get_max_num_simulation_per_root())
		{
			i++;
			mcts->explore();
		}
		#pragma omp barrier
	}
	return i;
}

void Bench::run()
{

	Bitboard* Bb;
	mcts::Mcts* mcts;

	for (u_int i = 0; i < _num_cpu; ++i)
	{
		std::cout << "bench " << (i+1) << "/" << _num_cpu << std::endl;
		omp_set_num_threads(i+1);
		Bb = _bb->clone();
		mcts = new mcts::Mcts(_game, Bb, _param);
		Move move = Move(4);
		mcts->update_root();
		mcts->move_played(move);
		_results[i] = stress(mcts);
		delete mcts;
		delete Bb;
	}

	std::cout << "Results : " << std::endl;
	printf("cores  --- simulations\n");
	for (u_int i = 0; i < _num_cpu; ++i)
	{
		printf("  %2d    |   ", (i+1));
		std::cout << Count::format(_results[i]) << std::endl;
	}
	printf("----------------------\n");
	std::cout << std::endl;
}

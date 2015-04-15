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
	Memento<mcts::Node*> parents = Memento<mcts::Node*>(_param->get_max_depth() + 1);

	#pragma omp parallel shared(i,end_time) firstprivate(parents)
	{
		while (high_resolution_clock::now() < end_time && i < _param->get_max_num_simulation_per_root())
		{
			i++;
			mcts->explore(parents);
		}
		#pragma omp barrier
	}
	return i;
}

void Bench::status_bar(int state, int full, int num ,int total)
{
    int barWidth = 70;
	float progress = static_cast<float>(state)/full;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i <= pos) std::cout << "\u2591";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " \u0025 : ";
	std::cout << num << "/" << total << "\r";
    std::cout.flush();
}


void Bench::run()
{

	Bitboard* Bb;
	mcts::Mcts* mcts;

	for (u_int i = 0; i < _num_cpu; ++i)
	{
		// std::cout << "bench " << (i+1) << "/" << _num_cpu << " : " << std::flush;
		status_bar(4*i+1, _num_cpu*4,(i+1),_num_cpu);
		omp_set_num_threads(i+1);
		Bb = _bb->clone();
		mcts = new mcts::Mcts(_game, Bb, _param);
		Move move = Move(4);
		mcts->update_root();
		mcts->move_played(move);
		status_bar(4*i+2, _num_cpu*4,(i+1),_num_cpu);
		_results[i] = stress(mcts);
		status_bar(4*i+4, _num_cpu*4,(i+1),_num_cpu);
		delete mcts;
		delete Bb;
		status_bar(4*i+4, _num_cpu*4,(i+1),_num_cpu);
	}


	std::cout << "\nResults : " << std::endl;
	printf("cores  --- simulations\n");
	for (u_int i = 0; i < _num_cpu; ++i)
	{
		printf("  %2d    |   ", (i+1));
		std::cout << Count::format(_results[i]) << std::endl;
	}
	printf("----------------------\n");
	std::cout << std::endl;

	// prepare data for graph
	std::vector<double> to_graph = std::vector<double>(0);
	double reference_val = static_cast<double>(_results[0]);
	for (u_int i = 0; i < _num_cpu; ++i)
	{
		to_graph.push_back(_results[i]/reference_val);
	}

	for (int i = 0; i < 81; ++i)
	{
		printf("-");
	}
	printf("\n");

	Graph* g = new Graph(to_graph);
	g->draw();

	delete g;

}

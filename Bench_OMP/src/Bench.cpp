#include "Bench.h"


Bench::Bench() :
	_num_cpu(omp_get_num_procs()),
	_results(std::vector<u_long>(omp_get_num_procs(),0))
{
}

Bench::~Bench()
{
}

u_long Bench::stress()
{
	u_long i = 0;
	auto start_time = high_resolution_clock::now();
	auto end_time = start_time + milliseconds(10000);

	#pragma omp parallel shared(end_time) reduction(+:i)
	{
		u_long j = 0;
		while (high_resolution_clock::now() < end_time)
		{
			j++;
		}
		i+=j;
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
	for (u_int i = 0; i < _num_cpu; ++i)
	{
		status_bar(4*i+1, _num_cpu*4,(i+1),_num_cpu);
		omp_set_num_threads(i+1);
		status_bar(4*i+2, _num_cpu*4,(i+1),_num_cpu);
		_results[i] = stress();
		status_bar(4*i+4, _num_cpu*4,(i+1),_num_cpu);
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





	for (u_int i = 0; i < _num_cpu; ++i)
	{
		printf("  %2d    |   ", (i+1));
		std::cout << to_graph[i] << std::endl;
	}
	printf("----------------------\n");

	for (int i = 0; i < 81; ++i)
	{
		printf("-");
	}
	printf("\n");

	Graph* g = new Graph(to_graph);
	g->draw();

	delete g;

}

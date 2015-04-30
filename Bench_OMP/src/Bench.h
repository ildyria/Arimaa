#pragma once
#include <algorithm>    // std::min_element, std::max_element
#include "Graph.h"
#include "Count.h"
#include <omp.h>
#include <chrono>

using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::high_resolution_clock;

class Bench
{
	u_long _num_cpu;
	std::vector<u_long> _results;

	u_long stress();
	void status_bar(int state, int full, int num ,int total);

public:
	Bench();
	~Bench();
	void run();
};
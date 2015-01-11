#include "Random.h"
#include <iostream>
#include <omp.h>
#include <ctime>

using std::srand;
using std::rand;
using std::cout;
using std::endl;

Random* Random::UniqueInstance = nullptr;
uint64_t Random::_seed = 0;
std::mt19937_64 Random::rng;

Random::Random(uint64_t new_seed)
{
	_seed = new_seed;
	rng.seed(new_seed);
	cout << endl << "Random created with seed : " << _seed;
}

Random::~Random()
{
}

Random* Random::I(unsigned int seed)
{
	if (UniqueInstance != nullptr)
	{
		return UniqueInstance;
	}
	
	if (seed == 0)
	{
		UniqueInstance = new Random(time(nullptr) + omp_get_thread_num());
	}
	else
	{
		UniqueInstance = new Random(seed);
	}
	return UniqueInstance;
}
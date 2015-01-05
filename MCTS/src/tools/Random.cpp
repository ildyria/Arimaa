#include "Random.h"
#include <iostream>
#include <omp.h>
#include <ctime>

using std::srand;
using std::rand;
using std::cout;
using std::endl;

Random* Random::UniqueInstance = nullptr;
unsigned int Random::_seed = 0;

Random::Random()
{
	_seed = time(nullptr) + omp_get_thread_num();;
	srand(_seed);
	cout << endl << "Random created with seed : " << _seed;
}

Random::Random(unsigned int seed)
{
	_seed = seed;
	srand(seed);
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
		UniqueInstance = new Random();
	}
	else
	{
		UniqueInstance = new Random(seed);
	}
	return UniqueInstance;
}
#include "Random.h"
#include <iostream>

using std::srand;
using std::rand;
using std::cout;
using std::endl;

Random* Random::UniqueInstance = nullptr;
unsigned int Random::_seed = 0;

Random::Random()
{
	_seed = time(nullptr);
	srand(_seed);
	cout << "Random created with seed : " << _seed << endl;
}

Random::Random(unsigned int seed)
{
	_seed = seed;
	srand(seed);
	cout << "Random created with seed : " << _seed << endl;
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
#include "Random.h"

Random* Random::UniqueInstance = nullptr;
uint64_t Random::_seed = 0;
std::mt19937_64 Random::rng;

Random::Random(uint64_t new_seed)
{
	_seed = new_seed;
	rng.seed(static_cast<u_long>(new_seed));
}

Random::~Random()
{
}
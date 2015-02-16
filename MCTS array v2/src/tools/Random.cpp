//#define DISPLAY_RNG
#include "Random.h"

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
	rng.seed(static_cast<u_long>(new_seed));
#ifdef DISPLAY_RNG
	cout << endl << "Random created with seed : " << _seed;
#endif // DISPLAY_RNG
}

Random::~Random()
{
}
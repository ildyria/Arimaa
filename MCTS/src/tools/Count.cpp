#include "Count.h"
#include <iostream>

using std::cout;
using std::endl;

Count* Count::UniqueInstance = nullptr;

Count::Count() : _nodes(0), _bitboads(0), _bitboadsC4(0), _nbleaves(0), _nodes_created(0), _nodes_saved(1), _bitboads_saved(1), _bitboadsC4_saved(1), _nbleaves_saved(1), _max_depth(0)
{}

Count::~Count()
{
}

Count* Count::I()
{
	if (UniqueInstance == nullptr)
	{
		UniqueInstance = new Count();
	}
	return UniqueInstance;
}
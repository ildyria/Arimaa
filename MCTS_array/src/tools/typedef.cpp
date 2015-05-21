#include "typedef.h"

//Adds up the 2 statistics
p_stat operator += (p_stat a, p_stat b)
{
	a.first += b.first;
	a.second += b.second;
	return a;
}

//Adds up the statistics that share the same IDs (the vector is considered unordered)
v_stat operator += (v_stat a, v_stat b)
{
	for (auto itA = a.begin(); itA != a.end(); ++itA)
	{
		bool found = false;
		for (auto itB = b.begin(); itB != b.end() && !found; ++itB)
		{
			if (itA->first == itB->first)
			{
				itA->second += itB->second;
				found = true;
			}
			if (!found)
			{
				a.push_back(*itB);
			}
		}
	}
	return a;
}

bool first (n_stat a, n_stat b)
{
	return a.first < b.first;
}

bool second (n_stat a, n_stat b)
{
	return (a.second.first/a.second.second) < (b.second.first/b.second.second);
}

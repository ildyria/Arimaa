#include "typedef.h"

//Adds up the 2 statistics
p_stat& operator += (p_stat& a, p_stat b)
{
	if((a.first == 42 && a.second == 1)|| (b.first == 42 && b.second == 1))
	{
		a.first = 42;
		a.second = 1;
	}
	if((a.first == -1 && a.second == 1)|| (b.first == -1 && b.second == 1))
	{
		a.first = -1;
		a.second = 1;
	}
	else
	{
		a.first += b.first;
		a.second += b.second;
	}
	return a;
}

p_stat operator + (p_stat a, p_stat b)
{
	p_stat p = a;
	p += b;
	return p;
}

//Adds up the statistics that share the same IDs (the vector is considered unordered)
v_stat operator + (v_stat a, v_stat b)
{
	auto itA = a.begin();
	auto itB = b.begin();

	auto itAEnd = a.end();
	auto itBEnd = b.end();

	v_stat res = v_stat(a.size() + b.size(),n_stat(0,p_stat(0,0)));

	auto itres = res.begin();

	int size = 0;
	while(itA != itAEnd || itB != itBEnd)
	{
		if(itA == itAEnd || itA->first > itB->first) // should not crash because of lazy evaluation. /!\ itA->first does not exist here !!!
		{
			*itres = *itB;
			itB++;
		}
		elseif(itB == itBEnd || itA->first < itB->first) // should not crash because of lazy evaluation. /!\ itB->first does not exist here !!!
		{
			*itres = *itA;
			itA++;
		}
		elseif (itA->first == itB->first)
		{
			itres->first = itA->first;
			itres->second = itA->second + itB->second;
			itA ++;
			itB ++;
		}
		else
		{
			printf("SHOULD NOT GO THERE %s : %d .\n",__FILE__,__LINE__);
		}
		size++;
		itres++;
	}
	res.resize(size);
	return res;
	// for ( itA != a.end(); ++itA)
	// {
	// 	bool found = false;
	// 	for (auto itB = b.begin(); itB != b.end() && !found; ++itB)
	// 	{
	// 		if (itA->first == itB->first)
	// 		{
	// 			itA->second += itB->second;
	// 			found = true;
	// 		}
	// 		if (!found)
	// 		{
	// 			a.push_back(*itB);
	// 		}
	// 	}
	// }
	// return a;
}

v_stat& operator += (v_stat& a, v_stat b)
{
	a = a + b;
	return a;
}


std::ostream& operator<< (std::ostream& stream, v_stat &stat)
{
	for (auto it = stat.begin(), end = stat.end(); it != end; ++it)
	{
		stream << it->first << " : " << it->second.first << "/" << it->second.second << std::endl;
	}
	return stream;
}


bool first (n_stat a, n_stat b)
{
	return a.first < b.first;
}

bool second (n_stat a, n_stat b)
{
	return (a.second.first/a.second.second) > (b.second.first/b.second.second);
}

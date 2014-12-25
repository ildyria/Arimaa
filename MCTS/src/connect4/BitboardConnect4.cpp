#include "BitboardConnect4.h"

using std::list;


BitboardConnect4::BitboardConnect4() : Bitboard(7,6,2,1)
{
}


BitboardConnect4::~BitboardConnect4()
{
}

list<int> BitboardConnect4::getEmpty(int n) const
{
	list<int> rtm;
	int i = getSizeX();
	unsigned long long board = (getBoard(0) | getBoard(1)) >> ((getSizeY() - 1) * getSizeX());
	while (board != 0 && i > 0)
	{
		if ((board & 1) == 0)
		{
			rtm.push_back(i);
		}
		i--;
		board >>= 1;
	}

	while (i > 0)
	{
		rtm.push_back(i);
		i--;
	}
	return rtm;
}
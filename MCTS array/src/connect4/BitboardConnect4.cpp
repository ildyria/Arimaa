#include "BitboardConnect4.h"

using std::list;


BitboardConnect4::BitboardConnect4() : Bitboard(2,1)
{
	Count::I()->addBitBoardsC4();
}


BitboardConnect4::~BitboardConnect4()
{
	Count::I()->rmBitBoardsC4();
}

list<int> BitboardConnect4::get_empty(int n) const
{
	list<int> rtm;
	int i = SIZEX;
	unsigned long long board = (get_board(0) | get_board(1)) >> ((SIZEY - 1) * SIZEX);
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
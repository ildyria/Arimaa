#include "Bitboard.h"

using std::vector;
using std::list;
using std::cin;
using std::cout;
using std::endl;
using std::max;

Bitboard::Bitboard() : _boards(vector<numtyp>(3)), _toplay(1), _number(3)
{
	for (int i = 0; i < _number; i++)
	{
		_boards[i] = static_cast<numtyp>(0);
	}
	Count::I()->addBitBoards();
}

Bitboard::Bitboard(int n, int toplay) : _boards(vector<numtyp>(n)), _toplay(toplay), _number(n)
{
	for (int i = 0; i < _number; i++)
	{
		_boards[i] = static_cast<numtyp>(0);
	}
	Count::I()->addBitBoards();
}

Bitboard::~Bitboard()
{
	_boards.clear();
#ifdef DEBUG_BOARD
	cout << "B*" << this << " destroyed." << endl;
#endif //DEBUG_BOARD
	Count::I()->rmBitBoards();
}

list<int> Bitboard::getOccupied(int n)
{
	list<int> rtm;
	int i = 0;
	int max = SIZEX*SIZEY;
	numtyp board = _boards[n];

	while (board != 0 && i < max)
	{
		if ((board & 1) == 1)
		{
			rtm.push_front(i);
		}
		i++;
		board >>= 1;
	}

	return rtm;
}

list<int> Bitboard::getEmpty(int n) const
{
	list<int> rtm;
	int i = 0;
	int max = SIZEX*SIZEY;
	numtyp board = _boards[n];

	while (board != 0 && i < max)
	{
		if ((board & 1) == 0)
		{
			rtm.push_back(i);
		}
		i++;
		board >>= 1;
	}

	while (i < max)
	{
		rtm.push_back(i);
		i++;
	}
	return rtm;
}
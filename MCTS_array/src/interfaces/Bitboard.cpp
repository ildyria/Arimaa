#include "Bitboard.h"

using std::vector;
using std::list;
using std::cin;
using std::cout;
using std::endl;

Bitboard::Bitboard() : _toplay(1), _number(3), _boards(vector<u_long>(3))
{
	for (u_int i = 0; i < _number; i++)
	{
		_boards[i] = static_cast<u_long>(0);
	}
	Count::I()->addBitBoards();
}

Bitboard::Bitboard(int n, int toplay) : _toplay(toplay), _number(n), _boards(vector<u_long>(n))
{
	for (u_int i = 0; i < _number; i++)
	{
		_boards[i] = static_cast<u_long>(0);
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

list<int> Bitboard::get_occupied(int n)
{
	list<int> rtm;
	int i = 0;
	int max = SIZEX*SIZEY;
	u_long board = _boards[n];

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

list<int> Bitboard::get_empty(int n) const
{
	list<int> rtm;
	int i = 0;
	int max = SIZEX*SIZEY;
	u_long board = _boards[n];

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

std::vector<u_long> Bitboard::serialize()
{
	std::vector<u_long> res = std::vector<u_long>(_number + 1);
	res[_number] = 	static_cast<u_long>(_toplay);
	for(u_int i = 0; i < _number; ++i)
	{
		res[i] = _boards[i];
	}
	return res;
}

void Bitboard::import(std::vector<u_long>& data)
{
	_toplay = static_cast<u_int>(data[_number]);
	for(u_int i = 0; i < _number; ++i)
	{
		_boards[i] = data[i];
	}
}


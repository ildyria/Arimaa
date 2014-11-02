#include "Bitboard.h"
#define elseif else if

Bitboard::Bitboard()
{
	_boards = *(new vector<unsigned long> (3));
	_number = 3;
	_size = 3;
	_toplay = 1;
	for (int i = 0; i < _number; i++)
	{
		_boards[i] = static_cast<unsigned long>(0);
	}
#ifdef DEBUG_BOARD
	cout << "B*" << this << " (" << _number << " & " << _size << "x" << _size << ") created." << endl;
#endif //DEBUG_BOARD
}

Bitboard::Bitboard(int size, int n, int toplay)
{
	_boards = *(new vector<unsigned long> (n));
	_number = n;
	_size = size;
	_toplay = toplay;
	for (int i = 0; i < _number; i++)
	{
		_boards[i] = static_cast<unsigned long>(0);
	}
#ifdef DEBUG_BOARD
	cout << "B*" << this << " (" << _number << " & " << _size << "x" << _size << ") created." << endl;
#endif //DEBUG_BOARD
}

Bitboard::~Bitboard()
{
#ifdef DEBUG_BOARD
	cout << "B*" << this << " destroyed." << endl;
#endif //DEBUG_BOARD
}

int Bitboard::getBit(int n, int x, int y) const
{
	int index = x + y*_size;
	return int((_boards[n]) >> index) & 1;
}

int Bitboard::getBit(int n, int pos) const
{
	return int((_boards[n]) >> pos) & 1;
}

void Bitboard::setBit(int n, int x, int y)
{
	int index = x + y*_size;
	_boards[n] |= (unsigned long(1) << index);
}

void Bitboard::setBit(int n, int pos)
{
	_boards[n] |= (unsigned long(1) << pos);
}

void Bitboard::clearBit(int n, int x, int y)
{
	int index = x + y*_size;
	_boards[n] &= ~(unsigned long(1) << index);
}

void Bitboard::clearBit(int n, int pos)
{
	_boards[n] &= ~(unsigned long(1) << pos);
}

list<int> Bitboard::getOccupied(int n)
{
	list<int> rtm;
	int i = 0;
	int max = _size*_size;
	unsigned long board = _boards[n];

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
	int max = _size*_size;
	unsigned long board = _boards[n];

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
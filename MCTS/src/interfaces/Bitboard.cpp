#include <iostream>     // std::cout
#include <algorithm>    // std::for_each
#include "Count.h"
#include "Bitboard.h"
#define elseif else if

using std::vector;
using std::list;
using std::cin;
using std::cout;
using std::endl;
using std::max;

Bitboard::Bitboard() : _boards(vector<unsigned long long>(3)), _toplay(1), _number(3), _sizeX(3), _sizeY(3)
{
	for (int i = 0; i < _number; i++)
	{
		_boards[i] = static_cast<unsigned long long>(0);
	}
#ifdef DEBUG_BOARD
	cout << "B*" << this << " (" << _number << " & " << _size << "x" << _size << ") created." << endl;
#endif //DEBUG_BOARD
	Count::I()->addBitBoards();
}

Bitboard::Bitboard(int sizeX, int sizeY, int n, int toplay) : _boards(vector<unsigned long long>(n)), _toplay(toplay), _number(n), _sizeX(sizeX), _sizeY(sizeY)
{
	for (int i = 0; i < _number; i++)
	{
		_boards[i] = static_cast<unsigned long long>(0);
	}
#ifdef DEBUG_BOARD
	cout << "B*" << this << " (" << _number << " & " << _sizeX << "x" << _sizeY << ") created." << endl;
#endif //DEBUG_BOARD
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

int Bitboard::getBit(int n, int x, int y) const
{
	int index = x + y*_sizeX;
	return int((_boards[n]) >> index) & 1;
}

int Bitboard::getBit(int n, int pos) const
{
	return int((_boards[n]) >> pos) & 1;
}

void Bitboard::setBit(int n, int x, int y)
{
	int index = x + y*_sizeX;
	_boards[n] |= (unsigned long long(1) << index);
}

void Bitboard::setBit(int n, int pos)
{
	_boards[n] |= (unsigned long long(1) << pos);
}

void Bitboard::clearBit(int n, int x, int y)
{
	int index = x + y*_sizeX;
	_boards[n] &= ~(unsigned long long(1) << index);
}

void Bitboard::clearBit(int n, int pos)
{
	_boards[n] &= ~(unsigned long long(1) << pos);
}

list<int> Bitboard::getOccupied(int n)
{
	list<int> rtm;
	int i = 0;
	int max = _sizeX*_sizeY;
	unsigned long long board = _boards[n];

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
	int max = _sizeX*_sizeY;
	unsigned long long board = _boards[n];

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
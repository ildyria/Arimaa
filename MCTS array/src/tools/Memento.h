#pragma once
#include "typedef.h"
#include <vector>

template <class N> class Memento
{
	std::vector<N> _states;
	short _readNext;
	short _writeNext;

public:
	explicit Memento(short n) : _states(std::vector<N>(n)), _readNext(0), _writeNext(0) {};
	~Memento() {};

	inline void rewind()
	{
		_readNext = _writeNext - 1;
	};

	inline void reset()
	{
		_writeNext = 0;
	};

	inline void push(const N& state)
	{
		_states[_writeNext] = state;
		_writeNext++;
	}

	inline N get()
	{
		if (_readNext == -1) return nullptr;
		auto data = _states[_readNext];
		_readNext--;
		return data;
	}
};

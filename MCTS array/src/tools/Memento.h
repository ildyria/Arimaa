#pragma once
#include "typedef.h"
#include <vector>

/**
 * \brief Memento template class
 * \details allow us to remember the previous states/nodes visited in the algorithm
 * 	std::vector<N> _states = states memorized;
 *	short _readNext = pointer to the next reading state, -1 if nothing to read;
 *	short _writeNext = pointer to the next writting state;
 *	WARNING THERE IS NO OVERFLOW CHECK, IF YOU GO OVER THE LIMIT, IT WILL CRASH
 *	Works best with a pointer type
 */
template <class N> class Memento
{
	std::vector<N> _states;
	short _readNext;
	short _writeNext;

public:

	/**
	 * \fn Memento(short n)
	 * \details constructor
	 * \param n number of states you want to save
	 */
	explicit Memento(short n) : _states(std::vector<N>(n)), _readNext(0), _writeNext(0) {};

	/**
	 * \fn ~Memento()
	 * \details destructor
	 */
	~Memento() {};


	/**
	 * \fn rewind()
	 * \details reset the value of readNext to the bottom of the stack
	 */
	inline void rewind()
	{
		_readNext = _writeNext - 1;
	};

	/**
	 * \fn reset()
	 * \details reset the value of writeNext to the head of the stack
	 */
	inline void reset()
	{
		_writeNext = 0;
	};

	/**
	 * \fn push(const N& state)
	 * \details push a value in the stack and update writeNext
	 *
	 * \param state to be memorized
	 */
	inline void push(const N& state)
	{
		_states[_writeNext] = state;
		_writeNext++;
	}

	/**
	 * \fn get()
	 * \details get a value of the stack and update readNext
	 *
	 * \return nullptr if nothing else to return
	 */
	inline N get()
	{
		if (_readNext == -1) return nullptr;
		auto data = _states[_readNext];
		_readNext--;
		return data;
	}
};

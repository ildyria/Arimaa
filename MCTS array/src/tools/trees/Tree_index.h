#pragma once
#include "../typedef.h"
#include <omp.h>
#include <vector>

template <class N> class Tree_index
{
	N*** _next;					// next empty space (Yup, not kidding a pointer to a pointer to a pointer of N ! And no, i'm not crazy ! =D)
	std::vector<N*> _address;	// address book
	std::vector<N**> _empty;	// free spaces in the book
	omp_lock_t _lockAddrBook;

public:

	/**
	 * \fn Memento(short n)
	 * \details constructor
	 * \param n number of states you want to save
	 */
	explicit Tree_index(u_long n = 1) : _next(nullptr), _address(std::vector<N*>(n,nullptr)), _empty(std::vector<N**>(n,nullptr))
	{
		init();
		omp_init_lock(&_lockAddrBook);
	};

	/**
	 * \fn ~Tree_index()
	 * \details destructor
	 */
	~Tree_index() {};

	inline void init()
	{
		for (u_long i = 0, j = 0; i < _address.size(); i++)
		{
			if (_address[i] == nullptr)
			{
				_empty[j] = &_address[i];
				j++;
			}
		}
		_next = &_empty[0];
	}


	inline N**& get(N**& buff)
	{
		omp_set_lock(&_lockAddrBook);
		buff = *(_next++);
		omp_unset_lock(&_lockAddrBook);
		return buff; // return the next free space in the _address book
	}

};

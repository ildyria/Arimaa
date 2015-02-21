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
		u_long j = 0;
		// let's speed things up a bit !
		// or not... 10 times slower ?? DAFUCK
/*
		#pragma omp parallel shared(j)
		{
			int id, Nthread;
			u_long i, istart, iend;
			id = omp_get_thread_num();
			Nthread = omp_get_num_threads();
			
			istart = id*static_cast<u_long>(_address.size()) / Nthread;
			iend = (id + 1)*static_cast<u_long>(_address.size()) / Nthread;
			if (id == Nthread - 1) iend = static_cast<u_long>(_address.size());
			for (i = istart; i < iend; i++)
			{
				_empty[i] = nullptr;
			}
			
			#pragma omp barrier
			i = id;
			iend = static_cast<u_long>(_address.size());
			while (i < iend)
			{
				if (_address[i] == nullptr)
				{
					#pragma omp critical
					{
						_empty[j] = &_address[i];
						j++;
					}
				}
				i += Nthread;
			}
		}
*/
		for (u_long i = 0; i < _address.size(); i++)
		{
			_empty[i] = nullptr;
			if (_address[i] == nullptr)
			{
				_empty[j] = &_address[i];
				j++;
			}
		}
		
		_next = &_empty[0];
	}


	inline N** get()
	{
		N** buff;
		omp_set_lock(&_lockAddrBook);
		buff = *_next;
		++_next;
		omp_unset_lock(&_lockAddrBook);
		return buff; // return the next free space in the _address book
	}

	inline u_long count()
	{
		u_long counter = 0;
		for (u_long i = 0; i < _address.size(); i++)
		{
			if (_address[i] != nullptr)
			{
				++counter;
			}
		}
		return counter;
	}

	
	inline N*** get_next() { return _next; };	// address book
	inline std::vector<N*>* get_address() { return &_address; };	// address book
	inline std::vector<N**>* get_empty()
	{
		return &_empty;
	};	// free spaces in the book

};

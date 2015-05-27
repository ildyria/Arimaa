/**
 * \file Node.h
 * \brief Define class Node
 * \author Benoit Viguier
 * \version 1.0
 * \date November 2nd 2014
 *
 */
#pragma once
#include <list>
#include "Count.h"
#include "Memory.h"

#define TN template<class N>
#define FNN FreeObjects<N>::

template<class N> class FreeObjects
{
	static std::list<N*> _freeNodes;
	static FreeObjects<N>* UniqueInstance;

	explicit FreeObjects();

public:
	~FreeObjects()
	{
		for (auto iter = _freeNodes.begin(); iter != _freeNodes.end(); ++iter)
		{
			delete *iter;
		}
	};

	static FreeObjects<N>* I();

	void set(int n = 2000000);

	N* getNode();

	void storeNode(N* node);
};


TN FreeObjects<N>* FNN UniqueInstance = nullptr;
TN std::list<N*> FNN _freeNodes;

TN FNN FreeObjects(){};

TN void FNN set(int n)
{

	// size_t m = (Memory::get_free_memory() / (sizeof(N)+ sizeof(Bitboard(7, 6, 2, 1))));
	// std::cout << (sizeof(N)+sizeof(Bitboard(7, 6, 2, 1))) << " size of Node ?? " << std::endl;
//	std::cout << m << " nodes to be created" <<  std::endl;
//	m = (n != 1000) ? m : n;
	for (size_t i = 0; i < n; i++)
	{
		_freeNodes.push_front(new N());
	}
};

TN FreeObjects<N>* FNN I()
{
	if (UniqueInstance == nullptr)
	{
		UniqueInstance = new FreeObjects<N>();
	}
	return UniqueInstance;
}

TN N* FNN getNode()
{
	if (_freeNodes.empty())
	{
//		std::cout << std::endl << "we are lacking of nodes !! Generating some more (1000)...";
		set(10000);
		Count::I()->createNode(1000);
	}
	N* node = _freeNodes.front();
	_freeNodes.pop_front();
	return node;
};

TN void FNN storeNode(N* node)
{
	_freeNodes.push_front(node);
};

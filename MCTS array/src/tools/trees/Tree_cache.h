#pragma once
#include "../typedef.h"
#include <vector>
#include <iostream>

template<class N> class Tree
{
public:
	static void execute(N* iter, std::vector<N>& _tree, std::vector<N>& _buff, N*& _next)
	{
		copyTree(iter, _buff);
		cleanTree(_tree);
		copyTree(&_buff[0], _tree);
		cleanTree(_buff);
		findNext(_tree, _next);
	}

	static void cleanTree(std::vector<N> &T)
	{
		N* ptr = &T[1];
		N* lstptr = &T[(T.size() - 1)];
		while (ptr != lstptr && ptr->hasParent())// && ptr->getChildren().second != static_cast<u_int>(-1))
		{
			ptr->unset();
			++ptr;
		}
		std::cout << "clean tree : " << (ptr - &T[0]) << std::endl;
	}

	static void copyTree(N* NewRoot, std::vector<N> &Tdest)
	{
		auto next = &Tdest[0];					// place ptrDest at the begining of the destination array
		auto ptrDest = &Tdest[0];				// place ptrDest at the begining of the destination array
		N* ptTemp;							// pointer to loop on the childrens of each ptr

		*ptrDest = *NewRoot;					// copy root
		ptrDest->releaseLock();
		++next;									// next free space in Tdest

		while (ptrDest->hasParent() || ptrDest == &Tdest[0]) // no parents OR root
		{
			auto ListOfNodes = ptrDest->getChildren();
			ptTemp = ListOfNodes.first;
			if (ptTemp != nullptr)				// if children add them to _buff
			{
				ptrDest->setChildrens(next, ListOfNodes.second); // update the node of the parents
				for (u_int i = 0; i < ListOfNodes.second; i++)
				{
					*next = *ptTemp;			// recopy of chidlren
					next->releaseLock();		// makes sure that all nodes are unlocked !
					++ptTemp;
					++next;
				}
			}
			++ptrDest;
		}
	}

	static void findNext(std::vector<N>& T, N*& n)
	{
		n = &T[1];
		while (n->hasParent())
		{
			++n;
		}
		std::cout << "next: " << (n - &T[0]) << std::endl;
	}
};

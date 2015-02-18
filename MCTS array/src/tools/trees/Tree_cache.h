#pragma once
#include "../typedef.h"
#include <vector>
#include <iostream>
#include "../Timer.h"

template<class N> class Tree
{
public:
	static void execute(N* iter, std::vector<N>& _tree, std::vector<N>& _buff, N*& _next)
	{
		Timer* t = new Timer();
		t->start();

		copyTree(iter, _buff);
		cleanTree(_tree);
		copyTree(&_buff[0], _tree);
		cleanTree(_buff);
		findNext(_tree, _next);

		t->stop();
		std::cout << "recycling duration : " << duration_cast<milliseconds>(t->result()).count() << "ms." << std::endl;
		delete t;
	}

	static void cleanTree(std::vector<N> &T)
	{
		N* ptr = &T[1];
		N* lstptr = &T[(T.size() - 1)];
		while (ptr != lstptr && ptr->hasParent())
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
		N* ptTemp;								// pointer to loop on the childrens of each ptr

		*ptrDest = *NewRoot;					// copy root
		ptrDest->releaseLock();
		ptrDest->unlockTerminal();
		++next;									// next free space in Tdest

		std::pair<N*, u_int> ListOfNodes;
		// could be improved : 2 calls of terminals...
		while (ptrDest->hasParent() || ptrDest == &Tdest[0]) // no parents OR root
		{
			if (ptrDest->getTerminal() == 0)	// has children
			{
				ListOfNodes = ptrDest->getChildren();
				ptTemp = ListOfNodes.first;
				ptrDest->setChildrens(next, ListOfNodes.second); // update the node of the parents
				for (u_int i = 0; i < ListOfNodes.second; i++)
				{
					*next = *ptTemp;			// recopy of chidlren
					next->releaseLock();		// makes sure that all nodes are unlocked !
					next->unlockTerminal();
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

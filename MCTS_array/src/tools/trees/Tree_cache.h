#pragma once
#include "../typedef.h"
#include <vector>
#include <iostream>
#include "../Timer.h"
#include "../Count.h"

template<class N> class Tree
{
	static void copy_tree(N* NewRoot, std::vector<N> &Tdest)
	{
		auto next = &Tdest[0];					// place ptrDest at the begining of the destination array
		auto ptrDest = &Tdest[0];				// place ptrDest at the begining of the destination array
		N* ptTemp;								// pointer to loop on the childrens of each ptr

		*ptrDest = *NewRoot;					// copy root
		ptrDest->release_lock();
		ptrDest->unlock_terminal();
		++next;									// next free space in Tdest

		std::pair<N*, u_int> ListOfNodes;
		// could be improved : 2 calls of terminals...
		while (ptrDest->has_parent() || ptrDest == &Tdest[0]) // no parents OR root
		{
			if (ptrDest->get_terminal() == 0)	// has children
			{
				ListOfNodes = ptrDest->get_children();
				ptTemp = ListOfNodes.first;
				ptrDest->set_children(next, ListOfNodes.second); // update the node of the parents
				for (u_int i = 0; i < ListOfNodes.second; i++)
				{
					*next = *ptTemp;			// recopy of chidlren
					next->release_lock();		// makes sure that all nodes are unlocked !
					next->unlock_terminal();
					++ptTemp;
					++next;
				}
			}
			++ptrDest;
		}
	}

public:
	static void execute(N* iter, std::vector<N>& _tree, std::vector<N>& _buff, N*& _next)
	{
		Timer t = Timer();
		t.start();
		copy_tree(iter, _buff);
		clean_tree(_tree);
		copy_tree(&_buff[0], _tree);
		clean_tree(_buff);
		find_next(_tree, _next);

		t.stop();
#ifdef TREE_CLEAN_DISPLAY
		std::cout << "recycling duration : " << Count::format(duration_cast<milliseconds>(t.result()).count()) << " ms." << std::endl;
#endif // TREE_CACHE_H
	}

	static void clean_tree(std::vector<N> &T)
	{
		N* ptr = &T[1];
		N* lstptr = &T[(T.size() - 1)];
		while (ptr != lstptr && ptr->has_parent())
		{
			ptr->unset();
			++ptr;
		}

#ifdef TREE_CLEAN_DISPLAY
		std::cout << "clean tree : " << Count::format(ptr - &T[0]) << std::endl;
#endif // TREE_CACHE_H
	}



	static void find_next(std::vector<N>& T, N*& n)
	{
		n = &T[1];
		while (n->has_parent())
		{
			++n;
		}
#ifdef TREE_CLEAN_DISPLAY
		std::cout << "next: " << Count::format(n - &T[0]) << std::endl;
#endif // TREE_CACHE_H
	}
};

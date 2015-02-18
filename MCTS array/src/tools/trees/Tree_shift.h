#pragma once
#include "../typedef.h"
#include <vector>
#include <iostream>
#include "../Timer.h"
//#include "Tree_dump.h"
#include "Tree_index.h"

template<class N> class Tree
{
public:
	static void execute(N* iter, std::vector<N>& _tree, Tree_index<N>& _index, N*& _next)
	{
		Timer* t = new Timer();
		t->start();

//		Tree_dump<N>::out(_tree,"before.txt");
		markTrash(iter, _tree);
		std::cout << "trash marked" << std::endl;
//		Tree_dump<N>::out(_tree,"marked.txt");
		compactTree(_tree);
//		Tree_dump<N>::out(_tree,"compacted.txt");
		std::cout << "compacted" << std::endl;
		resetNodes(_tree);
		_index.init();
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

	static void findNext(std::vector<N>& T, N*& n)
	{
		n = &T[1];
		while (n->hasParent())
		{
			++n;
		}
		std::cout << "next: " << (n - &T[0]) << std::endl;
	}

	// mark all nodes to be removed as not attributed (no parents)
	static void markTrash(N* NewRoot, std::vector<N> &T)
	{
		u_long numberOfTrashes = 0;
		auto ListOfNodes = T[0].getChildren();
		N* ptTemp = ListOfNodes.first;
		for (u_int i = 0; i < ListOfNodes.second; ++i)
		{
			if (ptTemp != NewRoot)
			{
				ptTemp->removeFromIndex(); // remove them from index
				numberOfTrashes++;
			}
			++ptTemp;
		}

		N* ptr = &T[1];
		for (int i = 1; i < T.size(); ++i)
		{
			if (ptr->getAddress() == nullptr) // if doesn't have address, then children has to be cleared too !
			{
				if (ptr->getTerminal() == 0)
				{
					ListOfNodes = ptr->getChildren();
					ptTemp = ListOfNodes.first;
					for (u_int i = 0; i < ListOfNodes.second; ++i)
					{
						ptTemp->removeFromIndex();
						numberOfTrashes++;
						++ptTemp;
					}
				}
			}
			++ptr;
		}
		T[0] = *NewRoot;
		T[0].setNewAddress(&T[0]);
		NewRoot->unset();
		std::cout << "number of trashes : " << numberOfTrashes << std::endl;
	}

	static void compactTree(std::vector<N> &T)
	{
		N* empty = findNextHole(&T[1],T);
		N* child = nullptr;
		if (empty != nullptr)
		{
			child = findNextSegment(empty, T);
		}
		while (empty != nullptr && child != nullptr)
		{
			empty = copySubTree(child, empty, &T[T.size() - 1]);
			if (empty != nullptr)
			{
				child = findNextSegment(empty, T);
			}
		}
	}

	static N* findNextHole(N* startwith, std::vector<N>& _tree)
	{
		N* start = startwith;
		N* end = &_tree[_tree.size() - 1];
		while (start != end && start->getAddress() != nullptr)
		{
			++start;
		}

		if (start == end) return nullptr;
		
		return start;
	}

	static N* findNextSegment(N* startwith, std::vector<N>& _tree)
	{
		N* start = startwith;
		N* end = &_tree[_tree.size() - 1];
		while (start != end && start->getAddress() == nullptr)
		{
			++start;
		}

		if (start == end) return nullptr;

		return start;
	}

	// copy a portion into another and mark the moved as "available" : no parents, but do not remove them from index !!! else you would lose them.
	static N* copySubTree(N* from_p, N* to_p, N* end)
	{
		N* node_from = from_p;
		N* node_to = to_p;
		while (node_from->getAddress() != nullptr && node_from != end)
		{
			*node_to = *node_from;
			node_to->setNewAddress(node_to);
			node_from->cleanAddress();
			++node_to;
			++node_from;
		}

		if (node_from == end && node_from->getAddress() != nullptr)
		{
			*node_to = *node_from;
			node_to->setNewAddress(node_to);
			node_from->cleanAddress();
			return nullptr;
		}

		return node_to;
	}

	static void resetNodes(std::vector<N>& _tree)
	{
		N* ptr = &_tree[1];
		for (int i = 1; i < _tree.size(); ++i)
		{
			ptr->releaseLock(); // just to be sure
			ptr->unlockTerminal();
			if (ptr->getAddress() == nullptr)
			{
				ptr->unset();
			}
			++ptr;
		}
	}
};

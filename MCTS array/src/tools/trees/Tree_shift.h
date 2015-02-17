#pragma once
#include "../typedef.h"
#include <vector>
#include <iostream>

template<class N> class Tree
{
public:
	static void execute(N* iter, std::vector<N>& _tree, N*& _next)
	{
		markTrash(iter, _tree);
		compactTree(_tree);
		std::cout << "compacted" << std::endl;
		resetNodes(_tree);
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
				ptTemp->clearParent(); // define all first children but the chosen one as useless !
				numberOfTrashes++;
			}
			++ptTemp;
		}

		N* ptr = &T[1];
		for (int i = 1; i < T.size(); ++i)
		{
			if (!ptr->hasParent()) // if doesn't have parent, then children has to be cleared too !
			{
				ListOfNodes = ptr->getChildren();
				ptTemp = ListOfNodes.first;
				if (ptTemp != nullptr)
				{
					for (u_int i = 0; i < ListOfNodes.second; ++i)
					{
						ptTemp->clearParent();
						++ptTemp;
						numberOfTrashes++;
					}

				}
			}
			++ptr;
		}
		T[0] = *NewRoot;
		updateFirstChild(&T[0],ptr);
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
		u_int n;
		while (empty != nullptr && child != nullptr)
		{
			n = UpdateParent(child, empty);
			if (n != 0)
			{
				copySubTree(child, n, empty);
			}
			else
			{
				std::cout << "Houston, we lost contact !" << std::endl;
				std::cout << "distance to earth    : " <<  empty - &T[0] << std::endl;
				std::cout << "distance to the moon : " <<  child - &T[0] << std::endl;
				std::cout << "distance to sun      : " <<  child->getParent() - &T[0] << std::endl;
				exit(2);
			}
			empty = findNextHole((empty + n), T);
			if (empty != nullptr)
			{
				child = findNextSegment(child, T);
			}
		}
	}

	static N* findNextHole(N* startwith, std::vector<N>& _tree)
	{
		N* start = startwith;
		N* end = &_tree[_tree.size() - 1];
		while (start != end && start->hasParent())
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
		while (start != end && !start->hasParent())
		{
			++start;
		}

		if (start == end) return nullptr;

		return start;
	}

	static u_int UpdateParent(N* child, N* newadress)
	{
		N* parent = child->getParent();
		if (parent == nullptr)
			{
				std::cout << "Houston, we are losing contact !" << std::endl;
				return 0;
			}
		parent->updateFirstChild(newadress);
		return parent->getChildren().second;
	}

	// copy a portion into another and mark the moved as "available" : no parents
	static void copySubTree(N* from_p , u_int& from_n, N* to_p)
	{
		N* child_buff;
		N* node_from = from_p;
		N* node_to = to_p;
		N* papa = from_p->getParent();
		for (u_int i = 0; i < from_n; ++i)
		{
			*node_to = *node_from;
			node_to->setParent(papa);
			updateFirstChild(node_to,child_buff);
			node_from->clearParent();
			++node_to;
			++node_from;
		}
	}

	static void resetNodes(std::vector<N>& _tree)
	{
		N* ptr = &_tree[1];
		for (int i = 1; i < _tree.size(); ++i)
		{
			ptr->releaseLock(); // just to be sure
			if (!ptr->hasParent()) 
			{
				ptr->unset();
			}
		}
	}

	static inline void updateFirstChild(N* node, N*& child_buff)
	{
		child_buff = node->getChildren().first;
		if(child_buff != nullptr) child_buff->setParent(node);
	}
};

#pragma once
#include "../typedef.h"
#include <vector>
#include "../Timer.h"
#include "Tree_dump.h"
#include "Tree_index.h"
#include "../Count.h"
#include <iostream>
#include "Tree_integrity_check.h"

template<class N> class Tree
{
public:
	static void execute(N* iter, std::vector<N>& _tree, Tree_index<N>& _index, N*& _next)
	{
		Timer t = Timer();
		u_long full = _tree[0].count();
		u_long keep = iter->count();
		std::cout << "count full ...    " << Count::format(full) << std::endl;
		std::cout << "count index ...   " << Count::format(_index.count()) << std::endl;
		std::cout << "count to keep ... " << Count::format(keep) << std::endl;
		std::cout << "count to kill ... " << Count::format(full - keep) << std::endl;

		t.start();

		Tree_dump<N>::out(_tree,"before.txt");
		Tree_integrity_check<N>::execute(_tree, _index);
		markTrash(iter, _tree);
		std::cout << "trash marked" << std::endl;
		Tree_dump<N>::out(_tree,"marked.txt");
		compactTree(_tree);
		Tree_dump<N>::out(_tree,"compacted.txt");
		std::cout << "compacted" << std::endl;
		_index.init();
		Tree_integrity_check<N>::execute(_tree, _index);
		findNext(_tree, _next);

		u_long left = _tree[0].count();
		std::cout << "count left ...  " << Count::format(left) << std::endl;
		std::cout << "count index ... " << Count::format(_index.count()) << std::endl;

		t.stop();
		std::cout << "recycling duration : " << Count::format(duration_cast<milliseconds>(t.result()).count()) << " ms." << std::endl;
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
		std::cout << "clean tree : " << Count::format(ptr - &T[0]) << std::endl;
	}

	static void findNext(std::vector<N>& T, N*& n)
	{
		n = &T[1];
		while (n->getAddress() != nullptr)
		{
			++n;
		}
		auto t = n - &T[0];
		std::cout << std::endl << "next: " << Count::format(t) << std::endl;
	}

	// mark all nodes to be removed as not attributed (no parents)
	static void markTrash(N* NewRoot, std::vector<N> &T)
	{
		T[0].removeFromIndex();

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
		for (u_long i = 1; i < T.size(); ++i)
		{
			if (ptr->getAddress() == nullptr) // if doesn't have address, then children has to be cleared too !
			{
				if (ptr->getTerminal() == 0)
				{
					ListOfNodes = ptr->getChildren();
					ptTemp = ListOfNodes.first;
					for (u_int j = 0; j < ListOfNodes.second; ++j)
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
		NewRoot->cleanAddress();
		numberOfTrashes++;
		std::cout << std::endl << "number of trashes : " << Count::format(numberOfTrashes) << std::endl;
	}

	static void compactTree(std::vector<N> &T)
	{
		u_long i;
		N* to = &T[1];
		N* from = &T[1];
		N* end = &T[T.size()-1];
		N** address_to;
		N** address_from;
		
		// init from
		address_from = from->getAddress();
		while (address_from == nullptr && from < end)
		{
			++from;
			address_from = from->getAddress();
		}
		bool copied = (from == end);
		--from;


		for (i = 1; i < T.size(); ++i)
		{
			address_to = to->getAddress();
			if (address_to == nullptr)
			{
				if (!copied)
				{
					++from;
					if (from < to) from = to;
					address_from = from->getAddress();
					while (address_from == nullptr && from < end)
					{
						++from;
						address_from = from->getAddress();
					}
					if (address_from != nullptr)
					{
						*to = *from;
						to->setNewAddress(to);
						from->cleanAddress();
					}
					copied = (from == end);
				}
				else
				{
					to->unset();
				}
			}
			to->releaseLock(); // just to be sure
			to->unlockTerminal();
			++to;
		}
	}
};

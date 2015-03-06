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
	static u_long compacting;

	// mark all nodes to be removed as not attributed (no parents)
	static void mark_trash(N* NewRoot, std::vector<N> &T)
	{
		T[0].remove_from_index();

		u_long numberOfTrashes = 0;
		auto ListOfNodes = T[0].get_children();
		N* ptTemp = ListOfNodes.first;
		for (u_int i = 0; i < ListOfNodes.second; ++i)
		{
			if (ptTemp != NewRoot)
			{
				ptTemp->remove_from_index(); // remove them from index
				numberOfTrashes++;
			}
			++ptTemp;
		}

		N* ptr = &T[1];
		for (u_long i = 1; i < T.size(); ++i)
		{
			if (ptr->getAddress() == nullptr) // if doesn't have address, then children has to be cleared too !
			{
				if (ptr->get_terminal() == 0)
				{
					ListOfNodes = ptr->get_children();
					ptTemp = ListOfNodes.first;
					for (u_int j = 0; j < ListOfNodes.second; ++j)
					{
						ptTemp->remove_from_index();
						numberOfTrashes++;
						++ptTemp;
					}
				}
			}
#ifdef PRUNE_NUMBER
			else
			{
				if (ptr->get_terminal() == 0 && ptr->get_visits() < PRUNE_NUMBER)
				{
					ListOfNodes = ptr->get_children();
					ptTemp = ListOfNodes.first;
					for (u_int j = 0; j < ListOfNodes.second; ++j)
					{
						ptTemp->remove_from_index();
						numberOfTrashes++;
						++ptTemp;
					}
					ptr->set_terminal(32);
				}
			}
#endif
			++ptr;
		}
		T[0] = *NewRoot;
		T[0].set_new_address(&T[0]);
		NewRoot->clean_address();
		numberOfTrashes++;
		std::cout << std::endl << "number of trashes : " << Count::format(numberOfTrashes) << std::endl;
	}

	static void compact_tree(std::vector<N> &T)
	{
		N* to = &T[1];
		N* from = &T[2];
		N* end = &T[T.size() - 1];
		N** address_to;
		N** address_from;

		// init from
		address_from = from->getAddress();
		while (address_from == nullptr && from < end)
		{
			++from;
			address_from = from->getAddress();
		}
		bool copied = ((from == end) && (address_from == nullptr));
		--from;

		for (compacting = 1; compacting < T.size(); ++compacting)
		{
			address_to = to->getAddress();
			if (address_to == nullptr)
			{
				if (!copied)
				{
					++from;
					if (from < to) from = to; // never supposed to happen, but you never know...
					address_from = from->getAddress();
					while (address_from == nullptr && from < end)
					{
						++from;
						address_from = from->getAddress();
					}

					if (address_from != nullptr)
					{
						*to = *from;
						to->set_new_address(to);
					}
					else
					{
						to->unset();
					}
					from->unset();
					copied = (from == end);
				}
				else
				{
					to->unset();
				}
			}
			to->release_lock();
			to->unlock_terminal();
			++to;
		}
	}

public:
	static void execute(N* iter, std::vector<N>& _tree, Tree_index<N>& _index, N*& _next)
	{
		Timer t = Timer();
		u_long full = _tree[0].count();
		u_long keep = iter->count();
		std::cout << std::endl;
#ifdef TREE_SHIFT_PRINT
		std::cout << "count full ...    " << Count::format(full) << std::endl;
		std::cout << "count index ...   " << Count::format(_index.count()) << std::endl;
		std::cout << "count to keep ... " << Count::format(keep) << std::endl;
		std::cout << "count to kill ... " << Count::format(full - keep) << std::endl;
#endif

		t.start();

		Tree_dump<N>::out(_tree,"before.txt");
		Tree_integrity_check<N>::execute(_tree, _index);
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				printf("mark trash core : %d\n", omp_get_thread_num());
				mark_trash(iter, _tree);
				printf("trash marked\n");
			}

			#pragma omp section
			{
			printf("index clear core : %d\n", omp_get_thread_num());
			_index.clear();
			}
		}

		Tree_dump<N>::out(_tree, "marked.txt");
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				printf("compacting core : %d\n", omp_get_thread_num());
				compact_tree(_tree);
				Tree_dump<N>::out(_tree, "compacted.txt");
				printf("tree compacted\n");
			}

			#pragma omp section
			{
				printf("build index core : %d\n", omp_get_thread_num());
				_index.fill();
			}

			#pragma omp section
			{
				printf("reset index core : %d\n", omp_get_thread_num());
				_index.reset_next();
			}
		}
		Tree_integrity_check<N>::execute(_tree, _index);
		find_next(_tree, _next);
		t.stop();

		u_long left = _tree[0].count();
#ifdef TREE_SHIFT_PRINT
		std::cout << "count left ...  " << Count::format(left) << std::endl;
		std::cout << "count index ... " << Count::format(_index.count()) << std::endl;

		std::cout << "recycling duration : " << Count::format(duration_cast<milliseconds>(t.result()).count()) << " ms." << std::endl;
#endif
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
		std::cout << "clean tree : " << Count::format(ptr - &T[0]) << std::endl;
	}

	static void find_next(std::vector<N>& T, N*& n)
	{
		n = &T[1];
		while (n->getAddress() != nullptr)
		{
			++n;
		}
		auto t = n - &T[0];
		std::cout << std::endl << "next: " << Count::format(t) << std::endl;
	}
};

template<class N> u_long Tree<N>::compacting = 0;

#pragma once
#include "../typedef.h"
#include <vector>
#include "../Timer.h"
#include "Tree_index.h"
#include "../Count.h"
#include <iostream>

template<class N> class Tree_integrity_check
{
public:
	static void execute(std::vector<N>& _tree, Tree_index<N>& _index)
	{
#if defined(TREE_INTEGRITY)
		std::cout << "INTEGRITY CHECK START" << std::endl;
		N* ptr = &_tree[0];
		N** address;
		for (u_long i = 0; i < static_cast<u_long>(_tree.size()); ++i)
		{
			try
			{
				address = ptr->getAddress();
				if (address == nullptr && ptr->hasParent())
				{
					std::cout << "tree integrity failed with " << i << " : null address" << std::endl;
				}
			}
			catch (...)
			{
				std::cout << "tree integrity failed with " << i << std::endl;
			}
			++ptr;
		}


		std::vector<N*>* addresses = _index.get_address();
		std::vector<N**>* pt_to_addresses = _index.get_empty();
		N*** min = _index.get_next();
		N*** ptr_addr = &((*pt_to_addresses)[0]);
		for (u_long i = 0; i < static_cast<u_long>(_tree.size()); ++i)
		{
			if (*ptr_addr != nullptr && **ptr_addr != nullptr && !(ptr_addr < min))
			{
				std::cout << "index integrity failed with " << i << " : pointing to a not empty space !!" << std::endl;
			}
			++ptr_addr;
		}

		N** address_to = &((*addresses)[0]);
		ptr_addr = &((*pt_to_addresses)[0]);
		auto end = &((*pt_to_addresses)[_tree.size()-1]);
		for (u_long i = 0; i < static_cast<u_long>(_tree.size()); ++i)
		{
			if (*address_to == nullptr)
			{
				auto tmp_ptr = ptr_addr;
				while (*tmp_ptr != address_to && tmp_ptr < end)
				{
					++tmp_ptr;
				}
				if (*tmp_ptr == address_to)
				{
					++ptr_addr;
				}
				else
				{
					std::cout << "index integrity failed with " << i << " : empty space not referenced !!" << std::endl;
				}
			}
			++address_to;
		}
		std::cout << "INTEGRITY CHECK END" << std::endl;
#endif
	}
};

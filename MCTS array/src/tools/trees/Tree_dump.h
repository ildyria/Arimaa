#pragma once
#include "../typedef.h"
#include <vector>
#include <iostream>
//#include <fstream>

template<class N> class Tree_dump
{
public:
	static void out(std::vector<N>& T,std::string file_name)
	{
#if defined(DUMP_TREES)
		//		std::ofstream myfile;
//		myfile.open(file_name, std::ios::out | std::ios::trunc);

		N* ptr = &T[0];
		auto terminal = ptr->get_terminal();
		u_long modulo = (static_cast<u_long>(1) << 6) - 1;
//		for (u_long i = 0; (i < static_cast<u_long>(T.size()) && ptr->has_parent()); ++i)
		for (u_long i = 0; (i < static_cast<u_long>(T.size())); ++i)
		{
			terminal = ptr->get_terminal();
			if ((i & modulo) == 0) cout << std::endl;
			if (ptr->getAddress() == nullptr)
			{
				cout << "_";
			}
			else if (terminal == 64)
			{
				cout << "X";
			}
			else if (terminal == 32)
			{
				cout << "N";
			}
			else if (terminal == 16)
			{
				cout << "L";
			}
			else
			{
				cout << (terminal & 7);
			}

/*
			myfile << ptr;
			myfile << " : " << terminal;
			myfile << ", " << ptr->get_proba();
			if (ptr->get_terminal() == 0)
			{
				myfile << ", " << ptr->get_children().first;
				myfile << " (" << ptr->get_children().second << ")";
			}
			myfile << "\n";
*/
			++ptr;
		}
		cout << std::endl;
#endif
//		myfile.close();
	}

};

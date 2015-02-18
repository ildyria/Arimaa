#pragma once
#include "../typedef.h"
#include <vector>
#include <iostream>
#include <fstream>

template<class N> class Tree_dump
{
public:
	static void out(std::vector<N>& T,std::string file_name)
	{
		std::ofstream myfile;
		myfile.open(file_name, std::ios::out | std::ios::trunc);

		N* ptr = &T[0];
		auto terminal = ptr->getTerminal();
		for(auto i = 0; (i < T.size() && ptr->hasParent()); ++i)
		{
		
#if !defined(DOUBLE_TREE)
			myfile << ptr->getParent() << " => ";
#endif
			myfile << ptr;
			myfile << " : " << terminal;
			myfile << ", " << ptr->getProba();
			if (ptr->getTerminal() == 0)
			{
				myfile << ", " << ptr->getChildren().first;
				myfile << " (" << ptr->getChildren().second << ")";
			}
			myfile << "\n";
			++ptr;
			terminal = ptr->getTerminal();
		}

		myfile.close();
	}

};

/**
 * \file Mcts.h
 * \brief Define class Mcts
 * \author Benoit Viguier
 * \version 0.5
 * \date october 30th 2014
 *
 */
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "Node.h"

using namespace std;

namespace mcts
{

	class Mcts
	{
		int _visits;
		Node _root;

	public:
		Mcts();
		~Mcts();
	};
}
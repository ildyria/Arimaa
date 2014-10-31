/**
 * \file Mcts.h
 * \brief Define class Mcts
 * \author Benoit Viguier
 * \version 0.5
 * \date october 30th 2014
 *
 */
#pragma once
#include "Node.h"
#include "TicTacToe.h"

namespace mcts
{

	class Mcts
	{
//		static int _visits;
		Node* _root;

	public:
		// create the initial algorithm
		
		Mcts();

		explicit Mcts(Bitboard Bb);

		~Mcts();

		// in case a move is played, we need to update the tree (get the root 2 depth down and prune the useless branches) 
		void playerPlayed(string move);

		// return the best move after the algorithm is run
		string GetBestMove();




	};
}
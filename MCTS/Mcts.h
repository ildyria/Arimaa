/**
 * \file Mcts.h
 * \brief Define class Mcts
 * \author Benoit Viguier
 * \version 1.0
 * \date November 2nd 2014
 *
 */
#pragma once
#include "Random.h"
#include "Node.h"
#include "TicTacToe.h"

namespace mcts
{

	/**
	 * \brief Mcts class
	 * \details 
		Node* _root = adress of the node considered as the root of the tree.
		int _depth = depth to explore before running random simulations.
		int _simulationPerRoot = number of simulations to run per Root.
		int _simulationPerLeaves = number of simulations to run per Leaves.
		int _IAPlayer = player played by the algorithm (1 or 2).
	 */
	class Mcts
	{
		Node* _root;
		int _depth;
		int _simulationPerRoot;
		int _simulationPerLeaves;
		int _IAPlayer;

	public:
		/**
		 * \fn Mcts
		 * \brief Not implemented
		 */
		Mcts();

		/**
		 * \fn Mcts
		 * \brief create a mcts with specifics parameters
		 * \param  Bb       BitBoard for the root
		 * \param  depth    Depth to search, 4 by default
		 * \param  IAPlayer int corresponding to the player : 1 or 2, 1 by default
		 * \param  simulR   Number of simulations to be run starting from the root, 100 by default. => OpenMP ?
		 * \param  simulL   Number of random simulations to be run at the end of the search tree, 10 by default. => OpenAcc ?
		 */
		explicit Mcts(Bitboard Bb, int depth = 4, int IAPlayer = 1, int simulR = 100, int simulL = 10);

		/**
		 * \fn ~Mcts
		 * \brief Not implemented
		 */
		~Mcts();

		/**
		 * \fn movePlayed
		 * \brief play a move on the tree, prune all the node are useless.
		 * 
		 * \param move move played
		 */
		void movePlayed(string move);

		/**
		 * \fn UpdateNode
		 * \details Update a node :
		 * - check state of the node
		 * if -1 : fetch the children (possible moves) and update the state
		 * if 0 and no children : fetch the children and update the state
		 * else : return the state
		 * 
		 * \param  node to be updated
		 * \return      the state of the board : 0, 1, 2, or 3 .
		 */
		int UpdateNode(Node* node);

		/**
		 * \fn playRandom
		 * \details play _simulationPerLeaves simulations of games with completely randoms moves starting from the board of that node, upate the stats each time we reach a final state (win or loss, tie are considered as loss)
		 * 
		 * \param node node to run the simulations from
		 */
		void playRandom(Node* node);

		/**
		 * \fn explore
		 * \details if depth is > 0, fetch the possible moves and chose the best using UCT, and decrease depth by 1.
		 * if depth = 0, run playRandom
		 * if the node is a terminal, then update the stats :
		 * - set uct to 10 if it's a win
		 * - set parent's uct to -1 if it's a loss (we must never go to that node)
		 * 
		 * \param node  node we are exploring
		 * \param depth depth to search for possibilities
		 */
		void explore(Node* node, int depth);

		/**
		 * \fn GetBestMove
		 * \brief main function of the algorithm, run the exploration, simulations and returns the best move given the results
		 * 
		 * \return move chosen by the algorithm
		 */
		string GetBestMove();

		/**
		 * \fn print_tree
		 * \brief print the tree of MCTS starting from the root with a defined depth
		 * 
		 * \param depth How deep we want to search, 1 by default
		 */
		void print_tree(int depth = 1);

	};
}
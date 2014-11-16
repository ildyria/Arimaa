/**
 * \file Mcts.h
 * \brief Define class Mcts
 * \author Benoit Viguier
 * \version 1.0
 * \date November 2nd 2014
 *
 */
#pragma once
#include "Node.h"
#include "../Interfaces/TheGame.h"
#include <time.h>

namespace mcts
{

	/**
	 * \brief Mcts class
	 * \details 
		Node* _root = adress of the node considered as the root of the tree.
		int _depth = depth to explore before running random simulations.
		int _timeLimitsimulationPerRoot = time limit in ms
		int _simulationPerRoot = number of simulations to run per Root.
		int _simulationPerLeaves = number of simulations to run per Leaves.
	 */
	class Mcts
	{
		TheGame* _game;
		Node* _root;
		int _depth;
		int _timeLimitsimulationPerRoot;
		int _simulationPerRoot;
		int _simulationPerLeaves;

	public:
		/**
		 * \fn Mcts
		 * \brief Not implemented
		 */
		Mcts();

		/**
		 * \fn Mcts
		 * \brief create a mcts with specifics parameters
		 * \param  Bb			BitBoard for the root
		 * \param  depth		Depth to search, 4 by default
		 * \param  timelimit	maximum time to run the simulation
		 * \param  simulR		Number of simulations to be run starting from the root, 100 by default. => OpenMP ?
		 * \param  simulL		Number of random simulations to be run at the end of the search tree, 10 by default. => OpenAcc ?
		 */
		explicit Mcts(TheGame* game,Bitboard Bb, int depth = 4, int timelimit = 5000, int simulR = 1000, int simulL = 10);

		/**
		 * \fn ~Mcts
		 * \brief Not implemented
		 */
		~Mcts() {}

		/**
		 * \fn movePlayed
		 * \brief play a move on the tree, prune all the node that are useless.
		 * 
		 * \param move move played
		 */
		void movePlayed(Move& move);

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
		* \fn UpdateRoot
		* \brief calls UpdateNode(_root)
		*/
		inline void UpdateRoot() { UpdateNode(_root); };

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
		 */
		void explore();

		/**
		 * \fn GetBestMove
		 * \brief main function of the algorithm, run the exploration, simulations and returns the best move given the results
		 * 
		 * \return move chosen by the algorithm
		 */
		Move GetBestMove();

		/**
		 * \fn print_tree
		 * \brief print the tree of MCTS starting from the root with a defined depth
		 * 
		 * \param depth How deep we want to search, 1 by default
		 */
		void print_tree(int depth = 1);

	};
}
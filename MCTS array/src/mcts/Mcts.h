/**
 * \file Mcts.h
 * \brief Define class Mcts
 * \author Benoit Viguier
 * \version 1.0
 * \date November 2nd 2014
 *
 */
#pragma once
#include "../tools/typedef.h"
#include "Node.h"
#include "../tools/Memento.h"
#include "MctsArgs.h"
#include "../interfaces/TheGame.h"

namespace mcts
{

	/**
	 * \brief Mcts class
	 * \details 
		Node* _root = adress of the node considered as the root of the tree.
		MctsArgs _param = options of the MCTS algorithm
	 */
	class Mcts
	{
		TheGame* _game;
		std::vector<Node> _tree;
		std::vector<Node> _buff;
		std::vector<Memento<Node*>> _parents;

		MctsArgs* _param;
		Node* _next;
		Bitboard* _state;
		bool _maxdepthreached;

		/**
		* \fn UpdateNode(Node* node)
		* \details Update a node :
		* - check state of the node
		* if -1 : fetch the children (possible moves) and update the state
		* if 0 and no children : fetch the children and update the state
		* else : return the state
		*
		* \param  node to be updated
		* \return      the state of the board : 0, 1, 2, or 3 .
		*/
		u_int UpdateNode(Node* node, Bitboard* Bb);


		/**
		* \fn playRandom(Node* node)
		* \details play _simulationPerLeaves simulations of games with completely randoms moves starting from the board of that node, upate the stats each time we reach a final state (win or loss, tie are considered as loss)
		*
		* \param node node to run the simulations from
		*/
		void playRandom(Node* node, Bitboard* Bb);

		/**
		* \fn explore()
		* \details if depth is > 0, fetch the possible moves and chose the best using UCT, and decrease depth by 1.
		* if depth = 0, run playRandom
		* if the node is a terminal, then update the stats :
		* - set uct to 10 if it's a win
		* - set parent's uct to -1 if it's a loss (we must never go to that node)
		*/
		void explore();

		void feedbackWinLose();

		void cleanTree(std::vector<Node> &T);

		void copyTree(Node* NewRoot, std::vector<Node> &Tdest);

		void findNext(std::vector<Node> &T);
	public:
		/**
		 * \fn Mcts()
		 * \brief Not implemented
		 */
		Mcts();

		/**
		 * \fn Mcts(TheGame* game, Bitboard* Bb, MctsArgs args)
		 * \brief create a mcts with specifics parameters
		 * \param  game			game used (abstract class).
		 * \param  Bb			first state to start the search from.
		 * \param  args			options of the MCTS Algorithm
		 */
		explicit Mcts(TheGame* game, Bitboard* Bb, MctsArgs* args);

		/**
		 * \fn ~Mcts()
		 * \brief Not implemented
		 */
		~Mcts() {}

		/**
		* \fn UpdateRoot()
		* \brief calls UpdateNode(_root)
		*/
		inline void UpdateRoot() { UpdateNode(&_tree[0],_state); };

		/**
		 * \fn movePlayed(Move& move)
		 * \brief play a move on the tree, prune all the node that are useless.
		 * 
		 * \param move move played
		 */
		Bitboard* movePlayed(Move& move);


		/**
		 * \fn GetBestMove()
		 * \brief main function of the algorithm, run the exploration, simulations and returns the best move given the results
		 * 
		 * \return move chosen by the algorithm
		 */
		Move GetBestMove();

		/**
		 * \fn print_tree(int depth = 1)
		 * \brief print the tree of MCTS starting from the root with a defined depth
		 * 
		 * \param depth How deep we want to search, 1 by default
		 */
		void print_tree(u_int depth = 1);

		/**
		 * \fn kill_tree()
		 * @brief clear all the branches of the tree.
		 */
		void kill_tree();

		/**
		 * \fn get_Number_Leaves()
		 * \brief count the number of leaves of the tree.
		 */
		void get_Number_Leaves();

		double winning_Strategy();

		void take_a_chill_pill(u_long i);

		void feedback(u_int nodet);
	};
}
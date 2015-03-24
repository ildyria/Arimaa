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
#include <omp.h>
#if defined(DOUBLE_TREE)
	#include "../tools/trees/Tree_cache.h"
#else
	#include "../tools/trees/Tree_shift.h"
	#include "../tools/trees/Tree_index.h"
#endif
#include "../tools/Memento.h"
#include "../interfaces/TheGame.h"
#include "Node.h"
#include "MctsArgs.h"
#include "../tools/ChillPill.h"
#include <chrono>

namespace mcts
{

	/**
	 * \brief Mcts class
	 * \details 
		TheGame* _game = pointer to the game
		MctsArgs* _param = options of the MCTS algorithm;
		Node* _next = pointer for the next node to be set;
		Bitboard* _state = starting bitboard : current position of the game;
		omp_lock_t _lockNode : lock in order to get the lock of a node (critical);
		omp_lock_t _lockNext : lock in order to set the _next (critical);
		std::vector<Memento<Node*>> _parents : memento design pattern in order to feed back the results;
		std::vector<Node> _tree : main tree;
		std::vector<Node> _buff : buffer tree used for the prunning;
	 */
	class Mcts
	{
		TheGame* _game;
		MctsArgs* _param;
		Node* _next;
		Bitboard* _state;
		omp_lock_t _lockNode;
		omp_lock_t _lockNext;
		std::vector<Memento<Node*>> _parents;
		std::vector<Node> _tree;
#if defined(DOUBLE_TREE)
		std::vector<Node> _buff;
#else
		Tree_index<Node> _index;
#endif

		/**
		 * \fn update_node(Node* node, Bitboard* Bb)
		 * \details Update a node :
		 * - check state of the node
		 * if 128 : fetch the children (possible moves) and update the state
		 * if 0 and no children : fetch the children and update the state
		 * else : return the state
		 *
		 * \param  node to be updated
		 * \param  Bb state of the board
		 * \return the state of the board : 0, 1, 2, 3, or 255 if cannot be explored (locked / memory limit) .
		 */
		void expand_node(Node* node, Bitboard* Bb, u_int& nodet);

		u_int update_node(Node* node, Bitboard* Bb);

		/**
		 * \fn start_ramdom_playouts(Node* node,  Bitboard* Bb)
		 * \details play _simulationPerLeaves simulations of games with completely randoms moves starting from the board of that node, upate the stats each time we reach a final state (win or loss, tie are considered as loss)
		 *
		 * \param node node to run the simulations from
		 * \param Bb state of the board
		 */
		void start_ramdom_playouts(Node* node, Bitboard* Bb);

		/**
		 * \fn feedback_sure_wins_loss()
		 * \details feedback_results the results to the parents (winning and losing move) in order to fasten the search
		 */
		void feedback_sure_wins_loss();

		/**
		 * \fn feedback_results
		 * \brief feedback_results the results to the parents in order to update their _win value
		 * 
		 * \param nodet value of the simulation
		 */
		void feedback_results(u_int nodet);

	public:
		/**
		 * \fn Mcts()
		 * \brief Not used
		 */
		Mcts() {};

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
		* \fn update_root()
		* \brief calls update_node(_root)
		*/
		inline void update_root()
		{
			u_int tmp = update_node(&_tree[0],_state);
			expand_node(&_tree[0], _state, tmp);
		};

		/**
		 * \fn move_played(Move& move)
		 * \brief play a move on the tree, prune all the node that are useless.
		 * 
		 * \param move move played
		 */
		Bitboard* move_played(Move& move);

		/**
		* \fn explore()
		* \details if depth is > 0, fetch the possible moves and chose the best using UCT, and decrease depth by 1.
		* if depth = 0, run start_ramdom_playouts
		* if the node is a terminal, then update the stats :
		* - set uct to 42 if it's a win
		* - set parent's uct to -1 if it's a loss (we must never go to that node)
		*/
		void explore();

		/**
		 * \fn get_best_move()
		 * \brief main function of the algorithm, run the exploration, simulations and returns the best move given the results
		 * 
		 * \return move chosen by the algorithm
		 */
		Move get_best_move();

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
		 * \fn get_number_leaves()
		 * \brief count the number of leaves of the tree.
		 */
		void get_number_leaves();

		/**
		 * \fn winning_chances()
		 * \return the winning rate of the last move, -1 if losing, 2 if it was a winning strategy
		 */
		double winning_chances();

		v_stat get_moves_statistics();
	};
}
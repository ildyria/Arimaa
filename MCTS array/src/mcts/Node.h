/**
 * \file Node.h
 * \brief Define class Node
 * \author Benoit Viguier
 * \version 1.0
 * \date November 2nd 2014
 *
 */
#pragma once
#include <iostream>
#include <algorithm>
#include <omp.h>
#include "../interfaces/Bitboard.h"
#include "../interfaces/Move.h"
#include "../tools/FastLog.h"
#include "../tools/typedef.h"

using std::max;

namespace mcts
{

	/**
	 * \brief Node class
	 * \details 
		int _visits = number of visits on the node
		int _wins = number of wins on that node
		int _terminal = determine if the node has be visited or is a terminal :
		-1 : not visited
		0 : non terminal
		1/2 : player 1/2 wins
		3 : tie
		double _uct = value of uct on the node
		Bitboard* _state = current state of the board
		Move _move = last move been played
		list<Node*> _children = list of all possible Bitboards after that node;
		list<Node*> _parents = list of all parents, right now there is only 1 parent per node, but in order to improve the algorithm we need to factorise the number of nodes;
	 */
	class Node
	{
		double	_uct;		// 8 bytes
		u_long _visits;		// 8 bytes
		u_long _wins;		// 8 bytes

		u_int _nbchildren;	// 4 bytes (if linux, 8 bytes if Win...) // a passer en unsigned ?
		u_short	_toplay;	// 2 bytes (if linux, 8 bytes if Win ??)
		char	_terminal;	// 1 byte
		bool	_lock;		// 1 byte

		Move	_move;		// 8 bytes (uint64)
		Node*	_firstchild;// 8 bytes

		/**
		 * \fn UCT
		 * \brief calculate the UCT value of a node and record it.
		 * \details winrate + sqrt(2)*log(total of visit / visits)
		 * if uct is set to 42 => we don't update the value : move that has to be played (win)
		 * if uct is set to -1 => we don't change the value either : move that should not be played (loss)
		 * 
		 * \param visits number of visits on the parent node.
		 */
		inline void UCT(u_long visits) {
			if (_uct != -1 && _uct != 42)
			{
				_uct = _wins / static_cast<double>(_visits > 1 ? _visits : 1) + sqrt(2.0 * FastLog::fast_log((visits >> 1) + 1) / ((_visits > 2) ? _visits >> 1 : 1));
			}
		};

	public :

		Node();
		/**
		* \fn Node(Bitboard* state)
		* \brief create a node given a Bitboard
		*
		* \param state : Board to create the node with
		*/
		explicit Node(u_short player);

		/**
		 * \fn Node(Node* p_parent, Bitboard state, Move& move);
		 * \brief create a Node with parent as parent and move + Bitboard after the move played
		 * 
		 * \param parent : parent of the node
		 * \param state : Bitboard after the move
		 * \param move : move played
		 */
		Node(u_short player, Move& move);

		/**
		 * \fn ~Node()
		 * \brief Destructor of the Node
		 * \details call Delete on the bitboard.
		 */
		virtual ~Node();

		/**
		* \fn setTerminal(int terminal)
		* \brief setter for _terminal
		*
		* \param value of the terminal
		*/
		inline void setTerminal(u_int terminal) { _terminal = static_cast<char>(terminal & 255); };

		/**
		* \fn setTerminal(int terminal)
		* \brief setter for _terminal
		*
		* \param value of the terminal
		*/
		inline void setChildrens(Node* c, u_int n) { _firstchild = c; _nbchildren = n; };

		/**
		* \fn forceSetUCT
		* \brief setter for _uct(int uct)
		* \details it allows us to force a value to uct : 10 or -1 in order to make sure that some moves must or mustn't be played
		*
		* \param value of the uct
		*/
		inline void forceSetUCT(int uct) { if (_uct == -1) return; _uct = uct; };

		/**
		* \fn getUCT()
		* \brief getter for _uct
		*
		* \return value of the uct
		*/
		inline double getUCT() { return _uct; };

		/**
		* \fn getTerminal()
		* \brief getter for the _termianl
		*
		* \return 0 if not, 1 if player 1 wins, 2 if player 2 wins, 3 if tie
		*/
		inline u_int getTerminal() { return (_terminal & 255); };

		inline bool hasParent() { return _terminal != 64; };

		inline void setHasParent() { _terminal = static_cast<char>(128); };

		inline void clearParent() { _terminal = static_cast<char>(64); };

		inline bool toBeExplored() { return _terminal == static_cast<char>(128); }

		/**
		* \fn getState()
		* \brief getter for the Board
		*
		* \return the Board of the current node
		*/
		inline u_short getPlayer() { return _toplay; };

		/**
		* \fn getMove()
		* \brief getter for the move
		*
		* \return the last move played to access the current node
		*/
		inline Move getMove() { return _move; };

		/**
		 * \fn getChildren()
		 * \brief getter for the children
		 *
		 * \return return the list of the childrens
		 */
		inline std::pair<Node*, u_int> getChildren() { return std::pair<Node*, u_int>(_firstchild, _nbchildren); };

		/**
		 * \fn getProba()
		 * \brief getter for the winrate
		 *
		 * \return return the winrate of a node
		 */
		inline double getProba() { return (_uct != -1) ? ((_uct != 42) ? static_cast<double>(_wins) / static_cast<double>(_visits > 1? _visits : 1) : 2) : 0; };


		/**
		 * \fn addVirtualLoss(int i)
		 * \brief add virtual losses
		 */
		inline void addVirtualLoss(u_int i) { _visits += (i*2); };

		/**
		 * \fn getVisits()
		 * \brief getter for the number of visits
		 *
		 * \return return the number of visits
		 */
		inline u_long getVisits() { return _visits; };

		/**
		 * \fn compare()
		 * \brief compare Node* a and Node *b,
		 *
		 * \param Node a
		 * \param Node b
		 * \return true if a's uct is greater than b's uct value.
		 */
		static bool compareUCT(Node* a, Node* b);

		/**
		* \fn compareWR()
		* \brief compare Node* a and Node *b,
		*
		* \param Node a
		* \param Node b
		* \return true if a's WR is greater than b's WR value.
		*/
		static bool compareWR(Node* a, Node* b);

		/**
		 * \fn select_child_UCT()
		 * \brief fecth the childrens and select the one with the highest UCT
		 * 
		 * \return Return the Node with the best UCT
		 */
		Node* select_child_UCT();
		
		/**
		* \fn select_child_WR()
		* \brief fecth the childrens and select the one with the highest WR
		*
		* \return Return the Node with the best UCT
		*/
		Node* select_child_WR();

		/**
		* \fn set(Bitboard* state, Move& move, Node* parent)
		* \brief add a Child to a node, given his Bitboard and move played and reset its parameters
		*
		* \param state : Bitboard after the nove
		* \param move : move played
		* \param terminal : set if node is terminal or not. By default, not explored.
		*/
		void set(Move& move);

		/**
		* \fn unset()
		* \brief clear data of a node (it will be added to the aviable list later).
		*/
		void unset();

		/**
		 * \fn update(int win)
		 * \brief back propate the results of the last simulation to the parents of the node, update _visits & _wins
		 * 
		 * \param win int representing the last winner : 1 2 or 3
		 */
		inline void update(u_int win) {
			_wins += (win != _toplay && win != 3) ? 2 : (win == 3) ? 1 : 0;
		};

		/**
		 * \fn print_tree(int numtab, int depth)
		 * \brief print the sub-tree starting with the current node as a root
		 * 
		 * \param numtab the depth we are in the tree in order to insert \t in front of the display
		 * \param depth How deep we want to dive in the tree
		 */
		void print_tree(u_int numtab, u_int depth);

/*
		u_int max_depth();
*/

		inline void play(u_short i = 0)
		{
			_toplay = (i == 0) ? ((_toplay == 1) ? 2 : 1) : ((i == 1) ? 2 : 1);
		};

		inline bool getLock()
		{
			// printf("%p locked.\n",(void*)this); // trop lent !!!!
			bool t = _lock;
			_lock = true;
			return t;
		}

		inline void releaseLock()
		{
			// printf("%p unlocked.\n",(void*)this); // trop lent !!!!
			_lock = false;
		}

		inline void take_a_chill_pill(u_long i)
		{
			printf("%d : %ld\n", omp_get_thread_num(),i);
		}

	};
};
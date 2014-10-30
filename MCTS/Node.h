/**
 * \file Node.h
 * \brief Define class Bitboard (WIP)
 * \author Benoit Viguier
 * \version 0.5
 * \date october 30th 2014
 *
 */
#pragma once
#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include "Bitboard.h"

using std::string;
using std::list;

namespace mcts
{

	/**
	 * \brief Node class
	 * \details 
		int _visits = number of visits on the node
		int _wins = number of wins on that node
		double _uct = value of uct on the node
		Bitboard _state = current state of the board
		string _move = last move been played
		list<Node*> _children = list of all possible Bitboards after that node;
		list<Node*> _parents = list of all parents, right now there is only 1 parent per node, but in order to improve the algorithm we need to factorise the number of nodes;
	 */
	class Node
	{
		int _visits;
		int _wins;
		int _terminal;
		double _uct;
		Bitboard _state;
		string _move;
		list<Node*> _children;
		list<Node*> _parents;

		/**
		 * \fn UCT
		 * \brief calculate the UCT value of a node and record it.
		 * \details winrate + sqrt(2)*log(total of visit / visits)
		 * 
		 * \param visits number of visits on the parent node.
		 */
		inline void UCT(int visits) {
			_uct = (double)_wins / (double)(max(_visits,1)) +sqrt(2.0 * std::log(double(visits + 1)) / _visits);
		};

	public :
		/**
		 * \fn Node
		 * \brief Constructor by default.
		 */
		Node();

		/**
		* \fn Node(Node *parent)
		* \brief i don't know why i created that function...
		*
		* \param state Board to create the node with
		*/
		Node(Bitboard state);

		/**
		 * \fn Node(Node *parent)
		 * \brief i don't know why i created that function...
		 * 
		 * \param parent parent of the node
		 */
		Node(Node* parent);

		/**
		 * \fn Node(Node* p_parent, Bitboard state, string move);
		 * \brief create a Node with parent as parent and move + Bitboard after the move played
		 * 
		 * \param parent parent of the node
		 * \param state Bitboard after the move
		 * \param move move played
		 */
		Node(Node* parent, Bitboard state, string move);

		/**
		 * \fn ~Node
		 * \brief Destructor of the Node
		 * \details call Delete on all the childrens of the node.
		 */
		~Node();

		/**
		* \fn setTerminal
		* \brief setter for _terminal
		*
		* \param value of the terminal
		*/
		inline void setTerminal(int terminal) { _terminal = terminal; };

		/**
		* \fn getTerminal
		* \brief getter for the childs
		*
		* \return 0 if not, 1 if player 1 wins, 2 if player 2 wins
		*/
		inline int getTerminal() { return _terminal; };

		/**
		* \fn getState
		* \brief getter for the childs
		*
		* \return the state of the current node
		*/
		Bitboard& getState() { return _state; };

		/**
		* \fn getMove
		* \brief getter for the move
		*
		* \return the last move played to access the current node
		*/
		string getMove() { return _move; };

		/**
		 * \fn getChildren
		 * \brief getter for the children
		 *
		 * \return return the list of the childrens
		 */
		inline list<Node*> getChildren() { return _children; };

		/**
		 * \fn getParents
		 * \brief getter for the parents
		 *
		 * \return return the list of the parents
		 */
		inline list<Node*> getParents() { return _parents; };

		/**
		 * \fn getProba
		 * \brief getter for the winrate
		 *
		 * \return return the winrate of a node
		 */
		inline double getProba() { return (double)_wins / (double)_visits; };

		/**
		 * \fn getVisits
		 * \brief getter for the number of visits
		 *
		 * \return return the number of visits
		 */
		inline int getVisits() { return _visits; };

		/**
		 * \fn select_child_UCT
		 * \brief fecth the childrens and select the one with the highest UCT
		 * 
		 * \param visit number of visit of the parent node
		 * \return Return the Node with the best UCT
		 */
		Node select_child_UCT(int visit=1);

		/**
		 * \fn addChild
		 * \brief add a Child to a node, given his Bitboard and move played
		 * 
		 * \param state Bitboard after the nove
		 * \param move move played
		 */
		void addChild(Bitboard& state, string move, int terminal = -1);

		/**
		 * \fn update
		 * \brief back propate the results of the last simulation to the parents of the node, update _visits & _wins
		 * 
		 * \param win true if last simulation was a win, false if last simulation was a lost
		 */
		void update(bool win);
	};
};
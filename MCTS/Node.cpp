#include "Node.h"

namespace mcts {

	Node::Node()
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_state = Bitboard(3,3, 1);
		_move = "";
		list<Node*> _children;
		list<Node*> _parents;
	}

	Node::Node(Node* p_parent)
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_state = Bitboard(3, 3, 1);
		_move = "";
		list<Node*> _children;
		list<Node*> _parents;
		_parents.push_front(p_parent);
	}

	Node::Node(Node* p_parent, Bitboard state, string move)
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_state = state;
		_move = move;
		list<Node*> _children;
		list<Node*> _parents;
		_parents.push_front(p_parent);
	}

	Node::~Node() // need to add a function to back propagate a destruction to parents => case to go down the tree to create a new root
	{
		list<Node*>::iterator itL;
		for (itL = _children.begin(); itL != _children.end(); itL++)
		{
			// ONLY if 1 parent !!
			delete(*itL);
		}
	}

	void Node::addChild(Bitboard state, string move){
		_parents.push_front(&Node(this,state,move));
	};

	void Node::update(bool win){
		list<Node*>::iterator itL;
		if (win)
		{
			_wins++;
		}
		_visits++;

		if (!_parents.empty())
		{
			for (itL = _parents.begin(); itL != _parents.end(); itL++)
			{
				(**itL).update(win);
			}
		}
	};


	Node Node::select_child_UCT(int visits){
		list<Node*>::iterator itL;
		for (itL = _children.begin(); itL != _children.end(); itL++)
		{
			(*itL)->UCT(visits);
		}

		return *max_element(_children.begin(), _children.end(), [](Node* a, Node* b) { return (*a)._uct < (*b)._uct; });
	};


}

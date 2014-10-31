#include "Node.h"
#define elseif else if

using std::max_element;

namespace mcts {

	Node::Node()
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_state = Bitboard(3,3, 1);
		_terminal = -1;
		_move = "";
		list<Node*> _children;
		list<Node*> _parents;
		cout << "node created by default" << endl;
	}

	Node::Node(Bitboard state): _state(state)
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_terminal = -1;
		_move = "";
		list<Node*> _children;
		list<Node*> _parents;
		cout << "node created with board " << &state <<  endl;
	}

	Node::Node(Node* p_parent)
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_state = Bitboard(3, 3, 1);
		_terminal = -1;
		_move = "";
		list<Node*> _children;
		list<Node*> _parents;
		_parents.push_front(p_parent);
		cout << "node created with parent " << p_parent << endl;
	}

	Node::Node(Node* p_parent, Bitboard state, string move) : _state(state), _move(move)
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_terminal = -1;
		list<Node*> _children;
		list<Node*> _parents;
		_parents.push_front(p_parent);
		cout << "node created with parent " << p_parent << ", board " << &state << " and move : " << move << endl;
	}

	Node::~Node() // need to add a function to back propagate a destruction to parents => case to go down the tree to create a new root
	{
		cout << "starting destruction of node" << endl;

		list<Node*>::iterator itL;
		for (itL = _children.begin(); itL != _children.end(); itL++)
		{
			// ONLY if 1 parent !!
//			if (&(*itL) != ){
				delete(*itL);
//			}
		}
		cout << "destruction of node ended" << endl;
	}


	void Node::addChild(Bitboard& state, string move, int terminal){
		Node* node = new Node(this, state, move);
		node->setTerminal(terminal);
		_children.push_back(node);
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

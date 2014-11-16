#include "Node.h"
#define elseif else if
//#define DEBUG_NODE

using std::max_element;
using std::min_element;
using std::max;
using std::cout;
using std::endl;
using std::string;
using std::list;
using std::log;


namespace mcts {

	Node::Node()
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_state = Bitboard(3,3, 1);
		_terminal = -1;
		_move = Move();
#ifdef DEBUG_NODE
		cout << "N*" << this << " created by default" << endl;
#endif //DEBUG_NODE
	}

	Node::Node(Bitboard state): _state(state)
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_terminal = -1;
		_move = Move();
#ifdef DEBUG_NODE
		cout << "N*" << this << " created with B*" << &state << endl;
#endif //DEBUG_NODE
	}

	Node::Node(Node* p_parent)
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_state = Bitboard(3, 3, 1);
		_terminal = -1;
		_move = Move();
		_parents.push_front(p_parent);
#ifdef DEBUG_NODE
		cout << "N*" << this << " created with P*" << p_parent << endl;
#endif //DEBUG_NODE
	}

	Node::Node(Node* p_parent, Bitboard state, Move move) : _state(state), _move(move)
	{
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_terminal = -1;
		_parents.push_front(p_parent);
#ifdef DEBUG_NODE
		cout << "N*" << this << " created with P*" << p_parent << ", B*" << &state << " and move : " << move << endl;
#endif //DEBUG_NODE
	}

	void Node::killChildrens(Node* exception)
	{
#ifdef DEBUG_NODE
		cout << "destruction of N*" << this << " childrens" << endl;
#endif //DEBUG_NODE
		list<Node*>::iterator itL;
		list<Node*>::iterator itL2;
		for (itL = _children.begin(); itL != _children.end(); ++itL)
		{
#ifdef DEBUG_NODE
			cout << "N*" << *itL << " vs N*" << exception << endl;
#endif //DEBUG_NODE

			if (*itL != exception)
			{
				if ((*itL)->_parents.size() < 2){
					(*itL)->killChildrens(exception);
					delete(*itL);
				}
				else
				{
					itL2 = find((*itL)->_parents.begin(), (*itL)->_parents.end(), this);
					(*itL)->_parents.erase(itL2);
				}
			}
#ifdef DEBUG_NODE
			else
			{
				cout << "N*" << exception << "is safe" << endl;
			}
#endif //DEBUG_NODE
		}
		_parents.clear();
		_children.clear();
	}

	void Node::addChild(Bitboard& state, Move& move, int terminal){
		Node* node = new Node(this, state, move);
		node->setTerminal(terminal);
		_children.push_back(node);
	};

	void Node::update(int win){
		list<Node*>::iterator itL;
		if (win != _state.getPlayer() && win != 3)
		{
			_wins += 1;
		}
		elseif(win == 3)
		{
			_wins += 0.5;
		}

		_visits += 1;
#ifdef DEBUG_NODE
		cout << "update N*" << this << ", " << _move << " : " << win << " = " << _wins << " & " << _visits << endl;
#endif //DEBUG_NODE
		if (!_parents.empty())
		{
			for (itL = _parents.begin(); itL != _parents.end(); ++itL)
			{
				(*itL)->update(win);
			}
		}
	}

	bool Node::compareUCT(Node* a, Node* b)
	{
		return (*a)._uct < (*b)._uct;
	}

	bool Node::compareWR(Node* a, Node* b)
	{
		return (*a).getProba() < (*b).getProba();
	}

	Node* Node::select_child_UCT(){

		list<Node*>::iterator itL;
		for (itL = _children.begin(); itL != _children.end(); ++itL)
		{
			(*itL)->UCT(max(_visits,1));
		}
		itL = max_element(_children.begin(), _children.end(), compareUCT);

		return *itL;
	}

	Node* Node::select_child_WR()
	{
		list<Node*>::iterator itL;
		itL = max_element(_children.begin(), _children.end(), compareWR);

		return *itL;
	}

	void Node::print_tree(int numtab, int depth)
	{
		if (depth >= 0)
		{
			string tabs = "";
			list<Node*>::iterator itL;
			for (int i = 0; i < numtab; ++i)
			{
				tabs += " ";
				if (i + 1 < numtab)
				{
					tabs += "|";
				}
			}
			cout << tabs << "-> N*" << this;
			cout << ", m: " << _move;
			cout << ", p: " << _state.getPlayer();
			cout << ", _t: " << _terminal;
			cout << ", _w/_v: " << _wins << "/" << _visits << " (" << round(getProba()*100) << "%)";
			cout << ", _uct: " << _uct;
			cout << endl;

			for (itL = _children.begin(); itL != _children.end(); ++itL)
			{
				(*itL)->print_tree(numtab + 1, depth - 1);
			}
		}
	}


}

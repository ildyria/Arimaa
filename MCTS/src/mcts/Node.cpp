#include "Node.h"
#define elseif else if
#include "../tools/Count.h"
#include "../tools/FreeObjects.h"
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
	Node::Node() : _visits(0), _wins(0), _terminal(-1), _uct(0), _state(nullptr), _move(Move())
	{
	}

	Node::Node(Bitboard* state) : _visits(0), _wins(0), _terminal(-1), _uct(0), _state(state), _move(Move())
	{
#ifdef DEBUG_NODE
		cout << endl << "N*" << this << " created with B*" << &state;
#endif //DEBUG_NODE
	}

	Node::Node(Node* p_parent, Bitboard* state, Move& move) : _visits(0), _wins(0), _terminal(-1), _uct(0), _state(state), _move(move)
	{
		_parents.push_front(p_parent);
#ifdef DEBUG_NODE
		cout << endl << "N*" << this << " created with P*" << p_parent << ", B*" << &state << " and move : " << move;
#endif //DEBUG_NODE
		Count::I()->addNode();
	}

	void Node::killChildrens(Node* exception)
	{
#ifdef DEBUG_NODE
		cout << endl << "destruction of N*" << this << " childrens";
#endif //DEBUG_NODE
		list<Node*>::iterator itL;
		list<Node*>::iterator itL2;
		for (itL = _children.begin(); itL != _children.end(); ++itL)
		{
#ifdef DEBUG_NODE
			cout << endl << "N*" << *itL << " vs N*" << exception;
#endif //DEBUG_NODE

			if (*itL != exception)
			{
				if ((*itL)->_parents.size() < 2){
					(*itL)->killChildrens(exception);
					(*itL)->unset();
					FreeObjects<Node>::I()->storeNode(*itL);
					//delete(*itL);
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
				cout << endl << "N*" << exception << "is safe";
			}
#endif //DEBUG_NODE
		}
		_parents.clear();
		_children.clear();
	}

	void Node::addChild(Bitboard* state, Move& move, int terminal){
		Node* node = FreeObjects<Node>::I()->getNode();
		node->set(state, move, this);
		node->setTerminal(terminal);
		_children.push_back(node);
	};

	void Node::set(Bitboard* state, Move& move, Node* parent)
	{
		_parents.push_front(parent);
		_move = move;
		_state = state;
		Count::I()->addNode();
	}

	void Node::unset()
	{
		_terminal = -1;
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_parents.clear();
		_children.clear();
		delete(_state);
		_state = nullptr;
		Count::I()->rmNode();
	}

	void Node::update(int win){
		list<Node*>::iterator itL;
		if (win != _state->getPlayer() && win != 3)
		{
			_wins += 1;
		}
		elseif(win == 3)
		{
			_wins += 0.5;
		}

		_visits += 1;
#ifdef DEBUG_NODE
		cout << endl << "update N*" << this << ", " << _move << " : " << win << " = " << _wins << " & " << _visits;
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
		return a->getProba() < b->getProba();
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
					tabs += " ";
				}
			}
			cout << endl << tabs << "-> " ;
			cout << _move;
//			cout << ", p: " << _state->getPlayer();
			cout << " (" << _terminal;
			cout << ", " << _visits << " (" << round(getProba()*100) << "%)";
			cout << ", " << _uct << ")";

			for (itL = _children.begin(); itL != _children.end(); ++itL)
			{
				(*itL)->print_tree(numtab + 1, depth - 1);
			}
		}
	}

	int Node::count()
	{
		int val = 0;
		list<Node*>::iterator itL;
		for (itL = _children.begin(); itL != _children.end(); ++itL)
		{
			val += (*itL)->count();
		}

		return val + 1;
	}

	Node::~Node()
	{
#ifdef DEBUG_NODE
		cout << endl << "kill N*" << this << ", " << _move << " : " << _state;
#endif //DEBUG_NODE
		delete _state;
		Count::I()->rmNode();
	}
}

#include "Node.h"
#define elseif else if
#include "../tools/Count.h"

using std::max_element;
using std::min_element;
using std::max;
using std::cout;
using std::endl;
using std::string;
using std::list;
using std::log;


namespace mcts {
	Node::Node() : _visits(0), _wins(0), _terminal(-1), _uct(0), _toplay(1), _move(Move()), _firstchild(nullptr), _nbchildren(0), _parent(nullptr), _addr(this), _lock(false)
	{
	}

	Node::Node(int player) : _visits(0), _wins(0), _terminal(-1), _uct(0), _toplay(player), _move(Move()), _firstchild(nullptr), _nbchildren(0), _parent(nullptr), _addr(this), _lock(false)
	{
	}

	Node::Node(Node* p_parent, int player, Move& move) : _visits(0), _wins(0), _terminal(-1), _toplay(player), _move(move), _firstchild(nullptr), _nbchildren(0), _parent(p_parent), _addr(this), _lock(false)
	{
	}

	void Node::set(Move& move, Node* parent)
	{
		_parent = parent;
		_move = move;
	}

	void Node::unset()
	{
		_terminal = -1;
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_parent = nullptr;
		_firstchild = nullptr;
		_nbchildren = 0;
		_toplay = 1;
	}

	void Node::update(int win){
		if (win != _toplay && win != 3)
		{
			_wins += 1;
		}
		elseif(win == 3)
		{
			_wins += 0.5;
		}

		_visits += 1;
		if (_parent != nullptr)
		{
			_parent->update(win);
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

		Node* max = _firstchild;
		Node* itL = _firstchild;
		for (int i = 0; i < _nbchildren; ++i)
		{
			itL->UCT(max(_visits,1));
			if (compareUCT(max, itL))
			{
				max = itL;
				if (max->getUCT() == 10) return max; // quicker get out because there is nothing better to be played (winning move)
			}
			itL++;
		}
		return max;
	}

	Node* Node::select_child_WR()
	{
		Node* max = _firstchild;
		Node* itL = _firstchild;
		for (int i = 0; i < _nbchildren; ++i)
		{
			if (compareWR(max, itL))
			{
				max = itL;
				if (max->getProba() == 2) return max; // quicker get out because there is nothing better to be played (winning move)
			}
			itL++;
		}
		return max;
	}

	void Node::print_tree(int numtab, int depth)
	{
		if (depth >= 0)
		{
			string tabs = "";
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
			cout << " (" << _terminal;
			cout << ", " << _visits << " (" << round(getProba()*100) << "%)";
			cout << ", " << _uct << ")";

			auto itL = _firstchild;
			for (int i = 0; i < _nbchildren; ++i)
			{
				itL->print_tree(numtab + 1, depth - 1);
				itL++;
			}
		}
	}

	int Node::count()
	{
		auto val = 0;
		auto itL = _firstchild;
		for (int i = 0; i < _nbchildren; ++i)
		{
			val += itL->count();
			itL++;
		}

		return val + 1;
	}

	int Node::max_depth()
	{
		auto val = 0;
		auto itL = _firstchild;
		for (int i = 0; i < _nbchildren; ++i)
		{
			auto cur = itL->max_depth();
			if (val < cur) val = cur;
			itL++;
		}
		return val + 1;
	}

	Node::~Node()
	{
	}
}

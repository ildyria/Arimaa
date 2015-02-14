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
	Node::Node() : _uct(0), _visits(0), _wins(0), _nbchildren(0), _toplay(1), _terminal(static_cast<char>(64)), _lock(false), _move(Move()), _firstchild(nullptr)
	{
	}

	Node::Node(u_short player) : _uct(0), _visits(0), _wins(0), _nbchildren(0), _toplay(player), _terminal(static_cast<char>(64)), _lock(false), _move(Move()), _firstchild(nullptr)
	{
	}

	Node::Node(u_short player, Move& move) : _visits(0), _wins(0), _nbchildren(-1), _toplay(player), _terminal(static_cast<char>(64)), _lock(false), _move(move), _firstchild(nullptr)
	{
	}

	void Node::set(Move& move)
	{
		_lock = false;
		setHasParent();
		_nbchildren = 0;
		_move = move;
	}

	void Node::unset()
	{
		_lock = false;
		clearParent();
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_firstchild = nullptr;
		_nbchildren = 0;
		_toplay = 1;
	}

	bool Node::compareUCT(Node* a, Node* b)
	{
		return (*a)._uct < (*b)._uct;
	}

	bool Node::compareWR(Node* a, Node* b)
	{
		return a->getProba() < b->getProba();
	}

	Node* Node::select_child_UCT()
	{

		Node* max = _firstchild;
		Node* itL = _firstchild;
		for (u_int i = 0; i < _nbchildren; ++i)
		{
			itL->UCT((_visits < 2) ? 2 : _visits);
			if (compareUCT(max, itL))
			{
				max = itL;
				if (max->getUCT() == 42) return max; // quicker get out because there is nothing better to be played (winning move)
			}
			itL++;
		}
		return max;
	}

	Node* Node::select_child_WR()
	{
		Node* max = _firstchild;
		Node* itL = _firstchild;
		for (u_int i = 0; i < _nbchildren; ++i)
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

	void Node::print_tree(u_int numtab,u_int depth)
	{
		if (depth > 0)
		{
			string tabs = "";
			for (u_int i = 0; i < numtab; ++i)
			{
				tabs += " ";
				if (i + 1 < numtab)
				{
					tabs += " ";
				}
			}
			cout << endl << tabs << "-> " ;
			cout << _move;
			cout << " (" << (_terminal & 255);
			cout << ", " << _visits/2 << " (" << round(getProba()*100) << "%)";
			cout << ", " << _uct << ")";

			auto itL = _firstchild;
			for (u_int i = 0; i < _nbchildren; ++i)
			{
				itL->print_tree(numtab + 1, depth - 1);
				itL++;
			}
		}
	}

/*	u_int Node::max_depth()
	{
		if (_firstchild == nullptr) return 1;

		auto itL = _firstchild;
		u_int val = 0;
		for (u_int i = 0; i < _nbchildren; ++i)
		{
			auto cur = itL->max_depth();
			if (val < cur) val = cur;
			itL++;
		}
		return val + 1;
	}*/

	Node::~Node()
	{
	}
}

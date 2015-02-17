#include "Node.h"

using std::cout;
using std::endl;
using std::string;
using std::list;


namespace mcts {
	Node::Node() : _uct(0), _visits(0), _wins(0), _nbchildren(0), _toplay(1), _terminal(static_cast<unsigned char>(0x40)), _lock(false), _move(Move()), _firstchild(nullptr)
	{
#if !defined(DOUBLE_TREE)
		_parent = nullptr;
#endif
	}

	Node::Node(u_short player) : _uct(0), _visits(0), _wins(0), _nbchildren(0), _toplay(player), _terminal(static_cast<unsigned char>(0x40)), _lock(false), _move(Move()), _firstchild(nullptr)
	{
#if !defined(DOUBLE_TREE)
		_parent = nullptr;
#endif
	}

	Node::Node(u_short player, Move& move) : _visits(0), _wins(0), _nbchildren(-1), _toplay(player), _terminal(static_cast<unsigned char>(0x40)), _lock(false), _move(move), _firstchild(nullptr)
	{
#if !defined(DOUBLE_TREE)
		_parent = nullptr;
#endif
	}

#if defined(DOUBLE_TREE)
	void Node::set(Move& move)
#else
	void Node::set(Move& move, Node* parent)
#endif
	{
		_lock = false;
		setHasParent();
#if !defined(DOUBLE_TREE)
		_parent = parent;
#endif
		_nbchildren = 0;
		_move = move;
	}

	void Node::unset()
	{
		_lock = false;
		clearParent();
#if !defined(DOUBLE_TREE)
		_parent = nullptr;
#endif
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_firstchild = nullptr;
		_nbchildren = 0;
		_toplay = 1;
	}

	Node* Node::select_child_UCT()
	{

		Node* max = _firstchild;
		Node* itL = _firstchild;
		// save the value : we can't have the risk of it being modified during the selection
		int visit = _visits;
		for (u_int i = 0; i < _nbchildren; ++i)
		{
			itL->UCT(visit);
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

	Node::~Node()
	{
	}
}

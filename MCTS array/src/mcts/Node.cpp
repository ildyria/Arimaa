#include "Node.h"

using std::cout;
using std::endl;
using std::string;


namespace mcts {
	Node::Node() : _uct(0), _visits(0), _wins(0), _nbchildren(0), _toplay(1), _terminal(static_cast<unsigned char>(128)), _lock(false), _move(Move()), _firstchild(nullptr)
#if !defined(DOUBLE_TREE)
		, _self(nullptr)
#endif
	{
	}

	Node::Node(u_short player) : _uct(0), _visits(0), _wins(0), _nbchildren(0), _toplay(player), _terminal(static_cast<unsigned char>(128)), _lock(false), _move(Move()), _firstchild(nullptr)
#if !defined(DOUBLE_TREE)
		, _self(nullptr)
#endif
	{
	}

	Node::Node(u_short player, Move& move) : _visits(0), _wins(0), _nbchildren(-1), _toplay(player), _terminal(static_cast<unsigned char>(128)), _lock(false), _move(move), _firstchild(nullptr)
#if !defined(DOUBLE_TREE)
		, _self(nullptr)
#endif
	{
	}

#if defined(DOUBLE_TREE)
	void Node::set(Move& move)
#else
	void Node::set(Move& move, Node** self)
#endif
	{
		_lock = false;
		set_has_parent();
#if !defined(DOUBLE_TREE)
		_self = self;
		*self = this;
#endif
		_nbchildren = 0;
		_move = move;
	}

	void Node::unset()
	{
		_lock = false;
		clear_parent();
#if !defined(DOUBLE_TREE)
		_self = nullptr;
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
#if defined(DOUBLE_TREE)
		Node* max = _firstchild;
		Node* itL = _firstchild;
#else
		Node* max = *_firstchild;
		Node* itL = *_firstchild;
#endif
		// save the value : we can't have the risk of it being modified during the selection
		int visit = _visits;
		for (u_int i = 0; i < _nbchildren; ++i)
		{
			itL->UCT(visit);
			if (compare_UCT(max, itL))
			{
				max = itL;
				if (max->get_UCT() == 42) return max; // quicker get out because there is nothing better to be played (winning move)
			}
			itL++;
		}
		return max;
	}

	Node* Node::select_child_WR()
	{
#if defined(DOUBLE_TREE)
		Node* max = _firstchild;
		Node* itL = _firstchild;
#else
		Node* max = *_firstchild;
		Node* itL = *_firstchild;
#endif
		for (u_int i = 0; i < _nbchildren; ++i)
		{
			if (compare_WR(max, itL))
			{
				max = itL;
				if (max->get_proba() == 2) return max; // quicker get out because there is nothing better to be played (winning move)
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
			cout << ", " << _visits/2 << " (" << round(get_proba()*100) << "%)";
			cout << ", " << _uct << ")";

#if defined(DOUBLE_TREE)
			Node* itL = _firstchild;
#else
			Node* itL = *_firstchild;
#endif
			for (u_int i = 0; i < _nbchildren; ++i)
			{
				itL->print_tree(numtab + 1, depth - 1);
				itL++;
			}
		}
	}

	u_long Node::count()
	{
		if (_terminal != 0) return 1;
#if defined(DOUBLE_TREE)
		auto itL = _firstchild;
#else
		auto itL = *_firstchild;
#endif
		auto val = 0;
		for (u_int i = 0; i < _nbchildren; ++i)
		{
			val += itL->count();
			itL++;
		}
		return val + 1;
	}

	Node::~Node()
	{
	}
}

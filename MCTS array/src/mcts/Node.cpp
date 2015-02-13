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
	Node::Node() : _uct(0), _visits(0), _wins(0), _nbchildren(0), _toplay(1), _terminal(static_cast<char>(255)), _lock(false), _move(Move()), _firstchild(nullptr), _parent(nullptr)//, _addr(this)
	{
	}

	Node::Node(unsigned short player) : _uct(0), _visits(0), _wins(0), _nbchildren(0), _toplay(player), _terminal(static_cast<char>(255)), _lock(false), _move(Move()), _firstchild(nullptr), _parent(nullptr)//, _addr(this)
	{
	}

	Node::Node(Node* p_parent, unsigned short player, Move& move) : _visits(0), _wins(0), _nbchildren(0), _toplay(player), _terminal(static_cast<char>(255)), _lock(false), _move(move), _firstchild(nullptr), _parent(p_parent)//, _addr(this)
	{
	}

	void Node::set(Move& move, Node* parent)
	{
		_lock = false;
		_parent = parent;
		_move = move;
	}

	void Node::unset()
	{
		_lock = false;
		_terminal = static_cast<char>(255);
		_visits = 0;
		_wins = 0;
		_uct = 0;
		_parent = nullptr;
		_firstchild = nullptr;
		_nbchildren = 0;
		_toplay = 1;
	}

	void Node::update(unsigned int win){
		if (win != _toplay && win != 3)
		{
			_wins += 2;
		}
		elseif(win == 3)
		{
			_wins += 1;
		}

//		_visits += 1;
		if (_parent != nullptr)
		{
			take_a_chill_pill(0); // HELLO LEEK !
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

	Node* Node::select_child_UCT()
	{

		Node* max = _firstchild;
		Node* itL = _firstchild;
		for (unsigned int i = 0; i < _nbchildren; ++i)
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
		for (unsigned int i = 0; i < _nbchildren; ++i)
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

	void Node::print_tree(unsigned int numtab,unsigned int depth)
	{
		if (depth > 0)
		{
			string tabs = "";
			for (unsigned int i = 0; i < numtab; ++i)
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
			for (unsigned int i = 0; i < _nbchildren; ++i)
			{
				itL->print_tree(numtab + 1, depth - 1);
				itL++;
			}
		}
	}

	unsigned int Node::count()
	{
		if (_firstchild == nullptr) return 1;

		auto itL = _firstchild;
		auto val = 0;
		for (unsigned int i = 0; i < _nbchildren; ++i)
		{
			val += itL->count();
			itL++;
		}

		return val + 1;
	}

	unsigned int Node::max_depth()
	{
		if (_firstchild == nullptr) return 1;

		auto itL = _firstchild;
		unsigned int val = 0;
		for (unsigned int i = 0; i < _nbchildren; ++i)
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

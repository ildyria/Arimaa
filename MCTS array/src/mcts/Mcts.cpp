#include "Mcts.h"

using std::list;
using std::cout;
using std::endl;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

namespace mcts{

	Mcts::Mcts(	TheGame* game,
				Bitboard* Bb,
				MctsArgs* args)
				:_game(game),
				_param(args),
				_next(nullptr),
				_state(Bb),
				_parents(std::vector<Memento<Node*>> (omp_get_num_procs(), Memento<Node*>(args->getDepth() + 1))),
				_tree(std::vector<Node>(args->getMaxNumberOfLeaves()))
#if defined(DOUBLE_TREE)
				, _buff(std::vector<Node>(args->getMaxNumberOfLeaves()))
#else
				, _index(Tree_index<Node>(args->getMaxNumberOfLeaves()))
#endif
	{
		cout << "number of thread : " << omp_get_num_procs() << endl;
		// #pragma omp parallel
		// {
		// 	int tid = omp_get_thread_num();
		// 	printf("Hello World from thread = %d\n", tid);
		// }
		omp_init_lock(&_lockNode);
		omp_init_lock(&_lockNext);
		Tree<Node>::cleanTree(_tree);
#if defined(DOUBLE_TREE)
		Tree<Node>::cleanTree(_buff);
#endif
		Count::I()->clear();
		Move* random = new Move();
#if defined(DOUBLE_TREE)
		_tree[0].set(*random);
#else
		Node** address = _index.get();
		_tree[0].set(*random, address);
#endif 
		_tree[0].hasParent();
		_next = (&_tree[0]) + 1;
	}

	Bitboard* Mcts::movePlayed(Move& move)
	{
		UpdateNode(&_tree[0], _state);
		auto ListOfNodes = _tree[0].getChildren();
		auto iter = ListOfNodes.first;
		for (u_int i = 0; i < ListOfNodes.second; ++i)
		{
			if (iter->getMove() == move) // we got the move played !
			{
				break;
			}
			iter++;
		}

#if defined(DOUBLE_TREE)
		Tree<Node>::execute(iter, _tree, _buff, _next);
#else
		Tree<Node>::execute(iter, _tree, _index, _next);
#endif
		_game->play(move, _state);
		return _state;
	}

	u_int Mcts::UpdateNode(Node* node, Bitboard* Bb)
	{
		u_int nodet = node->getTerminal();

		if (nodet == 64) { // first time we are here, check if terminate
			nodet = _game->end(Bb);
			if ((nodet & 7) > 0)
			{
				node->setTerminal(nodet);  // terminal node : winner or tie
			}
			else
			{
				node->setTerminal(32);	// non terminal but not explored YET
				nodet = 32;
			}
		}
		return nodet;
	}

	void Mcts::Expand(Node* node, Bitboard* Bb, u_int& nodet) {
		if (nodet != 32)
		{
			return;
		}
		bool locked;

		// super small critical region =D
//		if (omp_in_parallel())
//		{
//			#pragma omp critical
			omp_set_lock(&_lockNode);
			locked = node->getLock();
			omp_unset_lock(&_lockNode);
//		}

		if (locked)
		{
			nodet = 16;
			return;
		}
		if (node == &_tree[0]) printf("expanding\n");
		list<Move> ListOfMoves;
		list<Move>::iterator iter;

		ListOfMoves = _game->listPossibleMoves(Bb);
		if (&_tree[0] + _param->getMaxNumberOfLeaves() < _next + ListOfMoves.size())
		{
			node->setTerminal(16);	// non terminal but not explored YET
			nodet = 16;
			return; // this is extremly dangerous : the node stays locked ! We need to unlock it when we will prune the tree
		}
		Node* tmp2;
		Node* tmp;

		// _next concurency
		omp_set_lock(&_lockNext);
		tmp2 = _next;
		_next += ListOfMoves.size();
		omp_unset_lock(&_lockNext);
#if !defined(DOUBLE_TREE)
		Node** buff;
#endif
		for (iter = ListOfMoves.begin(), tmp = tmp2; iter != ListOfMoves.end(); ++iter, ++tmp2)
		{
#if defined(DOUBLE_TREE)
			tmp2->set(*iter);
#else
			buff = _index.get();
			tmp2->set(*iter, buff);
#endif
			tmp2->play(node->getPlayer());
		}
		node->setChildrens(tmp, static_cast<u_int>(ListOfMoves.size())); // update at the end, concurency race...
		nodet = 0;
		node->releaseLock();

		return;
	}

	void Mcts::playRandom(Node* node, Bitboard* Bb)
	{
		u_int nodet;

		for (u_int i = 0; i < _param->getSimulationPerLeaves(); ++i)
		{
			Bitboard* Bb2 = Bb->clone();
			nodet = _game->playRandomMoves(Bb2);
			delete Bb2;
			feedback(nodet);
		}
	}

	void Mcts::feedback(u_int nodet)
	{
		_parents[omp_get_thread_num()].rewind();
		Node* n = _parents[omp_get_thread_num()].get();
		while(n != nullptr)
		{
			n->update(nodet);
			n = _parents[omp_get_thread_num()].get();
		}
	}

	void Mcts::feedbackWinLose()
	{
		_parents[omp_get_thread_num()].rewind();

		Node* n = _parents[omp_get_thread_num()].get(); // take the second
		n->forceSetUCT(42);

		n = _parents[omp_get_thread_num()].get();
		bool winloser = true;
		bool loser = true;
		Node* ptr;
		std::pair<Node*, u_int> ListChilds;
		while(n != nullptr && winloser)
		{
			if(loser)
			{
				n->forceSetUCT(-1);
				loser = false;
			}
			else
			{
				loser = true;
				ListChilds = n->getChildren();
				ptr = ListChilds.first;
				for (u_int i = 0; (i < ListChilds.second && loser); ++i)
				{
					if (ptr->getUCT() != -1) loser = false;
					ptr++;
				}

				if (loser)
				{
					n->forceSetUCT(42);
				}
				else
				{
					winloser = false;
				}
			}
			n = _parents[omp_get_thread_num()].get();
		}
	}

	void Mcts::explore()
	{
		Node* node = &_tree[0];
		u_int depth = 0;
		Bitboard* Bb = _state->clone();
		node->addVirtualLoss(_param->getSimulationPerLeaves());
		u_int nodet = UpdateNode(node, Bb);

		_parents[omp_get_thread_num()].reset();
		_parents[omp_get_thread_num()].push(&_tree[0]);
		Move m;
		while (depth < _param->getDepth() && (nodet == 0 || nodet > 16))
		{
			/*
			10000000 : has no parents				: 2^7 = 128
			01000000 : not visited					: 2^6 = 64
			00100000 : non terminal not explored	: 2^5 = 32
			00010000 : non terminal locked			: 2^4 = 16
			00001000 :
			00000100 : tie							: 2^2 = 4
			00000010 : player 2 wins				: 2^1 = 2
			00000001 : player 1 wins				: 2^0 = 1
			00000000 : non terminal explored		: 0
			*/
			if (nodet == 32)
			{
				if (node->getVisits() > _param->getNumberOfVisitBeforeExploration())
				{
					Expand(node, Bb, nodet);
					if (nodet == 16) break;
				}
				else
				{
					break;
				}
			}

			node = node->select_child_UCT();
			_parents[omp_get_thread_num()].push(node);
			node->addVirtualLoss(_param->getSimulationPerLeaves());
			m = node->getMove();
			_game->play(m, Bb);
			nodet = UpdateNode(node, Bb);
			++depth;
		}

		if (nodet == 0 || nodet > 4) // no victory found yet
		{
			playRandom(node, Bb);
		}
//		elseif(nodet < 4 && nodet > 0 && nodet != node->getPlayer()) // losing move !
		elseif((nodet | node->getPlayer()) == 3) // losing move !
		{
			feedback(nodet);
			feedbackWinLose();
		}
		else
		{
			feedback(nodet);
		}
		delete Bb;
	}

	Move Mcts::GetBestMove()
	{
		u_long i = 0;
		auto start_time = high_resolution_clock::now();
		auto end_time = start_time + milliseconds(_param->getTimeLimitSimulationPerRoot());
#ifdef OPENMP
		#pragma omp parallel shared(i,end_time)
		{
#endif
		while (high_resolution_clock::now() < end_time && i < _param->getSimulationPerRoot())
		{
			i++;
			explore();
		}
#ifdef OPENMP
	#pragma omp barrier
	}
#endif
		cout << endl << "Searched for " << Count::format(duration_cast<milliseconds>(high_resolution_clock::now() - start_time).count()) << "ms and " << Count::format(i*_param->getSimulationPerLeaves()) << " simulations." << endl;
		return _tree[0].select_child_WR()->getMove();
	}

	void Mcts::print_tree(u_int depth)
	{
		auto d = (depth == 0) ? _param->getDepth() : depth;
		_tree[0].print_tree(0, d);
	}

	void Mcts::kill_tree()
	{
		_tree.clear();
	}

	void Mcts::get_Number_Leaves()
	{
		auto num = _next - &_tree[0];
		Count::I()->saveNbLeaves(static_cast<int>(num));
//		Count::I()->saveMaxDepth(_tree[0].max_depth());
	}

	double Mcts::winning_Strategy()
	{
		return (_tree[0].getUCT() != 42 && _tree[0].getUCT() != -1) ? _tree[0].getProba() : _tree[0].getUCT();
	}
}
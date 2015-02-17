#define OPENMP

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
				_tree(std::vector<Node>(args->getMaxNumberOfLeaves())),
				_buff(std::vector<Node>(args->getMaxNumberOfLeaves()))
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
		Tree<Node>::cleanTree(_buff);
		Count::I()->clear();
		Move* random = new Move();
		_tree[0].set(*random);
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
		Tree<Node>::execute(iter, _tree, _buff, _next);

		_game->play(move, _state);
		return _state;
	}

	u_int Mcts::UpdateNode(Node* node, Bitboard* Bb)
	{
		bool locked;
		u_int nodet = node->getTerminal();

		if (nodet == 128) { // first time we are here, check if terminate
			nodet = _game->end(Bb);
			node->setTerminal(nodet);
		}

		if (nodet > 0)
		{
			return nodet;
		}

		if (nodet == 0 && node->getChildren().first != nullptr)
		{
			return nodet;
		}

/*
		if (node->getChildren().first == nullptr) // not explored yet => explore
		{
*/
			// super small critical region =D
/*
			if (omp_in_parallel())
			{
*/
//				#pragma omp critical
				omp_set_lock(&_lockNode);
				locked = node->getLock();
				omp_unset_lock(&_lockNode);
//			}
			if (locked) return 255;

			list<Move> ListOfMoves;
			list<Move>::iterator iter;

			ListOfMoves = _game->listPossibleMoves(Bb);
			if (&_tree[0] + _param->getMaxNumberOfLeaves() < _next + ListOfMoves.size())
			{
				return 255; // this is extremly dangerous : the node stays locked ! We need to unlock it when we will prune the tree
			}
			Node* tmp2;
			Node* tmp;

			// _next concurency
			omp_set_lock(&_lockNext);
			tmp2 = _next;
			_next += ListOfMoves.size();
			omp_unset_lock(&_lockNext);

			//				tmp = tmp2;
			for (iter = ListOfMoves.begin(), tmp = tmp2; iter != ListOfMoves.end(); ++iter, ++tmp2)
			{
				tmp2->set(*iter);
				tmp2->play(node->getPlayer());
				//					tmp2++;
			}
			node->setChildrens(tmp, static_cast<u_int>(ListOfMoves.size())); // update at the end, concurency race...
			node->releaseLock();
//		}

		return nodet;
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
		u_int nodet = UpdateNode(node, Bb); // update and exploration
		node->addVirtualLoss(_param->getSimulationPerLeaves());

		_parents[omp_get_thread_num()].reset();
		_parents[omp_get_thread_num()].push(&_tree[0]);

		while (depth < _param->getDepth() && (nodet == 0 || nodet > 3) && node->getVisits() > _param->getNumberOfVisitBeforeExploration() && nodet != 255)
		{
			++depth;
			node = node->select_child_UCT();
			_parents[omp_get_thread_num()].push(node);

			node->addVirtualLoss(_param->getSimulationPerLeaves());
			auto m = node->getMove();
			_game->play(m, Bb);
			nodet = UpdateNode(node,Bb);
		}

		if (nodet == 0 || nodet > 3) // no victory found yet
		{
			playRandom(node, Bb);
		}
		elseif(nodet < 3 && nodet > 0 && nodet != node->getPlayer()) // losing move !
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
#endif
		while (high_resolution_clock::now() < end_time && i < _param->getSimulationPerRoot())
		{
			i++;
			explore();
		}
		cout << endl << "Searched for " << duration_cast<milliseconds>(high_resolution_clock::now()-start_time).count() << "ms and " << i*_param->getSimulationPerLeaves() << " simulations." << endl;
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
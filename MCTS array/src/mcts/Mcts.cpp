#define OPENMP

#include "Mcts.h"
#define elseif else if
#define SLOW 0 // 5 ms 
// #include <time.h>
#include <chrono>

//#define DEBUG_MCTS
//#define DISPLAY_MCTS

using std::list;
using std::cout;
using std::endl;

namespace mcts{

	Mcts::Mcts()
	{
	}

	Mcts::Mcts(	TheGame* game,
				Bitboard* Bb,
				MctsArgs* args)
				:_game(game),
				_tree(std::vector<Node>(args->getMaxNumberOfLeaves())),
				_buff(std::vector<Node>(args->getMaxNumberOfLeaves())),
				_parents(std::vector<std::pair<std::vector<Node*>,unsigned int>> (omp_get_num_procs(), std::pair<std::vector<Node*>,unsigned int>(std::vector<Node*>(args->getDepth() + 1, nullptr),0))),
				_param(args),
				_state(Bb),
				_maxdepthreached(false)
	{
		cout << "number of thread : " << omp_get_num_procs() << endl;
		// #pragma omp parallel
		// {
		// 	int tid = omp_get_thread_num();
		// 	printf("Hello World from thread = %d\n", tid);
		// }

		cleanTree(_tree);
		cleanTree(_buff);
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
		for (unsigned int i = 0; i < ListOfNodes.second; ++i)
		{
			if (iter->getMove() == move) // we got the move played !
			{
				auto m = iter->getMove();
				cout << "move played by ... : " << m << endl;
				break;
			}
			iter++;
		}
		copyTree(iter,_buff);
		copyTree(&_buff[0], _tree);
		findNext(_tree);
/*
		_buff[0].setHasParent();		// set root parent to null
		_tree[0].setHasParent();		// set root parent to null
*/

		_game->play(move, _state);
		return _state;
	}

	unsigned int Mcts::UpdateNode(Node* node, Bitboard* Bb)
	{
		bool locked = false;
		unsigned int nodet = node->getTerminal();

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


		if (node->getChildren().first == nullptr) // not explored yet => explore
		{
			// super small critical region =D
			if (omp_in_parallel())
			{
				#pragma omp critical
				locked = node->getLock();
			}
			if (locked) return 255;

			list<Move> ListOfMoves;
			list<Move>::iterator iter;

			// will need to rethink the concurency on the _next variable... might be ugly...
			ListOfMoves = _game->listPossibleMoves(Bb);
			if (&_tree[0] + _param->getMaxNumberOfLeaves() < _next + ListOfMoves.size())
			{
				nodet = 254;
			}
			else
			{
				auto tmp = _next;
				auto tmp2 = _next;
				_next += ListOfMoves.size();
				for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); ++iter)
				{
					tmp2->set(*iter);
					tmp2->play(node->getPlayer());
					tmp2++;
				}
				node->setChildrens(tmp, static_cast<unsigned int>(ListOfMoves.size())); // update at the end, concurency race...
			}
			node->releaseLock();
		}

		return nodet;
	}

	void Mcts::playRandom(Node* node, Bitboard* Bb)
	{
		unsigned int nodet;

		for (unsigned int i = 0; i < _param->getSimulationPerLeaves(); ++i)
		{
			Bitboard* Bb2 = Bb->clone();
			nodet = _game->playRandomMoves(Bb2);
			delete Bb2;
			feedback(nodet);
			//node->update(nodet);
		}
	}

	void Mcts::feedback(unsigned int nodet)
	{
		for (int i = _parents[omp_get_thread_num()].second; i >= 0; --i)
		{
			_parents[omp_get_thread_num()].first[i]->update(nodet);
		}
	}

	void Mcts::explore()
	{
		Node* node = &_tree[0];
		unsigned int depth = 0;
		Bitboard* Bb = _state->clone();
		unsigned int nodet = UpdateNode(node, Bb); // update and exploration
		node->addVirtualLoss(_param->getSimulationPerLeaves());

		_parents[omp_get_thread_num()].first[depth] = &_tree[0];
		_parents[omp_get_thread_num()].second = 0;

		while (depth < _param->getDepth() && (nodet == 0 || nodet > 3) && node->getVisits() > _param->getNumberOfVisitBeforeExploration() && nodet != 255)
		{
			++depth;
			node = node->select_child_UCT();
			// record the parent => should make a function for it.
			_parents[omp_get_thread_num()].first[depth] = node;
			_parents[omp_get_thread_num()].second = depth;

			node->addVirtualLoss(_param->getSimulationPerLeaves());
			auto m = node->getMove();
			_game->play(m, Bb);
			nodet = UpdateNode(node,Bb);
		}

		// if (depth == _param->getDepth() && !_maxdepthreached)
		// {
		// 	_maxdepthreached = true;
		// 	cout << endl << "max depth reached : " << _param->getDepth() << "moves ahead.";
		// }

		if (nodet == 0 || nodet > 3) // no victory found yet
		{
			playRandom(node, Bb);
		}
		elseif(nodet < 3 && nodet > 0 && nodet != node->getPlayer()) // losing move !
		{
			node->forceSetUCT(42);
			// feedbackWinningMove(node);
			feedback(nodet);
//			node->update(nodet);
		}
		else
		{
			feedback(nodet);
			// node->update(nodet);
		}
		delete Bb;
	}

/*	void Mcts::updateLosingParent(Node* node) // for each parent check if uct = -1 => if yes it's a winning move for the opponent, he has to play here : uct = 42
	{
		bool loser = true;
		if (node->getParent() == nullptr) return; // root hahaha
		auto ListChilds = node->getParent()->getChildren();
		Node* ptr = ListChilds.first;
		for (unsigned int i = 0; (i < ListChilds.second && loser); ++i)
		{
			if (ptr->getUCT() != -1) loser = false;
			ptr++;
		}
		if (loser)
		{
			node->getParent()->forceSetUCT(42);
			feedbackWinningMove(node->getParent());
		}
	}

	void Mcts::feedbackWinningMove(Node* node) // if someone has a winning move given a position, we must not go to that position.
	{
		if (node->getUCT() != 42 || node->getParent() == nullptr) return;

		node->getParent()->forceSetUCT(-1);
		updateLosingParent(node->getParent());
	}*/

	void Mcts::cleanTree(std::vector<Node> &T)
	{
		Node* ptr = &T[1];
		Node* lstptr = &T[(_param->getMaxNumberOfLeaves() -1)];
		while (ptr != lstptr)// && ptr->getChildren().second != static_cast<unsigned int>(-1))
		{
			ptr->unset();
			ptr++;
		}
		cout << "clean tree : " << (ptr - &T[0]) << endl;
	}

	void Mcts::copyTree(Node* NewRoot, std::vector<Node> &Tdest)
	{
		cleanTree(Tdest);
		auto next = &Tdest[0];					// place ptrDest at the begining of the destination array
		auto ptrDest = &Tdest[0];				// place ptrDest at the begining of the destination array
		Node* ptTemp;							// pointer to loop on the childrens of each ptr

		*ptrDest = *NewRoot;					// copy root
		next++;									// next free space in Tdest

		while (ptrDest->hasParent() || ptrDest == &Tdest[0]) // no parents OR root
		{
			auto ListOfNodes = ptrDest->getChildren();
			ptTemp = ListOfNodes.first;
			if (ptTemp != nullptr)				// if children add them to _buff
			{
				ptrDest->setChildrens(next, ListOfNodes.second); // update the node of the parents
				for (unsigned int i = 0; i < ListOfNodes.second; i++)
				{
					*next = *ptTemp;			// recopy of chidlren
					ptTemp++;
					next++;
				}
			}
			ptrDest++;
		}
	}

	void Mcts::findNext(std::vector<Node>& T)
	{
		_next = &T[1];
		while (_next->hasParent())
		{
			_next++;
		}
		cout << "next: " << (_next - &T[0]) << endl;
	}


	Move Mcts::GetBestMove()
	{
		_maxdepthreached = false;
		unsigned long i = 0;
		auto start_time = std::chrono::high_resolution_clock::now();
		auto end_time = start_time + std::chrono::milliseconds(_param->getTimeLimitSimulationPerRoot());
#ifdef OPENMP
#pragma omp parallel shared(i,end_time)
#endif
		while (std::chrono::high_resolution_clock::now() < end_time && i < _param->getSimulationPerRoot())
		{
			i++;
			explore();
		}
		cout << endl << "Searched for " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start_time).count() << "ms and " << i*_param->getSimulationPerLeaves() << " simulations." << endl;
		return _tree[0].select_child_WR()->getMove();
	}

	void Mcts::print_tree(unsigned int depth)
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
//		uint64_t num;
		auto num = _next - &_tree[0];

		Count::I()->saveNbLeaves(static_cast<int>(num));
//		Count::I()->saveMaxDepth(_tree[0].max_depth());

	}

	double Mcts::winning_Strategy()
	{
		return (_tree[0].getUCT() != 42 && _tree[0].getUCT() != -1) ? _tree[0].getProba() : _tree[0].getUCT();
	}

	void Mcts::take_a_chill_pill(unsigned long i)
	{
		// struct timespec tim, tim2;
		// tim.tv_sec = 0;
		// tim.tv_nsec = SLOW; // 1 ms sec
		// nanosleep(&tim , &tim2);
		printf("%d : %ld\n", omp_get_thread_num(),i);
	}
}
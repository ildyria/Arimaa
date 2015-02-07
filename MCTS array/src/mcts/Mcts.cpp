#define OPENMP

#include "Mcts.h"
#define elseif else if
#define OPTIMIZED_MCTS
#include <time.h>

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
				_param(args),
				_state(Bb),
				_maxdepthreached(false)
	{
		cleanTree(_tree);
		cleanTree(_buff);
		Count::I()->clear();
		Move* random = new Move();
		_tree[0].set(*random, nullptr);
		_next = (&_tree[0]) + 1;
	}

	Bitboard* Mcts::movePlayed(Move& move)
	{
		UpdateNode(&_tree[0], _state);
		auto ListOfNodes = _tree[0].getChildren();
		auto iter = ListOfNodes.first;
		for (auto i = 0; i < ListOfNodes.second; ++i)
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
		_buff[0].setParent(nullptr);		// set root parent to null
		_tree[0].setParent(nullptr);		// set root parent to null

		_game->play(move, _state);
		return _state;
	}

	int Mcts::UpdateNode(Node* node, Bitboard* Bb)
	{
		bool locked = false;
		int nodet = node->getTerminal();

		if (nodet == -1) { // first time we are here, check if terminate
			nodet = _game->end(Bb);
			node->setTerminal(nodet);
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
			if (locked) return -2;

			list<Move> ListOfMoves;
			list<Move>::iterator iter;

			ListOfMoves = _game->listPossibleMoves(Bb);
			if (&_tree[0] + _param->getMaxNumberOfLeaves() < _next + ListOfMoves.size())
			{
				nodet = -2;
			}
			else
			{
				auto tmp = _next;
				for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); ++iter)
				{
					_next->set(*iter, node);
					_next->play(node->getPlayer());
					_next++;
				}
				node->setChildrens(tmp, static_cast<int>(ListOfMoves.size())); // update at the end, concurency race...
			}
			node->releaseLock();
		}

		return nodet;
	}

	void Mcts::playRandom(Node* node, Bitboard* Bb)
	{
		int nodet;

		for (int i = 0; i < _param->getSimulationPerLeaves(); ++i)
		{
#ifdef DEBUG_MCTS
			cout << endl << "round n " << (i + 1) << endl;
#endif // DEBUG_MCTS
			Bitboard* Bb2 = Bb->clone();
			nodet = _game->playRandomMoves(Bb2);
			delete Bb2;
			node->update(nodet);
		}
	}


	void Mcts::explore()
	{
		Node* node = &_tree[0];
		int depth = 0;
		int nodet;
		Bitboard* Bb = _state->clone();
		nodet = UpdateNode(node,Bb); // update and exploration
		while (depth < _param->getDepth() && nodet <= 0 && node->getVisits() > _param->getNumberOfVisitBeforeExploration() && nodet != -2)
		{
			node = node->select_child_UCT();
			auto m = node->getMove();
			_game->play(m, Bb);
			nodet = UpdateNode(node,Bb);
#ifdef DISPLAY_MCTS
			if (depth == 0)
			{
				cout << endl << "Node to explore : " << endl;

			}
			cout << node->getMove() << " > ";
#endif // DISPLAY_MCTS
			++depth;
			if (depth == _param->getDepth() && !_maxdepthreached)
			{
				_maxdepthreached = true;
				cout << endl << "max depth reached : " << _param->getDepth() << "moves ahead.";
			}
		}

		if (nodet <= 0) // no victory found yet
		{
#ifdef DISPLAY_MCTS
			cout << endl << "depth reached : start random";
#endif // DISPLAY_MCTS
			playRandom(node, Bb);
		}
#ifdef OPTIMIZED_MCTS
		elseif(nodet < 3 && nodet > 0 && nodet != node->getPlayer()) // losing move !
		{
			node->forceSetUCT(10);
			feedbackWinningMove(node);
			node->update(nodet);
		}
#endif //OPTIMIZED_MCTS
		else
		{
			node->update(nodet);
		}
		delete Bb;
	}

	void Mcts::updateLosingParent(Node* node) // for each parent check if uct = -1 => if yes it's a winning move for the opponent, he has to play here : uct = 10
	{
		bool loser = true;
		if (node->getParent() == nullptr) return; // root hahaha
		auto ListChilds = node->getParent()->getChildren();
		Node* ptr = ListChilds.first;
		for (int i = 0; (i < ListChilds.second && loser); ++i)
		{
			if (ptr->getUCT() != -1) loser = false;
			ptr++;
		}
		if (loser)
		{
			node->getParent()->forceSetUCT(10);
			feedbackWinningMove(node->getParent());
		}
	}

	void Mcts::feedbackWinningMove(Node* node) // if someone has a winning move given a position, we must not go to that position.
	{
		if (node->getUCT() != 10) return;
			node->getParent()->forceSetUCT(-1);
			updateLosingParent(node->getParent());
	}

	void Mcts::cleanTree(std::vector<Node> &T)
	{
		Node* ptr = &T[1];
		Node* lstptr = &T[(_param->getMaxNumberOfLeaves() -1)];
		while (ptr->getParent() != nullptr && ptr != lstptr)
		{
			ptr->unset();
			ptr++;
		}
	}

	void Mcts::copyTree(Node* NewRoot, std::vector<Node> &Tdest)
	{
		cleanTree(Tdest);
		auto next = &Tdest[0];					// place ptrDest at the begining of the destination array
		auto ptrDest = &Tdest[0];				// place ptrDest at the begining of the destination array
		Node* ptTemp;							// pointer to loop on the childrens of each ptr

		*ptrDest = *NewRoot;					// copy root
		next++;									// next free space in Tdest

		while (ptrDest->getParent() != nullptr || ptrDest == &Tdest[0]) // no parents OR root
		{
			auto ListOfNodes = ptrDest->getChildren();
			ptTemp = ListOfNodes.first;
			if (ptTemp != nullptr)				// if children add them to _buff
			{
				ptrDest->setChildrens(next, ListOfNodes.second); // update the node of the parents
				for (int i = 0; i < ListOfNodes.second; i++)
				{
					*next = *ptTemp;			// recopy of chidlren
					next->setParent(ptrDest);	// set their parent to current ptr
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
		while (_next->getParent() != nullptr)
		{
			_next++;
		}
	}


	Move Mcts::GetBestMove()
	{
#ifdef DISPLAY_MCTS
		cout << endl << "turn : " << _root->getState()->getPlayer();
#endif // DISPLAY_MCTS
		_maxdepthreached = false;
		int i = 0;
		int start = clock();
		int timeend = start + static_cast<int>(_param->getTimeLimitSimulationPerRoot() / 1000 * CLOCKS_PER_SEC);
#ifdef OPENMP
#pragma omp parallel shared(i,timeend)
#endif
		while (clock() < timeend && i < _param->getSimulationPerRoot())
		{
			i++;
#ifdef DISPLAY_MCTS
			cout << endl << "simulation n : " << i;
#endif // DISPLAY_MCTS
			explore();
		}
		cout << endl << "start search : " << static_cast<double>(start) / CLOCKS_PER_SEC << "s.";
		cout << endl << "end search : " << static_cast<double>(clock()) / CLOCKS_PER_SEC << "s in " << static_cast<double>(clock() - start) * 1000 / CLOCKS_PER_SEC << "ms and " << i*_param->getSimulationPerLeaves() << " simulations." << endl;
		return _tree[0].select_child_WR()->getMove();
	}

	void Mcts::print_tree(int depth)
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
		Count::I()->saveNbLeaves(_tree[0].count());
		Count::I()->saveMaxDepth(_tree[0].max_depth());
	}

	double Mcts::winning_Strategy()
	{
		return (_tree[0].getUCT() != 10 && _tree[0].getUCT() != -1) ? _tree[0].getProba() : _tree[0].getUCT();
	}
}
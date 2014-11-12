#include "Mcts.h"
#define elseif else if
#define OPTIMIZED_MCTS

//#define DEBUG_MCTS
//#define DISPLAY_MCTS

namespace mcts{

	Mcts::Mcts()
	{
	}

	Mcts::Mcts(	TheGame* game,
				Bitboard Bb,
				int depth,
				int timelimit,
				int simulR,
				int simulL)
	:_game(game), _depth(depth), _simulationPerRoot(simulR), _simulationPerLeaves(simulL), _timeLimitsimulationPerRoot(timelimit)
	{
		_root = new Node(Bb);
	}

	void Mcts::movePlayed(Move& move)
	{
		UpdateNode(_root);
		list<Node*> ListOfNodes = _root->getChildren();
		list<Node*>::iterator iterN;
		for (iterN = ListOfNodes.begin(); iterN != ListOfNodes.end(); ++iterN)
		{
			if ((*iterN)->getMove() == move) // we got the move played !
			{
				break;
			}
		}
		// TO CHECK AND IMPROVE
		_root->killChildrens(*iterN);
		delete(_root);
		_root = *iterN;
		_root->clearParents();
	}

	int Mcts::UpdateNode(Node* node)
	{
		Bitboard* Bb = &(node->getState());
		int nodet = node->getTerminal();

		if (nodet == -1) { // first time we are here, check if terminate
			nodet = _game->end(*Bb);
			node->setTerminal(nodet);
		}

		if (nodet == 0 && node->getChildren().empty())  // not explored yet => explore
		{
			list<Move> ListOfMoves;
			list<Move>::iterator iter;

			Bitboard* Bb2;

			ListOfMoves = _game->listPossibleMoves(*Bb);
			for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); ++iter)
			{
				Bb2 = Bb->clone();
				_game->play(*iter, *Bb2);
				node->addChild(*Bb2, *iter);
			}
		}

		return nodet;
	}

	void Mcts::playRandom(Node* node) // return 
	{
		int nodet;
		for (int i = 0; i < _simulationPerLeaves; ++i)
		{

#ifdef DEBUG_MCTS
			cout << "round n " << (i + 1) << endl;
#endif // DEBUG_MCTS
			Bitboard* Bb = (node->getState()).clone();
			nodet = _game->playRandomMoves(*Bb);
			delete Bb;
			node->update(nodet);
		}
	}


	void Mcts::explore()
	{
		Node* node = _root;
		int depth = 0;
		int nodet = UpdateNode(node);
		while (depth < _depth && nodet <= 0)
		{
			node = node->select_child_UCT();
			nodet = UpdateNode(node);
#ifdef DISPLAY_MCTS
			if (depth == 0)
			{
				cout << "Node to explore : ";

			}
			cout << node->getMove() << " > ";
#endif // DISPLAY_MCTS
			++depth;
		}

		if (nodet <= 0) // no victory found yet
		{
#ifdef DISPLAY_MCTS
			cout << "depth reached : start random" << endl;
#endif // DISPLAY_MCTS
			playRandom(node);
		}
#ifdef OPTIMIZED_MCTS
		elseif(nodet < 3 && nodet > 0 && nodet != node->getState().getPlayer()) // no victory found yet
		{
			list<Node*> ListParents;
			list<Node*>::iterator iter;
			ListParents = node->getParents();
			for (iter = ListParents.begin(); iter != ListParents.end(); ++iter)
			{
				(*iter)->forceSetUCT(-1);
			}
			node->update(nodet);
		}
#endif //OPTIMIZED_MCTS
		else
		{
#ifdef DISPLAY_MCTS
			cout << endl;
			cout << "not implemented yet : nodet 3" << endl;
#endif // DISPLAY_MCTS
			node->update(nodet);
		}
	}

	Move Mcts::GetBestMove()
	{
#ifdef DISPLAY_MCTS
		cout << "turn : " << (_root->getState()).getPlayer() << endl;
#endif // DISPLAY_MCTS
		int i = 0;
		int end = 0;
		int start = clock();
		int timeend = start + (static_cast<double>(_timeLimitsimulationPerRoot) / 1000 * CLK_TCK);
		while(end < timeend && i < _simulationPerRoot)
		{
			i++;
#ifdef DISPLAY_MCTS
			cout << "simulation n : " << i << endl;
#endif // DISPLAY_MCTS
			explore();
			end = clock();
		}
		cout << "start search : " << static_cast<double>(start) / CLK_TCK << "s." << endl;
		cout << "end search : " << static_cast<double>(clock()) / CLK_TCK << "s in " << static_cast<double>(end - start) * 1000 / CLK_TCK << "ms and " << i << " simulations." << endl;
		Node* node = _root->select_child_WR();
		return node->getMove();
	}

	void Mcts::print_tree(int depth)
	{
		int d = (depth == 0) ? _depth : depth;
		_root->print_tree(0, d);
	}
}
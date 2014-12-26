#include "Mcts.h"
#define elseif else if
#define OPTIMIZED_MCTS
#define LIMITED_EXPLORATION

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
				MctsArgs args)
	:_game(game), _param(args)
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
		Bitboard* Bb = node->getState();
		int nodet = node->getTerminal();

		if (nodet == -1) { // first time we are here, check if terminate
			nodet = _game->end(Bb);
			node->setTerminal(nodet);
		}

		if (nodet == 0 && node->getChildren().empty())  // not explored yet => explore
		{
			list<Move> ListOfMoves;
			list<Move>::iterator iter;

			Bitboard* Bb2;

			ListOfMoves = _game->listPossibleMoves(Bb);
			for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); ++iter)
			{
				Bb2 = Bb->clone();
				_game->play(*iter, Bb2);
				node->addChild(Bb2, *iter);
			}
		}

		return nodet;
	}

	void Mcts::playRandom(Node* node)
	{
		int nodet;
		for (int i = 0; i < _param.getSimulationPerLeaves(); ++i)
		{

#ifdef DEBUG_MCTS
			cout << endl << "round n " << (i + 1) << endl;
#endif // DEBUG_MCTS
			Bitboard* Bb = node->getState()->clone();
			nodet = _game->playRandomMoves(Bb);
			delete Bb;
			node->update(nodet);
		}
	}


	void Mcts::explore()
	{
		Node* node = _root;
		int depth = 0;
		int nodet = UpdateNode(node); // update and exploration
#ifndef LIMITED_EXPLORATION
		while (depth < _param.getDepth() && nodet <= 0)
		{
#endif
#ifdef LIMITED_EXPLORATION
		while (depth < _param.getDepth() && nodet <= 0 && node->getVisits() > _param.getNumberOfVisitBeforeExploration())
		{
#endif
			node = node->select_child_UCT();
			nodet = UpdateNode(node);
#ifdef DISPLAY_MCTS
			if (depth == 0)
			{
				cout << endl << "Node to explore : " << endl;

			}
			cout << node->getMove() << " > ";
#endif // DISPLAY_MCTS
			++depth;
			if (depth == _param.getDepth())
			{
				cout << endl << "max depth reached : " << _param.getDepth() << "moves ahead.";
			}
		}

		if (nodet <= 0) // no victory found yet
		{
#ifdef DISPLAY_MCTS
			cout << endl << "depth reached : start random";
#endif // DISPLAY_MCTS
			playRandom(node);
		}
#ifdef OPTIMIZED_MCTS
		elseif(nodet < 3 && nodet > 0 && nodet != node->getState()->getPlayer()) // losing move !
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
	}

	void Mcts::updateLosingParent(Node* node) // for each parent check if uct = -1 => if yes it's a winning move for the opponent, he has to play here : uct = 10
	{
		list<Node*> ListParents;
		list<Node*> ListChilds;
		list<Node*>::iterator iter;
		list<Node*>::iterator iter2;
		bool loser;
		ListParents = node->getParents();
		for (iter = ListParents.begin(); iter != ListParents.end(); ++iter)
		{
			loser = true;
			ListChilds = (*iter)->getChildren();
			for (iter2 = ListChilds.begin(); (iter2 != ListChilds.end() && loser); ++iter2)
			{
				if ((*iter2)->getUCT() != -1) { loser = false; }
			}
			if (loser)
			{
				(*iter)->forceSetUCT(10);
				feedbackWinningMove(*iter);
			}
		}
	}

	void Mcts::feedbackWinningMove(Node* node) // if someone has a winning move given a position, we must not go to that position.
	{
		if (node->getUCT() != 10) return;
		list<Node*> ListParents;
		list<Node*>::iterator iter;
		ListParents = node->getParents();
		for (iter = ListParents.begin(); iter != ListParents.end(); ++iter)
		{
			(*iter)->forceSetUCT(-1);
			updateLosingParent(*iter);
		}
	}

	Move Mcts::GetBestMove()
	{
#ifdef DISPLAY_MCTS
		cout << endl << "turn : " << _root->getState()->getPlayer();
#endif // DISPLAY_MCTS
		int i = 0;
		int end = 0;
		int start = clock();
		int timeend = start + (static_cast<double>(_param.getTimeLimitSimulationPerRoot()) / 1000 * CLK_TCK);
		while (end < timeend && i < _param.getSimulationPerRoot())
		{
			i++;
#ifdef DISPLAY_MCTS
			cout << endl << "simulation n : " << i;
#endif // DISPLAY_MCTS
			explore();
			end = clock();
		}
		cout << endl << "start search : " << static_cast<double>(start) / CLK_TCK << "s.";
		cout << endl << "end search : " << static_cast<double>(clock()) / CLK_TCK << "s in " << static_cast<double>(end - start) * 1000 / CLK_TCK << "ms and " << i << " simulations." << endl;
		Node* node = _root->select_child_WR();
		return node->getMove();
	}

	void Mcts::print_tree(int depth)
	{
		int d = (depth == 0) ? _param.getDepth() : depth;
		_root->print_tree(0, d);
	}
}
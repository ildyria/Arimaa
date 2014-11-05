#include "Mcts.h"
#define elseif else if
//#define DEBUG_MCTS
//#define DISPLAY_MCTS

namespace mcts{

	Mcts::Mcts()
	{
	}

	Mcts::Mcts(TheGame* game, Bitboard Bb, int depth, int IAPlayer, int simulR, int simulL) :_game(game), _depth(depth), _IAPlayer(IAPlayer), _simulationPerRoot(simulR), _simulationPerLeaves(simulL)
	{
		_root = new Node(Bb);
	}

	void Mcts::movePlayed(Move& move)
	{
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


	void Mcts::explore(Node* node, int depth)
	{
		int nodet = UpdateNode(node);
		list<Node*> ListParents;
		list<Node*>::iterator iter;

		if (nodet == 0) // no victory found yet
		{
			node = node->select_child_UCT((node->getState()).getPlayer());

#ifdef DISPLAY_MCTS
			if (depth == 0)
			{
				cout << "Node to explore : ";

			}
			cout << node->getMove() << " > ";
#endif // DISPLAY_MCTS
			if (depth < _depth)
			{
				explore(node, depth + 1);
			}
			else
			{
#ifdef DISPLAY_MCTS
				cout << "depth reached : start random" << endl;
#endif // DISPLAY_MCTS
				playRandom(node);
			}
		}
//		elseif(nodet == 1) // victory of first player
//		{
#ifdef DISPLAY_MCTS
			cout << endl;
			TicTacToe::diplayBoard(node->getState());
			cout << "not implemented nodet 1 & 2 : " << nodet << endl;
#endif // DISPLAY_MCTS
			// it's a WIN SO WE NEED TO PLAY THIS
//			node->update(nodet);
		//	node->forceSetUCT(10);
//		}
//		elseif(nodet == 2) // tie
//		{
			// it's a LOSS : WE MUST NOT PLAY THE LAST MOVE
//			node->update(nodet);
			/*ListParents = node->getParents();
			for (iter = ListParents.begin(); iter != ListParents.end(); ++iter)
			{
				(*iter)->forceSetUCT(-1);
			}*/
//		}
		elseif(nodet > 0) // tie
		{
#ifdef DISPLAY_MCTS
			cout << endl;
			cout << "not implemented yet : nodet 3" << endl;
#endif // DISPLAY_MCTS
			node->update(nodet);
		}
		else
		{
			cout << endl;
			throw("Problem with a node and nodet");
		}
	}

	Move Mcts::GetBestMove()
	{
		//node = _root;

		for (int i = 0; i < _simulationPerRoot; ++i)
		{
#ifdef DISPLAY_MCTS
			cout << "simulation n : " << i << endl;
#endif // DISPLAY_MCTS
//			node = _root;
			explore(_root, 0);
		}

		Node* node = _root->select_child_UCT(_IAPlayer);
		return node->getMove();
	}

	void Mcts::print_tree(int depth)
	{
		int d = (depth == 0) ? _depth : depth;
		_root->print_tree(0, d);
	}
}
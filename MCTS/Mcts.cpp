#include "Mcts.h"
#define elseif else if
//#define DEBUG_MCTS
//#define DISPLAY_MCTS

namespace mcts{

	Mcts::Mcts()
	{
	}

	Mcts::Mcts(Bitboard Bb, int depth, int IAPlayer, int simulR, int simulL) :_depth(depth), _IAPlayer(IAPlayer), _simulationPerRoot(simulR), _simulationPerLeaves(simulL)
	{
		_root = new Node(Bb);
//		cout << "MCTS created with board" << endl;
	}

	Mcts::~Mcts()
	{
//		cout << "MCTS destroyed" << endl;
	}

	void Mcts::movePlayed(string move)
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
	}

	int Mcts::UpdateNode(Node* node)
	{
		Bitboard* Bb = &(node->getState());
		int nodet = node->getTerminal();

		if (nodet == -1) { // first time we are here, check if terminate
			nodet = TicTacToe::end(*Bb);
			node->setTerminal(nodet);
		}

		if (nodet == 0 && node->getChildren().empty())  // not explored yet => explore
		{
			list<string> ListOfMoves;
			list<string>::iterator iter;

			Bitboard* Bb2;

			ListOfMoves = TicTacToe::listPossibleMoves(*Bb);
			for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); ++iter)
			{
				Bb2 = Bb->clone();
				TicTacToe::play(*iter, *Bb2);
				node->addChild(*Bb2, *iter);
			}
		}

		return nodet;
	}

	void Mcts::playRandom(Node* node) // return 
	{
		list<string> ListOfMoves;
		list<string>::iterator iter;
		int chosen, nodet;
		for (int i = 0; i < _simulationPerLeaves; ++i)
		{

#ifdef DEBUG_MCTS
			cout << "round n " << (i + 1) << endl;
#endif // DEBUG_MCTS
			Bitboard* Bb = (node->getState()).clone();
			nodet = TicTacToe::end(*Bb);

			while (nodet < 1)
			{
				ListOfMoves = TicTacToe::listPossibleMoves(*Bb);
				chosen = Random::I()->getNum(0, ListOfMoves.size() - 1);
				for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); ++iter)
				{
					if (chosen == 0){
#ifdef DEBUG_MCTS
						cout << " > " << *iter << endl;
#endif // DEBUG_MCTS
						TicTacToe::play(*iter, *Bb);
						break;
					}
					--chosen;
				}
				nodet = TicTacToe::end(*Bb);
			}
			delete Bb;
			node->update(nodet == _IAPlayer);
		}
	}

	void Mcts::explore(Node* node, int depth)
	{
		int nodet = UpdateNode(node);
		list<Node*> ListParents;
		list<Node*>::iterator iter;

		if (nodet == 0) // no victory found yet
		{
			node = node->select_child_UCT();

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
		elseif(nodet == 1) // victory of first player
		{
#ifdef DISPLAY_MCTS
			cout << endl;
			TicTacToe::diplayBoard(node->getState());
			cout << "not implemented nodet 1 & 2 : " << nodet << endl;
#endif // DISPLAY_MCTS
			// it's a WIN SO WE NEED TO PLAY THIS
			node->update(nodet == _IAPlayer);
			node->forceSetUCT(10);
		}
		elseif(nodet == 2) // tie
		{
			// it's a LOSS : WE MUST NOT PLAY THE LAST MOVE
			node->update(nodet == _IAPlayer);
			ListParents = node->getParents();
			for (iter = ListParents.begin(); iter != ListParents.end(); ++iter)
			{
				(*iter)->forceSetUCT(-1);
			}
		}
		elseif(nodet == 3) // tie
		{
#ifdef DISPLAY_MCTS
			cout << endl;
			cout << "not implemented yet : nodet 3" << endl;
#endif // DISPLAY_MCTS
			node->update(nodet == _IAPlayer);
		}
		else
		{
			cout << endl;
			throw("Problem with a node and nodet");
		}
	}

	string Mcts::GetBestMove()
	{
		Node* node = _root;

		for (int i = 0; i < _simulationPerRoot; ++i)
		{
#ifdef DISPLAY_MCTS
			cout << "simulation n : " << i << endl;
#endif // DISPLAY_MCTS
			node = _root;
			explore(node, 0);
		}

		node = _root->select_child_UCT();
		return node->getMove();
	}

	void Mcts::print_tree(int depth)
	{
		int d = (depth == 0) ? _depth : depth;
		_root->print_tree(0, d);
	}
}
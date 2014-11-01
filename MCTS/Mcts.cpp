#include "Mcts.h"
#define elseif else if

namespace mcts{

	Mcts::Mcts()
	{
	}

	Mcts::Mcts(Bitboard Bb, int depth) :_depth(depth)
	{
		_root = new Node(Bb);
		cout << "MCTS created with board" << endl;
	}


	Mcts::~Mcts()
	{
		cout << "MCTS destroyed" << endl;
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

	void Mcts::explore(Node* node, int depth)
	{
		int nodet = UpdateNode(node);

		if (nodet == 0) // no victory found yet
		{
			cout << endl << endl;
			node = node->select_child_UCT();

			cout << "Node to explore : " << node->getMove();
			cout << " at depth : " << depth << "/" << _depth << endl;
			TicTacToe::diplayBoard(node->getState());

			explore(node, depth + 1);
		}
		elseif(nodet == 1) // victory of first player
		{
			cout << "not implemented yet : nodet 1" << endl;
		}
		elseif(nodet == 2) // victory of second player
		{
			cout << "not implemented yet : nodet 2" << endl;
		}
		elseif(nodet == 3) // tie
		{
			cout << "not implemented yet : nodet 3" << endl;
		}
		else
		{
			throw("Problem with a node and nodet");
		}
	}

	string Mcts::GetBestMove()
	{
		Node* node = _root;
//		Node* nodeToExplore;

		explore(node, 0);

		return "toto";
	}

}
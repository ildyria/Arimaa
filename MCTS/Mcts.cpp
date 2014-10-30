#include "Mcts.h"
#define elseif else if

namespace mcts{

	Mcts::Mcts()
	{
	}

	Mcts::Mcts(Bitboard Bb)
	{
		_root = new Node(Bb);
		cout << "MCTS created with board" << endl;
	}


	Mcts::~Mcts()
	{
		cout << "MCTS destroyed" << endl;
	}

	string Mcts::GetBestMove()
	{
		list<string> ListOfMoves;
		list<string>::iterator iter;
		list<Node*> ListOfNodes;
		list<Node*>::iterator iterNode;

		Node* node = _root;
		Bitboard* Bb = &(node->getState());
		Bitboard* Bb2;

		int nodet = node->getTerminal();
		if (nodet == -1) {
			nodet = TicTacToe::end(*Bb);
			node->setTerminal(nodet);
		}

		if (nodet == 0 && node->getChildren().empty()) // not explored yet => explore
		{
			ListOfMoves = TicTacToe::listPossibleMoves(*Bb);
			for (iter = ListOfMoves.begin(); iter != ListOfMoves.end(); iter++)
			{
				Bb2 = Bb->clone();
				TicTacToe::play(*iter, *Bb2);
				TicTacToe::diplayBoard(*Bb2);
				node->addChild(*Bb2, *iter);
			}
		}

		if(nodet == 0) // no victory found yet
		{
			cout << endl << endl;
			ListOfNodes = node->getChildren();
			for (iterNode = ListOfNodes.begin(); iterNode != ListOfNodes.end(); iterNode++)
			{
				TicTacToe::diplayBoard((*iterNode)->getState());
			}
			/*			for (int i = 0; i < 1000; i++)
			{
				node->select_child_UCT();
			}*/
			//			Node current = Selection(root, game);
			//			int value = Rollout(current, game, startPlayer);
			//			Update(current, value);
		}
		elseif(nodet == 1) // victory of first player
		{
		}
		elseif(nodet == 2) // victory of second player
		{
		}
		elseif(nodet == 3) // tie
		{
		}
		else
		{
			throw("Problem with a node and nodet");
		}


		return "toto";
	}

}
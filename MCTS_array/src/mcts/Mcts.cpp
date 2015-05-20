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
				_tree(std::vector<Node>(args->get_max_max_number_of_leaves()))
#if defined(DOUBLE_TREE)
				, _buff(std::vector<Node>(args->get_max_max_number_of_leaves()))
#else
				, _index(Tree_index<Node>(args->get_max_max_number_of_leaves()))
#endif
	{
/*
		cout << "number of thread : " << omp_get_num_procs() << endl;
*/		
		// #pragma omp parallel
		// {
		// 	int tid = omp_get_thread_num();
		// 	printf("Hello World from thread = %d\n", tid);
		// }
		omp_init_lock(&_lockNode);
		omp_init_lock(&_lockNext);
		Tree<Node>::clean_tree(_tree);
#if defined(DOUBLE_TREE)
		Tree<Node>::clean_tree(_buff);
#endif
		Count::I()->clear();
		Move* random = new Move();
#if defined(DOUBLE_TREE)
		_tree[0].set(*random);
#else
		Node** address = _index.get();
		_tree[0].set(*random, address);
#endif 
		_tree[0].has_parent();
		_next = (&_tree[0]) + 1;
	}

	Bitboard* Mcts::move_played(Move& move)
	{
		update_node(&_tree[0], _state);
		auto ListOfNodes = _tree[0].get_children();
		auto iter = ListOfNodes.first;
		for (u_int i = 0; i < ListOfNodes.second; ++i)
		{
			if (iter->get_move() == move) // we got the move played !
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

	u_int Mcts::update_node(Node* node, Bitboard* Bb)
	{
		u_int nodet = node->get_terminal();

		if (nodet == 64) { // first time we are here, check if terminate
			nodet = _game->end(Bb);
			if ((nodet & 7) > 0)
			{
				node->set_terminal(nodet);  // terminal node : winner or tie
			}
			else
			{
				node->set_terminal(32);	// non terminal but not explored YET
				nodet = 32;
			}
		}
		return nodet;
	}

	void Mcts::expand_node(Node* node, Bitboard* Bb, u_int& nodet) {
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
			locked = node->get_lock();
			omp_unset_lock(&_lockNode);
//		}

		if (locked)
		{
			nodet = 16;
			return;
		}
//		if (node == &_tree[0]) printf("expanding\n");
		list<Move> ListOfMoves;
		list<Move>::iterator iter;

		ListOfMoves = _game->list_possible_moves(Bb);
		if (&_tree[0] + _param->get_max_max_number_of_leaves() < _next + ListOfMoves.size())
		{
			node->set_terminal(16);	// non terminal but not explored YET
			nodet = 16;
			return; // this is extremly dangerous : the node stays locked ! We need to unlock it when we will prune the tree
		}
		Node* tmp2;
		Node* tmp;

		// _next concurency
		omp_set_lock(&_lockNext);
		// #pragma omp atomic capture
		// {
			tmp2 = _next;
			_next += ListOfMoves.size();
		// }
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
			tmp2->play(node->get_player());
		}
		node->set_children(tmp, static_cast<u_int>(ListOfMoves.size())); // update at the end, concurency race...
		nodet = 0;
		node->release_lock();

		return;
	}

	void Mcts::start_ramdom_playouts(Node* node, Bitboard* Bb, Memento<Node*>& parents)
	{
		u_int nodet;

		for (u_int i = 0; i < _param->get_max_num_simulation_per_leaves(); ++i)
		{
			Bitboard* Bb2 = Bb->clone();
			nodet = _game->play_random_moves(Bb2);
			delete Bb2;
			feedback_results(nodet, parents);
		}
	}

	void Mcts::feedback_results(u_int nodet, Memento<Node*>& parents)
	{
		parents.rewind();
		Node* n = parents.get();
		while(n != nullptr)
		{
			n->update(nodet);
			n = parents.get();
		}
	}

	void Mcts::feedback_sure_wins_loss(Memento<Node*>& parents)
	{
		parents.rewind();

		Node* n = parents.get(); // take the second
		n->force_set_UCT(42);

		n = parents.get();
		bool winloser = true;
		bool loser = true;
		Node* ptr;
		std::pair<Node*, u_int> ListChilds;
		while(n != nullptr && winloser)
		{
			if(loser)
			{
				n->force_set_UCT(-1);
				loser = false;
			}
			else
			{
				loser = true;
				ListChilds = n->get_children();
				ptr = ListChilds.first;
				for (u_int i = 0; (i < ListChilds.second && loser); ++i)
				{
					if (ptr->get_UCT() != -1) loser = false;
					ptr++;
				}

				if (loser)
				{
					n->force_set_UCT(42);
				}
				else
				{
					winloser = false;
				}
			}
			n = parents.get();
		}
	}

	void Mcts::explore(Memento<Node*>& parents)
	{
		Node* node = &_tree[0];
		u_int depth = 0;
		Bitboard* Bb = _state->clone();
		// node->add_virtual_loss(_param->get_max_num_simulation_per_leaves());
		u_int nodet = update_node(node, Bb);

		parents.reset();
		parents.push(&_tree[0]);
		Move m;
		while (depth < _param->get_max_depth() && (nodet == 0 || nodet > 16))
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

				// TESTING PURPOSE ONLU
				// 
				break;

				if (node->get_visits() > _param->get_number_of_number_of_visit_before_exploration())
				{
					expand_node(node, Bb, nodet);
					if (nodet == 16) break;
				}
				else
				{
					break;
				}
			}

			node = node->select_child_UCT();
			parents.push(node);
			node->add_virtual_loss(_param->get_max_num_simulation_per_leaves());
			m = node->get_move();
			_game->play(m, Bb);
			nodet = update_node(node, Bb);
			++depth;
		}

		if (nodet == 0 || nodet > 4) // no victory found yet
		{
			start_ramdom_playouts(node, Bb, parents);
		}
//		elseif(nodet < 4 && nodet > 0 && nodet != node->get_player()) // losing move !
		elseif((nodet | node->get_player()) == 3) // losing move !
		{
			feedback_results(nodet, parents);
			feedback_sure_wins_loss(parents);
		}
		else
		{
			feedback_results(nodet, parents);
		}
		delete Bb;
	}

	Move Mcts::get_best_move()
	{
		u_long i = 0;
		auto start_time = high_resolution_clock::now();
		auto end_time = start_time + milliseconds(_param->get_time_limit_simulation_per_root());
		Memento<Node*> parents = Memento<Node*>(_param->get_max_depth() + 1);
		#pragma omp parallel shared(end_time) firstprivate(parents) reduction(+:i)
		{
			while (high_resolution_clock::now() < end_time && i < _param->get_max_num_simulation_per_root())
			{
				i++;
				
				explore(parents);
			}
		#pragma omp barrier
		}
		cout << endl << "Searched for " << Count::format(duration_cast<milliseconds>(high_resolution_clock::now() - start_time).count()) << "ms and " << Count::format(i*_param->get_max_num_simulation_per_leaves()) << " simulations." << endl;
		return _tree[0].select_child_WR()->get_move();
	}

	void Mcts::print_tree(u_int depth)
	{
		auto d = (depth == 0) ? _param->get_max_depth() : depth;
		_tree[0].print_tree(0, d);
	}

	void Mcts::kill_tree()
	{
		Tree<Node>::clean_tree(_tree);
#if defined(DOUBLE_TREE)
		Tree<Node>::clean_tree(_buff);
#endif
//		_tree.clear();
	}

	void Mcts::get_number_leaves()
	{
		auto num = _next - &_tree[0];
		Count::I()->saveNbLeaves(static_cast<int>(num));
//		Count::I()->saveMaxDepth(_tree[0].max_depth());
	}

	double Mcts::winning_chances()
	{
		return (_tree[0].get_UCT() != 42 && _tree[0].get_UCT() != -1) ? _tree[0].get_proba() : _tree[0].get_UCT();
	}

	v_stat Mcts::get_moves_statistics()
	{
		auto terminal = _tree[0].get_terminal();

		if (terminal != 0) return v_stat(); // no children => no stats

		auto listChildren = _tree[0].get_children();
		v_stat return_vect = v_stat(listChildren.second);
		auto first = listChildren.first;
		for (u_int i = 0; i < listChildren.second; ++i)
		{
			return_vect[i] = n_stat(first->get_move().get_move(), p_stat(first->get_wins(), first->get_visits()));
			++first;
		}

		return return_vect;
	}
}
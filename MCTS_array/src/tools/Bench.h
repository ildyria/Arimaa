#pragma once
#include <algorithm>    // std::min_element, std::max_element
#include "typedef.h"
#include "Graph.h"
#include "../interfaces/TheGame.h"
#include "../mcts/Mcts.h"
#include "../mcts/MctsArgs.h"

class Bench
{
	TheGame* _game;
	mcts::MctsArgs* _param;
	Bitboard* _bb;
	u_long _num_cpu;
	std::vector<u_long> _results;

	u_long stress(mcts::Mcts* mcts);
	void status_bar(int state, int full, int num ,int total);

public:
	Bench(TheGame* game, Bitboard* Bb, mcts::MctsArgs* args);
	~Bench();
	void run();
};
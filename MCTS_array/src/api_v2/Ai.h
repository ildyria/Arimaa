#pragma once
#include "Game.h"
#include "../mcts/Mcts.h"
#include "../tools/typedef.h"
#include "../tools/Count.h"

namespace api_v2 {

	/**
	 * \class Ai
	 * \brief Converter to manage the AI by the GUI
	 */
	class Ai
	{
		mcts::MctsArgs* _param;
		mcts::Mcts* _ai;

	public:
		explicit Ai(prog_options& options);

		~Ai()
		{
			delete _param;
			delete _ai;
		}

		void init();

		void makeMove(u_long move);

		v_stat getMovesStatistics();

		void explore();

		void exploreSingle();
	};
}



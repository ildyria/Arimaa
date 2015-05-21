#pragma once
#include "Game.h"
#include "../mcts/Mcts.h"
#include "../tools/typedef.h"
#include "../tools/benching/Count.h"

namespace api_v2 {

	/**
	 * \class Ai
	 * \brief Converter to manage the AI by the GUI
	 */
	class Ai
	{
		mcts::MctsArgs* _param;
		mcts::Mcts* _ai;
		Bitboard* _board;
		TheGame* _game;

	public:
		Ai();

		explicit Ai(prog_options& options);

		~Ai()
		{
			delete _param;
			delete _ai;
		}

		void setThinkingTime(int t = 5);

		void setTheGame(TheGame* game);

		double getThinkingTime();

		void setThinkingTime(int t = 5);

		void init();

		void makeMove(u_long move);

		/**
		 * \fn getMovesStatistics(int num_of_best_moves = 1000)
		 * \brief return a table of the moves SORTED by move ID
		 * \param  num_of_best_moves [description]
		 * \return                   [description]
		 */
		v_stat getMovesStatistics(int num_of_best_moves = 1000);

		void explore();

		void exploreSingle();

		std::vector<u_long> getState();

		void setState(std::vector<u_long> state);

		double estimateWinChances();

		v_stat getMovesStatistics();

		void displayASCII();
	};
}



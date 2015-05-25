#pragma once
#include <vector>    	// std::vector
#include <algorithm>    // std::sort
#include "../connect4/Connect4.h"
#include "../connect4/BitboardConnect4.h"
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
	public:
		mcts::MctsArgs* _param;
		mcts::Mcts* _ai;
		Bitboard* _board;
		TheGame* _game;

		Ai();

		explicit Ai(prog_options& options);

		~Ai()
		{
			delete _param;
			delete _ai;
			delete _board;
			delete _game;
		}

		/**
		 * \fn setThinkingTime(int t = 5)
		 * \brief setter for the thinkingtime if you want to change it during a game.
		 * 
		 * \param t time in seconds
		 */	
		void setThinkingTime(int t = 5);

		/**
		 * \fn getThinkingTime() 
		 * \brief getter for the thinking time
		 * 
		 * \return thinking time in seconds
		 */
		int getThinkingTime();

		/**
		 * \fn makeMove(u_long move)
		 * \brief apply the move to the Bitboard and prune the tree.
		 * \param move : move played (either by AI or Player) 
		 */
		void makeMove(u_long move);

		void explore();

		// done
		void exploreSingle();

		/**
		 * \fn estimateWinChances()
		 * \brief FOR HISTORICAL PURPOSE, returns the winning chances of the last move played
		 * \details -1 if losing
		 * 10 if winning strategy
		 * [0,1] else
		 * 
		 * \return winning chances
		 */
		double estimateWinChances();


		/** \fn displayASCII
		 *	\details print the root Bitboard in Ascii
		 */
		void displayASCII();

		/**
		 * \fn getState()
		 * \details return the serialized version of the root Bitboard
		 */
		std::vector<u_long> getState();

		/**
		 * \fn setState
		 * \details this function redifine the root bitboard.
		 * /!\ Warning, that bitboard must be the same kind/size of the one imported.
		 * /!\ Beware this function also fully cleans the tree !!
		 * \param state serialized version of the Bitboard
		 */
		void setState(std::vector<u_long> state);

		/**
		 * \fn getMovesStatistics(int num_of_best_moves = 1000)
		 * \brief return a table of the moves SORTED by move ID
		 * \param  num_of_best_moves number of moves to be returned
		 * \return  statistics about the moves returned
		 */
		v_stat getMovesStatistics(int num_of_best_moves = 1000);
	};
}



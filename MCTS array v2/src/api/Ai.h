#pragma once
#include "Game.h"
#include "../mcts/Mcts.h"

#ifdef _WIN64
#define CLOCK 1000
#else
#define CLOCK 10000
#endif

namespace api {
	/**
	 * \class Ai
	 * \brief Converter to manage the AI by the GUI
	 */
	class Ai
	{
		mcts::MctsArgs* _param;
		mcts::Mcts* _ai;
		Game* _game;

	public:
		/**
		 * \fn Ai(int t = 5)
		 * \brief create the Ai with a time limit. 
		 * 
		 * \param t time in seconds
		 */
		explicit Ai(int t = 5);
		
		/**
		 * \fn ~Ai()
		 * \brief basic destructor
		 */
		~Ai()
		{
			delete _param;
			delete _ai;
		}
		
		/**
		 * \fn getThinkingTime() 
		 * \brief getter for the thinking time
		 * 
		 * \return thinking time in seconds
		 */
		double getThinkingTime();

		/**
		 * \fn setThinkingTime(int t = 5)
		 * \brief setter for the thinkingtime if you want to change it during a game.
		 * 
		 * \param t time in seconds
		 */	
		void setThinkingTime(int t = 5); // t in seconds

		/**
		 * \fn init(Game* g)
		 * \brief initialize the AI with a Game
		 * 
		 * \param g pointer to the game to initialize with
		 */
		void init(Game* g); // permet a l'IA de connaitre le jeu.

		/**
		 * \fn makeMove()
		 * \brief ask a move to the IA 
		 * \details play the player's move if i is not provided and the look for the best move and returns it.
		 * 
		 * \param player : did the player play ? 
		 * \return the column played by the IA.
		 */
		int makeMove(bool player = true);

		/**
		 * \fn estimateWinChances()
		 * \brief returns the winning chances of the last move played
		 * \details -1 if losing
		 * 10 if winning strategy
		 * [0,1] else
		 * 
		 * \return winning chances
		 */
		double estimateWinChances();
	};
}



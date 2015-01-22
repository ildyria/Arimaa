#pragma once
#include "../connect4/Connect4.h"

namespace converter
{
	class Game
	{
		Bitboard* _board;
		TheGame* _game;
		int _lastMove;

	public:

		/**
		 * \fn Game()
		 * \brief constructor of an empty game
		 */
		Game();
		~Game() {};
	
		// le [0,0] est en BAS A DROITE

		/**
		 * \fn at(int x, int y)
		 * \brief return if tile is empty 
		 * 
		 * \param x [0,6]
		 * \param y [0,5]
		 * 
		 * \return 1 if player 1, 2 if player 2, 0 if empty
		 */
		int at(int x, int y);
		
		/**
		 * \fn getLastMove() 
		 * \brief getter for _lastMove
		 * 
		 * \return _lastMove
		 */
		int getLastMove();

		//  Savoir la hauteur de chaque colonne (je peux le déduire de la fonction précédente mais ça serait plus joli comme ça)
		/**
		 * \fn colHeight(int col)
		 * \brief return the heigh of the column
		 * \details [long description]
		 * 
		 * \param col [1,7]
		 * 
		 * \return height [0,6], 0 if nothing, 6 if full
		 */
		int colHeight(int col);

		/**
		 * \fn activePlayer()
		 * \brief return the active player
		 * 
		 * \return 1 if player 1, 2 if player 2
		 */
		int activePlayer();

		/**
		 * \fn canMakeMove(int col)
		 * \brief return a player can make a move in the given column or not
		 * 
		 * \param col [1,7]
		 * 
		 * \return true if the Move is possible
		 */
		bool canMakeMove(int col);

		/**
		 * \fn makeMove(int col)
		 * \brief play a move in the column col
		 * 
		 * \param col where to play the move [1,7]
		 * 
		 * \return true if possible movement, false else.
		 */
		bool makeMove(int col);

		/**
		 * \fn getWinner()
		 * \brief return if the game reached its end or not.
		 * \details returns 
		 * 0 if the game is not ended yet
		 * 1 if player 1 wins
		 * 2 if player 2 wins
		 * 3 if it is tie
		 * 
		 * \return integer defining the winner.
		 */
		int getWinner();

		/**
		 * \fn getWinningLine()
		 * \brief return the list of the positions of the tiles won the game.
		 * 
		 * \return list of pairs<X,Y>
		 */
		std::list<std::pair<int,int>> getWinningLine();
			
		inline TheGame* getGame() { return _game; }
		inline Bitboard* getBitboard() { return _board; }
	};
}
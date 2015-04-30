#pragma once

#include <mpi.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
//#include <unistd.h>
#include <ctime>

#define TALKATIVE 2 // How much info is displayed

#define SAYT(X, Y)  if(TALKATIVE > Y) cout << X << endl
#define SAY(X)  if(TALKATIVE > 1) cout << X << endl


//void srandom(unsigned seed);
#define MASTER 0        // task ID of master task
#define POSSIBILITIES 7

#define TIME_LIMIT_S 5

#include "../../Connect4/API/Game.h"
#include "../../Connect4/API/Ai.h"

enum MessageTag { THINK_TIME, GAME_STATE, RESUTLS };

class VoteAI
{
public:
	/**
	* \fn Ai(int t = 5)
	* \brief create the Ai with a time limit.
	*
	* \param t time in seconds
	*/
	explicit VoteAI(int t = 5);

	/**
	* \fn ~Ai()
	* \brief basic destructor
	*/
	~VoteAI();

	/**
	* \fn getThinkingTime()
	* \brief getter for the thinking time
	*
	* \return thinking time in seconds
	*/
	int getThinkingTime();

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
	void init(api::Game* g); // permet a l'IA de connaitre le jeu.

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
	
private:
	api::Game* m_game;
	api::Ai m_ai;
};

static int getMPIRank()
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	return rank;
}
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

#include "api_v2/Game_v2.h"
#include "api_v2/Ai_v2.h"

enum MessageTag { OPTIONS, THINK_TIME, GAME_STATE, RESUTLS };

class VoteAI
{
public:
	/**
	* \fn Ai(int t = 5)
	* \brief create the Ai with a time limit.
	*
	* \param t time in seconds
	*/
	explicit VoteAI(prog_options& options);

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
	* \fn setState
	* \details this function redefine the root bitboard.
	* /!\ Warning, that bitboard must be the same kind/size of the one imported.
	* /!\ Beware this function also fully cleans the tree !!
	* \param state serialized version of the Bitboard
	*/
	void setState(std::vector<u_long> state);

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

	static double getValue(n_stat ns);
	
private:
	int rank;
	int size;

	api_v2::Game* m_game;
	api_v2::Ai m_ai;
	int nextMove;
	double nextMoveChances;

	void sendTime(int* t);
	void sendOptions(prog_options* options);
};

static int getMPIRank()
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	return rank;
}
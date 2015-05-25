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

#if defined(_WIN32) || defined(_WIN64)
	#include "api_v2/Game_v2.h"
	#include "api_v2/Ai_v2.h"
#else
	#include "../../MCTS_array/src/api_v2/Game_v2.h"
	#include "../../MCTS_array/src/api_v2/Ai_v2.h"
#endif
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
	* \brief chosses and plays a move.
	*
	* \return the column played by the IA.
	*/
	u_long makeMove();


	/**
	* \fn acknowledgeMove()
	* \brief acknowledges a move done by the oponent
	*
	* \return true if valid move.
	*/
	bool acknowledgeMove(u_long move);

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
	int thinkTime;

	int workerThinkTime() { return thinkTime - 1; };

	void sendTime(int* t);
	void sendOptions(prog_options* options);
};

static int getMPIRank()
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	return rank;
}
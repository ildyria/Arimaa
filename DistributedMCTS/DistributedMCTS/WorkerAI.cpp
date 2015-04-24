#include "WorkerAI.h"

WorkerAI::WorkerAI(int t) : m_ai()
{
	if (getMPIRank() == MASTER)
	{
		std::cerr << "Worker created on master thread" << std::endl;
	}
	m_ai.setThinkingTime(t);
	run();
}


WorkerAI::~WorkerAI()
{
	delete m_game;
}

void WorkerAI::run()
{
	int msgRecieved;
	MPI_Status status;
	bool keepGoing = true;

	while(keepGoing)
	{
		//checks message for thinking time
		MPI_Iprobe(MASTER, THINK_TIME, MPI_COMM_WORLD, &msgRecieved, &status);
		if (msgRecieved)
		{
			int ttime;
			MPI_Request r;
			MPI_Recv(&ttime, 1, MPI_INT, MASTER, THINK_TIME, MPI_COMM_WORLD, &status);
			keepGoing = onTimeRecv(ttime);
		}

		//checks message for game state
		MPI_Iprobe(MASTER, GAME_STATE, MPI_COMM_WORLD, &msgRecieved, &status);
		if (msgRecieved)
		{
			int ttime;
			MPI_Request r;
			//TODO : make game from message
			//MPI_Recv(&ttime, 1, MPI_INT, MASTER, GAME_STATE, MPI_COMM_WORLD, &status);
			//keepGoing = onGameRecv(game);
		}
	}
}


bool WorkerAI::onTimeRecv(int ttime)
{
	m_ai.setThinkingTime(ttime);
	vote();
}

bool WorkerAI::onGameRecv(api::Game* game)
{
	m_ai.init(game);
}

void WorkerAI::vote()
{

}
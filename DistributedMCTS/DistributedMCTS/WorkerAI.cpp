#include "WorkerAI.h"

WorkerAI::WorkerAI(prog_options& options) : m_ai(options)
{
	if (getMPIRank() == MASTER)
	{
		fprintf(stderr,"Worker created on master thread\n");
	}
	else
	{
		run();
	}
}


WorkerAI::~WorkerAI()
{
	delete m_game;
}

void WorkerAI::run()
{
	//int msgRecieved;
	MPI_Status status;
	bool keepGoing = true;

	while(keepGoing)
	{

		//checks message for game state
		//MPI_Iprobe(MASTER, GAME_STATE, MPI_COMM_WORLD, &msgRecieved, &status);
		//if (msgRecieved)
		//{
			std::cout << "waiting for state : " << getMPIRank() << std::endl;

			u_long* state = &(m_ai.getState()[0]);

			MPI_Recv((void *)state,			//data
				(int) m_ai.getState().size(),		//nb items
				MPI_UNSIGNED_LONG,			//item type
				MASTER,						//source
				GAME_STATE,					//tag
				MPI_COMM_WORLD,				//comm
				&status);

			keepGoing = onStateRecv();

			std::cout << "state recieved by " << getMPIRank() << std::endl;
		//}

		//checks message for thinking time
		//MPI_Iprobe(MASTER, THINK_TIME, MPI_COMM_WORLD, &msgRecieved, &status);
		//if (msgRecieved)
		//{
			int ttime;
			MPI_Recv(&ttime,
				1,
				MPI_INT,
				MASTER,
				THINK_TIME,
				MPI_COMM_WORLD,
				&status);

			printf("tmie recieved by %d\n",getMPIRank());

			keepGoing = onTimeRecv(ttime);
		//}
	}
}


bool WorkerAI::onTimeRecv(int ttime)
{
	m_ai.setThinkingTime(ttime);

	printf("starting vote...\n");

	if (ttime > 0)
		vote(); //also trigegrs the vote

	printf("vote done.\n");

	return (ttime > 0);
}

bool WorkerAI::onStateRecv()//std::vector<u_long> state)
{
	//m_ai.setState(state); //already done on message recieved
	return true;
}

void WorkerAI::vote()
{
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	m_ai.explore();
	v_stat scores = m_ai.getMovesStatistics(POSSIBILITIES);

	printf("sending result %d ...\n",getMPIRank());

	//sends message
	MPI_Send(
		(void *)&(scores[0]),		//data
		POSSIBILITIES * sizeof(n_stat),	//nb items
		MPI_BYTE,			//item type
		MASTER,			//dest
		RESUTLS,				//tag
		MPI_COMM_WORLD
		);

	printf("%d : result %d sent.\n",(int)MPI_Wtime() % 60,getMPIRank());

}
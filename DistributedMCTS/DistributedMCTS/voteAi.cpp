#include "voteAi.h"

using namespace std;

int VoteAI::kill;

VoteAI::VoteAI(prog_options& options) : m_ai(options)
{
	setThinkingTime(options.time_to_search);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank != MASTER)
	{
		fprintf(stderr, "Master created on worker thread\n");
	}

	VoteAI::kill = -1;
}

int VoteAI::getThinkingTime()
{
	return thinkTime;
}

VoteAI::~VoteAI()
{
	sendState(); //necessary before sending time
	sendTime(&kill); //orders the workers to exit
}

void VoteAI::setThinkingTime(int t)
{
	if (t <= 1)
		fprintf(stderr,"Low think time not supported\n");
	thinkTime = t;
	m_ai.setThinkingTime(workerThinkTime());
}

void VoteAI::setState(std::vector<u_long> state)
{
	m_ai.setState(state);
}


u_long VoteAI::makeMove()
{

	int rc = MPI_SUCCESS; // Return code
	//MPI_Status status;

	//sends the allowed time to the workers, also works as a start order
	int ttime = workerThinkTime();
	printf("Sending data...\n");
	sendState();
	sendTime(&ttime);
	printf("Data sent.\n");

	double begin = MPI_Wtime(); //start time

	// Compute its own results
	printf("Master process...\n");
	m_ai.explore();
	v_stat scores = m_ai.getMovesStatistics(POSSIBILITIES);
	printf("Master process done.\n");

	//prepares buffer		
	//v_stat* buf = new v_stat[size - 1];
	//for (int i = 0; i < (size - 1); ++i)
	//	for (int j = 0; j < POSSIBILITIES; ++j)
	//		buf[i].push_back(n_stat());
	std::vector<v_stat> buf;

	//check for messages from other nodes
	std::vector<MPI_Request> requests;
	std::vector<MPI_Status> status;

	SAY("Waiting for results...");
	while ((MPI_Wtime() - begin) < (double) thinkTime) //while there is still time
	{
		for (int node = 1; node < size; node++) //for all nodes except master
		{
			MPI_Status s;
			int msg_recieved = 0;
			MPI_Iprobe(node, RESUTLS, MPI_COMM_WORLD, &msg_recieved, &s);
			if (msg_recieved)
			{
				printf("Recieved message from %d\n", node);

				//MPI_Request r;
				requests.push_back(MPI_Request());
				status.push_back(MPI_Status());
				buf.push_back(v_stat());
				for (int i = 0; i < POSSIBILITIES; ++i)
					buf[buf.size() - 1].push_back(n_stat());
				MPI_Irecv((void*) &(buf[buf.size() - 1][0]), POSSIBILITIES * sizeof(n_stat), MPI_BYTE, node, RESUTLS, MPI_COMM_WORLD, &requests[requests.size() - 1]);
			}
		}
	}

	printf("%d results recieved.\n",buf.size());

	printf("\n================\n");
	for (auto s : scores)
	{
		printf("%lu : %f / %f\n", s.first, s.second.first, s.second.second);
	}
	for (int i = 0; i < buf.size(); i++)
	{
		int recv;
		MPI_Test(&requests[i], &recv, &status[i]);
		if (recv)
		{
			printf("================\n");
			for (auto s : buf[i])
			{
				printf("%lu : %f / %f\n", s.first, s.second.first, s.second.second);
			}
		}
	}
	printf("================\n\n");

	printf("Combining data...\n");
	//addition
	for (int i = 1; i < buf.size(); i++)
	{
		int recv;
		MPI_Test(&requests[i], &recv, &status[i]);
		if (recv)
		{
			//adds the results to the score
			scores += buf[i];
		}
	}
	printf("Combinned.\n");

	if (rc != MPI_SUCCESS)
		printf("%d : failure on something\n",rank);

	// The master thread displays the percentage for each value and the maximum value
	nextMoveChances = 0;
	nextMove = -1;
	if (rank == MASTER)
	{
		for (int i = 0; i < POSSIBILITIES; i++)
		{
			if (nextMoveChances < getValue(scores.at(i)))
			{
				nextMoveChances = getValue(scores.at(i));
				nextMove = scores.at(i).first;
			}
#if TALKATIVE > 0
			printf("%ld : %f %% \n",scores.at(i).first,getValue(scores.at(i))*100);
#endif
		}
		printf("Vote : %d (%lf %% )\n",nextMove,nextMoveChances*100);
	}

	m_ai.makeMove(nextMove);

	return nextMove;
}


bool VoteAI::acknowledgeMove(u_long move)
{
	m_ai.makeMove(move);
	return true;
}


double VoteAI::estimateWinChances()
{
	return nextMoveChances;
}


void VoteAI::sendTime(int* t)
{
	MPI_Request request;
	for (int node = 1; node < size; node++) //for all nodes except master
	{
		//sends message
		MPI_Isend(
			(void *)t,		//data
			1,	//nb items
			MPI_INT,			//item type
			node,			//dest
			THINK_TIME,				//tag
			MPI_COMM_WORLD,
			&request
			);
	}
}

void VoteAI::sendOptions(prog_options* options)
{
	MPI_Request request;
	for (int node = 1; node < size; node++) //for all nodes except master
	{
		//sends message
		MPI_Isend(
			(void *)options,		//data
			sizeof(prog_options),	//nb items
			MPI_BYTE,			//item type
			node,			//dest
			OPTIONS,				//tag
			MPI_COMM_WORLD,
			&request
			);
	}
}

void VoteAI::sendState()
{
	auto state = m_ai.getState();

	std::vector<MPI_Request> requests;
	std::vector<MPI_Status> status;
	for (int node = 1; node < size; node++) //for all nodes except master
	{
		status.push_back(MPI_Status());
		requests.push_back(MPI_Request());
		//sends message
		MPI_Isend(
			(void *)&state,		//data
			(int)state.size(),	//nb items
			MPI_UNSIGNED_LONG,			//item type
			node,			//dest
			GAME_STATE,				//tag
			MPI_COMM_WORLD,
			&requests[node - 1]
			);
	}

	//wait since the state will be gone after function ends
	MPI_Waitall(size - 1, &requests[0], &status[0]);
}

double VoteAI::getValue(n_stat ns)
{
	return ns.second.first / ns.second.second;
}
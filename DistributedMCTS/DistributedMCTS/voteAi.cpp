#include "voteAi.h"

using namespace std;


VoteAI::VoteAI(prog_options& options) : m_ai(options)
{
	if (getMPIRank() != MASTER)
	{
		std::cerr << "Master created on worker thread" << std::endl;
	}
	//else
	//{
	//	sendOptions(&options); //options are not lost at the end of function because they are a reference
	//}
}

int VoteAI::getThinkingTime()
{
	return (int) m_ai.getThinkingTime();
}

VoteAI::~VoteAI()
{
	int kill = -1;
	sendTime(&kill); //orders the workers to exit
}

void VoteAI::setThinkingTime(int t)
{
	m_ai.setThinkingTime(t);
}

void VoteAI::setState(std::vector<u_long> state)
{
	std::vector<MPI_Request> requests;
	std::vector<MPI_Status> status;
	for (int node = 1; node < size; node++) //for all nodes except master
	{
		requests.push_back(MPI_Request());
		//sends message
		MPI_Isend(
			(void *)&state,		//data
			(int) state.size(),	//nb items
			MPI_UNSIGNED_LONG,			//item type
			node,			//dest
			GAME_STATE,				//tag
			MPI_COMM_WORLD,
			&requests[node-1]
			);
	}

	m_ai.setState(state);

	//wait since the state will be gone after function ends
	MPI_Waitall(size - 1, &requests[0], &status[0]);
}


u_long VoteAI::makeMove()
{
	clock_t begin = clock(); //start time

	int rc = MPI_SUCCESS; // Return code
	//MPI_Status status;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//sends the allowed time to the workers, also works as a start order
	int ttime = (int) m_ai.getThinkingTime();
	sendTime(&ttime);

	// Compute its own results
	v_stat scores = m_ai.getMovesStatistics();

	int nbRes = 1; //the number of results recieved

	//prepares buffer		
	v_stat* buf = new v_stat[size - 1];
	for (int i = 0; i < (size - 1); ++i)
		for (int j = 0; j < POSSIBILITIES; ++j)
			buf[i].push_back(n_stat());

	//check for messages from other nodes
	SAY("start while");
	while ((double(clock() - begin) / CLOCKS_PER_SEC) < TIME_LIMIT_S) //while there is still time
	{
		for (int node = 1; node < size; node++) //for all nodes except master
		{
			MPI_Status s;
			int msg_recieved;
			MPI_Iprobe(node, RESUTLS, MPI_COMM_WORLD, &msg_recieved, &s);
			if (msg_recieved)
			{
				MPI_Request r;
				MPI_Irecv(&buf[node - 1], POSSIBILITIES * sizeof(n_stat), MPI_BYTE, node, RESUTLS, MPI_COMM_WORLD, &r);
			}
		}
	}

	//addition
	for (int node = 1; node < size; node++) //all but master
	{
		//adds the results to the score
		scores += buf[node - 1];
		nbRes++; //one more result received
	}

	if (rc != MPI_SUCCESS)
		cout << rank << " : failure on something" << endl;

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
			cout << scores.at(i).first << " : " << getValue(scores.at(i)) << "%" << endl;
#endif
		}
		cout << "Vote : " << nextMove << " (" << nextMoveChances << "%)" << endl;
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

double VoteAI::getValue(n_stat ns)
{
	return ns.second.first / ns.second.second;
}
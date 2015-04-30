#include "VoteAi.h"

using namespace std;


VoteAI::VoteAI(int t) : m_ai()
{
	if (getMPIRank() != MASTER)
	{
		std::cerr << "Master created on worker thread" << std::endl;
	}
	m_ai.setThinkingTime(t);
}

int VoteAI::getThinkingTime()
{
	return m_ai.getThinkingTime();
}

void VoteAI::setThinkingTime(int t)
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

}

void VoteAI::init(api::Game* g)
{
	//TODO : make game into transferrable data and send it
	//MPI_Request request;
	//for (int node = 1; node < size; node++) //for all nodes except master
	//{
	//	//sends message
	//	MPI_Isend(
	//		(void *)&game,		//data
	//		1,	//nb items
	//		MPI_INT,			//item type
	//		node,			//dest
	//		THINK_TIME,				//tag
	//		MPI_COMM_WORLD,
	//		&request
	//		);
	//}
	m_ai.init(g);
}

int VoteAI::makeMove(bool player)
{
	clock_t begin = clock(); //start time

	int rc = MPI_SUCCESS; // Return code
	MPI_Status status;

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//sends the allowed time to the workers, also works as a start order
	int ttime = m_ai.getThinkingTime();
	MPI_Request request;
	for (int node = 1; node < size; node++) //for all nodes except master
	{
		//sends message
		MPI_Isend(
			(void *)&ttime,		//data
			1,	//nb items
			MPI_INT,			//item type
			node,			//dest
			THINK_TIME,				//tag
			MPI_COMM_WORLD,
			&request
			);
	}

	// Initialize vector scores
	vector<int> scores;
	for (int i = 0; i < POSSIBILITIES; i++)
	{
		scores.push_back(0);
	}

	//processing done by each task
	vector<int> temp_scores = m_ai.getVotes();

	/* Use MPI_Reduce to sum values of homepi across all tasks
	* Master will store the accumulated value in pisum
	* - homepi is the send buffer
	* - pisum is the receive buffer (used by the receiving task only)
	* - the size of the message is sizeof(double)
	* - MASTER is the task that will receive the result of the reduction
	*   operation
	* - MPI_SUM is a pre-defined reduction function (double-precision
	*   floating-point vector addition).  Must be declared extern.
	* - MPI_COMM_WORLD is the group of tasks that will participate.
	*/

	int nbRes = 1; //the number of results recieved

	//Gather all results in sum :
	//stores the master's results
	for (int i = 0; i < POSSIBILITIES; ++i)
	{
		scores[i] = temp_scores[i];
	}

	//prepares buffer		
	int** buf = new int*[size - 1];
	for (int i = 0; i < (size - 1); ++i)
		buf[i] = new int[POSSIBILITIES];

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
				MPI_Irecv(buf[node - 1], POSSIBILITIES, MPI_INT, node, RESUTLS, MPI_COMM_WORLD, &r);
			}
		}
	}

	//addition
	for (int node = 1; node < size; node++) //all but master
	{
		//adds the results to the score
		for (int i = 0; i < POSSIBILITIES; ++i)
		{
			scores[i] += buf[node - 1][i];
		}
		nbRes++; //one more result received
	}

	//division
	for (int i = 0; i < POSSIBILITIES; ++i)
	{
		scores[i] /= nbRes;
	}

	if (rc != MPI_SUCCESS)
		cout << rank << " : failure on something" << endl;


#if TALKATIVE > 1
	for (int i = 0; i < POSSIBILITIES; i++)
	{
		cout << "Rank " << rank << " - " << i << " : " << temp_scores.at(i) << endl;
	}
#endif

	// The master thread displays the percentage for each value and the maximum value
	int max = 0;
	int maxID = -1;
	if (rank == MASTER)
	{
		for (int i = 0; i < POSSIBILITIES; i++)
		{
			if (max < scores.at(i))
			{
				max = scores.at(i);
				maxID = i;
			}
#if TALKATIVE > 0
			cout << i << " : " << scores.at(i) << "%" << endl;
#endif
		}
		cout << "Vote : " << maxID << " (" << scores.at(maxID) << "%)" << endl;
	}

	return maxID;
}

double VoteAI::estimateWinChances()
{

}
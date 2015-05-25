// DistributedMCTS.cpp : Defines the entry point for the console application.
//

#if defined(_WIN32) || defined(_WIN64)
	#include "stdafx.h"
	#include "WorkerAI.h"
	#include "tools/read_args.h"
	#include "api_v2/Game_v2.h"
#else
	#include "WorkerAI.h"
	#include "../../MCTS_array/src/tools/read_args.h"
	#include "../../MCTS_array/src/api_v2/Game_v2.h"
#endif


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include <stdio.h>
#include <string.h>
#include <mpi.h>

int
main(int argc, char *argv[])
{

	int rc = MPI_SUCCESS; // Return code
	//MPI_Status status;

	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Get the name of this processor (usually the hostname).  We call                                                      
	// memset to ensure the string is null-terminated.  Not all MPI                                                        
	// implementations null-terminate the processor name since the MPI                                                     
	// standard specifies that the name is *not* supposed to be returned                                                   
	// null-terminated.                                                                                                    
	char name[MPI_MAX_PROCESSOR_NAME];
	int len;
	memset(name, 0, MPI_MAX_PROCESSOR_NAME);
	MPI_Get_processor_name(name, &len);
	memset(name + len, 0, MPI_MAX_PROCESSOR_NAME - len);

#if TALKATIVE > 1
	std::cout << "New task : Number of tasks=" << size << " My rank=" << rank << " My name=" << name << "." << std::endl;
#endif

	srand((unsigned)time(NULL) + rank*size + len); // Different seed for each process


	prog_options options = read_args(argc, (const char**) argv);
	options.percent_memory = 0.2;
	options.think_while_waiting = false;


	if (rank == MASTER)
	{
		VoteAI master(options);

		std::cout << "created master" << std::endl;

		api_v2::Game game;
		//master.setState(game.getState());

		std::cout << "state sent (or not)" << std::endl;

		int result = 0;
		int IA = 2;

		while (result == 0)
		{
			game.displayASCII();

			if (game.activePlayer() != IA)
			{
				int moveok = 0;

				while (moveok == 0)
				{
					std::string tmp;
					std::cout << std::endl << "Your move ?" << std::endl;
					std::cin >> tmp;
					int move = std::stoi(tmp);

					std::cout << "move recognized as " << move << std::endl;

					if (game.canMakeMove(move))
					{

						std::cout << "can make move" << std::endl;

						moveok += 1;
						std::cout << "sending move to master" << std::endl;
						master.acknowledgeMove(move);
						std::cout << "sending move to game" << std::endl;
						game.makeMove(move);
					}
					elseif(tmp == "exit")
					{
						std::cout << "exiting" << std::endl;
						exit(0);
					}
					elseif(tmp == "-1")
					{
						std::cout << "AI" << std::endl;
						IA = (IA == 2) ? 1 : 2;
						moveok += 1;
					}
					std::cout << "Done." << std::endl;
				}
			}
			elseif(game.activePlayer() == IA)
			{
				std::cout << std::endl << "AI turn... please wait." << std::endl;
				int move = master.makeMove();
				game.makeMove(move);
				std::cout << std::endl << "chosen move : " << move << std::endl;
			}

			result = game.getWinner();
		}
		std::cout << std::endl;
		game.displayASCII();

		if (result == 1)
		{
			std::cout << std::endl << "player 1 wins." << std::endl;
		}
		elseif(result == 2)
		{
			std::cout << std::endl << "player 2 wins." << std::endl;
		}
		else
		{
			std::cout << std::endl << "Board full : TIE." << std::endl;
		}

	}
	else //WORKER
	{
		WorkerAI worker(options);
		std::cout << "created worker " << rank << std::endl;
	}

	
	MPI_Finalize();


	//OLD TEST

//	const char *s = "HELLO FROM THE MASTER PROCESS!";
//	int par_rank, par_size;
//	FILE *fp = NULL;
//	char msgbuf[100], filename[100];
//
//	/* Init MPI */
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &par_rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &par_size);
//
//	char pname[MPI_MAX_PROCESSOR_NAME];
//	int plen = 0;
//	memset(pname, 0, sizeof(char)*MPI_MAX_PROCESSOR_NAME);
//	MPI_Get_processor_name(pname, &plen);
//
//	printf("%s: Rank %d is part of a %d processor job\n", pname, par_rank, par_size);
//
//	// The following code can trip up MPI if the Windows firewall is 
//	// being too restrictive and you are running on multiple nodes.
//	// If you get MPI timeout errors then disable the following code
//	// and try again.
//#if 1
//	msgbuf[0] = '\0';
//
//	/* Broadcast message from master to all other processors. */
//	if (par_rank == 0)
//	{
//		MPI_Bcast((void*)s, strlen(s) + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
//		strcpy_s(msgbuf, s);
//	}
//	else
//		MPI_Bcast((void*)msgbuf, strlen(s) + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
//
//	/* Write the message from the master to a file. */
//	sprintf_s(filename, "%s.%04d.log", argv[0], par_rank);
//	fopen_s(&fp, filename, "wt");
//	if (fp != NULL)
//	{
//		fprintf(fp, "Running %s with %d processors.\n", argv[0], par_size);
//		fprintf(fp, "This is the log for processor %d.\n", par_rank);
//		fprintf(fp, "Message: \"%s\"\n", msgbuf);
//		fclose(fp);
//	}
//#endif
//
//	/* Finalize MPI */
//	MPI_Finalize();

	return 0;
}
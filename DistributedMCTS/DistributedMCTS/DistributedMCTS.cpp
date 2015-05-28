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

//	int rc = MPI_SUCCESS; // Return code
	//MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
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
	printf("New task : Number of tasks=%d My rank=%d My name=%s.\n",size,rank,name);
#endif

	srand((unsigned)time(NULL) + rank*size + len); // Different seed for each process


	prog_options options = read_args(argc, (const char**) argv);
	options.think_while_waiting = false;


	if (rank == MASTER)
	{
		VoteAI master(options);

		api_v2::Game game;
		master.setState(game.getState());

		bool exit = false;
		int result = 0;
		int IA = 2;

		while (result == 0 && !exit)
		{
			game.displayASCII();
			if (game.activePlayer() != IA)
			{
				int moveok = 0;

				while (moveok == 0)
				{
					std::string tmp;
					printf("\nYour move ?\n");
					std::cin >> tmp;

					if(tmp == "exit")
					{
						moveok += 1;
						exit = true;
					}
					elseif(tmp == "-1")
					{
						IA = (IA == 2) ? 1 : 2;
						moveok += 1;
					}
					else
					{
						int move = std::stoi(tmp);
						if(game.canMakeMove(move))
						{
							moveok += 1;
							master.acknowledgeMove(move); //sending move to master
							game.makeMove(move); //sending move to game
						}
					}

				}
			}
			elseif(game.activePlayer() == IA)
			{
				printf("\nAI turn... please wait.\n");
				int move = master.makeMove();
				printf("\nChosen move : %d\n", move);
				game.makeMove(move);
			}

			result = game.getWinner();
		}

		if (!exit)
		{
			printf("\n");;
			game.displayASCII();

			if (result == 1)
			{
				printf("\nplayer 1 wins.\n");
			}
			elseif(result == 2)
			{
				printf("\nplayer 2 wins.\n");
			}
			else
			{
				printf("\nBoard full : TIE.\n");
			}
		}
	}
	else //WORKER
	{
		WorkerAI worker(options);
	}

	MPI_Finalize();
	return 0;
}
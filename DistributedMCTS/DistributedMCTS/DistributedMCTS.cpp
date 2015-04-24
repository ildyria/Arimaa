// DistributedMCTS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


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
	const char *s = "HELLO FROM THE MASTER PROCESS!";
	int par_rank, par_size;
	FILE *fp = NULL;
	char msgbuf[100], filename[100];

	/* Init MPI */
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &par_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &par_size);

	char pname[MPI_MAX_PROCESSOR_NAME];
	int plen = 0;
	memset(pname, 0, sizeof(char)*MPI_MAX_PROCESSOR_NAME);
	MPI_Get_processor_name(pname, &plen);

	printf("%s: Rank %d is part of a %d processor job\n", pname, par_rank, par_size);

	// The following code can trip up MPI if the Windows firewall is 
	// being too restrictive and you are running on multiple nodes.
	// If you get MPI timeout errors then disable the following code
	// and try again.
#if 1
	msgbuf[0] = '\0';

	/* Broadcast message from master to all other processors. */
	if (par_rank == 0)
	{
		MPI_Bcast((void*)s, strlen(s) + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
		strcpy_s(msgbuf, s);
	}
	else
		MPI_Bcast((void*)msgbuf, strlen(s) + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

	/* Write the message from the master to a file. */
	sprintf_s(filename, "%s.%04d.log", argv[0], par_rank);
	fopen_s(&fp, filename, "wt");
	if (fp != NULL)
	{
		fprintf(fp, "Running %s with %d processors.\n", argv[0], par_size);
		fprintf(fp, "This is the log for processor %d.\n", par_rank);
		fprintf(fp, "Message: \"%s\"\n", msgbuf);
		fclose(fp);
	}
#endif

	/* Finalize MPI */
	MPI_Finalize();

	return 0;
}
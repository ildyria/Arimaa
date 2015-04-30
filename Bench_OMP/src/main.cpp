#include <iostream>
#include <omp.h>
#include "Graph.h"
#include "Bench.h"
#define elseif else if

int main(int argc, char *argv[])
{
	Bench bench = Bench();
	bench.run();
	exit(1);
	return 0;
}
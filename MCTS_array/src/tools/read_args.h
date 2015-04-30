#pragma once
#include "tools/typedef.h"

prog_options read_args(int& argc, char const* argv[])
{

	prog_options options;
	options.think_while_waiting = true;
	options.memory_limited = false;
	options.hard_memory = 2048; // 2 Mo RAM
	options.time_to_search = 5;
	options.percent_memory = 0.95;
	options.updated = false;
	options.bench = false;

	options.test_api = false;
	options.foo1 = false;
	options.foo2 = false;
	options.foo3 = false;
	options.foo4 = false;
	options.foo5 = false;
	options.foo6 = false;
	options.foo7 = false;

	// report settings
	for (int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i],"-nothx") == 0)
		{
			options.think_while_waiting = 0;
			std::cout << "Ok, i won't think while waiting for your move." << std::endl;
		}
		elseif(strcmp(argv[i],"-n") == 0)
		{
			if(i + 1 < argc)
			{
				i++;
				int num_proc_to_use = 1;
				num_proc_to_use = max(num_proc_to_use, std::stoi(argv[i],nullptr,0));
				num_proc_to_use = num_proc_to_use > omp_get_num_procs() ? omp_get_num_procs() : num_proc_to_use ;
				std::cout << num_proc_to_use << " core selected over " << omp_get_num_procs() << "." << std::endl;

			 	omp_set_num_threads(num_proc_to_use);
			}
			else
			{
				std::cout << "-n require an additionnal parameter." << std::endl;
				exit(1);
			}
		}
		elseif(strcmp(argv[i],"-m") == 0)
		{
			if(i + 1 < argc)
			{
				i++;
				options.hard_memory = options.hard_memory > std::stoi(argv[i],nullptr,0) ? options.hard_memory : std::stoi(argv[i],nullptr,0);
				options.memory_limited = true;
				std::cout << options.hard_memory << "Mo will be used." << std::endl;
				options.updated = true;
			}
			else
			{
				std::cout << "-m require an additionnal parameter." << std::endl;
				exit(1);
			}
		}
		elseif(strcmp(argv[i],"-t") == 0)
		{
			if(i + 1 < argc)
			{
				i++;
				options.time_to_search = 1 > std::stoi(argv[i],nullptr,0) ? options.time_to_search : std::stoi(argv[i],nullptr,0);
				std::cout << "I will think for " << options.time_to_search << " seconds." << std::endl;
				options.updated = true;
			}
			else
			{
				std::cout << "-m require an additionnal parameter." << std::endl;
				exit(1);
			}
		}
		elseif(strcmp(argv[i],"-p") == 0)
		{
			if(i + 1 < argc)
			{
				i++;
				options.percent_memory = options.percent_memory < std::stod(argv[i],nullptr) ? options.percent_memory : std::stod(argv[i],nullptr); // maximum 95%
				options.percent_memory = options.percent_memory < 0.20 ? 0.20 : options.percent_memory; // minimum 20%
				std::cout << options.percent_memory << "% of the free memory will be used." << std::endl;
				options.updated = true;
			}
			else
			{
				std::cout << "-p require an additionnal parameter." << std::endl;
				exit(1);
			}
		}
		elseif(strcmp(argv[i],"-hm") == 0)
		{
			if(!options.memory_limited)
			{
				options.memory_limited = true;
				std::cout << options.hard_memory << "Mo will be used." << std::endl;
				options.updated = true;
			}
		}
		elseif(strcmp(argv[i],"-b") == 0 || strcmp(argv[i],"--bench") == 0)
		{
			options.bench = true;
			options.updated = true;
			options.time_to_search = 30;
		}
		elseif(strcmp(argv[i],"-a") == 0 || strcmp(argv[i],"--api") == 0)
		{
			options.test_api = true;
			options.updated = true;
			options.time_to_search = 2;
		}
		elseif(strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0 || strcmp(argv[i],"help") == 0)
		{
				std::cout << "-nothx : programm doesn't think while waiting for player's move." << std::endl;
				std::cout << "-n <num> : number of process to use while thinking." << std::endl;
				std::cout << "-m <num> : max memory to use." << std::endl;
				std::cout << "-hm : memory limited (2 Go by default)." << std::endl;
				std::cout << "-p <num> : percent of max memory to use." << std::endl;
				std::cout << "-----------------" << std::endl;
				std::cout << "-a : test api." << std::endl;
				std::cout << "-b : benchmark." << std::endl;
	//			std::cout << "-A : define the game as Arimaa." << std::endl;
				exit(1);
		}
		else
		{
			printf("Argument %d:%s\n",i,argv[i]);
		}
	}

	return options;
}
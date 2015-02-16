#pragma once
#include "typedef.h"
#include <omp.h>
#include <chrono>

/**
 * \brief ChillPill class
 * \details use it if you want to slow the program !
 */
class ChillPill
{
	ChillPill() {};
	~ChillPill() {};
public:
	static void take_it(u_long i, u_int time)
	{

/*
		struct timespec tim, tim2;
		tim.tv_sec = 0;
		tim.tv_nsec = time; // 1 ms sec
		nanosleep(&tim , &tim2);
		printf("%d : %ld\n", omp_get_thread_num(), i);
*/
	}

};

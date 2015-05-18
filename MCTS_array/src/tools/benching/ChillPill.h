#pragma once
#include "typedef.h"
#include <chrono>

using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::chrono::duration_cast;

/**
 * \brief ChillPill class
 * \details use it if you want to slow the program !
 */
class ChillPill
{
	ChillPill() {};
	~ChillPill() {};
public:

#if defined(_WIN32) || defined(_WIN64)
	static void take_it(u_long i, u_int time_to_wait = 1000)
	{
		u_int temp = 0;
		auto end_time = high_resolution_clock::now() + microseconds(time_to_wait);
		// printf("ChillPill %ld !\n", i);

		// YES THIS IS SUPER UGLY BUT THERE IS NO OTHER FUCKING SOLUTION
		while(high_resolution_clock::now() < end_time)
		{
			temp++;
		}
	}
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
	static void take_it(u_long i, u_int time_to_wait = 1000)
	{
		// printf("ChillPill %ld !\n", i);
		struct timespec tim, tim2;
		tim.tv_sec = 0;
		tim.tv_nsec = (time_to_wait*1000); // 1 μs sec
		nanosleep(&tim , &tim2);
	}
#else
	static void take_it(u_long i, u_int time_to_wait = 1000)
	{
		printf("I don't wanna sleep! :(\n");
	}
#endif
};

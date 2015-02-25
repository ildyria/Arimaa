#pragma once
#include <chrono>

using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::high_resolution_clock;

/**
 * WIP
 */
class Timer
{
	time_point<system_clock, system_clock::duration> start_time;
	time_point<system_clock, system_clock::duration> end_time;
public:
	Timer() {};
	~Timer() {};

	inline void start()
	{
		start_time = high_resolution_clock::now();
	}

	inline void stop()
	{
		end_time = high_resolution_clock::now();
	}
	
	duration<system_clock::rep, system_clock::period> result()
	{
		return end_time - start_time;
	}
};
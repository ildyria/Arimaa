#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <tchar.h>

// Use to convert bytes to KB
#define DIV 1024

// Specify the width of the field in which to print the numbers. 
// The asterisk in the format specifier "%*I64d" takes an integer 
// argument and uses it to pad and right justify the number.
#define WIDTH 7

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <stdio.h>

#endif

#include <iostream>
#include <ostream>


class Memory
{
	Memory() {};
	~Memory() {};

public:
	static size_t WinFUllInfo()
	{
		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof (statex);

		GlobalMemoryStatusEx(&statex);
		std::cout << std::endl;
		_tprintf(TEXT("There is  %*ld percent of memory in use.\n"), WIDTH, statex.dwMemoryLoad);
		_tprintf(TEXT("There are %*I64d total MB of physical memory.\n"), WIDTH, statex.ullTotalPhys / (DIV*DIV));
		_tprintf(TEXT("There are %*I64d free  MB of physical memory.\n"), WIDTH, statex.ullAvailPhys / (DIV*DIV));
//		_tprintf(TEXT("There are %*I64d total KB of paging file.\n"), WIDTH, statex.ullTotalPageFile / DIV);
//		_tprintf(TEXT("There are %*I64d free  KB of paging file.\n"), WIDTH, statex.ullAvailPageFile / DIV);
//		_tprintf(TEXT("There are %*I64d total KB of virtual memory.\n"), WIDTH, statex.ullTotalVirtual / DIV);
//		_tprintf(TEXT("There are %*I64d free  KB of virtual memory.\n"), WIDTH, statex.ullAvailVirtual / DIV);
		// Show the amount of extended memory available.
//		_tprintf(TEXT("There are %*I64d free  KB of extended memory.\n"), WIDTH, statex.ullAvailExtendedVirtual / DIV);
		return statex.ullAvailPhys;
	}


};

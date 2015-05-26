#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
//	#include <psapi.h>
	#include <stdio.h>
	#include <tchar.h>
	// Specify the width of the field in which to print the numbers. 
	// The asterisk in the format specifier "%*I64d" takes an integer 
	// argument and uses it to pad and right justify the number.
	#define WIDTH 7

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
	#include <stdio.h>
	#include <string.h>
#endif
#include <iostream>
#include <ostream>


class Memory
{
	Memory() {};
	~Memory() {};

public:


public:
#if defined(_WIN32) || defined(_WIN64)
	static DWORDLONG get_free_memory()
	{
		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof (statex);

		GlobalMemoryStatusEx(&statex);
		std::cout << std::endl;
		_tprintf(TEXT("There is  %*ld percent of memory in use.\n"), WIDTH, statex.dwMemoryLoad);
		_tprintf(TEXT("There are %*I64d total MB of physical memory.\n"), WIDTH, statex.ullTotalPhys >> 20);
		_tprintf(TEXT("There are %*I64d free  MB of physical memory.\n"), WIDTH, statex.ullAvailPhys >> 20);
		return statex.ullAvailPhys;
	}

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
	static unsigned long get_free_memory()
	{
			// Return the amount of free memory in kbytes.
			// Returns -1 if something went wrong.
			long returnValue;
			long returnValue2;
			const int BUFFER_SIZE = 1000;
			char buffer[BUFFER_SIZE];
			FILE *fInput;
			int loop;
			char ch;
			returnValue = -1;
			returnValue2 = -1;

			fInput = fopen("/proc/meminfo", "r");
			if (fInput != nullptr)
			{
				while (!feof(fInput))
				{
					if(fgets(buffer, BUFFER_SIZE - 1, fInput) != nullptr){}; // remove another warning... thx Rodrigue...
					if (feof(fInput))
					{
						break;
					}
					buffer[BUFFER_SIZE - 1] = 0;
					// Look for serial number
					if (strncmp(buffer, "MemFree:", 8) == 0)
					{
						// Extract mem free from the line.
						for (loop = 0; loop<BUFFER_SIZE; loop++)
						{
							ch = buffer[loop];
							if (ch == ':')
							{
								returnValue = 0;
								continue;
							}
							if (ch == 0)
							{
								break;
							}
							if (returnValue >= 0)
							{
								if (ch >= 'A')
								{
									break;
								}
								if ((ch >= '0') && (ch <= '9'))
								{
									returnValue = returnValue * 10 + (ch - '0');
								}
							}
						}
//						break;
					}
					if (strncmp(buffer, "MemAvailable:", 13) == 0)
					{
						for (loop = 0; loop<BUFFER_SIZE; loop++)
						{
							ch = buffer[loop];
							if (ch == ':')
							{
								returnValue2 = 0;
								continue;
							}
							if (ch == 0)
							{
								break;
							}
							if (returnValue2 >= 0)
							{
								if (ch >= 'A')
								{
									break;
								}
								if ((ch >= '0') && (ch <= '9'))
								{
									returnValue2 = returnValue2 * 10 + (ch - '0');
								}
							}
						}
						break;
					}

				}
				fclose(fInput);
			}
			// take the maximum
			returnValue = (returnValue2 > returnValue) ? returnValue2 : returnValue;
			std::cout << "There are " << (returnValue)/1024 <<" free MB of physical memory.\n" << std::endl;
			return static_cast<unsigned long>(returnValue <<10);
		}
#endif
};

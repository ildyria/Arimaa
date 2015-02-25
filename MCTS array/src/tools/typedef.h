#pragma once
#include <cinttypes>

typedef uint64_t numtyp;

typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;

#define elseif else if

#define SIZEX 8
#define SIZEY 8
#define TIME_SEARCH 2

//#define LIMIT_MEMORY		// 2 Go max
//#define HARD_LIMIT_MEMORY	// 2048 nodes max

//#define DUMP_TREES			// dump trees in the console (only use with HARD_LIMIT_MEMORY)
//#define TREE_INTEGRITY		// check the integrity of the tree, slow down the prunning

#define OPENMP				// enable multi threading

//#define DOUBLE_TREE			// double tree strategy : safer but less nodes

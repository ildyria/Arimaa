#pragma once
#include <cinttypes>

typedef uint64_t numtyp;

typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;

#define elseif else if

#define TIME_SEARCH 1

//#define TEST_API

//#define ARIMAA

#ifdef ARIMAA
	#define SIZEX 6
	#define SIZEY 6
#else
	#define SIZEX 7
	#define SIZEY 6
#endif

#define NB_PIECE 3
//#define LIMIT_MEMORY		// 2 Go max
//#define HARD_LIMIT_MEMORY	// 2048 nodes max

//#define DUMP_TREES			// dump trees in the console (only use with HARD_LIMIT_MEMORY)
//#define TREE_INTEGRITY		// check the integrity of the tree, slow down the prunning
#define TREE_SHIFT_PRINT

#define OPENMP				// enable multi threading

//#define DOUBLE_TREE			// double tree strategy : safer but less nodes
#define PRUNE_NUMBER			10

#define SAFE_TREE

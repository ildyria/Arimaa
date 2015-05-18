#pragma once
#include <cinttypes>
#include <utility>
#include <vector>

typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;

typedef std::pair<u_long, u_long> p_stat;
typedef std::pair <u_long, p_stat> n_stat;
typedef std::vector<n_stat> v_stat;


//Adds up the 2 statistics
p_stat operator += (p_stat a, p_stat b);

//Adds up the statistics that share the same IDs (the vector is considered unordered)
v_stat operator += (v_stat a, v_stat b);

typedef struct prog_options {
	double percent_memory; // 8
	short hard_memory; // 2
	short time_to_search; // 2 
	bool memory_limited; // 1
	bool think_while_waiting; // 1
	bool updated;	// 1
	bool bench;		// 1
	bool test_api;	// 1
	bool foo1;		// 1
	bool foo2;		// 1
	bool foo3;		// 1
	bool foo4;		// 1
	bool foo5;		// 1
	bool foo6;		// 1
	bool foo7;		// 1
} prog_options;

#define elseif else if

#define TIME_SEARCH 2

//#define TEST_API

// #define ARIMAA

#ifdef ARIMAA
	#define SIZEX 6
	#define SIZEY 6
#else
	#define SIZEX 7
	#define SIZEY 6
#endif

#define TOP_BORDER 67645734912
#define BOTTOM_BORDER 63
#define LEFT_BORDER 34905131040
#define RIGHT_BORDER 1090785345

#define NB_PIECE 3


//#define DUMP_TREES			// dump trees in the console (only use with HARD_LIMIT_MEMORY)
//#define TREE_INTEGRITY		// check the integrity of the tree, slow down the prunning
//#define TREE_SHIFT_PRINT
//#define TREE_CLEAN_DISPLAY

#define OPENMP				// enable multi threading

#define DOUBLE_TREE			// double tree strategy : safer but less nodes
#define PRUNE_NUMBER			10

#define SAFE_TREE


// number to check borders : 

//	8 x 8
//  top : 18374686479671623680
//  bottom : 255 (2⁸ - 1)
//	border left : 9259542123273814144
//	border right : 72340172838076673

// static const unsigned long toto = 1 
// | (static_cast<unsigned long>(1) << 8) 
// | (static_cast<unsigned long>(1) << 16) 
// | (static_cast<unsigned long>(1) << 24) 
// | (static_cast<unsigned long>(1) << 32) 
// | (static_cast<unsigned long>(1) << 40) 
// | (static_cast<unsigned long>(1) << 48) 
// | (static_cast<unsigned long>(1) << 56);

// << 7 = the other side

// static const unsigned long toto = 1
// | (static_cast<unsigned long>(1) << 1)
// | (static_cast<unsigned long>(1) << 2)
// | (static_cast<unsigned long>(1) << 3)
// | (static_cast<unsigned long>(1) << 4)
// | (static_cast<unsigned long>(1) << 5)
// | (static_cast<unsigned long>(1) << 6)
// | (static_cast<unsigned long>(1) << 7);

// << 56 = the top


//	6 x 6 
//	top : 67645734912
//	bottom : 63 (2⁶ - 1)
//	border left : 34905131040
//	border right : 1090785345

// static const unsigned long toto = 1 
// | (static_cast<unsigned long>(1) << 6) 
// | (static_cast<unsigned long>(1) << 12) 
// | (static_cast<unsigned long>(1) << 18) 
// | (static_cast<unsigned long>(1) << 24) 
// | (static_cast<unsigned long>(1) << 30);

// << 5 = the other side

// static const unsigned long toto = 1
// | (static_cast<unsigned long>(1) << 1)
// | (static_cast<unsigned long>(1) << 2)
// | (static_cast<unsigned long>(1) << 3)
// | (static_cast<unsigned long>(1) << 4)
// | (static_cast<unsigned long>(1) << 5);

// << 30 = the top
#pragma once
#include <cinttypes>

typedef uint64_t numtyp;

typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;

#define elseif else if

#define TIME_SEARCH 3

//#define TEST_API

//#define ARIMAA

#ifdef ARIMAA
	#define SIZEX 6
	#define SIZEY 6

	#define TOP_BORDER 67645734912
	#define BOTTOM_BORDER 63
	#define LEFT_BORDER 34905131040
	#define RIGHT_BORDER 1090785345
#else
	#define SIZEX 7
	#define SIZEY 6
#endif

#define NB_PIECE 3
//#define LIMIT_MEMORY		// 2 Go max
//#define HARD_LIMIT_MEMORY	// 2048 nodes max

//#define DUMP_TREES			// dump trees in the console (only use with HARD_LIMIT_MEMORY)
//#define TREE_INTEGRITY		// check the integrity of the tree, slow down the prunning
//#define TREE_SHIFT_PRINT

#define OPENMP				// enable multi threading

//#define DOUBLE_TREE			// double tree strategy : safer but less nodes
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
#pragma once
#include "typedef.h"
#include <algorithm>
#include <iostream>
#include "../arimaa_simple/Arimaa.h"
#include "../arimaa_simple/arimaa_tools.h"
#include "../interfaces/Bitboard.h"

void fun_test1 ();

void display_board_num();

// move test
void fun_test_Arimaa1(prog_options options);

// board test
void fun_test_Arimaa2(prog_options options);

// trap test
void fun_test_Arimaa3(prog_options options);

// move generation test
void fun_test_Arimaa4(prog_options options);


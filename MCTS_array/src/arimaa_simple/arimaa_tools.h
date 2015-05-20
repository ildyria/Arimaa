/**
* \file Arimaa_tools.h
* \brief Define class Arimaa
* \author Benoit Viguier
* \version 1.0
* \date March 4rf 2015
*
*/
#pragma once
#include "../interfaces/Move.h"
#include <string>
#include <sstream>
#include <map>


/**
 * \brief Arimaa class
 * \details Class that contains the functions required to play the game.
 */
class Arimaa_tools
{

public:
	static inline u_long compare_0_u(const u_long& u1, const u_long& u2) {
		return static_cast<u_long>((u1 & u2) == 0);
	};

	static inline u_long compare_1_u(const u_long& u1, const u_long& u2) {
		return static_cast<u_long>((u1 & u2) != 0);
	};

	static inline bool compare_0_b(const u_long& u1, const u_long& u2) {
		return (u1 & u2) == 0;
	};

	static inline bool compare_1_b(const u_long& u1, const u_long& u2) {
		return (u1 & u2) != 0;
	};

	static inline u_short get_pos_next(const u_short& n) {
		static const short lookup[] = {1,-SIZEX,-1,SIZEX};
		return lookup[n];
	};

	static Move convert_move(std::string move)
	{
		static const std::map<char, int> transform = 
			{
				{'n', 3}, // north
				{'e', 2}, // east
				{'s', 1}, // south
				{'w', 0}, // west
				{'A', SIZEX - 1}, // first col
				{'B', SIZEX - 2}, // second col...
				{'C', SIZEX - 3},
				{'D', SIZEX - 4},
				{'E', SIZEX - 5},
				{'F', SIZEX - 6},
				{'G', SIZEX - 7},
				{'H', SIZEX - 8},
				{'1', 1}, // first row
				{'2', 2}, // second row...
				{'3', 3},
				{'4', 4},
				{'5', 5},
				{'6', 6},
				{'7', 7},
				{'8', 8}
			};

		u_long futurmove;
		// futurmove =  board_num | pos | NESW
		futurmove = ((transform.at(move[0]) + (transform.at(move[1]) - 1)*SIZEX) << 2) | transform.at(move[2]);
		std::cout << futurmove << std::endl;
		std::cout << (futurmove & 3) << std::endl;
		return Move(futurmove);
	};


	static std::string convert_move(Move& move)
	{
		static const std::map<int, char> map_nsew = 
			{
				{3 ,'n'}, // north
				{2 ,'e'}, // east
				{1 ,'s'}, // west
				{0 ,'w'} // south
			};

		static const std::map<int, char> map_coord = 
			{
				{SIZEX - 1,'A'}, // first col
				{SIZEX - 2,'B'}, // second col...
				{SIZEX - 3,'C'},
				{SIZEX - 4,'D'},
				{SIZEX - 5,'E'},
				{SIZEX - 6,'F'},
				{SIZEX - 7,'G'},
				{SIZEX - 8,'H'}
			};

		#if NB_PIECE == 3
			static const std::map<int, char> map_piece = 
				{
					{0 ,'R'}, // rabbit gold
					{1 ,'C'}, // cat gold
					{2 ,'D'}, // dog gold
					{3 ,0x03A9},
					{4 ,'r'}, // rabbit silver
					{5 ,'c'}, // cat silver
					{6 ,'d'}, // dog silver
					{7 ,0x03C9}
				};
		#else
			static const std::map<int, char> map_piece = 
				{
					{0 ,'R'}, // rabbit gold
					{1 ,'C'}, // cat gold
					{2 ,'D'}, // dog gold
					{3 ,'H'}, // horse gold
					{4 ,'M'}, // camel gold
					{5 ,'E'}, // elephant gold
					{6 ,0x03A9},
					{7 ,'r'}, // rabbit silver
					{8 ,'c'}, // cat silver
					{9 ,'d'}, // dog silver
					{10 ,'h'}, // horse silver
					{11 ,'m'}, // camel silver
					{12 ,'e'}, // elephant silver
					{13 ,0x03C9}
				};
		#endif

		constexpr u_long move_mask = (static_cast<u_long>(1) << 16) - 1;
		constexpr u_long num_mask = (static_cast<u_long>(1) << 8) - 1;
		constexpr u_long pos_mask = (static_cast<u_long>(1) << 6) - 1;
		constexpr u_long nsew_mask = (static_cast<u_long>(1) << 2) - 1;

		int num_board;
		int pos;
		int nsew;

		std::stringstream res;

		u_long moves = move.get_move();

		for(int i = 0 ; i < 4; ++i)
		{
			nsew = static_cast<int>((moves & move_mask) & nsew_mask);
			pos = static_cast<int>(((moves & move_mask) >> 2) & pos_mask);
			num_board = static_cast<int>(((moves & move_mask) >> 8) & num_mask);
			
			res << map_piece.at(num_board);
			res << map_coord.at(pos%SIZEX);
			res << (pos/SIZEX) + 1;
			res << map_nsew.at(nsew);

			res << " ";
			moves >>= 16;
		}

		return res.str();
	};

};
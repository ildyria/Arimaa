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
		auto transform = std::map<char, int>();
		transform.insert(std::pair<char, int>('n', 3)); // north
		transform.insert(std::pair<char, int>('e', 2)); // east
		transform.insert(std::pair<char, int>('w', 1)); // west
		transform.insert(std::pair<char, int>('s', 0)); // south
		transform.insert(std::pair<char, int>('A', SIZEX - 1)); // first col
		transform.insert(std::pair<char, int>('B', SIZEX - 2)); // second col...
		transform.insert(std::pair<char, int>('C', SIZEX - 3));
		transform.insert(std::pair<char, int>('D', SIZEX - 4));
		transform.insert(std::pair<char, int>('E', SIZEX - 5));
		transform.insert(std::pair<char, int>('F', SIZEX - 6));
		transform.insert(std::pair<char, int>('G', SIZEX - 7));
		transform.insert(std::pair<char, int>('H', SIZEX - 8));
		transform.insert(std::pair<char, int>('1', 1)); // first row
		transform.insert(std::pair<char, int>('2', 2)); // second row...
		transform.insert(std::pair<char, int>('3', 3));
		transform.insert(std::pair<char, int>('4', 4));
		transform.insert(std::pair<char, int>('5', 5));
		transform.insert(std::pair<char, int>('6', 6));
		transform.insert(std::pair<char, int>('7', 7));
		transform.insert(std::pair<char, int>('8', 8));

		u_long futurmove;
		// futurmove = NESW | pos  
		futurmove = transform[move[0]] + (transform[move[1]] - 1)*SIZEX + 64 * transform[move[2]];
		std::cout << futurmove << std::endl;
		std::cout << (futurmove >> 6) << std::endl;
		return Move(futurmove);
	};
};
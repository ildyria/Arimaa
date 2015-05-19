/**
* \file Arimaa_tools.h
* \brief Define class Arimaa
* \author Benoit Viguier
* \version 1.0
* \date March 4rf 2015
*
*/
#pragma once

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
};
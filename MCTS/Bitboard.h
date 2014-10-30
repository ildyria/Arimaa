/**
 * \file Bitboard.h
 * \brief Define class Bitboard
 * \author Benoit Viguier
 * \version 1.0
 * \date october 30th 2014
 *
 */
#pragma once
#include <iostream>     // std::cout
#include <algorithm>    // std::for_each
#include <vector>       // std::vector
#include <list>			// std::list

using namespace std;

/**
 * \brief Bitboard class
 * \details vector<unsigned long> _boards : bitboards of the game;
	short _toplay : player to play (1 or 2);
	short _number : number of bitboards;
	short _size   : size of the bitboards;

 */
class Bitboard
{
	vector<unsigned long> _boards;
	short _toplay;
	short _number;
	short _size;

public:
	/**
	 * \fn Bitboard
	 * \brief Default constructor
	 */
	Bitboard();

	/**
	 * \fn Bitboard
	 * \brief Default constructor
	 * 
	 * \param size	 : size of the boards
	 * \param n		 : number of boards to be created
	 * \param toplay : player to play
	 */
	Bitboard(int size, int n, int toplay);

	/**
	 * \fn ~Bitboard
	 * \brief Destructor of the class
	 */
	~Bitboard();

	/**
	* \fn clone
	* \brief clone a Bitboard
	*/
	inline Bitboard* clone() const { return new Bitboard(*this); }

	/**
	 * \fn getBit
	 * \brief return the value of the bit on a given board and position
	 * 
	 * \param n bitboard to search
	 * \param x position x
	 * \param y position y
	 * \return the value of the bit (0 or 1)
	 */
	int getBit(int n, int x, int y) const;

	/**
	* \fn getBit
	* \brief return the value of the bit on a given board and position
	*
	* \param n bitboard to search
	* \param pos position of the bit
	* \return the value of the bit (0 or 1)
	*/
	int getBit(int n, int pos) const;

	/**
	 * \fn setBit
	 * \brief set to 1 the bit on a given board and position
	 * 
	 * \param n bitboard to search
	 * \param x position x
	 * \param y position y
	 */
	void setBit(int n, int x, int y);

	/**
	* \fn setBit
	* \brief set to 1 the bit on a given board and position
	*
	* \param n bitboard to search
	* \param pos position of the bit
	*/
	void setBit(int n, int pos);

	/**
	 * \fn clearBit
	 * \brief set to 0 the bit on a given board and position
	 * 
	 * \param n bitboard to search
	 * \param x position x
	 * \param y position y
	 */
	void clearBit(int n, int x, int y);

	/**
	* \fn clearBit
	* \brief set to 0 the bit on a given board and position
	*
	* \param n bitboard to search
	* \param pos position of the bit
	*/
	void clearBit(int n, int pos);

	/**
	* \fn getBoard
	* \brief return the given board
	*
	* \param n board to look for
	* \return board number n
	*/
	inline unsigned long getBoard(int n) const { return _boards[n]; } ;

	/**
	 * \fn getEmpty
	 * \brief list 0 bits of a given board
	 * 
	 * \param n board to look at
	 * \return list of the empty positions (0 to size² -1)
	 */
	list<int> getEmpty(int n) const;

	/**
	 * \fn getOccupied
	 * \brief list 1 bits of a given board
	 * 
	 * \param n board to look at
	 * \return list of the set positions (0 to size² -1)
	 */
	list<int> getOccupied(int n);

	/**
	* \fn getPlayer
	* \brief return the player to play
	*
	* \return player to play (1 or 2)
	*/
	inline int getPlayer() const { return _toplay; };

	/**
	* \fn play
	* \brief swap players
	*/
	inline void play() { _toplay = (_toplay == 1) ? 2 : 1; };
};


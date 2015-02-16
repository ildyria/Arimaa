/**
 * \file Bitboard.h
 * \brief Define class Bitboard
 * \author Benoit Viguier
 * \version 1.0
 * \date October 30th 2014
 *
 */
#pragma once
#include <stdint.h>
#include <iostream>     // std::cout
#include <algorithm>    // std::for_each
#include <vector>
#include <list>
#include "../tools/Count.h"
#include "../tools/typedef.h"

/**
 * \brief Bitboard class
 * \details vector<unsigned long> _boards : bitboards of the game;
 *	short _toplay : player to play (1 or 2);
 *	short _number : number of bitboards;
 *	short _sizeX  : number of rows of the bitboards;
 *	short _sizeY  : number of lines of the bitboards;
 */
typedef uint64_t numtyp;

class Bitboard
{

protected:
	std::vector<numtyp> _boards;
	short _toplay;
	short _number;

public:
	/**
	 * \fn Bitboard
	 * \brief Default constructor
	 */
	Bitboard();

	/**
	 * \fn Bitboard
	 * \brief Constructor of the Bitboard given the sizes and number of bitboards to store.
	 * 
	 * \param sizeX	 : number of rows
	 * \param sizeY	 : number of lines
	 * \param n		 : number of boards to be created
	 * \param toplay : player to play
	 */
	Bitboard(int n, int toplay);

	/**
	 * \fn ~Bitboard
	 * \brief Destructor of the class
	 */
	virtual ~Bitboard();

	/**
	* \fn clone
	* \brief clone a Bitboard
	*/
	virtual inline Bitboard* clone() const
	{
		Count::I()->addBitBoards();
		return new Bitboard(*this);
	}

	/**
	* \fn getSizeX
	* \brief blabla
	*/
	virtual inline int getSizeX() const { return SIZEX; }

	/**
	* \fn getSizeY
	* \brief blabla
	*/
	virtual inline int getSizeY() const { return SIZEY; }

	/**
	 * \fn getBit
	 * \brief return the value of the bit on a given board and position
	 * 
	 * \param n bitboard to search
	 * \param x position x
	 * \param y position y
	 * \return the value of the bit (0 or 1)
	 */
	virtual inline int getBit(int n, int x, int y) const { return int((_boards[n]) >> (x + y*SIZEX)) & 1; };

	/**
	* \fn getBit
	* \brief return the value of the bit on a given board and position
	*
	* \param n bitboard to search
	* \param pos position of the bit
	* \return the value of the bit (0 or 1)
	*/
	virtual inline int getBit(int n, int pos) const { return int((_boards[n]) >> pos) & 1; };

	/**
	 * \fn setBit
	 * \brief set to 1 the bit on a given board and position
	 * 
	 * \param n bitboard to search
	 * \param x position x
	 * \param y position y
	 */
	virtual inline void setBit(int n, int x, int y) { _boards[n] |= (static_cast<numtyp>(1) << (x + y*SIZEX)); };

	/**
	* \fn setBit
	* \brief set to 1 the bit on a given board and position
	*
	* \param n bitboard to search
	* \param pos position of the bit
	*/
	virtual inline void setBit(int n, int pos) { _boards[n] |= (static_cast<numtyp>(1) << pos); };

	/**
	 * \fn clearBit
	 * \brief set to 0 the bit on a given board and position
	 * 
	 * \param n bitboard to search
	 * \param x position x
	 * \param y position y
	 */
	virtual inline void clearBit(int n, int x, int y) { _boards[n] &= ~(static_cast<numtyp>(1) << (x + y*SIZEX)); };

	/**
	* \fn clearBit
	* \brief set to 0 the bit on a given board and position
	*
	* \param n bitboard to search
	* \param pos position of the bit
	*/
	virtual inline void clearBit(int n, int pos) { _boards[n] &= ~(static_cast<numtyp>(1) << (pos)); };

	/**
	* \fn getBoard
	* \brief return the given board
	*
	* \param n board to look for
	* \return board number n
	*/
	virtual inline numtyp getBoard(int n) const { return _boards[n]; };

	/**
	 * \fn getEmpty
	 * \brief list 0 bits of a given board
	 * 
	 * \param n board to look at
	 * \return list of the empty positions (0 to size² -1)
	 */
	virtual std::list<int> getEmpty(int n) const;

	/**
	 * \fn getOccupied
	 * \brief list 1 bits of a given board
	 * 
	 * \param n board to look at
	 * \return list of the set positions (0 to size² -1)
	 */
	virtual std::list<int> getOccupied(int n);

	/**
	* \fn getPlayer
	* \brief return the player to play
	*
	* \return player to play (1 or 2)
	*/
	virtual inline int getPlayer() const { return _toplay; };

	/**
	* \fn play
	* \brief swap players
	*/
	virtual inline void play()
	{
		static const short lookup[3] = { 0, 2, 1 };
		_toplay = lookup[_toplay];
	};
};


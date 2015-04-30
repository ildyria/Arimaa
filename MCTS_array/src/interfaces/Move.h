/**
 * \file Move.h
 * \brief Define class Move
 * \author Benoit Viguier
 * \version 1.0
 * \date October 30th 2014
 *
 */
#pragma once
#include <string>
#include <iostream>
#include <stdint.h>
#include "../tools/typedef.h"

/**
 * \brief Move class
 * \details Move objects, nothing more to say.
 */
class Move
{
	u_long _data;

public:
	/**
	 * \fn Move()
	 * \brief basic constructor
	 */
	Move();

	/**
	 * \fun Move(std::string move)
	 * \brief create a Move given a string
	 * 
	 * \param move : move to be recorded
	 */
	explicit Move(std::string move);

	/**
	 * \fun Move(int move)
	 * \brief create a Move given an int
	 * 
	 * \param move : move to be recorded
	 */
	explicit Move(int move);	
	explicit Move(u_long move);

	/**
	 * \fn ~Move()
	 * \brief basic destructor
	 */
	virtual ~Move();

	/**
	 * \fn get_move()
	 * \brief return the move as a string.
	 */
	virtual inline u_long get_move() { return _data; };

	/**
	 * \fun set()
	 * \brief setter of the move. Used with the >> operator. Required for the class to stay inheritable.
	 * 
	 * \param data
	 */
	virtual inline void set(u_long data) { _data = data; };

	/**
	 * \fun get()
	 * @brief getter of the move. Used with the << operator. Required for the class to stay inheritable.
	 */
	virtual inline std::string get() { return std::to_string(_data); };

	/**
	 * \fun compare(const Move b)
	 * \brief Used with the == operator. Required for the class to stay inheritable. 
	 */
	virtual inline bool compare(const Move& b) const { return _data == b._data; };


	/**
	 * \fun operator<<
	 */
	friend std::ostream& operator<< (std::ostream& stream, Move &move);

	/**
	 * \fun operator>>
	 */
	friend std::istream& operator>> (std::istream& stream, Move &move);

	/**
	 * \fun operator==
	 */
	friend inline bool operator== (const Move &ma, const Move &mb) { return ma.compare(mb); };
};


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

/**
 * \brief Move class
 * \details Move objects, nothing more to say.
 */
class Move
{
	std::string _data;

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

	/**
	 * \fn ~Move()
	 * \brief basic destructor
	 */
	virtual ~Move();

	/**
	 * \fn getInt()
	 * 
	 * \return the int value of the move.
	 */
	virtual int getInt();

	/**
	 * \fn getMove()
	 * \brief return the move as a string.
	 */
	virtual inline std::string getMove() { return _data;  };

	/**
	 * \fun set()
	 * \brief setter of the move. Used with the >> operator. Required for the class to stay inheritable.
	 * 
	 * \param data
	 */
	virtual inline void set(std::string data);

	/**
	 * \fun get()
	 * @brief getter of the move. Used with the << operator. Required for the class to stay inheritable.
	 */
	virtual inline std::string get();

	/**
	 * \fun compare(const Move b)
	 * \brief Used with the == operator. Required for the class to stay inheritable. 
	 */
	virtual inline bool compare(const Move b) const { return _data == b._data; };


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


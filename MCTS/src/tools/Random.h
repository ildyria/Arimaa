/**
 * \file Random.h
 * \brief Define singleton Random
 * \author Benoit Viguier
 * \version 1.0
 * \date November 2nd 2014
 *
 */
#pragma once
#include <cstdlib>

/**
 * \brief Random class : SINGLETON
 * \details 
	static unsigned int _seed : seed used to initialize the srand(), by default time(null);
	static Random* UniqueInstance : instance of the following class;
 */
class Random
{
	static unsigned int _seed;
	static Random* UniqueInstance;
	
	Random();
	explicit Random(unsigned int seed);
	~Random();
public:
	
	/**
	 * \fn I
	 * \brief get the instance of the singleton.
	 * \param  seed optionnal, allow us to set the seed in case singleton hasn't been initialized
	 * \return      the object.
	 */
	static Random* I(unsigned int seed = 0);
	
	/**
	 * \fn getNum
	 * \brief returns an interger random number between min and max included.
	 * \param  min minimum integer to be returned
	 * \param  max maximum integer to be returned
	 * \return     random integer 
	 */
	inline int getNum(int min, int max) {
		int n = rand() % (max - min + 1) + min;
		return n;
	}

};

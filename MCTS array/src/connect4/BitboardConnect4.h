/**
* \file BitboardConnect4.h
* \brief Define class BitboardConnect4
* \author Benoit Viguier
* \version 1.0
* \date December 25th 2014
*
*/
#pragma once
#include "../interfaces/Bitboard.h"

/**
 * \brief BitboardConnect4 class
 * \details Class inherited from Bitboard class. This Bitboards are used to store the positions of the boards in Connect4.
 */
class BitboardConnect4 : public Bitboard
{
public:
	/**
	 * \fn BitboardConnect4()
	 * \brief constructor of BitboardConnect4, calls Bitboard(7,6,2,1) : 7 rows, 6 lines, 2 boards, 1st player to play.
	 */
	BitboardConnect4();

	/**
	 * \fn ~BitboardConnect4()
	 * \brief destructor of BitboardConnect4.
	 */
	~BitboardConnect4();

	/**
	 * \fn clone()
	 * \brief clone function for BitboardConnect4.
	 * 
	 * \param I Instance of Count to increment the number of Bitboards created
	 * \return Bitboard cloned
	 */
	virtual inline BitboardConnect4* clone() const override
	{
		Count::I()->addBitBoardsC4();
		return new BitboardConnect4(*this);
	}

	/**
	 * \fn get_empty(int n)
	 * \brief return the list of the empty positions of the boards, in this cases the columns that are not full.
	 * 
	 * \param n useless parameter but required by the parent class.
	 * \return list of the number of the available positions.
	 */
	virtual std::list<int> get_empty(int n) const override;
};


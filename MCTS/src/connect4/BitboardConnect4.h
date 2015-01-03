#pragma once
#include "Bitboard.h"

class BitboardConnect4 : public Bitboard
{
public:
	BitboardConnect4();
	~BitboardConnect4();
	virtual inline BitboardConnect4* clone() const override { return new BitboardConnect4(*this); }

	virtual std::list<int> getEmpty(int n) const override;
};


#pragma once
#include <string>
#include <iostream>

class Move
{
	std::string _data;

public:
	Move();
	explicit Move(std::string move);
	explicit Move(int move);
	int getInt();
	virtual inline std::string getMove() { return _data;  };
	virtual ~Move();
	friend std::ostream& operator<< (std::ostream& stream, Move &move);
	friend std::istream& operator>> (std::istream& stream, Move &move);
	friend inline bool operator== (const Move &a, const Move &b) { return a._data == b._data; };
};


#pragma once
#include <string>
#include <iostream>

using std::string;
using std::ostream;
using std::istream;
using std::to_string;

class Move
{
	string _data;

public:
	Move();
	explicit Move(string move);
	explicit Move(int move);
	int getInt();
	virtual inline string getMove() { return _data;  };
	virtual ~Move();
	friend ostream& operator<< (ostream& stream, Move &move);
	friend istream& operator>> (istream& stream, Move &move);
	friend inline bool operator== (const Move &a, const Move &b) { return a._data == b._data; };
};


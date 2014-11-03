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
	Move(string move);
	Move(int move);
	int getInt();
	~Move();
	friend ostream& operator<< (ostream& stream, Move &move);
	friend istream& operator>> (istream& stream, Move &chaine);
	friend bool operator== (const Move &a, const Move &b) { return a._data == b._data; };
};


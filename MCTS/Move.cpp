#include "Move.h"


Move::Move()
{
	_data = "undefined";
}

Move::Move(string move) : _data(move)
{
}

Move::Move(int move) : _data(to_string(move))
{
}

Move::~Move()
{
}

int Move::getInt()
{
	return strtol(_data.c_str(), nullptr, 10);
}

ostream& operator<<(ostream& stream, Move& move)
{
	stream << move._data;
	return stream;
}

istream& operator>>(istream& stream, Move& move)
{
	stream >> move._data;
	return stream;
}
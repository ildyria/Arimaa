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
	virtual ~Move();

	virtual int getInt();
	virtual inline std::string getMove() { return _data;  };

	// for stream
	virtual inline void set(std::string data);
	virtual inline std::string get();
	virtual inline bool compare(const Move b) const { return _data == b._data; };

	friend std::ostream& operator<< (std::ostream& stream, Move &move);
	friend std::istream& operator>> (std::istream& stream, Move &move);
	friend inline bool operator== (const Move &ma, const Move &mb) { return ma.compare(mb); };
};


#pragma once
#include <iostream>
#include <fstream>
#include <string>


class Debug
{

	std::string _filename;
	bool _startline;
	std::ofstream _file;


	void Debug::newstart(bool force=false);

public:
	typedef struct Endl {} Endl;

	Debug();
	explicit Debug(std::string name);
	~Debug(void);

	Debug& Debug::operator<<(bool trueorfalse);
	Debug& Debug::operator<<(std::string data);
	Debug& Debug::operator<<(const char*);
	Debug& Debug::operator<<(int data);
	Debug& Debug::operator<<(float data);
	Debug& Debug::operator<<(double data);
	Debug& Debug::operator<<(void* ptr);
	Debug& Debug::operator<<(Endl e);
};

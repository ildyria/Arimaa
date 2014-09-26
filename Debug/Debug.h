#pragma once
#define ENDL Debug::Endl()
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Debug
{

	string _filename;
	bool _startline;
	ofstream _file;


	void Debug::newstart(bool force=false);

public:
	typedef struct Endl {} Endl;

	Debug();
	Debug(string name);
	~Debug(void);

	
	Debug& Debug::operator<<(bool trueorfalse);
	Debug& Debug::operator<<(string data);
	Debug& Debug::operator<<(const char*);
	Debug& Debug::operator<<(int data);
	Debug& Debug::operator<<(float data);
	Debug& Debug::operator<<(Endl e);
};

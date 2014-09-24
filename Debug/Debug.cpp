#include "Debug.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

Debug::Debug()
{
	_filename = "debug.txt";
	cout << "debugging start" << endl;
	cout << "log in " << _filename << endl;
	cout << "WARNING : Time in GMT format." << endl;

	_file.open ("debug.txt");
	_file << "debugging start" << endl;
	_file << "WARNING : Time in GMT format." << endl;
}

Debug::Debug(string name):_filename(name)
{
	_startline = true;
	cout << "debugging start" << endl;
	cout << "log in " << _filename << endl;
	cout << "WARNING : Time in GMT format." << endl;

	_file.open (_filename);
	_file << "debugging start" << endl;
	_file << "WARNING : Time in GMT format." << endl;
}

Debug::~Debug(void)
{
	_file << endl << "debugging end" << endl;
	cout << endl <<  "debugging end" << endl;
	_file.close ();
}

void Debug::newstart(bool force)
{
	if(_startline || force)
	{
		auto now = chrono::system_clock::now();
		int time = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count() % (24*3600*1000);
		int hour = time / 1000 / 3600;
		int min = (time / 1000) % 3600 / 60;
		int sec = (time / 1000) % 3600 % 60;
		int milli = time % 1000;
		cout << "[" <<  hour << ":" << min << ":" << sec << "." << milli << "] ";
		_file << "[" <<  hour << ":" << min << ":" << sec << "." << milli << "] ";
	}
	_startline = false;
}

Debug& Debug::operator<<(bool trueorfalse)
{
	newstart();
	cout << ( trueorfalse ? "true" : "false") << " ";
	_file << ( trueorfalse ? "true" : "false") << " ";
	return *this;
}

Debug& Debug::operator<<(string data)
{
	newstart();
	cout << data << " ";
	_file << data << " ";
	return *this;
}

Debug& Debug::operator<<(const char* data)
{
	newstart();
	cout << data << " ";
	_file << data << " ";
	return *this;
}

Debug& Debug::operator<<(int data)
{
	newstart();
	cout << data << " ";
	_file << data << " ";
	return *this;
}

Debug& Debug::operator<<(float data)
{
	newstart();
	cout << data << " ";
	_file << data << " ";
	return *this;
}

Debug& Debug::operator<<(Debug::Endl e)
{
	cout << endl;
	_file << "\n";
	_startline = true;
	return *this;
}


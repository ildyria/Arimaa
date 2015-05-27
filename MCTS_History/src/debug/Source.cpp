#include <iostream>
#include <string>
#include "Debug.h"

using namespace std;

int main ()
{

	Debug debug = Debug();

	debug << "test1" << ENDL;

	debug << "test2" << ENDL;

	debug << "test3" << ENDL;

	debug << "test4" << ENDL;

	cin.ignore();

	return 0;
}
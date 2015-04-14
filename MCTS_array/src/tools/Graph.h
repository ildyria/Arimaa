#pragma once
#include <vector>
#include <iostream>     // std::cout
#include <algorithm>    // std::min_element, std::max_element

typedef std::pair<u_short, u_short> coord;

class Graph
{
	std::vector<std::vector<u_short>> _map;
	std::vector<coord> _vals;
	std::vector<double>& _data;
	u_long _num_data;
public:
	Graph(std::vector<double>& data, int xscale = 0, double yscale = 0);
	~Graph(){};
	void compute();	
	void draw();

	static void test();
};
#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>     // std::cout
#include <algorithm>    // std::min_element, std::max_element

typedef std::pair<u_short, u_short> coord;

class Graph
{
	std::vector<std::vector<u_short>> _map;
	std::vector<coord> _vals;
	std::vector<double>& _data;
	u_long _num_data;
	u_long _scale_x;
	u_long _graph_size_y;
public:
	Graph(std::vector<double>& data, double xscale = 1.0);
	~Graph(){};
	void compute();	
	void draw();

	static void test();
};
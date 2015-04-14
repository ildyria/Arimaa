#include "Graph.h"
#define GRAPH_SIZE_X 80
#define GRAPH_SIZE_Y 40

Graph::Graph(std::vector<double>& data, int xscale, double yscale) : 
	_map(std::vector<std::vector<u_short>>(GRAPH_SIZE_X,std::vector<u_short>(GRAPH_SIZE_Y,0))),
	_vals(std::vector<coord>(0)),
	_data(data)
{
	double max_val = *std::max_element(data.begin(), data.end());

	u_int num_data = _data.size();
	for (u_int i = 0; i < num_data; ++i)
	{
		u_int xpos = (i+1)*GRAPH_SIZE_X/num_data;
		u_int ypos = _data[i]*GRAPH_SIZE_Y/max_val;
		_map[xpos][ypos] = 1;
		_vals.push_back(coord(xpos,ypos));
	}

}

void Graph::compute()
{

}

void Graph::draw()
{
	for (u_int y = 0; y < GRAPH_SIZE_Y; ++y)
	{
		printf("%c", 179);
		for (u_int x = 0; x < GRAPH_SIZE_X; ++x)
		{
			if(_map[x][y] == 1)
			{
				printf("%c", 248);
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("%c", 197);
	for (u_int x = 0; x < GRAPH_SIZE_X; ++x)
	{
		printf("%c", 196);
	}
}
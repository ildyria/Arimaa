#include "Graph.h"
#define GRAPH_SIZE_X 80
#define GRAPH_SIZE_Y 20

Graph::Graph(std::vector<double>& data, int xscale, double yscale) : 
	_map(std::vector<std::vector<u_short>>(GRAPH_SIZE_X+1,std::vector<u_short>(GRAPH_SIZE_Y+1,0))),
	_vals(std::vector<coord>(0)),
	_data(data)
{
	double max_val = *std::max_element(data.begin(), data.end());

	_num_data = _data.size();
	for (u_int i = 0; i < _num_data; ++i)
	{
		u_int xpos = (i+1)*(GRAPH_SIZE_X/(_num_data+1));
		u_int ypos = _data[i]*GRAPH_SIZE_Y/(max_val+1);
		_map[xpos][ypos] = 1;
		_vals.push_back(coord(xpos,ypos));
	}

}

void Graph::compute()
{

}

void Graph::draw()
{
	for (int y = GRAPH_SIZE_Y; y >=0; --y)
	{
		for (u_int x = 0; x < GRAPH_SIZE_X; ++x)
		{
			if(x == 0)
			{
				printf("\u2502");
			}else 			if(_map[x][y] == 1)
			{
				printf("+");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\u253C");
	// for (u_int x = 0; x < GRAPH_SIZE_X; ++x)
	// {
	// 	printf("\u2500");
	// }
	for (u_int x = 1; x < GRAPH_SIZE_X; ++x)
	{
		if(x % (GRAPH_SIZE_X/(_num_data+1)) == 0)
			printf("\u252C");
		else
			printf("\u2500");
	}
	printf("\n");

	u_int i = 1;
	for (u_int x = 0; x < GRAPH_SIZE_X; ++x)
	{
		if(x == 0)
		{
			printf("0");
			x++;
		}
		else if(x % (GRAPH_SIZE_X/(_num_data+1)) == 0)
		{
			printf("%2d",i);
			i++;
			x++;
		}
		else if(i > _num_data) break;
		else
			printf(" ");
	}
	printf("\n");
}

void Graph::test(){
	std::vector<double> to_graph = {
		1523721/1523721.0,1522181/1523721.0,
		1710637/1523721.0,2147699/1523721.0,
		2612841/1523721.0,3034967/1523721.0,
		3518702/1523721.0,3802678/1523721.0,
		4081793/1523721.0,4093273/1523721.0,
		4234395/1523721.0,4624071/1523721.0,
		4590408/1523721.0,4449650/1523721.0,
		4957074/1523721.0,4737981/1523721.0,
		4499383/1523721.0,4703540/1523721.0,
		4569058/1523721.0,4433250/1523721.0,
		4497940/1523721.0,4309490/1523721.0,
		4359662/1523721.0,4416140/1523721.0};

	Graph* g = new Graph(to_graph);
	g->draw();
}

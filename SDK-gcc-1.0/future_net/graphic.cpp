#include "graphic.h"

graphic::graphic(char* gra[], int edge_num)
{
	unsigned short edge = 0;
	unsigned short start = 0;
	unsigned short end = 0;
	char weight = 0;

	for(int i=0; i<edge_num; i++)
	{
		start = 0;
		end = 0;
		edge = 0;
		weight = 0;

		int j=0;
		for(; gra[i][j] != ','; j++)
			edge = edge*10 + gra[i][j] - '0';

		j++;
		for(; gra[i][j] != ','; j++)
			start = start*10 + gra[i][j] - '0';

		j++;
		for(; gra[i][j] != ','; j++)
			end = end*10 + gra[i][j] - '0';

		j++;
		for(; gra[i][j] != '\n' && gra[i][j] != 0; j++)
			weight = weight*10 + gra[i][j] - '0';

		if(vertex_name.find(start) == vertex_name.end())	// start not get
		{
			vertex_name[start] = data.size();
			adjnode_type adjnode(edge, end, weight);
			node_type node;
			node.adj_list.push_back(adjnode);
			data.push_back(node);
		}
		else
		{
			int index = vertex_name[start];
			auto it = data[index].adj_list.begin();
			for(; it != data[index].adj_list.end(); it++)	// adjacent edges is at most 8
			{
				if(end == it->to_node)
				{
					if(weight < it->weight)
						it->weight = weight;
					break;
				}
			}
			if(it == data[index].adj_list.end())
			{
				adjnode_type adjnode(edge, end, weight);
				data[index].adj_list.push_back(adjnode);
			}
		}

		if(vertex_name.find(end) == vertex_name.end())		// end not get
		{
			vertex_name[end] = data.size();
			data.push_back(node_type());
		}
	}

	// set all to_node from graphic index to data index
	for(int i=0; i<data.size(); i++)
	{
		for(auto it = data[i].adj_list.begin(); it != data[i].adj_list.end(); it++)
			it->to_node = vertex_name[it->to_node];
	}
}

std::ostream& operator<<(std::ostream& os, const graphic& g)
{
	for(int i=0; i<g.data.size(); i++)
	{
		os << i << " =====> ";
		for(auto it = g.data[i].adj_list.begin(); it != g.data[i].adj_list.end(); it++)
			//os << it->to_node << "(" << (int)it->weight << ")" << " ";
			os << it->to_node << " (E " << (int)it->name << " W " << (int)it->weight << ");" << " ";
		os << std::endl;
	}
	return os;
}

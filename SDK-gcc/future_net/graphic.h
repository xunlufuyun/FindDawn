#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include <vector>
#include <list>
#include <iostream>
#include <unordered_map>

class graphic
{
	friend std::ostream& operator<<(std::ostream& os, const graphic& g);

public:
	struct adjnode_type
	{
		unsigned short name;		// edge real index
		unsigned short to_node;
		char weight;				// 1~20
		adjnode_type(unsigned short n, unsigned short to, char w)
			: name(n), to_node(to), weight(w)
		{}
	};

	struct node_type
	{
		bool known;
		short path;					// last node
		unsigned short dist;
		std::list<adjnode_type> adj_list;
		node_type()
			: known(false), path(NO_PATH), dist(MAX_DIST)
		{}
	};

	graphic(char* gra[], int edge_num);

	static const unsigned short MAX_DIST = 65535;
	static const short NO_PATH = -1;

private:
	//int edge_num;
	//int node_num;
	std::unordered_map<int, int> vertex_name;	// vertex to index 
	std::vector<node_type> data;			// adjacent list
};

#endif

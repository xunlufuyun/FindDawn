#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include <vector>
#include <list>
#include <iostream>
#include <unordered_map>
#include <climits>

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
		int dist;
		std::list<adjnode_type> adj_list;
		
		node_type()
			: known(false), path(NO_PATH), dist(MAX_DIST)
		{}
	};

	graphic(char* gra[], int edge_num);

	static const int MAX_DIST = INT_MAX;
	static const short NO_PATH = -1;

	const std::unordered_map<unsigned short, unsigned short>& get_vertex_name() const
	{
		return vertex_name;
	}

	std::vector<node_type>& get_data()
	{
		return data;
	}

private:
	//int edge_num;
	//int node_num;
	std::unordered_map<unsigned short, unsigned short> vertex_name;	// vertex to index

	std::vector<node_type> data;			// adjacent list
};

#endif

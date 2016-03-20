
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <algorithm>
#include <queue>

using namespace std;

vector<bool> can_to_end;

void get_condition(const graphic& gra, char *condition, condition_type& cond)
{
	unsigned short temp = 0;
	int i=0;
	for(; condition[i] != ','; i++)
		temp = temp*10 + condition[i] - '0';
	cond.start = temp;
	temp = 0;
	i++;
	for(; condition[i] != '\n' && condition[i] != ','; i++)
		temp = temp*10 + condition[i] - '0';
	cond.end = temp;
	if(condition[i] == '\n')
		return;
	temp = 0;
	i++;
	while(1)
	{
		if(condition[i] == '\n' || condition[i] == 0)
		{
			cond.pass.push_back(temp);
			break;
		}
		if(condition[i] == '|')
		{
			cond.pass.push_back(temp);
			temp = 0;
		}
		else
			temp = temp*10 + condition[i] - '0';
		i++;
	}
	// change vertex index to continuous
	unordered_map<unsigned short, unsigned short> vertex_name = gra.get_vertex_name();		// 这里有开销
	cond.start = vertex_name[cond.start];
	cond.end = vertex_name[cond.end];
	for(int i=0; i<cond.pass.size(); i++)
		cond.pass[i] = vertex_name[cond.pass[i]];
}

//你要完成的功能总入口
void search_route(char *graph[5000], int edge_num, char *condition)
{
    vector<unsigned short> answer(600, -1);		// 先分配足够内存
    int ed_num = 0;								// answer 中有几条边

    graphic 		gra(graph, edge_num);		// 解析出图
    condition_type 	cond;
    get_condition(gra, condition, cond);		// 解析出条件

    //cout << gra;

    //graphic_cut_down(gra, cond);

   	find_path2(gra, cond, answer, ed_num);
    if(ed_num > 0)
    {
    	for(int i=0; i<ed_num; i++)
    		record_result(answer[i]);
    }
}

bool is_valid_path(vector<unsigned short>& path, condition_type& cond)
{
	char exists[600] = {0};
	for(auto i : path)
		exists[i] = 1;
	for(auto i : cond.pass)
		if(exists[i] == 0)
			return false;
	return true;
}

// Dijkstra =================================================================================================================================


// DFS 		=====================================================================================================================================>

void find_path_dfs(graphic& gra, condition_type& cond, int& shortest_dist, vector<unsigned short>& path,
	vector<unsigned short>& edge, vector<unsigned short>& answer, int& edge_num, int curr_node)
{
	if(curr_node == cond.end)
	{
		if(gra.get_data()[curr_node].dist < shortest_dist && is_valid_path(path, cond))
		{
			shortest_dist = gra.get_data()[curr_node].dist;
			edge_num = edge.size();
			copy(edge.begin(), edge.end(), answer.begin());
			//cout << "get a result .............." << endl;
		}
		return;
	}
	//else if(!can_to_end[curr_node])
	//	return;

	for(auto it = gra.get_data()[curr_node].adj_list.begin(); it != gra.get_data()[curr_node].adj_list.end(); it++)
	{
		if(!gra.get_data()[it->to_node].known)
		{
			edge.push_back(it->name);
			path.push_back(it->to_node);
			gra.get_data()[it->to_node].known = true;
			gra.get_data()[it->to_node].dist = gra.get_data()[curr_node].dist + it->weight;

			find_path_dfs(gra, cond, shortest_dist, path, edge, answer, edge_num, it->to_node);

			gra.get_data()[it->to_node].known = false;
			path.pop_back();
			edge.pop_back();
		}
	}
}

void find_path(graphic& gra, condition_type& cond, vector<unsigned short>& answer, int& edge_num)
{
	int shortest_dist = graphic::MAX_DIST;

	vector<unsigned short> path;			// 记录活动顶点
	vector<unsigned short> edge;			// 记录活动的边

	gra.get_data()[cond.start].known = true;
	gra.get_data()[cond.start].dist = 0;
	path.push_back(cond.start);

	find_path_dfs(gra, cond, shortest_dist, path, edge, answer, edge_num, cond.start);
}

// DFS2 		=====================================================================================================================================>

void find_path_dfs2(graphic& gra, condition_type& cond, int& shortest_dist, vector<unsigned short>& path,
	vector<unsigned short>& answer, int& vertex_num, int curr_node)
{
	if(curr_node == cond.end)
	{
		if(gra.get_data()[curr_node].dist < shortest_dist && is_valid_path(path, cond))
		{
			shortest_dist = gra.get_data()[curr_node].dist;
			vertex_num = path.size();
			copy(path.begin(), path.end(), answer.begin());
			//cout << "get a result .............." << endl;
		}
		return;
	}
	//else if(!can_to_end[curr_node])
	//	return;

	auto end = gra.get_data()[curr_node].adj_list.end();
	for(auto it = gra.get_data()[curr_node].adj_list.begin(); it != end; it++)
	//for(auto it = gra.get_data()[curr_node].adj_list.begin(); it != gra.get_data()[curr_node].adj_list.end(); it++)
	{
		if(!gra.get_data()[it->to_node].known)
		{
			path.push_back(it->to_node);
			gra.get_data()[it->to_node].known = true;
			gra.get_data()[it->to_node].dist = gra.get_data()[curr_node].dist + it->weight;

			find_path_dfs2(gra, cond, shortest_dist, path, answer, vertex_num, it->to_node);

			gra.get_data()[it->to_node].known = false;
			path.pop_back();
		}
	}
}

void find_path2(graphic& gra, condition_type& cond, vector<unsigned short>& answer, int& edge_num)
{
	int shortest_dist = graphic::MAX_DIST;

	vector<unsigned short> path;			// 记录活动顶点
	int vertex_num = 0;

	gra.get_data()[cond.start].known = true;
	gra.get_data()[cond.start].dist = 0;
	path.push_back(cond.start);

	find_path_dfs2(gra, cond, shortest_dist, path, answer, vertex_num, cond.start);
	if(shortest_dist < graphic::MAX_DIST)		// 表明有解
	{
		edge_num = vertex_num - 1;
		for(int i=0; i<edge_num; i++)
		{
			auto end = gra.get_data()[answer[i]].adj_list.end();
			for(auto it = gra.get_data()[answer[i]].adj_list.begin(); it != end; it++)
				if(it->to_node == answer[i+1])
				{
					answer[i] = it->name;
					break;
				}
		}
	}
}

// 减枝  	=========================================================================================================================================>

void reverse_graphic(const graphic& gra, graphic& to_gra)
{
	//to_gra.get_vertex_name() = gra.get_vertex_name();

	int sz = gra.get_data().size();
	to_gra.get_data().resize(sz);

	for(int i=0; i<sz; i++)
	{
		for(auto it = gra.get_data()[i].adj_list.begin(); it != gra.get_data()[i].adj_list.end(); it++)
		{
			graphic::adjnode_type adj(it->name, i, it->weight);
			to_gra.get_data()[it->to_node].adj_list.push_back(adj);
		}
	}
}

// BFS

void get_can_to_end(graphic& gra, vector<bool>& can_to_end, unsigned short start)
{
	queue<unsigned short> q;
	q.push(start);

	while(!q.empty())
	{
		int vertex = q.front();
		q.pop();
		can_to_end[vertex] = true;
		gra.get_data()[vertex].known = true;

		for(auto it = gra.get_data()[vertex].adj_list.begin(); it != gra.get_data()[vertex].adj_list.end(); it++)
		{
			if(!gra.get_data()[it->to_node].known)
				q.push(it->to_node);
		}
	}
}

// 减枝

void graphic_cut_down(graphic& gra, condition_type& cond)
{
	graphic reverse;

	/*can_to_end.resize(gra.get_data().size());
    fill(can_to_end.begin(), can_to_end.end(), false);
    get_can_to_end(gra, can_to_end, cond.start);

    for(int i=0; i<can_to_end.size(); i++)
    	cout << "vertex " << i << " : " << (can_to_end[i] ? "true" : "false") << endl;*/

    reverse_graphic(gra, reverse);
    can_to_end.resize(gra.get_data().size());
    fill(can_to_end.begin(), can_to_end.end(), false);
    get_can_to_end(reverse, can_to_end, cond.end);

    /*for(int i=0; i<can_to_end.size(); i++)
    	cout << "vertex " << i << " : " << (can_to_end[i] ? "true" : "false") << endl;*/
}

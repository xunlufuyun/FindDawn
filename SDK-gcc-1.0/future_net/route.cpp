
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <unordered_set>
#include <algorithm>

using namespace std;

void get_condition(const graphic& gra, char *condition, condition_type& cond)
{
	unsigned short temp = 0;
	int i=0;
	for(; condition[i] != ','; i++)
		temp = temp*10 + condition[i] - '0';
	cond.start = temp;
	temp = 0;
	i++;
	for(; condition[i] != ','; i++)
		temp = temp*10 + condition[i] - '0';
	cond.end = temp;
	temp = 0;
	i++;
	while(1)
	{
		if(condition[i] == 0)
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

    graphic 		gra(graph, edge_num);	// 解析出图
    condition_type 	cond;
    get_condition(gra, condition, cond);	// 解析出条件

    //cout << gra;

   	find_path(gra, cond, answer, ed_num);
    if(ed_num > 0)
    {
    	for(int i=0; i<ed_num; i++)
    		record_result(answer[i]);
    }
}

/*int dijkstra(graphic& gra, const unordered_set<unsigned short>& mid_point, int start, int end)
{
	gra.get_data()[start].known = true;
	gra.get_data()[start].dist = 0;

	while(1)
	{
		int vertex = 0;
		int dist = MAX_DIST;
		int vertex_num = gra.get_data().size();
		for(int i=0; i<vertex_num; i++)
		{
			if(!gra.get_data().known && data[i].dist < dist && )
		}
	}
}*/

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

// DFS

void find_path_help(graphic& gra, condition_type& cond, int& shortest_dist, vector<unsigned short>& path,
	vector<unsigned short>& edge, vector<unsigned short>& answer, int& edge_num, int curr_node)
{
	if(curr_node == cond.end)
	{
		//if(is_valid_path(path, cond) && gra.get_data()[curr_node].dist < shortest_dist)
		if(gra.get_data()[curr_node].dist < shortest_dist && is_valid_path(path, cond))
		{
			shortest_dist = gra.get_data()[curr_node].dist;
			edge_num = edge.size();
			copy(edge.begin(), edge.end(), answer.begin());
			/*cout << "solution : ";
			for(auto i : edge)
				cout << i << " ";
			cout << endl;*/
		}
		return;
	}

	for(auto it = gra.get_data()[curr_node].adj_list.begin(); it != gra.get_data()[curr_node].adj_list.end(); it++)
	{
		if(!gra.get_data()[it->to_node].known)
		{
			edge.push_back(it->name);
			path.push_back(it->to_node);
			gra.get_data()[it->to_node].known = true;
			gra.get_data()[it->to_node].dist = gra.get_data()[curr_node].dist + it->weight;

			find_path_help(gra, cond, shortest_dist, path, edge, answer, edge_num, it->to_node);

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

	find_path_help(gra, cond, shortest_dist, path, edge, answer, edge_num, cond.start);

	/*cout << "dist : " << shortest_dist << endl;
	cout << "path : ";
	for(auto i : answer)
		cout << i << " ";
	cout << endl;*/
}

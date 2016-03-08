
#include "route.h"
#include "lib_record.h"
#include <stdio.h>

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
	unordered_map<unsigned short, unsigned short> vertex_name = gra.get_vertex_name();
	cond.start = vertex_name[cond.start];
	cond.end = vertex_name[cond.end];
	for(int i=0; i<cond.pass.size(); i++)
		cond.pass[i] = vertex_name[cond.pass[i]];
}

//你要完成的功能总入口
void search_route(char *graph[5000], int edge_num, char *condition)
{
    //unsigned short result[] = {2, 6, 3};	//示例中的一个解
    vector<unsigned short> answer;

    graphic 		gra(graph, edge_num);	// 解析出图
    condition_type 	cond;
    get_condition(gra, condition, cond);	// 解析出条件
    int ret = find_path(gra, cond, answer);
    if(ret >= 0)
    {
    	for(auto i : answer)
    		record_result(i);
    }

    //for (int i = 0; i < 3; i++)
    //    record_result(result[i]);
}

int find_path(graphic& gra, condition_type& cond, vector<unsigned short>& answer)
{
	return -1;
}

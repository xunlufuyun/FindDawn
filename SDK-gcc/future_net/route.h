#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "graphic.h"
#include <vector>

struct condition_type
{
	unsigned short start;
	unsigned short end;
	std::vector<unsigned short> pass;
};

void get_condition(const graphic& gra, char *condition, condition_type& cond);
void search_route(char *graph[5000], int edge_num, char *condition);
int find_path(graphic& gra, condition_type& cond, std::vector<unsigned short>& answer);

#endif

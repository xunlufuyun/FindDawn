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
void find_path (graphic& gra, condition_type& cond, std::vector<unsigned short>& answer, int& edge_num);
void find_path2(graphic& gra, condition_type& cond, std::vector<unsigned short>& answer, int& edge_num);

void reverse_graphic(const graphic& gra, graphic& to_gra);
void get_can_to_end(graphic& gra, std::vector<bool>& can_to_end, unsigned short start);
void graphic_cut_down(graphic& gra, condition_type& cond);

#endif

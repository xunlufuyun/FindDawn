
#include "route.h"
#include "lib_record.h"
#include "graphic.h"
#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

//��Ҫ��ɵĹ��������
void search_route(char *graph[5000], int edge_num, char *condition)
{
    unsigned short result[] = {2, 6, 3};//ʾ���е�һ����
    graphic gra(graph, edge_num);
    cout << gra;

    for (int i = 0; i < 3; i++)
        record_result(result[i]);
}

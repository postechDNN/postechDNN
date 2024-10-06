#pragma once
#include "Hourglass.h"

void printVector(vector<int> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		printf("%d ", list[i]);
	}
	cout << endl;
}

void printSummary(Hourglass _left, Hourglass _right, Hourglass concatenated_hourglass, bool open)
{
	printf("left Hourglass edges: (%d %d), (%d %d)\n", _left.get_first_edge().get_origin(), _left.get_first_edge().get_dest(), _left.get_second_edge().get_origin(), _left.get_second_edge().get_dest());
	printf("right Hourlgasss edges: (%d %d), (%d %d)\n", _right.get_first_edge().get_origin(), _right.get_first_edge().get_dest(), _right.get_second_edge().get_origin(), _right.get_second_edge().get_dest());
	if (open)
	{
		printf("new Hourglass : open\n");
		printf("new Hourglass edges: (%d %d), (%d %d)\n", concatenated_hourglass.get_edge_list()[0].get_origin(), concatenated_hourglass.get_edge_list()[0].get_dest(), concatenated_hourglass.get_edge_list()[1].get_origin(), concatenated_hourglass.get_edge_list()[1].get_dest());
		printf("first chain: \n");
		printVector(concatenated_hourglass.get_first_chain()[0]->get_point_list());
		printVector(concatenated_hourglass.get_first_chain()[1]->get_point_list());
	}
	else
	{
		printf("new Hourglass : closed \n");
		printf("new Hourglass edges: (%d %d), (%d %d)\n", concatenated_hourglass.get_edge_list()[0].get_origin(), concatenated_hourglass.get_edge_list()[0].get_dest(), concatenated_hourglass.get_edge_list()[1].get_origin(), concatenated_hourglass.get_edge_list()[1].get_dest());
		printf("first chain: \n");
		printVector(concatenated_hourglass.get_first_chain()[0]->get_point_list());
		printVector(concatenated_hourglass.get_first_chain()[1]->get_point_list());
		printf("second chain: \n");
		printVector(concatenated_hourglass.get_second_chain()[0]->get_point_list());
		printVector(concatenated_hourglass.get_second_chain()[1]->get_point_list());
		printf("string:\n");
		printVector(concatenated_hourglass.get_string()->get_chain()->get_point_list());
		printf("apexes : %d %d", concatenated_hourglass.get_apaxes()[0], concatenated_hourglass.get_apaxes()[1]);
	}
}

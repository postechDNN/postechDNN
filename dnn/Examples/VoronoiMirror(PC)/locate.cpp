#include "voronoi_camera.h"

int locate(vector <sites> &s, long double x, long double y)
{

	long double dist, min_dist;
	int index;
	int start_index, next_index;
	start_index = 0;
	min_dist = pow(s[0].x_cor - x, 2) + pow(s[0].y_cor - y, 2);
	for (int i = 0; i <= 3; i++)
	{
		dist = pow(s[i].x_cor - x, 2) + pow(s[i].y_cor - y, 2);
		if (dist < min_dist)
		{
			min_dist = dist;
			start_index = i;
		}
	}
	index = start_index;
	next_index = start_index;

	while (1)
	{
		min_dist = 100000000000000000.0;

		for (int i = 0; i < (int)s[index].near_site.size(); i++)
		{

			if (s[index].near_site[i] == -1) continue;

			dist = pow(s[s[index].near_site[i]].x_cor - x, 2) + pow(s[s[index].near_site[i]].y_cor - y, 2);

			if (dist < min_dist)
			{
				min_dist = dist;
				next_index = s[index].near_site[i];
			}

		}

		if (min_dist < pow(s[index].x_cor - x, 2) + pow(s[index].y_cor - y, 2)) index = next_index;
		else break;
	}

	return index;

}
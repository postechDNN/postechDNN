#include "voronoi_camera.h"

void bisector_intersection(long double xx, long double yy, long double slope, long double a1, long double a2, long double b1, long double b2, long double * x, long double * y)
{

	if (slope == -32768.0)
	{
		*x = xx;
		*y = (xx - a1)*(a2 - b2) / (a1 - b1) + a2;
	}
	else if (b1 == a1)
	{
		*x = a1;
		*y = slope * (*x - xx) + yy;
	}
	else
	{
		*x = (-a1 * (b2 - a2) / (b1 - a1) + (a2 - yy) + slope * xx) / (slope - ((b2 - a2) / (b1 - a1)));
		*y = slope * (*x - xx) + yy;
	}
}

bool progress(vector <sites> &s, vector <vertices> &v, long double x1, long double y1, long double xp, long double yp, int current_site, long double &x2, long double &y2, int &next_site, int prev_site)
{
	int j, index1, index2;
	long double x, y, xs, ys, a1, a2, b1, b2, slope;


	xs = s[current_site].x_cor;
	ys = s[current_site].y_cor;

	int flag = 0;

	for (j = 0; j < (int)s[current_site].near_site.size(); j++)
	{

		if (s[current_site].near_site[j] != prev_site)
		{

			index1 = s[current_site].list[j];
			if (j != ((int)s[current_site].near_site.size() - 1))
				index2 = s[current_site].list[j + 1];
			else
				index2 = s[current_site].list[0];

			next_site = s[current_site].near_site[j];

			a1 = v[index1].x_cor;
			a2 = v[index1].y_cor;
			b1 = v[index2].x_cor;
			b2 = v[index2].y_cor;


			if (yp == ys)
			{
				slope = -32768.0;
				if ((a1 - x1)*(b1 - x1) < 0) { flag = 1; bisector_intersection(x1, y1, slope, a1, a2, b1, b2, &x, &y); break; }
			}
			else
			{
				slope = -(xp - xs) / (yp - ys);
				if (((a2 - (slope*(a1 - x1) + y1)) * (b2 - (slope*(b1 - x1) + y1))) < 0) { flag = 1; bisector_intersection(x1, y1, slope, a1, a2, b1, b2, &x, &y); break; }
			}
		}
	}


	if (flag == 0)
	{
		cout << "\n\nunexceptional case ";

		cout << "\n\ncurrent site details ";

		int i = current_site;

		cout << "\n\ncurrent site details " << i << ":  (" << s[i].x_cor << "," << s[i].y_cor << ")";
		cout << "\nno of elements in list = " << s[i].near_site.size();

		cout << "\nlist elements are :";
		for (j = 0; j < (int)s[i].near_site.size(); j++)
			cout << "\n" << s[i].list[j] << "   coordinates: " << v[s[i].list[j]].x_cor << "     " << v[s[i].list[j]].y_cor;;

		cout << "\n\nsurrounding sites are : ";
		for (j = 0; j < (int)s[i].near_site.size(); j++)
			cout << " " << s[i].near_site[j];

		cout << "\n\n\n" << prev_site << "\n" << x1 << "\t" << y1 << "\n" << xp << "\t" << yp << "\n" << xs << "\t" << ys << "\n" << "     slope :" << slope;

		return false;
	}

	x2 = x; y2 = y;
	return true;
}


int add_point(vector <sites> &s, vector <vertices> &v, long double xp, long double yp)
{

	int begin_index, k, j, target_site, current_site, next_site, prev_site, index1, index2;
	long double xs, ys, xm, ym, x1, y1, x2, y2, slope, a1, a2, b1, b2;


	/* grid limit */
	if (get_grid_num(xp, yp) > MAXGRIDNUM)
	{
		//cout<<endl<<"Exceeding limit";
		return -1;
	}


	/* new changes*/
	int new_site_index;
	bool deleted_index;

	if ((int)deleted_pool.size() > 0)
	{
		new_site_index = deleted_pool.back();
		deleted_index = true;
	}
	else
	{
		new_site_index = (int)s.size();
		deleted_index = false;
	}


	// initializing new site
	sites site;
	site.x_cor = xp;
	site.y_cor = yp;
	site.active = 1;
	site.allowed = 0;


	// locate returns the index of the site in which the point (xp,yp) is located
	target_site = locate(s, xp, yp);


	xs = s[target_site].x_cor;               // s for target site
	ys = s[target_site].y_cor;
	xm = (xp + xs) / 2;                          // p for new point
	ym = (yp + ys) / 2;


	/* distance limit */
	if (pow(xp - xs, 2) + pow(yp - ys, 2) < MINSQDISTANCE)
	{
		//cout<<endl<<"so close";
		return -1;
	}


	// to finally push the vertex in the list of new voronoi cell completing the loop
	begin_index = (int)v.size();												// changes to be made 1


	int flag_1 = 0;
	/* loop to determine the first vertex (x1,y1) of the the new voronoi cell to be constructed */
	for (j = 0; j < (int)s[target_site].list.size(); j++)
	{

		index1 = s[target_site].list[j];
		if (j != ((int)s[target_site].list.size() - 1))
			index2 = s[target_site].list[j + 1];
		else
			index2 = s[target_site].list[0];

		a1 = v[index1].x_cor;
		a2 = v[index1].y_cor;
		b1 = v[index2].x_cor;
		b2 = v[index2].y_cor;

		//cout<<endl<<index1<<" "<<index2;


		current_site = s[target_site].near_site[j];

		//cout<<endl<<"cur, target_site : "<<current_site<<" "<<target_site;

		if (yp == ys)
		{
			slope = -32768.0;
			if ((a1 - xm)*(b1 - xm) < 0)
			{
				bisector_intersection(xm, ym, slope, a1, a2, b1, b2, &x1, &y1);
				flag_1 = 1;
				if (((x1 - xs)*(yp - ys) - (y1 - ys)*(xp - xs)) > 0) break;
			}
		}
		else
		{
			slope = -(xp - xs) / (yp - ys);
			if (((a2 - (slope*(a1 - xm) + ym)) * (b2 - (slope*(b1 - xm) + ym))) < 0)
			{
				bisector_intersection(xm, ym, slope, a1, a2, b1, b2, &x1, &y1);
				flag_1 = 1;
				if (((x1 - xs)*(yp - ys) - (y1 - ys)*(xp - xs)) > 0) break;
			}
		}

	}
	if (flag_1 == 0)
	{
		cout << "e";
	}


	//bisector_intersection(xm,ym,slope,a1,a2,b1,b2,&x1,&y1);

	prev_site = target_site;


	/* loop to traverse along and construct the new voronoi cell */

	int check_size = (int)s[current_site].near_site.size();
	int check_current_site = current_site;
	int check_prev_site = prev_site;
	int check_next_site;
	long double check_x1 = x1;
	long double check_y1 = y1;
	long double check_x2, check_y2;


	do
	{
		if (current_site == -1)
		{
			cout << "\n This is add point func and boundary encountered ..error!!"; getchar(); exit(0);
		}
		else if (!progress(s, v, check_x1, check_y1, xp, yp, check_current_site, check_x2, check_y2, check_next_site, check_prev_site))
		{
			deleted_pool.push_back(current_site); cout << "\ndeleted: " << target_site; return -1;
		}

		check_x1 = check_x2;
		check_y1 = check_y2;
		check_prev_site = check_current_site;
		check_current_site = check_next_site;

		if (check_size-- < -100)
		{
			deleted_pool.push_back(current_site);
			return -1;
		}

	} while (check_prev_site != target_site);


	do
	{
		if (!progress(s, v, x1, y1, xp, yp, current_site, x2, y2, next_site, prev_site))
		{
			cout << "\n Error after checked!!"; getchar(); exit(0);
		}


		//cout<<endl<<"prev,next,cur :"<<prev_site<<" "<<next_site<<" "<<current_site;

		vertices vec;
		vec.x_cor = x1;
		vec.y_cor = y1;

		v.push_back(vec);                                              // changes to be made 2

		//cout<<endl<<"put vertex "<<(int)v.size()-1<<" : "<<x1<<" "<<y1;

		site.list.push_back((int)v.size() - 1);                          // changes to be made

		site.near_site.push_back(current_site);


		//updating the current site
		vector <int> l;                                                // temporary list
		vector <int> n;                                                // temporary near site

		k = 0;
		int flag;
		for (k = 0; k < (int)s[current_site].near_site.size(); k++)
		{
			if (s[current_site].near_site[k] == prev_site)
			{
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			cout << endl << "There is no near_site == prev_site";
			continue;
		}


		do
		{
			n.push_back(s[current_site].near_site[k]);
			l.push_back(s[current_site].list[k]);

			if (k == (int)s[current_site].near_site.size() - 1) k = 0;
			else k++;
		} while (s[current_site].near_site[k] != next_site);


		n.push_back(s[current_site].near_site[k]);
		l.push_back(s[current_site].list[k]);

		n.push_back(new_site_index);
		l[0] = (int)v.size() - 1;										   // changes to be made 3

		if (current_site == target_site)
			l.push_back(begin_index);
		else
			l.push_back((int)v.size());                               // changes to be made 4


		//copying l and n into list and near_site of current_site respectively
		s[current_site].list.clear();
		s[current_site].near_site.clear();

		for (k = 0; k < (int)l.size(); k++)
			s[current_site].list.push_back(l[k]);

		for (k = 0; k < (int)n.size(); k++)
			s[current_site].near_site.push_back(n[k]);

		l.clear();
		n.clear();

		x1 = x2;
		y1 = y2;
		prev_site = current_site;
		current_site = next_site;

	} while (prev_site != target_site);


	site.grid_x = (int)((xp / (long double)wdth)*GRIDXNUM);
	site.grid_y = (int)((yp / (long double)ht)*GRIDYNUM);
	add_to_grid(xp, yp);

	site.create_count = 1;
	site.move_count = (int)unifRand(-0.5, MOVEINTERVAL + 0.5);


	if (deleted_index == false) { s.push_back(site); }
	else { s[new_site_index] = site; deleted_pool.pop_back(); }

	return new_site_index;

}

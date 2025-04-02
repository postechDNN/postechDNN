#include "voronoi_camera.h"

struct store
{
	int index_no;
	long double x_cor;
	long double y_cor;
};

struct relate
{
	int me;
	int point_to;
};

void add_pair(vector <relate> &pair, int a, int b)
{
	relate r;
	r.me = a;
	r.point_to = b;
	int flag = 1;

	for (int i = 0; i < (int)pair.size(); i++)
		if (pair[i].me == a && pair[i].point_to == b) { flag = 0; break; }

	if (flag)
		pair.push_back(r);
}

int check_store(vector <store> &storeobj, vertices ver, vector <sites> &s, vector <vertices> &v)
{

	for (int i = 0; i < (int)storeobj.size(); i++)
	{
		if (comp(storeobj[i].x_cor, ver.x_cor) && comp(storeobj[i].y_cor, ver.y_cor))
			return storeobj[i].index_no;
	}

	store st;
	st.index_no = (int)v.size();
	st.x_cor = ver.x_cor;
	st.y_cor = ver.y_cor;
	storeobj.push_back(st);
	v.push_back(ver);
	return st.index_no;
}

vertices circumcentre(int index, int p, int q, vector <sites> &s, vector <vertices> &v)
{

	vertices ver;
	long double ax, ay, bx, by, cx, cy, D;
	ax = s[index].x_cor;
	ay = s[index].y_cor;
	bx = s[p].x_cor;
	by = s[p].y_cor;
	cx = s[q].x_cor;
	cy = s[q].y_cor;

	D = 2 * (ax*(by - cy) + bx * (cy - ay) + cx * (ay - by));

	if (D == 0) { cout << "\n\n unexpected zero " << p << " " << q << " " << ax << " " << ay << " " << bx << " " << by << " " << cx << " " << cy; getchar(); }

	ver.x_cor = ((ay*ay + ax * ax)*(by - cy) + (by*by + bx * bx)*(cy - ay) + (cy*cy + cx * cx)*(ay - by)) / D;
	ver.y_cor = ((ay*ay + ax * ax)*(cx - bx) + (by*by + bx * bx)*(ax - cx) + (cy*cy + cx * cx)*(bx - ax)) / D;
	return ver;

}


int search(vector <relate> &pair, int a, int b)
{
	for (int i = 0; i < (int)pair.size(); i++)
		if (pair[i].me == a && pair[i].point_to == b) return 1;

	return 0;
}

void disp(vector <relate> &pair)
{
	//all pairs now in pair and displaying all pairs .... Note: the pairs in pair may be repeated
	for (int i = 0; i < (int)pair.size(); i++)
		cout << "\n " << pair[i].me << " points to " << pair[i].point_to;
}

long double det(long double x1, long double x2, long double x3, long double y1, long double y2, long double y3, long double z1, long double z2, long double z3)
{
	return (x1*(y2*z3 - z2 * y3) - x2 * (y1*z3 - z1 * y3) + x3 * (y1*z2 - z1 * y2));
}

int delete_site(int index, vector <sites> &s, vector <vertices> &v)
{

	if (get_grid_num(s[index].x_cor, s[index].y_cor) < MINGRIDNUM)
		return -1;

	delete_from_grid(s[index].x_cor, s[index].y_cor);

	/* preparing for deletion  ------------------------------------------------ */

	for (int i = 4; i < (int)s.size(); i++)
		s[i].create_count = false;

	// deactivating the site
	s[index].active = -1;

	// adding  site to be deleted to the pool of deleted sites
	deleted_pool.push_back(index);


	/* ------------------------------------------------------------------------ */


	vector <relate> pair;
	vector <store> storeobj;

	vector <int> n;
	for (int k = 0; k < (int)s[index].near_site.size(); k++)
		if (s[index].near_site[k] >= 0)
			n.push_back(s[index].near_site[k]);

	while ((int)n.size() > 3)
	{
		for (int i = 0; i < (int)n.size(); i++)
		{
			int ind1, ind2, ind3;
			ind1 = n[i];

			if (i == (int)n.size() - 2)
			{
				ind2 = n[(int)n.size() - 1];
				ind3 = n[0];
			}
			if (i == (int)n.size() - 1)
			{
				ind2 = n[0];
				ind3 = n[1];
			}

			if (i < (int)n.size() - 2)
			{
				ind2 = n[i + 1];
				ind3 = n[i + 2];
			}

			long double xa = s[ind1].x_cor;
			long double ya = s[ind1].y_cor;
			long double xb = s[ind2].x_cor;
			long double yb = s[ind2].y_cor;
			long double xc = s[ind3].x_cor;
			long double yc = s[ind3].y_cor;
			long double xp = s[index].x_cor;
			long double yp = s[index].y_cor;

			if (det(xa, ya, 1, xb, yb, 1, xc, yc, 1) < 0) continue;
			//cout <<"\n ind3 on left "<<ind1<<" "<<ind2<<" "<<ind3;
			if (det(xa, ya, 1, xc, yc, 1, xp, yp, 1) < 0) continue;
			//cout <<"\n p on left "<<ind1<<" "<<ind2<<" "<<ind3;

			int flag = 1;
			for (int j = 0; j < (int)n.size(); j++)
			{
				if (n[j] == ind1 || n[j] == ind2 || n[j] == ind3)
					continue;

				xp = s[n[j]].x_cor;
				yp = s[n[j]].y_cor;

				long double h = xa * det(yb, xb*xb + yb * yb, 1, yc, xc*xc + yc * yc, 1, yp, xp*xp + yp * yp, 1)
					- ya * det(xb, xb*xb + yb * yb, 1, xc, xc*xc + yc * yc, 1, xp, xp*xp + yp * yp, 1)
					+ (xa*xa + ya * ya)*det(xb, yb, 1, xc, yc, 1, xp, yp, 1)
					- 1 * det(xb, yb, xb*xb + yb * yb, xc, yc, xc*xc + yc * yc, xp, yp, xp*xp + yp * yp);

				if (h > 0) { flag = 0; /*cout <<"\n "<<n[j]<<" in cirumcircle "<<ind1<<" "<<ind2<<" "<<ind3;*/ break; }
			}

			if (flag == 0) { continue; }
			else
			{
				add_pair(pair, ind1, ind3);
				add_pair(pair, ind3, ind1);

				//cout<<"\n ind1 ind2 ind3 "<<ind1<<" "<<ind2<<" "<<ind3;
				//remove ind2 from n
				int j;
				for (j = 0; j < (int)n.size(); j++) { if (n[j] == ind2) break; }
				n.erase(n.begin() + j);
				break;

			}
		}

	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i != j)
			{
				int flag = 1;
				for (int k = 0; k < (int)s[n[i]].near_site.size(); k++)
					if (s[n[i]].near_site[k] == n[j]) flag = 0;

				if (flag)
					add_pair(pair, n[i], n[j]);

			}

		}
	}

	n.clear();
	//disp(pair);

	// perfectly fine uptil here
	// for  updating the near sites of the site to be deleted ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	for (int i = 0; i < (int)s[index].near_site.size(); i++)
	{
		int ptr;
		int current_index = s[index].near_site[i];

		if (current_index < 0) continue;


		int j;
		for (j = 0; j < (int)s[current_index].near_site.size(); j++)
		{
			if (s[current_index].near_site[j] == index)
			{
				ptr = j;
				break;
			}
		}

		// for handling the exceptional case of index being found at the end of current index near site
		if (j == (int)s[current_index].near_site.size() - 1)
		{
			vector<int>::iterator it;
			it = s[current_index].near_site.begin();
			s[current_index].near_site.insert(it, s[current_index].near_site[j]);
			s[current_index].near_site.pop_back();

			vector<int>::iterator it1;
			it1 = s[current_index].list.begin();
			s[current_index].list.insert(it1, s[current_index].list[j]);
			s[current_index].list.pop_back();

			ptr = 0;
			j = 0;
		}

		int k;
		if (i == (int)s[index].near_site.size() - 1) k = 0;
		else k = i + 1;

		while (k != i)
		{
			if (search(pair, current_index, s[index].near_site[k]))
			{
				vector<int>::iterator it;
				it = s[current_index].near_site.begin();
				s[current_index].near_site.insert(it + j, s[index].near_site[k]);

				//if(j==(int)s[current_index].near_site.size()-1)
				//j=0;
				//else
				j = j + 1;
			}

			if (k == (int)s[index].near_site.size() - 1) k = 0;
			else k = k + 1;
		}

		// remove index now from near site of current index
		s[current_index].near_site.erase(s[current_index].near_site.begin() + j);


		// near_site of current_index updated ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/*	cout<<"\n\n display near sites of "<<current_index<<" "<<"\n";
			for (int f=0; f<(int)s[current_index].near_site.size(); f++)
				cout<<s[current_index].near_site[f]<<" ";*/

		int start, stop;
		if (ptr == 0) start = (int)s[current_index].near_site.size() - 1;
		else start = ptr - 1;

		if (j == (int)s[current_index].near_site.size())
			stop = 0;
		else
			stop = j;

		// removing ptr and ptr+1(or 0 :p) and inserting new elements in current_index list

		//cout<<"\n\n yello "<<start<<" "<<stop<<" "<<ptr<<"\n\n\n";




		// scanning near_site list of current_index from start to stop

		int ind = start;


		//removing ptr and ptr+1
		if (ptr == (int)s[current_index].list.size() - 1)
		{
			s[current_index].list.erase(s[current_index].list.begin() + ptr);
			s[current_index].list.erase(s[current_index].list.begin());
			ptr = 0;
			//ptr=ptr-1;
		}
		else
		{
			s[current_index].list.erase(s[current_index].list.begin() + ptr);
			s[current_index].list.erase(s[current_index].list.begin() + ptr);
		}


		while (ind != stop)
		{
			vertices ver;
			if (ind == (int)s[current_index].near_site.size() - 1) {
				//cout<<endl<<"circumcircle of "<<current_index<<" "<<s[current_index].near_site[ind]<<" "<<s[current_index].near_site[0];
				ver = circumcentre(current_index, s[current_index].near_site[ind], s[current_index].near_site[0], s, v);
			}
			else {
				//cout<<endl<<"circumcircle of "<<current_index<<" "<<s[current_index].near_site[ind]<<" "<<s[current_index].near_site[ind+1];
				ver = circumcentre(current_index, s[current_index].near_site[ind], s[current_index].near_site[ind + 1], s, v);
			}

			int new_index = check_store(storeobj, ver, s, v);
			//cout<<"\n"<<new_index<<" "<<ptr<<" "<<(int)s[current_index].list.size();

			s[current_index].list.insert(s[current_index].list.begin() + ptr, new_index);

			ptr = ptr + 1;

			if (ind == (int)s[current_index].near_site.size() - 1) ind = 0;
			else ind = ind + 1;

		}

	}

	pair.clear();
	storeobj.clear();

	s[index].near_site.clear();
	s[index].list.clear();

	return 0;
}

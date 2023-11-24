#include "EpsGraphnD.h"
#include <queue>
#include <iostream>
#include <random>

using namespace std;
Eps_Graph_nD::Eps_Graph_nD(int _n, list<Free_Point> _fr_pts, vector<Polytope> _pols, double _eps) {
	n = _n;
	fr_pts = _fr_pts;
	pols = _pols;
	eps = _eps;
	NN_dist = {};
	ord_pol = 0;
	xs_min = std::vector<double>(n, DBL_MAX);
	xs_max = std::vector<double>(n, DBL_MIN);
	xs_num = std::vector<long long int>(n, 0);
	for (auto pol : pols) {
		//pol.set_vertices_size();
		pol.set_maxmin();
		pol.ord = ord_pol;
		ord_pol++;
		cout << "---------------------" << endl;
		cout << "Polytope " << ord_pol << endl;
		cout << "Number of vertices: " << pol.get_num_point() << endl;
		cout << "Max value: ";
		for (int j = 0; j < n; j++) {
			cout << pol.xs_max[j] << ' ';
		}
		cout << endl;
		cout << "Min value: ";
		for (int j = 0; j < n; j++) {
			cout << pol.xs_min[j] << ' ';
		}
		cout << endl;
		cout << "---------------------" << endl;
		//cout << "dimension of polytope: " << pol.xs_max.size() << endl;
		for (int i = 0;i < n;i++) {
			
			if (pol.xs_max[i] > this->xs_max[i]) { this->xs_max[i] = pol.xs_max[i]; }
			if (pol.xs_min[i] < this->xs_min[i]) { this->xs_min[i] = pol.xs_min[i]; }
		}

	}
	for (auto fr_pt : fr_pts) {
		for (int i = 0;i < n;i++) {
			if (fr_pt.xs[i] > this->xs_max[i]) { this->xs_max[i] = fr_pt.xs[i]; }
			if (fr_pt.xs[i] < this->xs_min[i]) { this->xs_min[i] = fr_pt.xs[i]; }
		}
	}
	init_grid();
	for (Free_Point& fr_pt : fr_pts) {
		anchor(fr_pt);
	}
}

void Eps_Graph_nD::init_grid() {
	for (int i = 0;i < n;i++) {
		xs_num[i] = 1 + long long int(ceil((xs_max[i] - xs_min[i]) / eps));
	}
	upper_left = Point(xs_min);
	tot_num = 1;
	for (int i = 0;i < n;i++) {
		tot_num = tot_num * xs_num[i];
	}

	// initialization step for BFS
	for (long long int i = 0; i < tot_num; i++) {
		dist.push_back(INT_MAX);
		visited.push_back(false);
	}

	for (long long int i = 0; i < tot_num; i++)
	{
		grid.push_back(Grid_Point(num2ind(i), upper_left, eps, xs_num));
		// grid.push_back(Grid_Point(num2ind(i).x_ind, num2ind(i).y_ind, num2ind(i).z_ind, upper_left.x, upper_left.y, upper_left.z, eps, y_num, z_num));
	}

	// for each grid & free point, count # of crossings of the rightward ray with each polytope
	for (Grid_Point& pt : grid) {
		for (Polytope& pol : pols) {
			if (pol.isIn(&pt)) {
				pt.encl = pol.ord;
				pol.encl_pts.push_back(&pt);
			}
		}
	}
	for (int i = 0; i < tot_num; i++) {
		if (grid[i].encl != -1) { continue; }
		std::vector<long long int> inds = num2ind(i);
		for (int j = 0;j < n;j++) {
			if (inds[j] != xs_num[j] - 1) {
				std::vector<long long int> _inds(n);
				std::copy(inds.begin(), inds.end(), _inds.begin());
				_inds[j] += 1;
				if (grid[ind2num(_inds)].encl == -1) {
					if (cmpNadd(inds, j)) { add_edge(inds, _inds); }
				}
			}
		}
	}
}

bool Eps_Graph_nD::get_step_comb(vector<int> arr, int index, int sum, int step, vector<long long int> xs, Free_Point& p) {
	if (index == arr.size() - 1) {
		arr[index] = step - sum;
		for (int i = 0;i < this->n;i++) {
			if (xs[i] + arr[i] < 0 or xs_num[i] <= xs[i] + arr[i]) {
				break;
			}
			std::vector<long long int> sums(this->n);
			for (int j = 0;j < this->n;j++) {
				sums[j] = arr[j] + xs[j];
			}
			if (grid[ind2num(sums)].encl == -1) {
				p.host = grid[ind2num(sums)].num;
				grid[ind2num(sums)].anchored.push_back(&p);
				return true;
			}
		}
		arr[index] = -step + sum;
		for (int i = 0;i < this->n;i++) {
			if (xs[i] + arr[i] < 0 or xs_num[i] <= xs[i] + arr[i]) {
				break;
			}
			std::vector<long long int> sums(this->n);
			for (int j = 0;j < this->n;j++) {
				sums[j] = arr[j] + xs[j];
			}
			if (grid[ind2num(sums)].encl == -1) {
				p.host = grid[ind2num(sums)].num;
				grid[ind2num(sums)].anchored.push_back(&p);
				return true;
			}
		}
		return false;
	}
	for (int x_step = -step + sum;x_step <= step - sum;x_step++) {
		arr[index] = x_step;

		bool check = get_step_comb(arr, index + 1, sum + abs(x_step), step, xs, p);
		if (check == true) {
			return true;
		}
		return false;
	}
}

void Eps_Graph_nD::anchor(Free_Point& p) { // cast anchor onto a grid point from a free point
	if (p.host != -1) {
		int gs = grid.size();
		assert(0 <= p.host && p.host < gs);
		for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end();) {
			if (p.xs == (*(*it)).xs) {
				it = grid[p.host].anchored.erase(it);
				if (it == grid[p.host].anchored.end()) break;
			}
			else {
				++it;
			}
		}
	}

	bool flag = false;
	vector<long long int> xs(this->n,0);
	for (int i = 0;i < this->n;i++) {
		xs[i] = int(ceil((p.xs[i] - this->upper_left.xs[i]) / eps - 0.5));
	}
	if (grid[ind2num(xs)].encl == -1) {
		p.host = grid[ind2num(xs)].num;
		grid[ind2num(xs)].anchored.push_back(&p);
		return;
	}

	for (int step = 1;step < this->tot_num;step++) { //From Here
		std::vector<int> xs_step(this->n, 0);
		if (get_step_comb(xs_step, 0, 0, step, xs, p)) {
			return;
		}
	}
}

Grid_Point Eps_Graph_nD::get_gridpt(vector<long long int> ind) {//O
	return grid[ind2num(ind)];
}

long long int Eps_Graph_nD::ind2num(vector<long long int> ind) {
	long long out = 0;
	long long mul = 1;
	for (int i = n - 1;i >= 0;i--) {
		out += ind[i] * mul;
		mul *= xs_num[i];
	}
	return out;
}

vector<long long int> Eps_Graph_nD::num2ind(long long int num) {//O
	long long t = 1;
	for (int i = 1;i < n;i++) {
		t *= xs_num[i];
	}
	vector<long long int> out(n);
	for (int i = 0;i < n - 1;i++) {
		out[i] = num / (t);
		num = num % t;
		t = t / xs_num[i + 1];
	}
	out[n - 1] = num;
	return out;
}

void Eps_Graph_nD::add_edge(vector<long long int> ind1, vector<long long int> ind2) { //O
	int i;
	for (i = 0;i < ind1.size();i++) {
		if (ind1[i] != ind2[i]) break;
	}
	grid[ind2num(ind1)].ip_u[i] = true;
	grid[ind2num(ind2)].ip_d[i] = true;
}

void Eps_Graph_nD::delete_edge(vector<long long int> ind1, vector<long long int> ind2) { //O
	int i;
	for (i = 0;i < ind1.size();i++) {
		if (ind1[i] != ind2[i]) break;
	}
	grid[ind2num(ind1)].ip_u[i] = false;
	grid[ind2num(ind2)].ip_d[i] = false;
}

bool Eps_Graph_nD::cmpNadd(vector<long long int> ind, int direc) {  //O
	// checks if the line connecting the gridpoint and its neighboring one is blocked by any polytope. if is not, add an edge between them.

	Grid_Point A = grid[ind2num(ind)];
	Grid_Point B = Grid_Point(this->n);
	long long t = 1;
	for (int i = 0;i < n - 1 - direc;i++) {
		t *= xs_num[n - 1 - i];
	}
	B = grid[ind2num(ind) + t];

	for (unsigned int ind1 = 0; ind1 < pols.size(); ind1++) {
		auto pol = pols[ind1];
		if (pol.isIn(&A) || pol.isIn(&B)) {
			return false;
		}

		// if a polygon edge crosses the line connecting two gridpoints, then the edge between them is not present
		Point _A = Point(A.xs);
		Point _B = Point(B.xs);
		if (pol.intersect(&_A, &_B)) { return false; }

	}

	return true;
}

bool Eps_Graph_nD::cmpNadd_SinPol(vector<long long int> ind, int direc, int ord) { // do the same with a specific polygon.

	Polytope pol;
	vector<Polytope>::iterator it;

	for (it = pols.end() - 1;; it--) {
		if ((*it).ord == ord) {
			pol = *it;
			break;
		}
	}
	Grid_Point A = grid[ind2num(ind)];
	Grid_Point B = Grid_Point(n);
	long long t = 1;
	for (int i = 0;i < n - 1 - direc;i++) {
		t *= xs_num[n - 1 - i];
	}
	B = grid[ind2num(ind) + t];
	if (pol.isIn(&A) || pol.isIn(&B)) {
		return false;
	}

	Point _A = Point(A.xs);
	Point _B = Point(B.xs);
	if (pol.intersect(&_A, &_B)) { return false; }
	return true;
}

void Eps_Graph_nD::add_freepts(Free_Point* p) { // add points to the point set P //O
	Free_Point& pt = fr_pts.back();
	for (Polytope& pol : pols) {
		assert(!pol.isIn(p)); // Assert error if a freepoint is contained in some polytope
	}
	anchor(*p);
	fr_pts.push_back(*p);
}

void Eps_Graph_nD::add_freepts(vector<Free_Point> p_vec) { // add points to the point set P //O
	for (auto p : p_vec) {
		for (Polytope& pol : pols) {
			assert(!pol.isIn(&p)); // Assert error if a freepoint is contained in some polytope
		}
		anchor(p);
		fr_pts.push_back(p);
	}
}

void Eps_Graph_nD::delete_freept(int ind) { // delete a point from P, specified by its index //O
	int s = fr_pts.size();
	if (ind < 0 || s - 1 < ind) { return; }

	list<Free_Point>::iterator iter = fr_pts.begin();
	advance(iter, ind);
	Free_Point& p = *iter;
	for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
		if ((*(*it)).xs == p.xs) {
			grid[p.host].anchored.erase(it);
			break;
		}
	}
	fr_pts.erase(remove(fr_pts.begin(), fr_pts.end(), p));
}

void Eps_Graph_nD::query_anchor(Grid_Point& g) {
	for (vector<Free_Point*>::iterator it = g.anchored.begin(); it != g.anchored.end();) {
		Free_Point& p = (*(*(it)));
		anchor(p);

		it = g.anchored.erase(it);
		if (it == g.anchored.end()) break;
	}
	return;
}

std::vector<vector<long long int>> Eps_Graph_nD::eff_region(Polytope P) { // returns a range indicating orthogonal rectangle bounding the polygon (effective region)
	static std::vector<vector<long long int>> ret(2, vector<long long int>(n, 0)); // ret[0] : minimum index containing polytope , ret[1] : maximum index containing polytope


	for (int i = 0;i < n;i++) {
		ret[0][i] = max((long long int) 0, long long int(floor((P.xs_min[i] - xs_min[i]) / eps)));
		ret[1][i] = min(xs_num[i], long long int(ceil((P.xs_max[i] - xs_min[i]) / eps)));
	}

	return ret;
}

void Eps_Graph_nD::add_pol(Polytope P) { // add a polygon to the set of obstacles 
	P.ord = ord_pol;

	for (Free_Point& pt : fr_pts)
	{
		bool in = P.isIn(&pt);
		assert(!in); // check if the polygon contains a free point
	}
	for (Grid_Point& gr_pt : grid) {
		bool in = P.isIn(&gr_pt);
		if (in) {
			assert(gr_pt.encl == -1);
			gr_pt.encl = P.ord;
			query_anchor(gr_pt);
			vector<Free_Point*>().swap(gr_pt.anchored);
		}
	}
	pols.push_back(P);
	ord_pol++;

	vector<vector<long long int>> diagonal = eff_region(P);

	long long int tot_d = 1;
	for (int i = 0;i < n;i++) {
		tot_d *= (diagonal[1][i] - diagonal[0][i]);
	}

	for (long long i = 0;i < tot_d;i++) {
		vector<long long int> eff(n, 0);
		for (int j = 0;j < n;j++) {
			eff[j] = j % (diagonal[1][i] - diagonal[0][i]);
			j = j / ((int)diagonal[1][i] - diagonal[0][i]);
		}
		Grid_Point cur = grid[ind2num(eff)];
		for (int j = 0;j < n;j++) {
			if (eff[j] != xs_num[j] - 1) {
				if (!cmpNadd_SinPol(eff, j, P.ord)) {
					vector<long long int> _eff(n, 0);
					for (int k = 0;k < n;k++) {
						if (j != k) _eff[k] = eff[k];
						else _eff[k] = eff[k] + 1;
					}
					delete_edge(eff, _eff);
				}
			}
		}
	}
}

void Eps_Graph_nD::delete_pol(int ord) { // delete a polygon from O, specified by its index

	bool check = true;
	Polytope P;

	for (Polytope& p : pols) {
		if (p.ord == ord) {
			P = p;
			check = false;
		}
	}

	if (check) { return; }

	// release them free; for gridpoints that was enclosed by the polygon
	for (Grid_Point& gr_pt : grid) {
		if (gr_pt.encl == ord) {
			gr_pt.encl = -1;
		}
	}

	vector<vector<long long int>> diagonal = eff_region(P);
	long long int tot_d = 1;
	for (int i = 0;i < n;i++) {
		tot_d *= (diagonal[1][i] - diagonal[0][i]);
	}
	pols.erase(remove(pols.begin(), pols.end(), P));

	for (long long i = 0;i < tot_d;i++) {
		vector<long long int> eff(n, 0);
		for (int j = 0;j < n;j++) {
			eff[j] = j % (diagonal[1][i] - diagonal[0][i]);
			j = j / (diagonal[1][i] - diagonal[0][i]);
		}
		Grid_Point cur = grid[ind2num(eff)];
		for (int j = 0;j < n;j++) {
			if (eff[j] != xs_num[j] - 1) {
				if (!cmpNadd_SinPol(eff, j, P.ord)) {
					vector<long long int> _eff(n, 0);
					for (int k = 0;k < n;k++) {
						if (j != k) _eff[k] = eff[k];
						else _eff[k] = eff[k] + 1;
					}
					add_edge(eff, _eff);
				}
			}
		}
	}
}


Eps_Graph_nD::Eps_Graph_nD(int n) { xs_num = vector<long long int>(n, 0); eps = 0; xs_min = vector<double>(n, 0), xs_max = vector<double>(n, 0); }


list<Free_Point> Eps_Graph_nD::get_free_points()
{
	return fr_pts;
}

Free_Point Eps_Graph_nD::get_free_point(int index) {
	list<Free_Point>::iterator iter = fr_pts.begin();
	std::advance(iter, index);
	return *iter;
}

vector<Polytope> Eps_Graph_nD::get_Polytope() {
	return pols;
}

vector<Grid_Point> Eps_Graph_nD::get_grid() {
	return grid;
}


vector<edge> Eps_Graph_nD::get_path(Free_Point p, int k) {
	vector<edge> path = path_kNN(p, k + 1);
	return path;
}

vector<Free_Point> Eps_Graph_nD::kNN(Free_Point p, int k) { // returns k approximate nearest neighbors of p

	for (Polytope& pol : pols) {
		assert(!pol.isIn(&p));
	}

	vector<Free_Point> ret = {};
	vector<int>().swap(NN_dist);

	anchor(p);
	Grid_Point s = grid[p.host];
	for (int& elem : dist) { elem = INT_MAX; }
	//for (long long int i = 0; i < visited.size(); i++) { visited[i] = false; }
	//for (bool elem : visited) { elem = false; }

	queue<int> q = {};

	dist[s.num] = 0;
	q.push(s.num);
	int grid_dist = 0;
	vector<Free_Point> FPs_temp = {};
	vector<Free_Point> FPs = {};

	while (k > 0 && !q.empty()) {

		// q.empty() is indeed needed twice
		while (!q.empty() && (dist[q.front()] == grid_dist)) {
			if (visited[q.front()] == true) {
				q.pop();
				continue;
			}
			visited[q.front()] = true;

			long long int cur = q.front();
			long long int mult = 1;
			for (int i = 0;i < n;i++) {
				if (grid[cur].ip_u[n - 1 - i] && visited[cur + mult] == false) { dist[cur + mult] = dist[cur] + 1; q.push(cur + mult); }
				if (grid[cur].ip_d[n - 1 - i] && visited[cur - mult] == false) { dist[cur - mult] = dist[cur] + 1; q.push(cur - mult); }
				mult *= xs_num[n - i - 1];
			}

			for (auto FP : grid[q.front()].anchored) {
				FPs_temp.push_back(*FP);
			}
			q.pop();
		}

		for (auto pt : FPs_temp) {
			FPs.push_back(pt);
		}


		sort(FPs.begin(), FPs.end(), [=](Free_Point first, Free_Point second)
			{
				double fx = 0;
				double fy = 0;
				for (int i = 0;i < n;i++) {
					fx += pow(first.xs[i] - p.xs[i], 2);
					fy += pow(second.xs[i] - p.xs[i], 2);
				}
				return fx < fy;
			});

		int sz = int(FPs.size());

		if (sz < k) {
			for (int ind2 = 0; ind2 < sz; ind2++) { NN_dist.push_back(grid_dist); }
			k -= sz;
			ret.insert(ret.end(), FPs.begin(), FPs.end());
		}
		else {
			for (int ind2 = 0; ind2 < k; ind2++) { NN_dist.push_back(grid_dist); }
			ret.insert(ret.end(), FPs.begin(), FPs.begin() + k);
			k = 0;
		}

		vector<Free_Point>().swap(FPs_temp);
		vector<Free_Point>().swap(FPs);
		grid_dist += 1;
	}
	for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {

		if ((*(*it)).xs == p.xs) {
			grid[p.host].anchored.erase(it);
			break;
		}
	}

	return ret;
}

vector<edge> Eps_Graph_nD::path_kNN(Free_Point p, int k) { // returns k approximate nearest neighbors of p

	for (Polytope& pol : pols) {
		assert(!pol.isIn(&p));
	}
	vector<edge> path = {};
	vector<int>().swap(NN_dist);
	long long int* previous = new long long int[tot_num];
	anchor(p);
	Grid_Point s = grid[p.host];
	for (int& elem : dist) { elem = INT_MAX; }
	for (unsigned int ind1 = 0; ind1 < visited.size(); ind1++) { visited[ind1] = false; }
	for (long long int i = 0; i < tot_num; i++) { previous[i] = i; }

	queue<int> q = {};

	dist[s.num] = 0;
	q.push(s.num);
	int grid_dist = 0;
	vector<Free_Point> FPs_temp = {};
	vector<Free_Point> FPs = {};

	while (k > 0 && !q.empty()) {

		// q.empty() is indeed needed twice
		while (!q.empty() && (dist[q.front()] == grid_dist)) {
			if (visited[q.front()] == true) {
				q.pop();
				continue;
			}
			visited[q.front()] = true;

			int cur = q.front();
			long long int mult = 1;
			for (int i = 0;i < n;i++) {
				if (grid[cur].ip_u[n - 1 - i] && visited[cur + mult] == false) { dist[cur + mult] = dist[cur] + 1; q.push(cur + mult); }
				if (grid[cur].ip_d[n - 1 - i] && visited[cur - mult] == false) { dist[cur - mult] = dist[cur] + 1; q.push(cur - mult); }
				mult *= xs_num[n - i - i];
			}

			bool once = true;
			for (auto FP : grid[q.front()].anchored) {
				FPs_temp.push_back(*FP);
				if (once) {
					while (previous[cur] != cur) {
						path.push_back({ &grid[cur], &grid[previous[cur]] });
						cur = previous[cur];
						once = false;
					}
				}
			}
			q.pop();
		}

		for (auto pt : FPs_temp) {
			FPs.push_back(pt);
		}


		sort(FPs.begin(), FPs.end(), [=](Free_Point first, Free_Point second)
			{
				double fx = 0;
				double fy = 0;
				for (int i = 0;i < n;i++) {
					fx += pow(first.xs[i] - p.xs[i], 2);
					fy += pow(second.xs[i] - p.xs[i], 2);
				}
				return fx < fy;
			});

		int sz = int(FPs.size());
		for (int i = 0; i < min(k, sz); i++) {
			path.push_back({ &FPs[i], &grid[FPs[i].host] });
			NN_dist.push_back(grid_dist);
		}
		k -= sz;

		vector<Free_Point>().swap(FPs_temp);
		vector<Free_Point>().swap(FPs);
		grid_dist += 1;
	}
	for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
		if ((*(*it)).xs == p.xs) {
			grid[p.host].anchored.erase(it);
			break;
		}
	}

	return path;
}

void Eps_Graph_nD::print_grid() {
	for (unsigned int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].getsize(); j++) {
			cout << grid[i].ind[j];
		}
		cout << ' ' << '|';
		for (int j = 0; j < grid[i].ip_u.size(); j++) {
			cout << grid[i].ip_u[j] << ' ';
		}
		cout << '|';
		if (num2ind(i)[grid[i].getsize() - 1] == this->xs_num[n - 1] - 1) { cout << endl; }
	}
}

void Eps_Graph_nD::print_free_point() {
	for (Free_Point& fr : fr_pts) {

		for (int i = 0; i < n; i++) {
			cout << fr.getx(i) << ' ';
		}

		cout << endl;

	}
}

void Eps_Graph_nD::print_kNN(Free_Point p, int k) {
	cout << "-------------------Print KNN-------------------" << endl;
	vector<Free_Point> nbhd = kNN(p, k + 1);
	for (auto nb : nbhd) {
		bool cond = (nb.getxs() == p.getxs());
		if (cond) continue;
		for (int j = 0; j < this->n; j++) {
			cout << nb.getx(j) << ' ';
		}
		cout << endl;
	}
}

//vector<pair<Point, double>>* Eps_Graph_nD::Visibility_polygon(Free_Point qry) {
//vector<vector<pair<double, int>>>* Eps_Graph_nD::Visibility_polygon(Free_Point qry) {
//	vector<Point> vp_vertex;
//	//vector<pair<Point, double>>* nb_list;
//	//vector<vector<pair<double, int>>> nb_list;
//	Point temp_qry(qry.getxs());
//	qry.is_Free_Point = true;
//	vp_vertex.push_back(temp_qry);
//
//	for (auto f_p : fr_pts) {
//		Point temp(f_p.getxs());
//		f_p.is_Free_Point = true;
//		vp_vertex.push_back(temp);
//	}
//	for (auto pol : pols) {
//		vector<Point*> temp = pol.get_vertices();
//		for (auto temp_temp : temp) {
//			temp_temp->is_Free_Point = false;
//			vp_vertex.push_back(*temp_temp);
//		}
//	}
//	//nb_list = new vector<pair<Point, double>>(vp_vertex.size());
//	vector<vector<pair<double, int>>>* nb_list = new vector<vector<pair<double, int>>> (vp_vertex.size());
//	for (int i = 0; i++; i < vp_vertex.size()) {
//		for (int j = i + 1; j++; j < vp_vertex.size()) {
//			bool intersect = false;
//			for (auto pol : pols) {
//				if (pol.intersect(&vp_vertex[i], &vp_vertex[j])) {
//					intersect = true;
//					break;
//				}
//			}
//			if (!intersect) {
//				nb_list[i].push_back(make_pair(vp_vertex[i].distance(vp_vertex[j]),j));
//				nb_list[j].push_back(make_pair(vp_vertex[i].distance(vp_vertex[j]),i));
//			}
//		}
//	}
//	return nb_list;
//}

//vector<vector<pair<double, int>>> Eps_Graph_nD::Visibility_polygon2(Free_Point qry) {
//	vector<Point> vp_vertex;
//	Point temp_qry(qry.getxs());
//	qry.is_Free_Point = true;
//	vp_vertex.push_back(temp_qry);
//	vector<vector<pair<double, int>>> nb_list(vp_vertex.size());
//	//vector<vector<pair<double, int>>>* nb_list = new vector<vector<pair<double, int>>> (vp_vertex.size());
//	for (int i = 0; i++; i < vp_vertex.size()) {
//		for (int j = i + 1; j++; j < vp_vertex.size()) {
//			bool intersect = false;
//			for (auto pol : pols) {
//				if (pol.intersect(&vp_vertex[i], &vp_vertex[j])) {
//					intersect = true;
//					break;
//				}
//			}
//			if (!intersect) {
//				nb_list[i].push_back(make_pair(vp_vertex[i].distance(vp_vertex[j]),j));
//				nb_list[j].push_back(make_pair(vp_vertex[i].distance(vp_vertex[j]),i));
//			}
//		}
//	}
//	return nb_list;
//}

vector<pair<Point, double>>* Eps_Graph_nD::Visibility_polygon(Free_Point qry) {
	vector<Point> vp_vertex;
	vector<pair<Point, double>>* nb_list;
	Point temp_qry(qry.getxs());
	qry.is_Free_Point = true;
	vp_vertex.push_back(temp_qry);

	for (auto f_p : fr_pts) {
		Point temp(f_p.getxs());
		temp.is_Free_Point = true;
		vp_vertex.push_back(temp);
	}
	for (auto pol : pols) {
		vector<Point*> temp = pol.get_vertices();
		for (auto temp_temp : temp) {
			temp_temp->is_Free_Point = false;
			vp_vertex.push_back(*temp_temp);
		}
	}
	nb_list = new vector<pair<Point, double>>[vp_vertex.size()];
	for (int i = 0; i < vp_vertex.size(); i++) {
		for (int j = i + 1; j < vp_vertex.size(); j++) {
			bool intersect = false;
			for (auto pol : pols) {
				if (pol.sameIn(&vp_vertex[i], &vp_vertex[j])) {
					intersect = true;
					vector<double> mid;
					for (int k = 0; k< vp_vertex[i].getsize(); k++) {
						mid.push_back((vp_vertex[i].getx(k) + vp_vertex[j].getx(k)) / 2);
					}
					for (int k = 0; k < vp_vertex[i].getsize() * 5; k++) {
						random_device rd;
						mt19937 gen(rd());
						vector<double> random_vec;
						uniform_real_distribution<float> dis(0, 1);
						for (int l = 0; l< vp_vertex[i].getsize(); l++) {
							random_vec.push_back(mid[l] + dis(gen)/100);
						}
						Point random_pt(random_vec);
						if (!pol.isIn(&random_pt)) {
							intersect = false;
						}
					}
				}
				else {
					if (pol.intersect(&vp_vertex[i], &vp_vertex[j])) {
						intersect = true;
						break;
					}
				}
			}
			if (!intersect) {
				nb_list[i].push_back(make_pair(vp_vertex[j], vp_vertex[i].distance(vp_vertex[j])));
				nb_list[j].push_back(make_pair(vp_vertex[i], vp_vertex[i].distance(vp_vertex[j])));
			}
		}
	}
	return nb_list;
}

vector<pair<Point, double>> Eps_Graph_nD::Dijkstra(Free_Point qry, int _num) {
	cout << "-------------------Print Dijkstra-------------------" << endl;
	vector<Point> vp_vertex;
	int check_num = 0;
	//vector<pair<Point, double>>* nb_list;
	Point temp_qry(qry.getxs());
	temp_qry.is_Free_Point = true;
	vp_vertex.push_back(temp_qry);

	for (auto f_p : fr_pts) {
		Point temp(f_p.getxs());
		temp.is_Free_Point = true;
		vp_vertex.push_back(temp);
	}
	for (auto pol : pols) {
		vector<Point*> temp = pol.get_vertices();
		for (auto temp_temp : temp) {
			temp_temp->is_Free_Point = false;
			vp_vertex.push_back(*temp_temp);
		}
	}
	//vector<vector<pair<double, int>>> nb_list = Visibility_polygon2(qry);
	vector<pair<Point, double>>* nb_list2 = Visibility_polygon(qry);
	vector<double> dist(vp_vertex.size(), 0);
	vector<bool> visited(vp_vertex.size(), false);
	priority_queue<pair<double, int>,vector<pair<double, int>>,greater<pair<double, int>>> pq;
	pq.push({ 0,0 });
	while (!pq.empty()) {
		pair<double, int> q1 = pq.top();pq.pop();
		if (visited[q1.second]) continue;
		visited[q1.second] = true;
		dist[q1.second] = q1.first;
		//vector<pair<Point, double>>* nb_list2 = Visibility_polygon(qry);
		vector<pair<Point, double>> nb = nb_list2[q1.second];
		if (vp_vertex[q1.second].is_Free_Point) {
			bool cond = (qry.getxs() == vp_vertex[q1.second].getxs());
			if (!cond) {
				for (int j = 0; j < this->n; j++) {
					cout << vp_vertex[q1.second].getx(j) << ' ';
				}
				cout << endl;
				check_num++;
				if (check_num >= _num) break;
			}
		}
		for (int j=0;j < nb.size();j++) {
			pair<Point, double> q = nb[j];
			for (int i=0;i < vp_vertex.size();i++) {
				if (q.first.getxs() == vp_vertex[i].getxs()) {
					if (visited[i] == false) {
						pq.push({ q.second + q1.first, i });
					}
					break;
				}
			}
		}
	}
	vector<pair<Point, double>> output;
	for (int i = 0;i < vp_vertex.size();i++) {
		output.push_back({ vp_vertex[i], dist[i] });
	}
	return output;

}
//pair<vector<Point>, vector<double>> Eps_Graph_nD::Dijkstra(int j, vector<Point> _P, vector<vector<double>> _mat) {
//	vector<Point> P(_P.size(), Point(this->n));
//	vector<double> dist(_P.size(), 0);
//	vector<bool> visited(_P.size(), false);
//	pair<vector<Point>, vector<double>> p1;
//	priority_queue<pair<double, int>> pq;
//	pq.push({ 0,j });
//	while (!pq.empty()) {
//		pair<double, int> q1 = pq.pop();
//		if (visited[q1.second]) continue;
//		visited[q1.second] =true
//			for (auto q : _mat[q1.second]) {
//				if 
//		}
//	}
//	p1 = make_pair(P, dist);
//	return p1;
//
//}
/*
void Eps_Graph_nD::print_encl() {
	for (Polytope& pol : pols) {
		for (auto pt : pol.encl_pts) {
			for (int i = 0; i < pt->n; i++) {
				cout << pt->getx(i) << ' ';
			}
			cout << endl;
		}
	}
}

void Eps_Graph_nD::print_free_point() {
	for (Free_Point& fr : fr_pts) {

		for (int i = 0; i < n; i++) {
			cout << fr.getx(i) << ' ';
		}

		cout << endl;

	}
}




//void Eps_Graph_nD::print_edges() {
//	for (auto gp : grid) {
//		bool criteria = gp.ip_u[gp.getsize()];
//		for (vector<long long int>::iterator it = gp.ind.begin(); it != gp.ind.end(); ++it) {
//			criteria = criteria && (gp.ind(it) == 10);
//		}
//		if (criteria) {
//			for (int i = 0; i < gp.getsize(); i++) {
//				cout << gp.getx(i) << ' ';
//			}
//
//			cout << '|';
//
//			for (int i = 0; i < gp.getsize() - 1; i++) {
//				cout << gp.getx(i) << ' ';
//			}
//
//			cout << gp.getx(gp.getsize() - 1) + eps << endl;
//		}
//	}
//}


void Eps_Graph_nD::print_anchor() {
	for (auto gp : grid) {
		for (vector<Free_Point*>::iterator it = gp.anchored.begin(); it != gp.anchored.end(); ++it) {
			for (int i = 0; i < this->n; i++) {
				cout << gp.ind[i] << ' ';
			}

			cout << '|';

			for (int j = 0; j < this->n; j++) {
				cout << (*(*it)).getx(j) << ' ';
			}

			cout << endl;
		}
	}
}


/*
void Eps_Graph_nD::print_dist() {
	int idx = 0;

	for (int i = 0; i < x_num; i++) {
		for (int j = 0; j < y_num; j++) {
			for (int k = 0; k < z_num; k++) {
				if (dist[i * y_num * z_num + j * z_num + k] == INT_MAX) { printf(" INF "); }
				else { printf("%4.1d ", dist[i * y_num * z_num+ j * z_num + k]); }
			}
		}
		printf("\n");
	}
}
*/


//void Eps_Graph_nD::print_kNN(Free_Point p, int k) {
//	vector<Free_Point> nbhd = kNN(p, k + 1);
//	for (auto nb : nbhd) {
//		bool cond = (nb.getx(0) == p.getx(0) && nb.getx(1) == p.getx(1));
//		for (int i = 2; i < this->n; i++) {
//			cond = cond && (nb.getx(i) == p.getx(i));
//		}
//		if (cond) continue;
//		for (int j = 0; j < this->n; j++) {
//			cout << nb.getx(j) << ' ';
//		}
//		cout << endl;
//	}
//}


//Eps_Graph_nD::Eps_Graph_nD() {
//	for (int i = 0; i < this->n; i++) {
//		xs_num[i] = 0;
//		xs_max[i] = 0;
//		xs_min[i] = 0;
//	}
//	eps = 0;
//}
//
//list<Free_Point> Eps_Graph_nD::get_free_points()
//{
//	return this->fr_pts;
//}
//
//Free_Point Eps_Graph_nD::get_free_point(int index) {
//	list<Free_Point>::iterator iter = fr_pts.begin();
//	std::advance(iter, index);
//	return *iter;
//}
//
//vector<Polytope> Eps_Graph_nD::get_Polytope() {
//	return pols;
//}
//
//vector<Grid_Point> Eps_Graph_nD::get_grid() {
//	return grid;
//}


/*vector<edge> Eps_Graph_nD::get_path(Free_Point p, int k) {
	vector<edge> path = kNN(p, k + 1);
	return path;
}*/
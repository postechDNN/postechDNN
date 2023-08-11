//#include "EpsGraphnD.h"
//#include <queue>
//#include <assert.h>
//#include <iostream>
//
//#define X 1 // ray direction
//#define Y 2
//#define Z 3
//
//using namespace std;
//Eps_Graph_nD::Eps_Graph_nD(int _n, list<Free_Point> _fr_pts, vector<Polytope> _pols, double _eps) {
//	n = _n;
//	fr_pts = _fr_pts;
//	pols = _pols;
//	eps = _eps;
//	NN_dist = {};
//	ord_pol = 0;
//	xs_min = std::vector<double>(n, DBL_MIN);
//	xs_max = std::vector<double>(n, DBL_MAX);
//	xs_num = std::vector<long long int>(n, 0);
//	for (auto pol : pols) {
//		pol.ord = ord_pol;
//		ord_pol++;
//		for (int i = 0;i < n;i++) {
//			if (pol.xs_max[i] > this->xs_max[i]) { this->xs_max[i] = pol.xs_max[i]; }
//			if (pol.xs_min[i] < this->xs_min[i]) { this->xs_min[i] = pol.xs_min[i]; }
//		}
//	}
//	for (auto fr_pt : fr_pts) {
//		for (int i = 0;i < n;i++) {
//			if (fr_pt.xs_max[i] > this->xs_max[i]) { this->xs_max[i] = fr_pt.xs_max[i]; }
//			if (fr_pt.xs_min[i] < this->xs_min[i]) { this->xs_min[i] = fr_pt.xs_min[i]; }
//		}
//	}
//	init_grid();
//	for (Free_Point& fr_pt : fr_pts) {
//		anchor(fr_pt);
//	}
//}
//
//void Eps_Graph_nD::init_grid() {
//	for (int i = 0;i < n;i++) {
//		xs_num[i] = 1 + long long int(ceil((xs_max[i] - xs_min[i]) / eps));
//	}
//	upper_left = Point(xs_min);
//	tot_num = 1;
//	for (int i = 0;i < n;i++) {
//		tot_num = tot_num * xs_num[i];
//	}
//
//	// initialization step for BFS
//	for (long long int i = 0; i < tot_num; i++) {
//		dist.push_back(INT_MAX);
//		visited.push_back(false);
//	}
//
//	for (long long int i = 0; i < tot_num; i++)
//	{
//		grid.push_back(Grid_Point(num2ind(i), upper_left, eps, xs_num));
//		// grid.push_back(Grid_Point(num2ind(i).x_ind, num2ind(i).y_ind, num2ind(i).z_ind, upper_left.x, upper_left.y, upper_left.z, eps, y_num, z_num));
//	}
//
//	// for each grid & free point, count # of crossings of the rightward ray with each polytope
//	for (Grid_Point& pt : grid) {
//		for (Polytope& pol : pols) {
//			if (pol.isIn(&pt)) {
//				pt.encl = pol.ord;
//				pol.encl_pts.push_back(&pt);
//			}
//		}
//	}
//	for (int i = 0; i < tot_num; i++) {
//		if (grid[i].encl != -1) { continue; }
//		std::vector<long long int> inds = num2ind(i);
//		for (int j = 0;j < n;j++) {
//			if (inds[j] != xs_num[j] - 1) {
//				std::vector<long long int> _inds(n);
//				std::copy(inds.begin(), inds.end(), _inds.begin());
//				_inds[j] += 1;
//				if (grid[ind2num(_inds)].encl == -1) {
//					if (cmpNadd(inds, j)) { add_edge(inds, _inds); }
//				}
//			}
//		}
//	}
//}
//bool Eps_Graph_nD::get_step_comb(vector<int> arr, int index, int sum, int step, vector<long long int> xs) {
//	if (index == arr.size() - 1) {
//		arr[index] = step - sum;
//		for (int i = 0;i < n;i++) {
//			if (xs[i] + arr[i] < 0 or xs_num[i] <= xs[i] + arr[i]) {
//				break;
//			}
//			std::vector<int>::sum(n);
//			for (int j = 0;j < n;j++) {
//				sum[j] = arr[j] + xs[j]
//			}
//			if (grid[ind2num(sum)].encl == -1) {
//				p.host = grid[ind2num(sum)].num;
//				grid[ind2num(sum)].anchored.push_back(&p);
//				return true;
//			}
//		}
//		arr[index] = -step + sum;
//		for (int i = 0;i < n;i++) {
//			if (xs[i] + arr[i] < 0 or xs_num[i] <= xs[i] + arr[i]) {
//				break;
//			}
//			std::vector<int>::sum(n);
//			for (int j = 0;j < n;j++) {
//				sum[j] = arr[j] + xs[j]
//			}
//			if (grid[ind2num(sum)].encl == -1) {
//				p.host = grid[ind2num(sum)].num;
//				grid[ind2num(sum)].anchored.push_back(&p);
//				return true;
//			}
//		}
//		return false;
//	}
//	for (int x_step = -step + sum;x_step <= step - sum;x_step++) {
//		arr[index] = x_step;
//
//		bool check = get_step_comb(arr, index + 1, sum + abs(x_step), step, xs);
//		if (check == true) {
//			return true;
//		}
//		return false;
//	}
//}
//
//void Eps_Graph_nD::anchor(Free_Point& p) { // cast anchor onto a grid point from a free point
//	if (p.host != -1) {
//		assert(0 <= p.host && p.host < grid.size());
//		for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end();) {
//			if (p.xs == (*(*it)).xs) {
//				it = grid[p.host].anchored.erase(it);
//				if (it == grid[p.host].anchored.end()) break;
//			}
//			else {
//				++it;
//			}
//		}
//	}
//
//	bool flag = false;
//	vector<long long int> xs;
//	for (int i=0;i < n;i++) {
//		xs[i] = int(ceil((p.xs[i] - upper_left.xs[i]) / eps - 0.5));
//	}
//	if (grid[ind2num(xs)].encl == -1){
//		p.host = grid[ind2num(xs)].num;
//		grid[ind2num(xs)].anchored.push_back(&p);
//		return;
//	}
//
//	for (int step = 1;step < tot_num;step++) { //From Here
//		std::vector<int> xs_step(n, 0);
//		if (get_step_comb(xs_step, 0, 0, step, xs) {
//			return;
//		}
//	}
//}
//
//Grid_Point Eps_Graph_nD::get_gridpt(vector<long long int> ind) { //O
//	return grid[ind2num(ind)];
//}
//
//long long int Eps_Graph_nD::ind2num(vector<long long int> ind) {
//	long long out = 0;
//	long long mul = 1;
//	for (int i = n-1;i >= 0;i--) {
//		out += ind[i] * mul;
//		mul *= xs_num[i];
//	}
//	return out;
//}
//
////long long int Eps_Graph_3D::ind2num(long long int _x, long long int _y, long long int _z) { //O
////	return _x * y_num * z_num + _y * z_num + _z;
////}
//
//vector<long long int> Eps_Graph_nD::num2ind(long long int num) {//O
//	long long t = 1;
//	for (int i = 1;i < n;i++) {
//		t *= xs_num[i];
//	}
//	vector<long long int> out(n);
//	for (int i = 0;i < n-1;i++) {
//		out[i] = num / (t);
//		num = num % t;
//		t = t / xs[i + 1];
//	}
//	out[n - 1] = num;
//	return out;
//}
//
//void Eps_Graph_nD::add_edge(vector<long long int> ind1, vector<long long int> ind2) { //O
//	for (int i = 0;i < ind1.size();i++) {
//		if (ind1[i] != ind2[i]) break;
//	}
//	grid[ind2num(ind1)].ip_u[i] = true;
//	grid[ind2num(ind2)].ip_d[i] = true;	
//}
//
//void Eps_Graph_nD::delete_edge(vector<long long int> ind1, vector<long long int> ind2) { //O
//	for (int i = 0;i < ind1.size();i++) {
//		if (ind1[i] != ind2[i]) break;
//	}
//	grid[ind2num(ind1)].ip_u[i] = false;
//	grid[ind2num(ind2)].ip_d[i] = false;
//}
//
//bool Eps_Graph_nD::cmpNadd(vector<long long int> ind, int direc) {  //O
//	// checks if the line connecting the gridpoint and its neighboring one is blocked by any polytope. if is not, add an edge between them.
//
//	Grid_Point A = grid[ind2num(ind)], B;
//	long long t = 1;
//	for (int i = 0;i < n - 1 - direc;i++) {
//		t *= xs_num[n - 1 - i];
//	}
//	B = grid[ind2num(ind) + t];
//
//	for (unsigned int ind1 = 0; ind1 < pols.size(); ind1++) {
//		auto pol = pols[ind1];
//		if (pol.isIn(&A) || pol.isIn(&B)) {
//			return false;
//		}
//
//		// if a polygon edge crosses the line connecting two gridpoints, then the edge between them is not present
//		if (pol.intersect(Point{ A.ind }, Point{ B.ind }, direc)) { return false; }
//
//	}
//
//	return true;
//}
//
//bool Eps_Graph_nD::cmpNadd_SinPol(vector<long long int> ind, int direc, int ord) { // do the same with a specific polygon.
//
//	Polytope pol;
//	vector<Polytope>::iterator it;
//
//	for (it = pols.end() - 1;; it--) {
//		if ((*it).ord == ord) {
//			pol = *it;
//			break;
//		}
//	}
//	Grid_Point A = grid[ind2num(ind)], B;
//	long long t = 1;
//	for (int i = 0;i < n - 1 - direc;i++) {
//		t *= xs_num[n - 1 - i];
//	}
//	B = grid[ind2num(ind) + t];
//	if (pol.isIn(&A) || pol.isIn(&B)) {
//		return false;
//	}
//
//	if (pol.intersect(Point{ A.ind }, Point{ B.ind }, direc)) { return false; }
//	return true;
//}
//
//void Eps_Graph_nD::add_freepts(Free_Point* p) { // add points to the point set P //O
//	Free_Point& pt = fr_pts.back();
//	for (Polytope& pol : pols) {
//		assert(!pol.isIn(p)); // Assert error if a freepoint is contained in some polytope
//	}
//	anchor(*p);
//	fr_pts.push_back(*p);
//}
//
//void Eps_Graph_nD::add_freepts(vector<Free_Point> p_vec) { // add points to the point set P //O
//	for (auto p : p_vec) {
//		for (Polytope& pol : pols) {
//			assert(!pol.isIn(&p)); // Assert error if a freepoint is contained in some polytope
//		}
//		anchor(p);
//		fr_pts.push_back(p);
//	}
//}
//
//void Eps_Graph_nD::delete_freept(int ind) { // delete a point from P, specified by its index //O
//	if (ind < 0 || fr_pts.size() - 1 < ind) { return; }
//
//	list<Free_Point>::iterator iter = fr_pts.begin();
//	advance(iter, ind);
//	Free_Point& p = *iter;
//	for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
//		if ((*(*it)).xs == p.xs) {
//			grid[p.host].anchored.erase(it);
//			break;
//		}
//	}
//	fr_pts.erase(remove(fr_pts.begin(), fr_pts.end(), p));
//}
///*
//void Eps_Graph_3D::query_anchor(Grid_Point& g) {
//	for (vector<Free_Point*>::iterator it = g.anchored.begin(); it != g.anchored.end();) {
//		Free_Point& p = (*(*(it)));
//		bool flag = false;
//		int x; int y; int z;
//		x = int(ceil((p.x - upper_left.x) / eps - 0.5)); // points on the midline anchors leftward 
//		y = int(ceil((p.y - upper_left.y) / eps - 0.5));
//		z = int(ceil((p.z - upper_left.z) / eps - 0.5));
//
//		if (grid[ind2num(x, y, z)].encl == -1) {
//			p.host = grid[ind2num(x, y, z)].num;
//			grid[ind2num(x, y, z)].anchored.push_back(&p);
//			break;
//		}
//
//		for (int step = 1; step < y_num + x_num + z_num; step++) {
//			for (int x_step = -step; x_step <= step; x_step++) {
//				for (int y_step = -(step - abs(x_step)); y_step <= step - abs(x_step); y_step++) {
//					int z_step = step - abs(x_step) - abs(y_step);
//					if (0 <= x + x_step && x + x_step < x_num && 0 <= y + y_step && y + y_step < y_num && 0 <= z + z_step && z + z_step < z_num) {
//						if (grid[ind2num(x + x_step, y + y_step, z + z_step)].encl == -1) {
//							p.host = grid[ind2num(x + x_step, y + y_step, z + z_step)].num;
//							grid[ind2num(x + x_step, y + y_step, z + z_step)].anchored.push_back(&p);
//							flag = true;
//						}
//					}
//					if (flag) break;
//					if (0 <= x + x_step && x + x_step < x_num && 0 <= y + y_step && y + y_step < y_num && 0 <= z - z_step && z - z_step < z_num) {
//						if (grid[ind2num(x + x_step, y + y_step, z - z_step)].encl == -1) {
//							p.host = grid[ind2num(x + x_step, y + y_step, z - z_step)].num;
//							grid[ind2num(x + x_step, y + y_step, z - z_step)].anchored.push_back(&p);
//							flag = true;
//						}
//					}
//					if (flag) break;
//				}
//				if (flag) break;
//			}
//			if (flag) break;
//		}
//
//		it = g.anchored.erase(it);
//		if (it == g.anchored.end()) break;
//	}
//	return;
//}
//
//void Eps_Graph_3D::add_pol(Polytope P) { // add a polygon to the set of obstacles O 
//	P.ord = ord_pol;
//
//	for (Free_Point& pt : fr_pts) 
//	{ 
//		bool in = P.isIn(&pt); 
//		assert(!in); // check if the polygon contains a free point
//	}
//	for (Grid_Point& gr_pt : grid) {
//		bool in = P.isIn(&gr_pt);
//		if (in) { 
//			assert(gr_pt.encl == -1); 
//			gr_pt.encl = P.ord;
//			query_anchor(gr_pt);
//			vector<Free_Point*>().swap(gr_pt.anchored);
//		}
//	}
//	pols.push_back(P);
//	ord_pol++;
//
//	indices* diagonal = eff_region(P);
//
//	int x_effmax = diagonal[1].x_ind;
//	int x_effmin = diagonal[0].x_ind;
//
//	int y_effmin = diagonal[0].y_ind;
//	int y_effmax = diagonal[1].y_ind;
//
//	int z_effmax = diagonal[1].z_ind;
//	int z_effmin = diagonal[0].z_ind;
//
//	// update grid edges among gridpoints in the effective region
//	for (int i = x_effmin ; i < x_effmax; i++) {
//		for (int j = y_effmin; j < y_effmax; j++) {
//			for (int k = z_effmin; k < z_effmax; k++) {
//				Grid_Point cur = grid[ind2num(i, j, k)];
//				if (i != x_num - 1) {
//					if (!cmpNadd_SinPol(indices{ i, j ,k }, X, P.ord)) { delete_edge(indices{ i, j ,k }, indices{ i + 1, j ,k }); }
//				}
//				if (j != y_num - 1) {
//					if (!cmpNadd_SinPol(indices{ i, j ,k }, Y, P.ord)) { delete_edge(indices{ i, j ,k }, indices{ i, j + 1 ,k }); }
//				}
//				if (k != z_num - 1) {
//					if (!cmpNadd_SinPol(indices{ i, j ,k }, Z, P.ord)) { delete_edge(indices{ i, j ,k }, indices{ i, j , k + 1 }); }
//				}
//			}
//		}
//	}
//
//
//}
//
//void Eps_Graph_3D::delete_pol(int ord) { // delete a polygon from O, specified by its index
//	
//	bool check = true;
//	Polytope P;
//	
//	for(Polytope& p : pols){
//		if (p.ord == ord){
//			P = p;
//			check = false;
//		}
//	}
//
//	if (check) { return; }
//
//	// release them free; for gridpoints that was enclosed by the polygon
//	for (Grid_Point& gr_pt : grid) {
//		if (gr_pt.encl == ord) {
//			gr_pt.encl = -1;
//		}
//	}
//
//	indices* diagonal = eff_region(P);
//
//	int x_effmax = diagonal[1].x_ind;
//	int x_effmin = diagonal[0].x_ind;
//
//	int y_effmin = diagonal[0].y_ind;
//	int y_effmax = diagonal[1].y_ind;
//
//	int z_effmax = diagonal[1].z_ind;
//	int z_effmin = diagonal[0].z_ind;
//	pols.erase(remove(pols.begin(), pols.end(), P));
//
//	// update grid edges among gridpoints in the effective region
//	for (int i = x_effmin; i < x_effmax; i++) {
//		for (int j = y_effmin; j < y_effmax; j++) {
//			for (int k = z_effmin; k < z_effmax; k++) {
//				Grid_Point cur = grid[ind2num(i, j, k)];
//				if (i != x_num - 1) {
//						if (cmpNadd(indices{ i, j ,k }, X)) { add_edge(indices{ i, j ,k }, indices{ i + 1, j ,k }); }
//				}
//				if (j != y_num - 1) {
//						if (cmpNadd(indices{ i, j ,k }, Y)) { add_edge(indices{ i, j ,k }, indices{ i, j + 1 ,k }); }
//				}
//				if (k != z_num - 1) {
//						if (cmpNadd(indices{ i, j ,k }, Z)) { add_edge(indices{ i, j ,k }, indices{ i, j ,k + 1 }); }
//				}
//			}
//		}
//	}
//}
//
//indices* Eps_Graph_3D::eff_region(Polytope P) { // returns a range indicating orthogonal rectangle bounding the polygon (effective region)
//	static indices ret[2]; // ret[0] : minimum index containing polytope , ret[1] : maximum index containing polytope
//
//	ret[0].x_ind = max((long long int) 0, long long int(floor((P.x_min - x_min) / eps)));
//	ret[0].y_ind = max((long long int) 0, long long int(floor((P.y_min - y_min) / eps)));
//	ret[0].z_ind = max((long long int) 0, long long int(floor((P.z_min - z_min) / eps)));
//	ret[1].x_ind = min(x_num, long long int(ceil((P.x_max - x_min) / eps)));
//	ret[1].y_ind = min(y_num , long long int(ceil((P.y_max - y_min) / eps)));
//	ret[1].z_ind = min(z_num, long long int(ceil((P.z_max - z_min) / eps)));
//
//	return ret;
//}
//
//vector<Free_Point> Eps_Graph_3D::kNN(Free_Point p, int k) { // returns k approximate nearest neighbors of p
//
//	for (Polytope& pol : pols) {
//		assert(!pol.isIn(&p));
//	}
//
//	vector<Free_Point> ret = {};
//	vector<int>().swap(NN_dist);
//
//	anchor(p);
//	Grid_Point s = grid[p.host];
//	for (int& elem : dist) { elem = INT_MAX; }
//	for (unsigned int ind1 = 0; ind1 < visited.size(); ind1++) { visited[ind1] = false; }
//
//	queue<int> q = {};
//
//	dist[s.num] = 0;
//	q.push(s.num);
//	int grid_dist = 0;
//	vector<Free_Point> FPs_temp = {};
//	vector<Free_Point> FPs = {};
//
//	while (k > 0 && !q.empty()) {
//
//		// q.empty() is indeed needed twice
//		while (!q.empty() && (dist[q.front()] == grid_dist)) {
//			if (visited[q.front()] == true) {
//				q.pop();
//				continue;
//			}
//			visited[q.front()] = true;
//
//			int cur = q.front();
//			if (grid[cur].ip.x_u && visited[cur + y_num * z_num] == false) { dist[cur + y_num * z_num] = dist[cur] + 1; q.push(cur + y_num * z_num); }
//			if (grid[cur].ip.x_d && visited[cur - y_num * z_num] == false) { dist[cur - y_num * z_num] = dist[cur] + 1; q.push(cur - y_num * z_num); }
//			if (grid[cur].ip.y_u && visited[cur + z_num] == false) { dist[cur + z_num] = dist[cur] + 1; q.push(cur + z_num); }
//			if (grid[cur].ip.y_d && visited[cur - z_num] == false) { dist[cur - z_num] = dist[cur] + 1; q.push(cur - z_num); }
//			if (grid[cur].ip.z_u && visited[cur + 1] == false) { dist[cur + 1] = dist[cur] + 1; q.push(cur + 1); }
//			if (grid[cur].ip.z_d && visited[cur - 1] == false) { dist[cur - 1] = dist[cur] + 1; q.push(cur - 1); }
//
//			for (auto FP : grid[q.front()].anchored) {
//				FPs_temp.push_back(*FP);
//			}
//			q.pop();
//		}
//
//		for (auto pt : FPs_temp) {
//			FPs.push_back(pt);
//		}
//
//
//		sort(FPs.begin(), FPs.end(), [=](Free_Point first, Free_Point second)
//			{
//				return pow(first.x - p.x, 2) + pow(first.y - p.y, 2) + pow(first.z - p.z, 2) < pow(second.x - p.x, 2) + pow(second.y - p.y, 2) + pow(second.z - p.z, 2);
//			});
//
//		int sz = int(FPs.size());
//
//		if (sz < k) {
//			for (int ind2 = 0; ind2 < sz; ind2++) { NN_dist.push_back(grid_dist); }
//			k -= sz;
//			ret.insert(ret.end(), FPs.begin(), FPs.end());
//		}
//		else {
//			for (int ind2 = 0; ind2 < k; ind2++) { NN_dist.push_back(grid_dist); }
//			ret.insert(ret.end(), FPs.begin(), FPs.begin() + k);
//			k = 0;
//		}
//
//		vector<Free_Point>().swap(FPs_temp);
//		vector<Free_Point>().swap(FPs);
//		grid_dist += 1;
//	}
//	for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
//		if ((*(*it)).x == p.x && (*(*it)).y == p.y && (*(*it)).z == p.z) {
//			grid[p.host].anchored.erase(it);
//			break;
//		}
//	}
//
//	return ret;
//}
//*/
//vector<Edge> Eps_Graph_nD::kNN(Free_Point p, int k) { // returns k approximate nearest neighbors of p
//
//	for (Polytope& pol : pols) {
//		assert(!pol.isIn(&p));
//	}
//	vector<Edge> path = {};
//	vector<int>().swap(NN_dist);
//	long long int product_num = 1;
//	for (int i = 0; i < xs_num.size(); i++) {
//		product_num *= xs_num[i];
//	}
//	long long int* previous = new long long int[product_num];
//	anchor(p);
//	Grid_Point s = grid[p.host];
//	for (int& elem : dist) { elem = INT_MAX; }
//	for (unsigned int ind1 = 0; ind1 < visited.size(); ind1++) { visited[ind1] = false; }
//	for (long long int i = 0; i <product_num; i++) { previous[i] = i; }
//
//	queue<int> q = {};
//
//	dist[s.num] = 0;
//	q.push(s.num);
//	int grid_dist = 0;
//	vector<Free_Point> FPs_temp = {};
//	vector<Free_Point> FPs = {};
//
//	while (k > 0 && !q.empty()) {
//
//		// q.empty() is indeed needed twice
//		while (!q.empty() && (dist[q.front()] == grid_dist)) {
//			if (visited[q.front()] == true) {
//				q.pop();
//				continue;
//			}
//			visited[q.front()] = true;
//
//			int cur = q.front();
//
//			for (int i = 0; i < this->n; i++) {
//				long long int idx = 1;
//				for (int j = i + 1; j < this->n; j++) {
//					idx *= xs_num[i];
//				}
//
//				idx += cur;
//
//				if (grid[cur].ip_u[i] && visited[idx] == false) { dist[idx] = dist[cur]; q.push(idx); previous[idx] = cur; }
//			}
//
//			bool once=true;
//			for (auto FP : grid[q.front()].anchored) {
//				FPs_temp.push_back(*FP);
//				if (once) {
//					while (previous[cur] != cur) {
//						path.push_back({ &grid[cur], &grid[previous[cur]] });
//						cur = previous[cur];
//						once = false;
//					}
//				}
//			}
//			q.pop();
//		}
//
//		for (auto pt : FPs_temp) {
//			FPs.push_back(pt);
//		}
//
//
//		sort(FPs.begin(), FPs.end(), [=](Free_Point first, Free_Point second)
//		{	
//			first_fp = 0;
//			second_fp = 0;
//			for (int i = 0; i < this->n; i++) {
//				first_fp += pow(first->getx(i), p->getx(i), 2);
//				second_fp += pow(second->getx(i), p->getx(i), 2);
//			}
//			return first_fp < second_fp;
//		});
//
//		int sz = int(FPs.size());
//		for (int i = 0; i < min(k, sz); i++) {
//			path.push_back({ &FPs[i], &grid[FPs[i].host] });
//			NN_dist.push_back(grid_dist);
//		}
//		k -= sz;
//
//		vector<Free_Point>().swap(FPs_temp);
//		vector<Free_Point>().swap(FPs);
//		grid_dist += 1;
//	}
//
//	
//
//
//	for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
//		bool std = true;
//		
//		for (int i = 0; i < this->n; i++) {
//			std = std && ((*(*it)).getx(i) == p.getx(i));
//		}
//		
//		if (std) {
//			grid[p.host].anchored.erase(it);
//			break;
//		}
//	}
//
//	return path;
//}
//
///*
//void Eps_Graph_3D::print_grid() {
//	for (unsigned int i = 0; i < grid.size(); i++) {
//		cout << grid[i].ind.x_ind << grid[i].ind.y_ind << grid[i].ind.z_ind << ' ' << '|' << grid[i].ip.x_u << ' ' << '|' << grid[i].ip.y_u << grid[i].ip.z_u << ' ' << '|';
//		if (num2ind(i).z_ind == z_num - 1) { cout << endl; }
//	}
//}
//
//void Eps_Graph_3D::print_encl() {
//	for (Polytope& pol : pols) {
//		for (auto pt : pol.encl_pts) {
//			for (int i = 0; i < pt->n; i++) {
//				cout << pt->getx(i) << ' ';
//			}
//			cout << endl;
//		}
//	}
//}
//
//void Eps_Graph_3D::print_free_point() {
//	for (Free_Point& fr : fr_pts) {
//
//		for (int i = 0; i < this->n; i++) {
//			cout << fr.getx(i) << ' ';
//		}
//		
//		cout << endl;
//
//	}
//}
//
//
///*
//void Eps_Graph_3D::print_edges() {
//	for (auto gp : grid) {
//		if (gp.ip.z_u == true && gp.ind.x_ind == 10 && gp.ind.y_ind == 10) {
//			cout << gp.getx() << ' ' << gp.gety() << ' ' << gp.getz() << '|' << gp.getx() << ' ' << gp.gety() << ' ' << gp.getz() + eps << endl;
//		}
//	}
//}
//*/
//
///*
//void Eps_Graph_3D::print_anchor() {
//	for (auto gp : grid) {
//		for (vector<Free_Point*>::iterator it = gp.anchored.begin(); it != gp.anchored.end(); ++it) {
//			for (int i = 0; i < this->n; i++) {
//				cout << gp.ind[i] << ' ';
//			}
//
//			cout << '|';
//
//			for (int j = 0; i < this->; j++) {
//				cout << (*(*it)).getx(j) << ' ';
//			}
//
//			cout << endl;
//		}
//	}
//}
//
///*
//void Eps_Graph_nD::print_dist() {
//	int idx = 0;
//
//	for (int i = 0; i < x_num; i++) {
//		for (int j = 0; j < y_num; j++) {
//			for (int k = 0; k < z_num; k++) {
//				if (dist[i * y_num * z_num + j * z_num + k] == INT_MAX) { printf(" INF "); }
//				else { printf("%4.1d ", dist[i * y_num * z_num+ j * z_num + k]); }
//			}
//		}
//		printf("\n");
//	}
//}
//*/
//
//
//void Eps_Graph_nD::print_kNN(Free_Point p, int k ) {
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
//
//
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
//
//
//vector<Edge> Eps_Graph_nD::get_path(Free_Point p, int k) {
//	vector<Edge> path = path_kNN(p, k + 1);
//	return path;
//}
//

#include "pch.h"
#include "EpsGraph3D.h"
#include <queue>
#include <assert.h>
#include <iostream>
#include <math.h>

#define X 1 // ray direction
#define Y 2
#define Z 3

EPS::Eps_Graph_3D::Eps_Graph_3D(std::list<Free_Point> _fr_pts, std::vector<Polytope> _pols, double _eps) { //O

	fr_pts = _fr_pts;
	pols = _pols;
	eps = _eps;
	NN_dist = {};
	ord_pol = 0;
	x_min = y_min = z_min = DBL_MAX;
	x_max = y_max = z_max = DBL_MIN;

	// among all points and polygons vertices, find the minimum/maximum x- and y-coordinate.
	// this is to find a reasonably small bounding box for P \setunion O.
	
	for (int i = 0; i < pols.size();i++) {
		pols[i].ord = ord_pol;
		ord_pol++;
		if (pols[i].x_max > this->x_max) { this->x_max = pols[i].x_max; }
		if (pols[i].y_max > this->y_max) { this->y_max = pols[i].y_max; }
		if (pols[i].z_max > this->z_max) { this->z_max = pols[i].z_max; }

		if (pols[i].x_min < this->x_min) { this->x_min = pols[i].x_min; }
		if (pols[i].y_min < this->y_min) { this->y_min = pols[i].y_min; }
		if (pols[i].z_min < this->z_min) { this->z_min = pols[i].z_min; }
	}

	for (auto fr_pt : fr_pts) {
		if (fr_pt.x > this->x_max) { this->x_max = fr_pt.x; }
		if (fr_pt.y > this->y_max) { this->y_max = fr_pt.y; }
		if (fr_pt.z > this->z_max) { this->z_max = fr_pt.z; }

		if (fr_pt.x < this->x_min) { this->x_min = fr_pt.x; }
		if (fr_pt.y < this->y_min) { this->y_min = fr_pt.y; }
		if (fr_pt.z < this->z_min) { this->z_min = fr_pt.z; }
	}

	init_grid();

	for (Free_Point& fr_pt : fr_pts) {
		anchor(fr_pt);
	}
}

void EPS::Eps_Graph_3D::init_grid() { //O

	x_num = 1 + long long int(ceil((x_max - x_min) / eps));
	y_num = 1 + long long int(ceil((y_max - y_min) / eps));
	z_num = 1 + long long int(ceil((z_max - z_min) / eps));

	upper_left = Point(x_min, y_min, z_min);
	//need to check why y is different!!

	// initialization step for BFS
	for (long long int i = 0; i < y_num * x_num * z_num; i++) {
		dist.push_back(INT_MAX);
		visited.push_back(false);
	}

	for (long long int i = 0; i < y_num * x_num * z_num; i++)
	{
		grid.push_back(Grid_Point(num2ind(i).x_ind, num2ind(i).y_ind, num2ind(i).z_ind, upper_left.x, upper_left.y, upper_left.z, eps, y_num, z_num));
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
	for (int i = 0; i < x_num; i++) {
		for (int j = 0; j < y_num; j++) {
			for (int k = 0; k < z_num; k++) {
				if (grid[ind2num(i, j, k)].encl != -1) { continue; }

				if (i != x_num - 1) {
					if (grid[ind2num(i + 1, j, k)].encl == -1) {
						if (cmpNadd(indices{ i, j, k }, 0)) { add_edge(indices{ i, j, k }, indices{ i + 1, j, k }); }
					}
				}
				if (j !=y_num - 1) {
					if (grid[ind2num(i, j + 1, k)].encl == -1) {
						if (cmpNadd(indices{ i, j, k }, 1)) { add_edge(indices{ i, j, k }, indices{ i, j + 1, k }); }
					}
				}
				if (k != z_num - 1) {
					if (grid[ind2num(i, j, k + 1 )].encl == -1) {
						if (cmpNadd(indices{ i, j, k }, 2)) { add_edge(indices{ i, j, k }, indices{ i, j, k + 1 }); }
					}
				}
			}
		}
	}
}

EPS::Grid_Point EPS::Eps_Graph_3D::get_gridpt(indices ind) { //O
	return grid[ind2num(ind)];
}

// one-to-one functions between 2-d indices and numbers
long long int EPS::Eps_Graph_3D::ind2num(indices ind) {
	return ind.x_ind * y_num * z_num + ind.y_ind * z_num + ind.z_ind;
}

long long int EPS::Eps_Graph_3D::ind2num(long long int _x, long long int _y, long long int _z) { //O
	return _x * y_num * z_num + _y * z_num + _z;
}

EPS::indices EPS::Eps_Graph_3D::num2ind(long long int num) { //O
	return indices{ num / (y_num * z_num), (num % (y_num * z_num)) / z_num, (num % (y_num * z_num)) % z_num};
}

// adds/deletes a grid edge
void EPS::Eps_Graph_3D::add_edge(indices ind1, indices ind2) { //O
	int x1 = ind1.x_ind; int y1 = ind1.y_ind; int z1 = ind1.z_ind;
	int x2 = ind2.x_ind; int y2 = ind2.y_ind; int z2 = ind2.z_ind;

	if (y1 == y2 && z1 == z2) { grid[x1 * y_num * z_num + y1 *z_num + z1].ip.x_u = true; grid[x2 * y_num * z_num + y2 * z_num + z2].ip.x_d = true; }
	else if (x1 == x2 && z1 == z2) { grid[x1 * y_num * z_num + y1 * z_num + z1].ip.y_u = true; grid[x2 * y_num * z_num + y2 * z_num + z2].ip.y_d = true; }
	else if (x1 == x2 && y1 == y2) { grid[x1 * y_num * z_num + y1 * z_num + z1].ip.z_u = true; grid[x2 * y_num * z_num + y2 * z_num + z2].ip.z_d = true; }
}


void EPS::Eps_Graph_3D::delete_edge(indices ind1, indices ind2) { //O
	int x1 = ind1.x_ind; int y1 = ind1.y_ind; int z1 = ind1.z_ind;
	int x2 = ind2.x_ind; int y2 = ind2.y_ind; int z2 = ind2.z_ind;

	if (y1 == y2 && z1 == z2) { grid[x1 * y_num * z_num + y1 * z_num + z1].ip.x_u = false; grid[x2 * y_num * z_num + y2 * z_num + z2].ip.x_d = false; }
	else if (x1 == x2 && z1 == z2) { grid[x1 * y_num * z_num + y1 * z_num + z1].ip.y_u = false; grid[x2 * y_num * z_num + y2 * z_num + z2].ip.y_d = false; }
	else if (x1 == x2 && y1 == y2) { grid[x1 * y_num * z_num + y1 * z_num + z1].ip.z_u = false; grid[x2 * y_num * z_num + y2 * z_num + z2].ip.z_d = false; }
}


bool EPS::Eps_Graph_3D::cmpNadd(indices ind, int direc) {  //O
	// checks if the line connecting the gridpoint and its neighboring one is blocked by any polytope. if is not, add an edge between them.

	Grid_Point A = grid[ind2num(ind)], B;
	if (direc == 0) { B = grid[ind2num(ind) + y_num * z_num]; }
	else if (direc == 1) { B = grid[ind2num(ind) + z_num]; }
	else { B = grid[ind2num(ind) + 1]; }

	for (unsigned int ind1 = 0; ind1 < pols.size(); ind1++) { 
		auto pol = pols[ind1];
		if (pol.isIn(&A) || pol.isIn(&B)) {
			return false;
		}

		// if a polygon edge crosses the line connecting two gridpoints, then the edge between them is not present
		if (pol.intersect(Point{ A.x, A.y, A.z}, Point{ B.x, B.y, B.z }, direc)) { return false; }
		
	}

	return true;
}

bool EPS::Eps_Graph_3D::cmpNadd_SinPol(indices ind, int direc, int ord) { // do the same with a specific polygon.

	Polytope pol;
	std::vector<Polytope>::iterator it;

	for (it = pols.end() - 1;; it--) {
		if ((*it).ord == ord) {
			pol = *it;
			break;
		}
	}
	Grid_Point A = grid[ind2num(ind)], B;
	if (direc == 0) { B = grid[ind2num(ind) + y_num * z_num]; }
	else if (direc == 1) { B = grid[ind2num(ind) + z_num]; }
	else { B = grid[ind2num(ind) + 1]; }
	if (pol.isIn(&A) || pol.isIn(&B)) {
		return false;
	}

	if (pol.intersect(Point{ A.x, A.y, A.z }, Point{ B.x, B.y, B.z }, direc)) { return false; }
	return true;

}

void EPS::Eps_Graph_3D::add_freepts(Free_Point *p) { // add points to the point set P //O
	Free_Point& pt = fr_pts.back();
	for (Polytope& pol : pols) {
		assert(!pol.isIn(p)); // Assert error if a freepoint is contained in some polytope
	}
	anchor(*p);
	fr_pts.push_back(*p);
}

void EPS::Eps_Graph_3D::add_freepts(std::vector<Free_Point> p_vec) { // add points to the point set P //O
	for (auto p : p_vec) {
		for (Polytope& pol : pols) {
			assert(!pol.isIn(&p)); // Assert error if a freepoint is contained in some polytope
		}
		anchor(p);
		fr_pts.push_back(p);
	}
}

void EPS::Eps_Graph_3D::delete_freept(int ind) { // delete a point from P, specified by its index //O
	if (ind < 0 || fr_pts.size() - 1 < ind) { return; }

	std::list<Free_Point>::iterator iter = fr_pts.begin();
	advance(iter, ind);
	Free_Point& p = *iter;
	for (std::vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
		if ((*(*it)).x == p.x && (*(*it)).y == p.y && (*(*it)).z == p.z) {
			grid[p.host].anchored.erase(it);
			break;
		}
	}
	fr_pts.erase(remove(fr_pts.begin(), fr_pts.end(), p));
}

void EPS::Eps_Graph_3D::anchor(Free_Point& p) { // cast anchor onto a grid point from a free point

	if (p.host != -1) {
		assert(0 <= p.host && p.host < grid.size());
		for (std::vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end();) {
			if (p.x == (*(*it)).x && p.y == (*(*it)).y && p.z == (*(*it)).z) {
				it = grid[p.host].anchored.erase(it);
				if (it == grid[p.host].anchored.end()) break;
			}
			else {
				++it;
			}
		}
	}

	Free_Point* q = new Free_Point(p);
	bool flag = false;
	int x; int y; int z; 
	x = int(ceil((p.x - upper_left.x) / eps - 0.5)); // points on the midline anchors leftward 
	y = int(ceil((p.y - upper_left.y) / eps - 0.5));
	z = int(ceil((p.z - upper_left.z) / eps - 0.5));
	if (grid[ind2num(x, y, z)].encl == -1) {
		p.host = grid[ind2num(x, y, z)].num;
		q->host = grid[ind2num(x, y, z)].num;
		grid[ind2num(x, y, z)].anchored.push_back(q);
		return;
	}

	// find a nearest gridpoint not enclosed by any polygon
	for (int step = 1; step < y_num + x_num + z_num; step++) {
		for (int x_step = -step; x_step <= step; x_step++) {
			for (int y_step = -(step-abs(x_step)); y_step <= step - abs(x_step); y_step++) {
				int z_step = step - abs(x_step) - abs(y_step);
				if (0 <= x + x_step && x + x_step < x_num && 0 <= y + y_step && y + y_step < y_num && 0 <= z + z_step && z + z_step < z_num) {
					if (grid[ind2num(x + x_step, y + y_step, z + z_step)].encl == -1) {
						p.host = grid[ind2num(x + x_step, y + y_step, z + z_step)].num;
						q->host = grid[ind2num(x, y, z)].num;
						grid[ind2num(x + x_step, y + y_step, z + z_step)].anchored.push_back(q);
						return;
					}
				}
				if (0 <= x + x_step && x + x_step < x_num && 0 <= y + y_step && y + y_step < y_num && 0 <= z - z_step && z - z_step < z_num) {
					if (grid[ind2num(x + x_step, y + y_step, z - z_step)].encl == -1) {
						p.host = grid[ind2num(x + x_step, y + y_step, z - z_step)].num;
						q->host = grid[ind2num(x, y, z)].num;
						grid[ind2num(x + x_step, y + y_step, z - z_step)].anchored.push_back(q);
						return;
					}
				}
			}
		}
	}
}
void EPS::Eps_Graph_3D::query_anchor(Grid_Point& g) {
	for (std::vector<Free_Point*>::iterator it = g.anchored.begin(); it != g.anchored.end();) {
		Free_Point& p = (*(*(it)));
		bool flag = false;
		int x; int y; int z;
		x = int(ceil((p.x - upper_left.x) / eps - 0.5)); // points on the midline anchors leftward 
		y = int(ceil((p.y - upper_left.y) / eps - 0.5));
		z = int(ceil((p.z - upper_left.z) / eps - 0.5));
		if (grid[ind2num(x, y, z)].encl == -1) {
			p.host = grid[ind2num(x, y, z)].num;
			grid[ind2num(x, y, z)].anchored.push_back(&p);
			break;
		}

		for (int step = 1; step < y_num + x_num + z_num; step++) {
			for (int x_step = -step; x_step <= step; x_step++) {
				for (int y_step = -(step - abs(x_step)); y_step <= step - abs(x_step); y_step++) {
					int z_step = step - abs(x_step) - abs(y_step);
					if (0 <= x + x_step && x + x_step < x_num && 0 <= y + y_step && y + y_step < y_num && 0 <= z + z_step && z + z_step < z_num) {
						if (grid[ind2num(x + x_step, y + y_step, z + z_step)].encl == -1) {
							p.host = grid[ind2num(x + x_step, y + y_step, z + z_step)].num;
							grid[ind2num(x + x_step, y + y_step, z + z_step)].anchored.push_back(&p);
							flag = true;
						}
					}
					if (flag) break;
					if (0 <= x + x_step && x + x_step < x_num && 0 <= y + y_step && y + y_step < y_num && 0 <= z - z_step && z - z_step < z_num) {
						if (grid[ind2num(x + x_step, y + y_step, z - z_step)].encl == -1) {
							p.host = grid[ind2num(x + x_step, y + y_step, z - z_step)].num;
							grid[ind2num(x + x_step, y + y_step, z - z_step)].anchored.push_back(&p);
							flag = true;
						}
					}
					if (flag) break;
				}
				if (flag) break;
			}
			if (flag) break;
		}

		it = g.anchored.erase(it);
		if (it == g.anchored.end()) break;
	}
	return;
}

void EPS::Eps_Graph_3D::add_pol(Polytope P) { // add a polygon to the set of obstacles O 
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
			std::vector<Free_Point*>().swap(gr_pt.anchored);
		}
	}
	pols.push_back(P);
	ord_pol++;

	indices* diagonal = eff_region(P);

	int x_effmax = diagonal[1].x_ind;
	int x_effmin = diagonal[0].x_ind;

	int y_effmin = diagonal[0].y_ind;
	int y_effmax = diagonal[1].y_ind;

	int z_effmax = diagonal[1].z_ind;
	int z_effmin = diagonal[0].z_ind;

	// update grid edges among gridpoints in the effective region
	for (int i = x_effmin ; i < x_effmax; i++) {
		for (int j = y_effmin; j < y_effmax; j++) {
			for (int k = z_effmin; k < z_effmax; k++) {
				Grid_Point cur = grid[ind2num(i, j, k)];
				if (i != x_num - 1) {
					if (!cmpNadd_SinPol(indices{ i, j ,k }, X, P.ord)) { delete_edge(indices{ i, j ,k }, indices{ i + 1, j ,k }); }
				}
				if (j != y_num - 1) {
					if (!cmpNadd_SinPol(indices{ i, j ,k }, Y, P.ord)) { delete_edge(indices{ i, j ,k }, indices{ i, j + 1 ,k }); }
				}
				if (k != z_num - 1) {
					if (!cmpNadd_SinPol(indices{ i, j ,k }, Z, P.ord)) { delete_edge(indices{ i, j ,k }, indices{ i, j , k + 1 }); }
				}
			}
		}
	}


}

void EPS::Eps_Graph_3D::delete_pol(int ord) { // delete a polygon from O, specified by its index
	
	bool check = true;
	Polytope P;
	
	for(Polytope& p : pols){
		if (p.ord == ord){
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

	indices* diagonal = eff_region(P);

	int x_effmax = diagonal[1].x_ind;
	int x_effmin = diagonal[0].x_ind;

	int y_effmin = diagonal[0].y_ind;
	int y_effmax = diagonal[1].y_ind;

	int z_effmax = diagonal[1].z_ind;
	int z_effmin = diagonal[0].z_ind;
	pols.erase(remove(pols.begin(), pols.end(), P));

	// update grid edges among gridpoints in the effective region
	for (int i = x_effmin; i < x_effmax; i++) {
		for (int j = y_effmin; j < y_effmax; j++) {
			for (int k = z_effmin; k < z_effmax; k++) {
				Grid_Point cur = grid[ind2num(i, j, k)];
				if (i != x_num - 1) {
						if (cmpNadd(indices{ i, j ,k }, X)) { add_edge(indices{ i, j ,k }, indices{ i + 1, j ,k }); }
				}
				if (j != y_num - 1) {
						if (cmpNadd(indices{ i, j ,k }, Y)) { add_edge(indices{ i, j ,k }, indices{ i, j + 1 ,k }); }
				}
				if (k != z_num - 1) {
						if (cmpNadd(indices{ i, j ,k }, Z)) { add_edge(indices{ i, j ,k }, indices{ i, j ,k + 1 }); }
				}
			}
		}
	}
}

EPS::indices* EPS::Eps_Graph_3D::eff_region(Polytope P) { // returns a range indicating orthogonal rectangle bounding the polygon (effective region)
	static indices ret[2]; // ret[0] : minimum index containing polytope , ret[1] : maximum index containing polytope

	ret[0].x_ind = std::max((long long int) 0, long long int(floor((P.x_min - x_min) / eps)));
	ret[0].y_ind = std::max((long long int) 0, long long int(floor((P.y_min - y_min) / eps)));
	ret[0].z_ind = std::max((long long int) 0, long long int(floor((P.z_min - z_min) / eps)));
	ret[1].x_ind = std::min(x_num, long long int(ceil((P.x_max - x_min) / eps)));
	ret[1].y_ind = std::min(y_num , long long int(ceil((P.y_max - y_min) / eps)));
	ret[1].z_ind = std::min(z_num, long long int(ceil((P.z_max - z_min) / eps)));

	return ret;
}

std::vector<EPS::Free_Point> EPS::Eps_Graph_3D::kNN(Free_Point p, int k) { // returns k approximate nearest neighbors of p

	for (Polytope& pol : pols) {
		assert(!pol.isIn(&p));
	}

	std::vector<Free_Point> ret = {};
	std::vector<int>().swap(NN_dist);

	anchor(p);
	Grid_Point s = grid[p.host];
	for (int& elem : dist) { elem = INT_MAX; }
	for (unsigned int ind1 = 0; ind1 < visited.size(); ind1++) { visited[ind1] = false; }

	std::queue<int> q = {};

	dist[s.num] = 0;
	q.push(s.num);
	int grid_dist = 0;
	std::vector<Free_Point> FPs_temp = {};
	std::vector<Free_Point> FPs = {};

	while (k > 0 && !q.empty()) {

		// q.empty() is indeed needed twice
		while (!q.empty() && (dist[q.front()] == grid_dist)) {
			if (visited[q.front()] == true) {
				q.pop();
				continue;
			}
			visited[q.front()] = true;

			int cur = q.front();
			if (grid[cur].ip.x_u && visited[cur + y_num * z_num] == false) { dist[cur + y_num * z_num] = dist[cur] + 1; q.push(cur + y_num * z_num); }
			if (grid[cur].ip.x_d && visited[cur - y_num * z_num] == false) { dist[cur - y_num * z_num] = dist[cur] + 1; q.push(cur - y_num * z_num); }
			if (grid[cur].ip.y_u && visited[cur + z_num] == false) { dist[cur + z_num] = dist[cur] + 1; q.push(cur + z_num); }
			if (grid[cur].ip.y_d && visited[cur - z_num] == false) { dist[cur - z_num] = dist[cur] + 1; q.push(cur - z_num); }
			if (grid[cur].ip.z_u && visited[cur + 1] == false) { dist[cur + 1] = dist[cur] + 1; q.push(cur + 1); }
			if (grid[cur].ip.z_d && visited[cur - 1] == false) { dist[cur - 1] = dist[cur] + 1; q.push(cur - 1); }

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
			return abs(first.x - p.x) + abs(first.y - p.y) + abs(first.z - p.z) < abs(second.x - p.x) + abs(second.y - p.y) + abs(second.z - p.z);
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

		std::vector<Free_Point>().swap(FPs_temp);
		std::vector<Free_Point>().swap(FPs);
		grid_dist += 1;
	}
	for (std::vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
		if ((*(*it)).x == p.x && (*(*it)).y == p.y && (*(*it)).z == p.z) {
			grid[p.host].anchored.erase(it);
			break;
		}
	}

	return ret;
}

std::vector<EPS::Edge> EPS::Eps_Graph_3D::path_kNN(Free_Point p, int k) { // returns k approximate nearest neighbors of p

	for (Polytope& pol : pols) {
		assert(!pol.isIn(&p));
	}
	std::vector<Edge> path = {};
	std::vector<int>().swap(NN_dist);
	long long int* previous = new long long int[x_num * y_num * z_num];
	anchor(p);
	Grid_Point s = grid[p.host];
	for (int& elem : dist) { elem = INT_MAX; }
	for (unsigned int ind1 = 0; ind1 < visited.size(); ind1++) { visited[ind1] = false; }
	for (long long int i = 0; i < y_num * x_num * z_num; i++) { previous[i] = i; }

	std::queue<int> q = {};

	dist[s.num] = 0;
	q.push(s.num);
	int grid_dist = 0;
	std::vector<Free_Point> FPs_temp = {};
	std::vector<Free_Point> FPs = {};

	while (k > 0 && !q.empty()) {

		// q.empty() is indeed needed twice
		while (!q.empty() && (dist[q.front()] == grid_dist)) {
			if (visited[q.front()] == true) {
				q.pop();
				continue;
			}
			visited[q.front()] = true;

			int cur = q.front();
			if (grid[cur].ip.x_u && visited[cur + y_num * z_num] == false) { dist[cur + y_num * z_num] = dist[cur] + 1; q.push(cur + y_num * z_num); 
			if (previous[cur + y_num * z_num] = cur + y_num * z_num) { previous[cur + y_num * z_num] = cur; }
			}
			if (grid[cur].ip.x_d && visited[cur - y_num * z_num] == false) { dist[cur - y_num * z_num] = dist[cur] + 1; q.push(cur - y_num * z_num);
			if (previous[cur - y_num * z_num] = cur - y_num * z_num) { previous[cur - y_num * z_num] = cur; }
			}
			if (grid[cur].ip.y_u && visited[cur + z_num] == false) { dist[cur + z_num] = dist[cur] + 1; q.push(cur + z_num); 
			if (previous[cur + z_num] = cur + z_num) { previous[cur + z_num] = cur; }
			}
			if (grid[cur].ip.y_d && visited[cur - z_num] == false) { dist[cur - z_num] = dist[cur] + 1; q.push(cur - z_num); 
			if (previous[cur - z_num] = cur - z_num) { previous[cur - z_num] = cur; }; }
			if (grid[cur].ip.z_u && visited[cur + 1] == false) { dist[cur + 1] = dist[cur] + 1; q.push(cur + 1); 
			if (previous[cur + 1] = cur + 1) { previous[cur + 1] = cur; }
			}
			if (grid[cur].ip.z_d && visited[cur - 1] == false) { dist[cur - 1] = dist[cur] + 1; q.push(cur - 1); 
			if (previous[cur - 1] = cur - 1) { previous[cur - 1] = cur; }
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
			return abs(first.x - p.x) + abs(first.y - p.y) + abs(first.z - p.z) < abs(second.x - p.x) + abs(second.y - p.y) + abs(second.z - p.z);
		});

		int sz = int(FPs.size());
		for (int i = 0; i < std::min(k, sz); i++) {
			int t = FPs[i].host;
			Point temp = { FPs[i].x ,  FPs[i].y ,  FPs[i].z };
			Point temp2 = { grid[t].x, grid[t].y , grid[t].z };
			path.push_back({ temp, temp2 });
			while (previous[t] != t) {
				temp = { grid[t].x , grid[t].y , grid[t].z };
				temp2 = { grid[previous[t]].x, grid[previous[t]].y , grid[previous[t]].z };
				path.push_back({ temp, temp2 });
				t = previous[t];
			}
			NN_dist.push_back(grid_dist);
		}
		k -= sz;

		std::vector<Free_Point>().swap(FPs_temp);
		std::vector<Free_Point>().swap(FPs);
		grid_dist += 1;
	}
	for (std::vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
		if ((*(*it)).x == p.x && (*(*it)).y == p.y && (*(*it)).z == p.z) {
			grid[p.host].anchored.erase(it);
			break;
		}
	}

	return path;
}

void EPS::Eps_Graph_3D::print_grid() {
	for (unsigned int i = 0; i < grid.size(); i++) {
		std::cout << grid[i].ind.x_ind << grid[i].ind.y_ind << grid[i].ind.z_ind << ' ' << '|' << grid[i].ip.x_u << ' ' << '|' << grid[i].ip.y_u << grid[i].ip.z_u << ' ' << '|';
		if (num2ind(i).z_ind == z_num - 1) { std::cout << std::endl; }
	}
}

void EPS::Eps_Graph_3D::print_encl() {
	for (Polytope& pol : pols) {
		for (auto pt : pol.encl_pts) {
			std::cout << pt->x << ' ' << pt->y << ' ' << pt->z << ' ' << std::endl;
		}
	}
}

void EPS::Eps_Graph_3D::print_free_point() {
	for (Free_Point& fr : fr_pts) {
		std::cout << fr.x << ' ' << fr.y << ' ' << fr.z << ' ' << std::endl;
	}
}

void EPS::Eps_Graph_3D::print_edges() {
	for (auto gp : grid) {
		if (gp.ip.z_u == true && gp.ind.x_ind == 10 && gp.ind.y_ind == 10) {
			std::cout << gp.getx() << ' ' << gp.gety() << ' ' << gp.getz() << '|' << gp.getx() << ' ' << gp.gety() << ' ' << gp.getz() + eps << std::endl;
		}
	}
}

void EPS::Eps_Graph_3D::print_anchor() {
	for (auto gp : grid) {
		for (std::vector<Free_Point*>::iterator it = gp.anchored.begin(); it != gp.anchored.end(); ++it) {
			std::cout << gp.ind.x_ind << ' ' << gp.ind.y_ind << ' ' << gp.ind.z_ind << '|' << (*(*it)).x << ' ' << (*(*it)).y << ' ' << (*(*it)).z << std::endl;
		}
	}
}

void EPS::Eps_Graph_3D::print_dist() {
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

void EPS::Eps_Graph_3D::print_kNN(Free_Point p, int k ) {
	std::vector<Free_Point> nbhd = kNN(p, k + 1);
	for (auto nb : nbhd) {
		if (nb.x == p.x && nb.y == p.y && nb.z == p.z) continue;
		std::cout << nb.x << ' ' << nb.y << ' ' << nb.z << std::endl;
	}
}


EPS::Eps_Graph_3D::Eps_Graph_3D() { y_num = x_num = z_num = 0; x_min = x_max = y_min = y_max = z_min = z_max = eps = 0; }

std::list<EPS::Free_Point> EPS::Eps_Graph_3D::get_free_points()
{
	return fr_pts;
}

EPS::Free_Point EPS::Eps_Graph_3D::get_free_point(int index) {
	std::list<Free_Point>::iterator iter = fr_pts.begin();
	std::advance(iter, index);
	return *iter;
}

std::vector<EPS::Polytope> EPS::Eps_Graph_3D::get_Polytope() {
	return pols;
}

std::vector<EPS::Grid_Point> EPS::Eps_Graph_3D::get_grid() {
	return grid;
}


std::vector<EPS::Edge> EPS::Eps_Graph_3D::get_path(Free_Point p, int k) {
	std::vector<Edge> path = path_kNN(p, k + 1);
	return path;
}
#include "EpsGraphnD.h"
#include <queue>
#include <assert.h>
#include <iostream>

using namespace std;


vector<long long int> Eps_Graph_nD::num2ind(long long int num) {//O
	long long t = 1;
	for (int i = 1;i < n;i++) {
		t *= xs_num[i];
	}
	vector<long long int> out(n);
	for (int i = 0;i < n-1;i++) {
		out[i] = num / (t);
		num = num % t;
		t = t / xs[i + 1];
	}
	out[n - 1] = num;
	return out;
}

void Eps_Graph_nD::add_edge(vector<long long int> ind1, vector<long long int> ind2) { //O
	for (int i = 0;i < ind1.size();i++) {
		if (ind1[i] != ind2[i]) break;
	}
	grid[ind2num(ind1)].ip_u[i] = true;
	grid[ind2num(ind2)].ip_d[i] = true;	
}

void Eps_Graph_nD::delete_edge(vector<long long int> ind1, vector<long long int> ind2) { //O
	for (int i = 0;i < ind1.size();i++) {
		if (ind1[i] != ind2[i]) break;
	}
	grid[ind2num(ind1)].ip_u[i] = false;
	grid[ind2num(ind2)].ip_d[i] = false;
}

bool Eps_Graph_nD::cmpNadd(vector<long long int> ind, int direc) {  //O
	// checks if the line connecting the gridpoint and its neighboring one is blocked by any polytope. if is not, add an edge between them.

	Grid_Point A = grid[ind2num(ind)], B;
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
		if (pol.intersect(Point{ A.ind }, Point{ B.ind }, direc)) { return false; }

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
	Grid_Point A = grid[ind2num(ind)], B;
	long long t = 1;
	for (int i = 0;i < n - 1 - direc;i++) {
		t *= xs_num[n - 1 - i];
	}
	B = grid[ind2num(ind) + t];
	if (pol.isIn(&A) || pol.isIn(&B)) {
		return false;
	}

	if (pol.intersect(Point{ A.ind }, Point{ B.ind }, direc)) { return false; }
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
	if (ind < 0 || fr_pts.size() - 1 < ind) { return; }

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
/*
void Eps_Graph_3D::query_anchor(Grid_Point& g) {
	for (vector<Free_Point*>::iterator it = g.anchored.begin(); it != g.anchored.end();) {
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

void Eps_Graph_3D::add_pol(Polytope P) { // add a polygon to the set of obstacles O 
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
*/
/*
void Eps_Graph_nD::delete_pol(int ord) { // delete a polygon from O, specified by its index
	
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

	vector <int> xs_effmax;
	vector <int> xs_effmin;

	for (int i = 0; i < this->n; i++) {
		xs_effmax.push_back(diagonal[0].ind[i]);
		xs_effmin.push_back(diagonal[1].ind[i]);
	}

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
*/
indices* Eps_Graph_nD::eff_region(Polytope P) { // returns a range indicating orthogonal rectangle bounding the polygon (effective region)
	static indices ret[2]; // ret[0] : minimum index containing polytope , ret[1] : maximum index containing polytope

	for (int i = 0; i < this->n; i++) {
		ret[0].ind[i] = max((long long int) 0, long long int(floor((P.xs_min[i] - xs_min[i]) / eps)));
		ret[1].ind[i] = min(xs_num[i], long long int(ceil((P.xs_max[i] - xs_min[i]) / eps)));
	}

	return ret;
}

vector<Edge> Eps_Graph_nD::kNN(Free_Point p, int k) { // returns k approximate nearest neighbors of p

	for (Polytope& pol : pols) {
		assert(!pol.isIn(&p));
	}
	vector<Edge> path = {};
	vector<int>().swap(NN_dist);
	long long int product_num = 1;
	for (int i = 0; i < xs_num.size(); i++) {
		product_num *= xs_num[i];
	}
	long long int* previous = new long long int[product_num];
	anchor(p);
	Grid_Point s = grid[p.host];
	for (int& elem : dist) { elem = INT_MAX; }
	for (unsigned int ind1 = 0; ind1 < visited.size(); ind1++) { visited[ind1] = false; }
	for (long long int i = 0; i < product_num; i++) { previous[i] = i; }

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

			for (int i = 0; i < this->n; i++) {
				long long int idx = 1;
				for (int j = i + 1; j < this->n; j++) {
					idx *= xs_num[i];
				}

				idx += cur;

				if (grid[cur].ip_u[i] && visited[idx] == false) { dist[idx] = dist[cur]; q.push(idx); previous[idx] = cur; }
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
				first_fp = 0;
				second_fp = 0;
				for (int i = 0; i < this->n; i++) {
					first_fp += pow(first->getx(i), p->getx(i), 2);
					second_fp += pow(second->getx(i), p->getx(i), 2);
				}
				return first_fp < second_fp;
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
		bool std = true;

		for (int i = 0; i < this->n; i++) {
			std = std && ((*(*it)).getx(i) == p.getx(i));
		}

		if (std) {
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
		for (int j = 0; j < grid[i].getsize(); j++) {
			cout << grid[i].ip_u << ' ';
		}
		cout << '|';
		if (num2ind(i).ind[grid[i].getsize() - 1] == this->xs_num[grid[i].getsize() - 1] - 1) { cout << endl; }
	}
}

void Eps_Graph_3D::print_encl() {
	for (Polytope& pol : pols) {
		for (auto pt : pol.encl_pts) {
			for (int i = 0; i < pt->n; i++) {
				cout << pt->getx(i) << ' ';
			}
			cout << endl;
		}
	}
}

void Eps_Graph_3D::print_free_point() {
	for (Free_Point& fr : fr_pts) {

		for (int i = 0; i < this->n; i++) {
			cout << fr.getx(i) << ' ';
		}

		cout << endl;

	}
}




void Eps_Graph_nD::print_edges() {
	for (auto gp : grid) {
		bool criteria = gp.ip_u[gp.getsize()];
		for (vector<long long int>::iterator it = gp.ind.begin(); it != gp.ind.end(); ++it) {
			criteria = criteria && (gp.ind[i] == 10)
		}
		if (criteria) {
			for (int i = 0; i < gp.getsize(); i++) {
				cout << gp.getx(i) << ' ';
			}

			cout << '|';

			for (int i = 0; i < gp.getsize() - 1; i++) {
				cout << gp.getx(i) << ' '
			}

			cout << gp.getx(gp.getsize() - 1) + eps << endl;
		}
	}
}


void Eps_Graph_3D::print_anchor() {
	for (auto gp : grid) {
		for (vector<Free_Point*>::iterator it = gp.anchored.begin(); it != gp.anchored.end(); ++it) {
			for (int i = 0; i < this->n; i++) {
				cout << gp.ind[i] << ' ';
			}

			cout << '|';

			for (int j = 0; i < this->; j++) {
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


void Eps_Graph_nD::print_kNN(Free_Point p, int k) {
	vector<Free_Point> nbhd = kNN(p, k + 1);
	for (auto nb : nbhd) {
		bool cond = (nb.getx(0) == p.getx(0) && nb.getx(1) == p.getx(1));
		for (int i = 2; i < this->n; i++) {
			cond = cond && (nb.getx(i) == p.getx(i));
		}
		if (cond) continue;
		for (int j = 0; j < this->n; j++) {
			cout << nb.getx(j) << ' ';
		}
		cout << endl;
	}
}


Eps_Graph_nD::Eps_Graph_nD() {
	for (int i = 0; i < this->n; i++) {
		xs_num[i] = 0;
		xs_max[i] = 0;
		xs_min[i] = 0;
	}
	eps = 0;
}

list<Free_Point> Eps_Graph_nD::get_free_points()
{
	return this->fr_pts;
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


vector<Edge> Eps_Graph_nD::get_path(Free_Point p, int k) {
	vector<Edge> path = path_kNN(p, k + 1);
	return path;
}
#include "EpsGraphnD.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>



class quadTree {

};

using namespace std;
// Eps_Graph_nD::Eps_Graph_nD(int _n, list<Free_Point> _fr_pts, vector<vector<Polytope>> _NonconvexPols, double _eps) {
GridGraph::GridGraph(int _n, list<Free_Point*> _fr_pts, vector<Polytope*> _NonconvexPols, 
	double _eps, string printDir = "") {
	n = _n;
	fr_pts = _fr_pts;
	pols = _NonconvexPols;
	eps = _eps;
	NN_dist = {};
	ord_pol = 0;
	xs_min = std::vector<double>(n, DBL_MAX);
	xs_max = std::vector<double>(n, DBL_MIN);
	xs_num = std::vector<long long int>(n, 0);

	//for (auto NonconvexPol : _NonconvexPols) {
	//	for (auto convexPol : NonconvexPol) {

	computeMinMax();

	// print 부분
	ofstream fout;
	fout.open(printDir + ".txt");
	fout << "started grid initialization" << endl;

	auto start = chrono::high_resolution_clock::now();
	// print 부분

	init_grid(printDir + "initGrid");

	// print 부분
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	auto exe_time = duration.count();

	fout << "finished grid initialized" << endl;
	// fout << "time taken: " << duration << endl;
	fout << "time taken: " << exe_time << " ms, or " << exe_time / 1000 << " s" << endl;

	fout << "started anchoring" << endl;

	start = chrono::high_resolution_clock::now();
	// print 부분

	for (Free_Point* fr_pt : fr_pts) {
		anchor(fr_pt);
	}

	// print 부분
	stop = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	exe_time = duration.count();

	fout << "finished anchoring" << endl;
	// fout << "time taken: " << duration << endl;
	fout << "time taken: " << exe_time << " ms, or " << exe_time / 1000 << " s" << endl;

	fout.close();
	// print 부분

	// cout << this->
}

void GridGraph::computeMinMax() {

	for (Polytope* convexPol : pols) {
		//pol.set_vertices_size();
		convexPol->set_maxmin();
		convexPol->ord = this->ord_pol;
		this->ord_pol++;

		for (int i = 0; i < n; i++) {
			if (convexPol->xs_max[i] > this->xs_max[i])
			{
				this->xs_max[i] = convexPol->xs_max[i];
			}
			if (convexPol->xs_min[i] < this->xs_min[i])
			{
				this->xs_min[i] = convexPol->xs_min[i];
			}
		}

	}
	for (Free_Point* fr_pt : fr_pts) {
		for (int i = 0; i < n; i++) {
			if (fr_pt->xs[i] > this->xs_max[i]) { this->xs_max[i] = fr_pt->xs[i]; }
			if (fr_pt->xs[i] < this->xs_min[i]) { this->xs_min[i] = fr_pt->xs[i]; }
		}
	}
}

//cout << "---------------------" << endl;
//cout << "Polytope " << ord_pol << endl;
//cout << "Number of vertices: " << pol.get_num_point() << endl;
//cout << "Max value: ";
//for (int j = 0; j < n; j++) {
//	cout << pol.xs_max[j] << ' ';
//}
//cout << endl;
//cout << "Min value: ";
//for (int j = 0; j < n; j++) {
//	cout << pol.xs_min[j] << ' ';
//}
//cout << endl;
//cout << "---------------------" << endl;
////cout << "dimension of polytope: " << pol.xs_max.size() << endl;

void GridGraph::init_grid(string printDir = "") {
	for (int i = 0;i < n;i++) {
		xs_num[i] = 1 + long long int(ceil((xs_max[i] - xs_min[i]) / eps));
	}

	upper_left = new Point(xs_min);

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
		grid.push_back(new Grid_Point(num2ind(i), upper_left, eps, xs_num));
	}

	ofstream fout;
	fout.open(printDir + ".txt");
	fout << "started checking encl" << endl;

	auto start = chrono::high_resolution_clock::now();

	// for each grid & free point, count # of crossings of the rightward ray with each polytope
	for (Grid_Point* pt : grid) {
		//	for (Polytope& conexPol : nonconvexPol) {
			for (auto& convexPol : pols) {
				if (convexPol->isIn(pt)) {
					pt->encl = convexPol->ord;
					convexPol->encl_pts.push_back(pt);
				}
			}
	}

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	auto exe_time = duration.count();

	fout << "finished checking encl" << endl;
	// fout << "time taken: " << duration << endl;
	fout << "time taken: " << exe_time << " ms, or " << exe_time / 1000 << " s" << endl;

	fout << "started adding grid edges" << endl;

	// 각 grid vertex, grid[i]에 대해서
	for (long long i = 0; i < tot_num; i++) {
		
		start = chrono::high_resolution_clock::now();

		// 만약 이 grid vertex가 어떤 polytope (장애물) 에 포함돼있다면 넘어감, 
		// 즉 어떤 edge도 추가하지 않음
		if (grid[i]->encl != -1) { 
			continue; 
		}

		// 그게 아니라면 해당 grid vertex를
		std::vector<long long int> inds = num2ind(i);

		for (int j = 0;j < n;j++) {
			if (inds[j] != xs_num[j] - 1) {
				std::vector<long long int> _inds(n);
				std::copy(inds.begin(), inds.end(), _inds.begin());
				_inds[j] += 1;

				// 연결할 (반대편) grid vertex도 장애물에 덮여있지 않다면
				if (grid[ind2num(_inds)]->encl == -1) {
					// edge를 추가
					if (cmpNadd(inds, j)) { add_edge(inds, _inds); }
				}
			}
		}

		stop = chrono::high_resolution_clock::now();
		duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
		exe_time = duration.count();

		fout << "finished adding grid edges for gridpoint #" << i << endl;

		fout << "time:  " << exe_time << " ns = ";
		fout << exe_time / 1000 << " μs = " << exe_time / 1000000 << " ms " << endl;
	}

	fout << endl;
	fout << "finished adding grid edges" << endl;
	// fout << "time taken: " << duration << endl;
	fout << "time taken: " << exe_time << " ms, or " << exe_time / 1000 << " s" << endl;

	fout.close();

}

bool GridGraph::get_step_comb(vector<int>& arr, int index, int sum, int& step, 
	vector<long long int>& xs, Free_Point* p) {

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
			if (grid[ind2num(sums)]->encl == -1) {
				p->host = grid[ind2num(sums)]->num;
				grid[ind2num(sums)]->anchored.push_back(p);
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
			if (grid[ind2num(sums)]->encl == -1) {
				p->host = grid[ind2num(sums)]->num;
				grid[ind2num(sums)]->anchored.push_back(p);
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

void GridGraph::anchor(Free_Point* p) { // cast anchor onto a grid point from a free point
	double verySmallVal = pow(0.1, 10);

	if (p->host != -1) {
		int gs = grid.size();
		assert(0 <= p->host && p->host < gs);
		for (vector<Free_Point*>::iterator it = grid[p->host]->anchored.begin(); 
			it != grid[p->host]->anchored.end();) {

			if (p->xs == (*(*it)).xs) {
				it = grid[p->host]->anchored.erase(it);
				if (it == grid[p->host]->anchored.end()) break;
			}
			else {
				++it;
			}
		}
	}

	bool flag = false;
	vector<long long int> xs(this->n,0);

	// 3.4 eps 만큼 오면, ceil(2.9)만큼이니까 3번째 index로 가개 되는 거고
	// 3.6 eps 만큼이면 ceil(3.1)만큼이니까 4번째 index로 가게 됨
	// 일단 적어도 그렇게 해서 한번 돌려 보자고.
	// upper_left가 
	for (int i = 0;i < this->n;i++) {
		xs[i] = int(ceil((p->xs[i] - this->upper_left->xs[i]) / eps - 0.5));
	}
	if (grid[ind2num(xs)]->encl == -1) {
		p->host = grid[ind2num(xs)]->num;
		grid[ind2num(xs)]->anchored.push_back(p);
		return;
	}

	for (int step = 1;step < this->tot_num;step++) { //From Here
		std::vector<int> xs_step(this->n, 0);
		if (get_step_comb(xs_step, 0, 0, step, xs, p)) {
			return;
		}
	}
}

Grid_Point* GridGraph::get_gridpt(vector<long long int>& ind) {//O
	return grid[ind2num(ind)];
}

long long int GridGraph::ind2num(vector<long long int> ind) {
	long long out = 0;
	long long mul = 1;
	for (int i = n - 1;i >= 0;i--) {
		out += ind[i] * mul;
		mul *= xs_num[i];
	}
	return out;
}

vector<long long int> GridGraph::num2ind(long long int num) {//O
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

void GridGraph::add_edge(vector<long long int>& ind1, vector<long long int>& ind2) { //O
	int i;
	for (i = 0;i < ind1.size();i++) {
		if (ind1[i] != ind2[i]) break;
	}
	grid[ind2num(ind1)]->ip_u[i] = true;
	grid[ind2num(ind2)]->ip_d[i] = true;
}

void GridGraph::delete_edge(vector<long long int>& ind1, vector<long long int>& ind2) { //O
	int i;
	for (i = 0;i < ind1.size();i++) {
		if (ind1[i] != ind2[i]) break;
	}
	grid[ind2num(ind1)]->ip_u[i] = false;
	grid[ind2num(ind2)]->ip_d[i] = false;
}

bool GridGraph::cmpNadd(vector<long long int>& ind, int& direc) {  //O
	// checks if the line connecting the gridpoint and its neighboring one is blocked by any polytope. if is not, add an edge between them.

	Grid_Point* A = grid[ind2num(ind)];

	// 새 grid point를 만들기?
	 // Grid_Point B = Grid_Point(this->n);
	long long t = 1;
	for (int i = 0;i < n - 1 - direc;i++) {
		t *= xs_num[n - 1 - i];
	}
	Grid_Point* B = grid[ind2num(ind) + t];

	for (unsigned int ind1 = 0; ind1 < pols.size(); ind1++) {

		//auto& nonconvexPol = pols[ind1];
		//for (auto& pol : nonconvexPol) {
			auto& pol = pols[ind1];

			// 참조자의 포인터
			if (pol->isIn(A) || pol->isIn(B)) {
				return false;
			}

			ofstream fout;
			fout.open("C:\\Users\\HWI\\Desktop\\finalResult\\pol.txt", ios::app);
			auto time1 = chrono::high_resolution_clock::now();

			// if a polygon edge crosses the line connecting two gridpoints, then the edge between them is not present
			if (pol->intersect(A, B)) { 
				return false; 
			}

			auto time2 = chrono::high_resolution_clock::now();
			auto duration1 = chrono::duration_cast<chrono::nanoseconds>(time2 - time1); 
			auto exe_time1 = duration1.count();

			fout << "time to check intersection: " << exe_time1 << " ns = ";
			fout << exe_time1 / 1000 << " μs = " << exe_time1 / 1000000 << " ms " << endl;

			fout.close();

			// if a polygon edge crosses the line connecting two gridpoints, then the edge between them is not present
			// Point _A = Point(A.xs);
			// Point _B = Point(B.xs);
			// if (pol.intersect(&_A, &_B)) { return false; }
		

	}

	return true;
}

//bool Eps_Graph_nD::cmpNadd(vector<long long int>& ind, int direc) {  //O
//	// checks if the line connecting the gridpoint and its neighboring one is blocked by any polytope. if is not, add an edge between them.
//
//	Grid_Point& A = grid[ind2num(ind)];
//	Grid_Point B = Grid_Point(this->n);
//	long long t = 1;
//	for (int i = 0; i < n - 1 - direc; i++) {
//		t *= xs_num[n - 1 - i];
//	}
//	B = grid[ind2num(ind) + t];
//
//	for (unsigned int ind1 = 0; ind1 < pols.size(); ind1++) {
//
//
//		//auto& nonconvexPol = pols[ind1];
//		//for (auto& pol : nonconvexPol) {
//		auto pol = pols[ind1];
//		if (pol.isIn(&A) || pol.isIn(&B)) {
//			return false;
//		}
//
//		// if a polygon edge crosses the line connecting two gridpoints, then the edge between them is not present
//		Point _A = Point(A.xs);
//		Point _B = Point(B.xs);
//		if (pol.intersect(&_A, &_B)) { return false; }
//
//
//	}
//
//	return true;
//}

bool GridGraph::cmpNadd_SinPol(vector<long long int>& ind, int& direc, int& ord) { // do the same with a specific polygon.

	Polytope* pol;
	vector<Polytope*>::iterator it;

	// for (auto& nonconvexPol : pols) { // pols. 
		// vector<Polytope>::iterator it;
		//for (int i = 0; i < nonconvexPol.size(); i++) {
		//	if (nonconvexPol[i].ord == ord) {
		//		pol = nonconvexPol[i];
		//		break;
		//	}
		//	
		//}

		// for (it = nonconvexPol.end() - 1;; it--) {
		for (it = pols.end() - 1;; it--) {
			if ((*it)->ord == ord) {
				pol = *it;
				break;
			}
		}


	Grid_Point* A = grid[ind2num(ind)];
	// Grid_Point* B = Grid_Point(n);
	long long t = 1;
	for (int i = 0;i < n - 1 - direc;i++) {
		t *= xs_num[n - 1 - i];
	}
	Grid_Point* B = grid[ind2num(ind) + t];
	if (pol->isIn(A) || pol->isIn(B)) {
		return false;
	}

	// Point _A = Point(A.xs);
	// Point _B = Point(B.xs);
	if (pol->intersect(A, B)) { return false; }
	return true;
}

void GridGraph::add_freepts(Free_Point* p) { // add points to the point set P //O
	Free_Point* pt = fr_pts.back();

	for (Polytope* pol : pols) {
	//for (auto& nonconvexPol : pols) {
	//	for (auto& pol : nonconvexPol) {
			assert(!pol->isIn(p)); // Assert error if a freepoint is contained in some polytope
		}

	anchor(p);
	fr_pts.push_back(p);
}

void GridGraph::add_freepts(vector<Free_Point*> p_vec) { // add points to the point set P //O
	for (Free_Point* p : p_vec) {
		for (Polytope* pol : pols) {
		//for (auto& nonconvexPol : pols) {
		//	for (auto& pol : nonconvexPol) {
				assert(!pol->isIn(p)); // Assert error if a freepoint is contained in some polytope
			}

		anchor(p);
		fr_pts.push_back(p);
	}
}

void GridGraph::delete_freept(int& ind) { // delete a point from P, specified by its index //O
	int s = fr_pts.size();
	if (ind < 0 || s - 1 < ind) { return; }

	list<Free_Point*>::iterator iter = fr_pts.begin();
	advance(iter, ind);
	Free_Point* p = *iter;
	for (vector<Free_Point*>::iterator it = grid[p->host]->anchored.begin(); 
		it != grid[p->host]->anchored.end(); ++it) {

		if ((*(*it)).xs == p->xs) {
			grid[p->host]->anchored.erase(it);

			break;
		}
	}

	fr_pts.erase(remove(fr_pts.begin(), fr_pts.end(), p));
}

void GridGraph::query_anchor(Grid_Point* g) {
	for (vector<Free_Point*>::iterator it = g->anchored.begin(); it != g->anchored.end();) {
		Free_Point* p = *it;
		anchor(p);

		it = g->anchored.erase(it);
		if (it == g->anchored.end()) break;
	}
	return;
}

std::vector<vector<long long int>> GridGraph::eff_region(Polytope* P) { // returns a range indicating orthogonal rectangle bounding the polygon (effective region)
	static std::vector<vector<long long int>> ret(2, vector<long long int>(n, 0)); // ret[0] : minimum index containing polytope , ret[1] : maximum index containing polytope


	for (int i = 0;i < n;i++) {
		ret[0][i] = max((long long int) 0, long long int(floor((P->xs_min[i] - xs_min[i]) / eps)));
		ret[1][i] = min(xs_num[i], long long int(ceil((P->xs_max[i] - xs_min[i]) / eps)));
	}

	return ret;
}

void GridGraph::add_pol(Polytope* P) { // add a polygon to the set of obstacles 
	P->ord = ord_pol;

	for (Free_Point* pt : fr_pts)
	{
		bool in = P->isIn(pt);
		assert(!in); // check if the polygon contains a free point
	}
	for (Grid_Point* gr_pt : grid) {
		bool in = P->isIn(gr_pt);
		if (in) {
			assert(gr_pt->encl == -1);
			gr_pt->encl = P->ord;
			query_anchor(gr_pt);
			vector<Free_Point*>().swap(gr_pt->anchored);
		}
	}

	pols.push_back(P);

	//vector<Polytope> nonconvexPol = {P};
	//pols.push_back(nonconvexPol);
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
		Grid_Point* cur = grid[ind2num(eff)];
		for (int j = 0;j < n;j++) {
			if (eff[j] != xs_num[j] - 1) {
				if (!cmpNadd_SinPol(eff, j, P->ord)) {
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

// delete a polygon from O, specified by its index
void GridGraph::delete_pol(int ord) { 

	//bool check = true;
	//Polytope* P;


	////for (auto& nonconvexPol: pols) {
	////	for (Polytope& p : nonconvexPol) {

	//for (auto& p : pols) {
	//		if (p->ord == ord) {
	//			P = p;
	//			check = false;
	//		}
	//}

	//if (check) { return; }

	//// release them free; for gridpoints that was enclosed by the polygon
	//for (Grid_Point* gr_pt : grid) {
	//	if (gr_pt->encl == ord) {
	//		gr_pt->encl = -1;
	//	}
	//}

	//vector<vector<long long int>> diagonal = eff_region(P);
	//long long int tot_d = 1;
	//for (int i = 0;i < n;i++) {
	//	tot_d *= (diagonal[1][i] - diagonal[0][i]);
	//}
	//pols.erase(remove(pols.begin(), pols.end(), P));

	//for (long long i = 0;i < tot_d;i++) {
	//	vector<long long int> eff(n, 0);
	//	for (int j = 0;j < n;j++) {
	//		eff[j] = j % (diagonal[1][i] - diagonal[0][i]);
	//		j = j / (diagonal[1][i] - diagonal[0][i]);
	//	}
	//	Grid_Point* cur = grid[ind2num(eff)];
	//	for (int j = 0;j < n;j++) {
	//		if (eff[j] != xs_num[j] - 1) {
	//			if (!cmpNadd_SinPol(eff, j, P->ord)) {
	//				vector<long long int> _eff(n, 0);
	//				for (int k = 0;k < n;k++) {
	//					if (j != k) _eff[k] = eff[k];
	//					else _eff[k] = eff[k] + 1;
	//				}
	//				add_edge(eff, _eff);
	//			}
	//		}
	//	}
	//}
}


GridGraph::GridGraph(int n) { xs_num = vector<long long int>(n, 0); eps = 0; xs_min = vector<double>(n, 0), xs_max = vector<double>(n, 0); }


list<Free_Point*> GridGraph::get_free_points()
{
	return fr_pts;
}

Free_Point* GridGraph::get_free_point(int& index) {
	list<Free_Point*>::iterator iter = fr_pts.begin();
	std::advance(iter, index);
	return *iter;
}

//vector<vector<Polytope>> Eps_Graph_nD::get_Polytope() {
//
//	return pols;
//}

vector<Polytope*> GridGraph::get_Polytopes() {

	return pols;
}

vector<Grid_Point*> GridGraph::get_grid() {
	return grid;
}


vector<edge*> GridGraph::get_path(Free_Point* p, int k) {
	vector<edge*> path = path_kNN(p, k + 1);
	return path;
}

// free point에 index를 줘야할 듯?
pair<vector<Free_Point*>, vector<double>> GridGraph::kNN(Free_Point* p, int k, string dir = "") { // returns k approximate nearest neighbors of p
	
	ofstream fout;
	fout.open(dir, ios::app);

	auto time1 = chrono::high_resolution_clock::now();

	for (int i = 0; i < visited.size(); i++) visited[i] = false;
	// visited[] for (auto& c : this->visited) c = false;

	vector<double> finalDistance;

	for (Polytope* pol : pols) {
	//for (auto& nonconvexPol : pols) {
	//	for (auto& pol : nonconvexPol) {
			assert(!pol->isIn(p));
	}

	vector<Free_Point*> ret = {};
	vector<int>().swap(NN_dist);

	auto time2 = chrono::high_resolution_clock::now();

	anchor(p);

	auto time3 = chrono::high_resolution_clock::now();

	Grid_Point* s = grid[p->host];
	for (int& elem : dist) { elem = INT_MAX; }
	//for (long long int i = 0; i < visited.size(); i++) { visited[i] = false; }
	//for (bool elem : visited) { elem = false; }

	queue<int> q = {};

	dist[s->num] = 0;
	q.push(s->num);
	int grid_dist = 0;
	vector<Free_Point*> FPs_temp = {};
	vector<Free_Point*> FPs = {};

	int rememberQ = -1;

	if (k == 500) {
		int _ = 0;
	}

	auto time4 = chrono::high_resolution_clock::now();
	// std::chrono::steady_clock time5, time6, time7;
	// std::chrono::high_resolution_clock time5, time6, time7;
	std::chrono::time_point<std::chrono::high_resolution_clock> time5, time6, time7;

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
				if (grid[cur]->ip_u[n - 1 - i] && visited[cur + mult] == false) 
					{ dist[cur + mult] = dist[cur] + 1; q.push(cur + mult); }
				if (grid[cur]->ip_d[n - 1 - i] && visited[cur - mult] == false) 
					{ dist[cur - mult] = dist[cur] + 1; q.push(cur - mult); }
				mult *= xs_num[n - i - 1];
			}

			for (auto FP : grid[q.front()]->anchored) {
				FPs_temp.push_back(FP);
			}
			rememberQ = q.front();
			q.pop();
		}

		for (auto pt : FPs_temp) {
			FPs.push_back(pt);
		}

		//sort(FPs.begin(), FPs.end(), [=](Free_Point first, Free_Point second)
		//	{
		//		double fx = 0;
		//		double fy = 0;
		//		for (int i = 0;i < n;i++) {
		//			fx += pow(first.xs[i] - p.xs[i], 2);
		//			fy += pow(second.xs[i] - p.xs[i], 2);
		//		}
		//		return fx < fy;
		//	});

		time5 = chrono::high_resolution_clock::now();

		sort(FPs.begin(), FPs.end(), [=](Free_Point* first, Free_Point* second)
		{

			return distanceBtwFreePoints(p, first) < distanceBtwFreePoints(p, second);
			//if (grid_dist == 0)
			//	return distanceBtwFreePoints(p, first) < distanceBtwFreePoints(p, second);
			//else
			//	return distanceBtwGPandFP(grid[rememberQ], first) < distanceBtwGPandFP(grid[rememberQ], second);
		});

		time6 = chrono::high_resolution_clock::now();

		int sz = int(FPs.size());

		if (sz < k) {
			for (int ind2 = 0; ind2 < sz; ind2++) { 
				NN_dist.push_back(grid_dist); 

				//if (grid_dist == 0) 
				//	finalDistance.push_back(distanceBtwFreePoints(p, FPs[ind2]));
				//else 
				//	finalDistance.push_back(double(grid_dist) * eps + distanceBtwGPandFP(grid[rememberQ], FPs[ind2]));

				// finalDistance.push_back(double(grid_dist) * eps + distanceBtwGPandFP(get_gridpt(num2ind(FPs[ind2].host)), FPs[ind2]));
				finalDistance.push_back(distanceBtwFreePoints(p, FPs[ind2]));
			}
			
			k -= sz;
			ret.insert(ret.end(), FPs.begin(), FPs.end());
		}
		else {
			for (int ind2 = 0; ind2 < k; ind2++) { 
				NN_dist.push_back(grid_dist); 

				//if (grid_dist == 0)
				//	finalDistance.push_back(distanceBtwFreePoints(p, FPs[ind2]));
				//else
				//	finalDistance.push_back(double(grid_dist)* eps + distanceBtwGPandFP(grid[rememberQ], FPs[ind2]));

				// finalDistance.push_back(double(grid_dist) * eps + distanceBtwFreePoints(p, FPs[ind2]));
				finalDistance.push_back(distanceBtwFreePoints(p, FPs[ind2]));
			}
			ret.insert(ret.end(), FPs.begin(), FPs.begin() + k);
			k = 0;
		}

		time7 = chrono::high_resolution_clock::now();

		vector<Free_Point*>().swap(FPs_temp);
		vector<Free_Point*>().swap(FPs);
		grid_dist += 1;
	}


	for (vector<Free_Point*>::iterator it = 
		grid[p->host]->anchored.begin(); it != grid[p->host]->anchored.end(); ++it) {

		if ((*(*it)).xs == p->xs) {
			grid[p->host]->anchored.erase(it);
			break;
		}
	}

	auto time8 = chrono::high_resolution_clock::now();

	auto duration1 = chrono::duration_cast<chrono::milliseconds>(time2 - time1); auto exe_time1 = duration1.count(); 
	auto duration2 = chrono::duration_cast<chrono::milliseconds>(time3 - time2); auto exe_time2 = duration2.count();
	auto duration3 = chrono::duration_cast<chrono::milliseconds>(time4 - time3); auto exe_time3 = duration3.count();
	auto duration4 = chrono::duration_cast<chrono::milliseconds>(time5 - time4); auto exe_time4 = duration4.count();
	auto duration5 = chrono::duration_cast<chrono::milliseconds>(time6 - time5); auto exe_time5 = duration5.count();
	auto duration6 = chrono::duration_cast<chrono::milliseconds>(time7 - time6); auto exe_time6 = duration6.count();
	auto duration7 = chrono::duration_cast<chrono::milliseconds>(time8 - time7); auto exe_time7 = duration7.count();

	fout << exe_time1 << endl;
	fout << exe_time2 << endl;
	fout << exe_time3 << endl;
	fout << exe_time4 << endl;
	fout << exe_time5 << endl;
	fout << exe_time6 << endl;
	fout << exe_time7 << endl;
	fout << endl;

	fout.close();

	// pair<vector<Free_Point*>, vector<double>>
	return make_pair(ret, finalDistance);
}

// vector<edge> Eps_Graph_nD::path_kNN(Free_Point p, int k){}

vector<edge*> GridGraph::path_kNN(Free_Point* p, int k) { // returns k approximate nearest neighbors of p

	//for (auto& nonconvexPol : pols) {
	//	for (auto& pol : nonconvexPol) {
	//		assert(!pol.isIn(&p));
	//	}
	//}

	for (Polytope* pol : pols) {
		assert(!pol->isIn(p));
	}

	vector<edge*> path = {};
	vector<int>().swap(NN_dist);
	long long int* previous = new long long int[tot_num];
	anchor(p);
	Grid_Point* s = grid[p->host];
	for (int& elem : dist) { elem = INT_MAX; }
	for (unsigned int ind1 = 0; ind1 < visited.size(); ind1++) { visited[ind1] = false; }
	for (long long int i = 0; i < tot_num; i++) { previous[i] = i; }

	queue<int> q = {};

	dist[s->num] = 0;
	q.push(s->num);
	int grid_dist = 0;
	vector<Free_Point*> FPs_temp = {};
	vector<Free_Point*> FPs = {};

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
				if (grid[cur]->ip_u[n - 1 - i] && visited[cur + mult] == false) 
					{ dist[cur + mult] = dist[cur] + 1; q.push(cur + mult); }
				if (grid[cur]->ip_d[n - 1 - i] && visited[cur - mult] == false) 
					{ dist[cur - mult] = dist[cur] + 1; q.push(cur - mult); }
				mult *= xs_num[n - i - i];
			}

			bool once = true;
			for (auto& FP : grid[q.front()]->anchored) {
				FPs_temp.push_back(FP);

				if (once) {
					while (previous[cur] != cur) {
						path.push_back(new edge{ grid[cur], grid[previous[cur]] });
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
					fx += pow(first.xs[i] - p->xs[i], 2);
					fy += pow(second.xs[i] - p->xs[i], 2);
				}
				return fx < fy;
			});

		int sz = int(FPs.size());
		for (int i = 0; i < min(k, sz); i++) {
			path.push_back(new edge{ FPs[i], grid[FPs[i]->host] });
			NN_dist.push_back(grid_dist);
		}
		k -= sz;

		vector<Free_Point*>().swap(FPs_temp);
		vector<Free_Point*>().swap(FPs);
		grid_dist += 1;
	}
	for (vector<Free_Point*>::iterator it = grid[p->host]->anchored.begin(); 
		it != grid[p->host]->anchored.end(); ++it) {

		if ((*(*it)).xs == p->xs) {
			grid[p->host]->anchored.erase(it);
			break;
		}
	}

	return path;
}

void GridGraph::print_grid() {
	for (unsigned int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i]->getsize(); j++) {
			cout << grid[i]->ind[j];
		}
		cout << ' ' << '|';
		for (int j = 0; j < grid[i]->ip_u.size(); j++) {
			cout << grid[i]->ip_u[j] << ' ';
		}
		cout << '|';
		// if (num2ind(i)[grid[i]->getsize() - 1] == this->xs_num[n - 1] - 1) { cout << endl; }
	}
}

void GridGraph::print_free_point() {
	for (Free_Point* fr : fr_pts) {

		for (int i = 0; i < n; i++) {
			cout << fr->getx(i) << ' ';
		}

		cout << endl;

	}
}

void GridGraph::print_kNN(Free_Point* p, int k) {
	cout << "-------------------Print KNN-------------------" << endl;
	// vector<Free_Point> nbhd = kNN(p, k + 1);
	// int kCopy = k;

	pair<vector<Free_Point*>, vector<double>> pr = kNN(p, k + 1);

	int prSize = pr.first.size();
	int curRank = 1;

	cout << "Query Point: "; p->print();
	cout << endl;
	cout << "k: " << k << endl;

	for (int i = 0; i < prSize; i++) {
		auto& nb = pr.first[i];
		auto& nbDist = pr.second[i];

		// query point는 제외
		bool cond = (nb->getxs() == p->getxs());
		if (cond) continue;

		cout << "Rank " << curRank << ", "; // ': ';
		cout << "#" << nb->id << ", ";
		nb->print();
		cout << endl;
		curRank++;
	}

	//for (auto nb : nbhd) {
	//	bool cond = (nb.getxs() == p.getxs());
	//	if (cond) continue;
	//	for (int j = 0; j < this->n; j++) {
	//		cout << nb.getx(j) << ' ';
	//	}
	//	cout << endl;
	//}
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

vector<pair<Point*, double>>* GridGraph::Visibility_polygon(Free_Point* qry) {
	
	// visibility polygon vertex
	vector<Point*> vp_vertex;

	vector<pair<Point*, double>>* nb_list;
	// qry->is_Free_Point = true;
	// Point* temp_qry(qry.getxs());
	// vp_vertex.push_back(temp_qry);
	vp_vertex.push_back(qry);

	for (Free_Point* f_p : fr_pts) {
		// Point* temp(f_p.getxs());
		// temp.is_Free_Point = true;
		// vp_vertex.push_back(temp);
		vp_vertex.push_back(f_p);
	}

	// query point와 free point들을 다 input으로 넣기

	for (auto pol : pols) {
	//for (auto nonconvexPol : pols) {
	//	for (auto pol : nonconvexPol) {
			vector<Point*> temp = pol->get_vertices();
			for (Point* temp_temp : temp) {
				temp_temp->is_Free_Point = false;
				vp_vertex.push_back(temp_temp);
			}
		
	}

	nb_list = new vector<pair<Point*, double>>[vp_vertex.size()];

	for (int i = 0; i < vp_vertex.size(); i++) {
		for (int j = i + 1; j < vp_vertex.size(); j++) {
			bool intersect = false;

			for (Polytope* pol : pols) {
			//for (auto nonconvexPol : pols) {
			//	for (auto pol : nonconvexPol) {
					if (pol->sameIn(vp_vertex[i], vp_vertex[j])) {
						intersect = true;
						vector<double> mid;
						for (int k = 0; k< vp_vertex[i]->getsize(); k++) {
							mid.push_back((vp_vertex[i]->getx(k) + vp_vertex[j]->getx(k)) / 2);
						}
						for (int k = 0; k < vp_vertex[i]->getsize() * 5; k++) {
							random_device rd;
							mt19937 gen(rd());
							vector<double> random_vec;
							uniform_real_distribution<float> dis(0, 1);
							for (int l = 0; l< vp_vertex[i]->getsize(); l++) {
								random_vec.push_back(mid[l] + dis(gen)/100);
							}
							Point random_pt(random_vec);
							if (!pol->isIn(&random_pt)) {
								intersect = false;
							}
						}
					}
					else {
						if (pol->intersect(vp_vertex[i], vp_vertex[j])) {
							intersect = true;
							break;
						}
					}
				
			}
			if (!intersect) {
				nb_list[i].push_back(make_pair(vp_vertex[j], vp_vertex[i]->distance(vp_vertex[j])));
				nb_list[j].push_back(make_pair(vp_vertex[i], vp_vertex[i]->distance(vp_vertex[j])));
			}
		}
	}
	return nb_list;
}

vector<pair<Point*, double>> GridGraph::Dijkstra(Free_Point* qry, int& _num) {
	cout << "-------------------Print Dijkstra-------------------" << endl;
	vector<Point*> vp_vertex;
	int check_num = 0;
	//vector<pair<Point, double>>* nb_list;

	// Free_Point temp_qry(qry.getxs());
	// temp_qry.is_Free_Point = true;
	// vp_vertex.push_back(temp_qry);
	vp_vertex.push_back(qry);

	for (Free_Point* f_p : fr_pts) {
		// Free_Point temp(f_p.getxs());
		// temp.is_Free_Point = true;
		vp_vertex.push_back(f_p);
	}
	
	for (Polytope* pol : pols) {
	//for (auto nonconvexPol : pols) {
	//	for (auto pol : nonconvexPol) {
			vector<Point*> temp = pol->get_vertices();
			for (Point* temp_temp : temp) {
				temp_temp->is_Free_Point = false;
				vp_vertex.push_back(temp_temp);
			}
		
	}


	//vector<vector<pair<double, int>>> nb_list = Visibility_polygon2(qry);
	vector<pair<Point*, double>>* nb_list2 = Visibility_polygon(qry);
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
		vector<pair<Point*, double>> nb = nb_list2[q1.second];
		if (vp_vertex[q1.second]->is_Free_Point) {
			bool cond = (qry->getxs() == vp_vertex[q1.second]->getxs());
			if (!cond) {
				for (int j = 0; j < this->n; j++) {
					cout << vp_vertex[q1.second]->getx(j) << ' ';
				}
				cout << endl;
				check_num++;
				if (check_num >= _num) break;
			}
		}
		for (int j=0;j < nb.size();j++) {
			pair<Point, double> q = nb[j];
			for (int i=0;i < vp_vertex.size();i++) {
				if (q.first.getxs() == vp_vertex[i]->getxs()) {
					if (visited[i] == false) {
						pq.push({ q.second + q1.first, i });
					}
					break;
				}
			}
		}
	}

	vector<pair<Point*, double>> output;
	for (int i = 0;i < vp_vertex.size();i++) {
		output.push_back({ vp_vertex[i], dist[i] });
	}
	return output;

}



void GridGraph::checkMemory(string dir) {

	ofstream fout;
	fout.open(dir + "\\" + "checkMemory.txt");

	double axisDifference = this->xs_max[0] - this->xs_min[0];
	fout << "size of bounding box: " << axisDifference << endl;
	fout << "epsilon: " << this->eps << endl;
	int numPerAxis = ceil(axisDifference / this->eps) + 1;
	fout << "number of grid lines per each axis (numPerAxis) : " << numPerAxis << endl;
	fout << "(numPerAxis)^(dim): " << pow(numPerAxis, this->n) << endl;
	fout << endl;

	fout << "Base" << endl;

	fout << "sizeof(Grid_Point) = " << sizeof(Grid_Point) << " byte" << endl;
	fout << "sizeof(Free_Point) = " << sizeof(Free_Point) << " byte" << endl;
	fout << "sizeof(Polytope) = " << sizeof(Polytope) << " byte" << endl;
	
	fout << endl;

	fout << "Result" << endl;

	int sizeGrid = sizeof(Grid_Point) * (this->grid).size();
	fout << "type: vector<Grid_Point>. sizeof(Eps_Graph_nD.grid) = "
		<< sizeGrid
		// sizeof(this->grid) 
		<< " byte" << " = " << double(sizeGrid) / 1024 << " KB = "
		<< double(sizeGrid) / 1024 / 1024 << " MB " << endl;
		
	fout << "size of this->grid: " << (this->grid).size() << endl;

	int sizeFreePoints = sizeof(Free_Point) * (this->fr_pts).size();
	fout << "type: list<Free_Point>. sizeof(Eps_Graph_nD.fr_pts) = "
		<< sizeFreePoints
		// sizeof(this->fr_pts) 
		<< " byte" << " = " << double(sizeFreePoints) / 1024 << " MB = "
		<< double(sizeFreePoints) / 1024 / 1024 << " MB " << endl;
	fout << "size of this->fr_pts: " << (this->fr_pts).size() << endl;
	fout << endl;

	vector<int> numDistribution;
	numDistribution.assign(50, 0);

	// vector<int> numFPsPerGP;

	for (int i = 0; i < this->grid.size(); i++) {
		auto& gridP = grid[i]; // numFPsPerGP[i];
		int numAnchored = gridP->anchored.size();
		if (numAnchored >= 50) continue;
		numDistribution[numAnchored]++;
	}

	for (int i = 0; i < numDistribution.size(); i++) {
		if (i % 10 == 0) fout << endl;
		fout << i << " pts - " << numDistribution[i] << " ||| ";
		// fout << "number of gridpoints with anchored.size() " << i << ": " << numDistribution[i];
	}

	fout << endl;

	fout.close();
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


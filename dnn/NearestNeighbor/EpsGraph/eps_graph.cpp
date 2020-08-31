#include "Eps_Graph.h"
#include <queue>

#define X true // ray direction
#define Y false

using namespace std;

Eps_Graph::Eps_Graph(list<Free_Point> _fr_pts, vector<Polygon> _pols, double _eps) {
	
	fr_pts = _fr_pts;
	pols = _pols;
	eps = _eps;

	x_min = y_min = DBL_MAX;
	x_max = y_max = DBL_MIN;

	for (auto pol : pols) {
		if (pol.x_max > this->x_max) { this->x_max = pol.x_max; }
		if (pol.y_max > this->y_max) { this->y_max = pol.y_max; }

		if (pol.x_min < this->x_min) { this->x_min = pol.x_min; }
		if (pol.y_min < this->y_min) { this->y_min = pol.y_min; }
	}

	for (auto fr_pt : fr_pts) {
		if (fr_pt.x > this->x_max) { this->x_max = fr_pt.x; }
		if (fr_pt.y > this->y_max) { this->y_max = fr_pt.y; }

		if (fr_pt.x < this->x_min) { this->x_min = fr_pt.x; }
		if (fr_pt.y < this->y_min) { this->y_min = fr_pt.y; }
	}

	init_grid();
	
	for (Free_Point& fr_pt : fr_pts) {
		anchor(fr_pt);
	}
}

void Eps_Graph::init_grid() {
	
	int x_ind = int(floor(x_min / eps)) - 1, y_ind = int(floor(y_min / eps)) - 1;

	row_num = 2 + int(ceil(y_max / eps)) - y_ind;
	col_num = 2 + int(ceil(x_max / eps)) - x_ind;

	upper_left = Point(x_ind * eps, (row_num + y_ind - 1) * eps);
	
	for (int i = 0; i < row_num * col_num; i++) {
		dist.push_back(INT_MAX);
		visited.push_back(false);
	}
	
	for (int i = 0; i < row_num * col_num; i++) 
	{ grid.push_back(Grid_Point(num2ind(i).row, num2ind(i).column, upper_left.x, upper_left.y, eps, col_num)); }

	for (Grid_Point& pt : grid) {
		for (Polygon& pol : pols) {
			int cro = pol.ray(pt);
			pt.cros.push_back(cro);

			if (cro > 0 && cro % 2 == 1) { 
				pt.encl = pol.ord;
				pol.encl_pts.push_back(pt);
			}
		}
	}

	for (Free_Point& pt : fr_pts) {
		for (Polygon& pol : pols) {
			int cro = pol.ray(pt);

			if (cro > 0 && cro % 2 == 1) {
				pt.encl = pol.ord;
				pol.encl_pts.push_back(pt);
			}
		}
	}

	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (grid[ind2num(i, j)].encl != -1) { continue; }

			if ((i != row_num - 1) && (j != col_num - 1)) {
				if (grid[ind2num(i+1, j)].encl == -1) { 
					if (cmpNadd(indices{ i, j }, Y)) { add_edge(indices{ i, j }, indices{ i + 1, j }); }
				}
				// 이미 add_edge 함수에서 양쪽 directed edge를 건드리므로, 여기서는 한 방향만 보면 됨.
				if (grid[ind2num(i, j+1)].encl == -1) { 
					if (cmpNadd(indices{ i, j }, X)) { add_edge(indices{ i, j }, indices{ i, j + 1 }); }
				}
			}
			else if (i != row_num - 1) {
				if (grid[ind2num(i + 1, j)].encl == -1) { 
					if (cmpNadd(indices{ i, j }, Y)) { add_edge(indices{ i, j }, indices{ i + 1, j }); }
				}
			}
			else if (j != col_num - 1) {
				if (grid[ind2num(i, j + 1)].encl == -1) { 
					if (cmpNadd(indices{ i, j }, X)) { add_edge(indices{ i, j }, indices{ i, j + 1 }); }
				}
			}
			else {}
		}
	}
}

Grid_Point Eps_Graph::get_gridpt(indices ind) {
	return grid[ind2num(ind)];
}


int Eps_Graph::ind2num(indices ind) {
	return ind.row * col_num + ind.column;
}

int Eps_Graph::ind2num(int row, int column) {
	return row * col_num + column;
}

indices Eps_Graph::num2ind(int num) {
	return indices{ num / col_num, num % col_num };
}


void Eps_Graph::add_edge(indices ind1, indices ind2) {
	int row1 = ind1.row; int column1 = ind1.column;
	int row2 = ind2.row; int column2 = ind2.column;

	if (row1 == row2) { grid[row1 * col_num + column1].ip.right = true; grid[row2 * col_num + column2].ip.left = true; }
	else { grid[row1 * col_num + column1].ip.lower = true; grid[row2 * col_num + column2].ip.upper = true; }
}

void Eps_Graph::add_edge(int row, int column, bool direc) {
	int row1 = row; int column1 = column;
	int row2, column2;

	if (direc == X) { row2 = row1; column2 = column1 + 1; }
	else { row2 = row1 + 1; column2 = column1; }
	
	if (row1 == row2) { grid[row1 * col_num + column1].ip.right = true; grid[row2 * col_num + column2].ip.left = true; }
	else { grid[row1 * col_num + column1].ip.lower = true; grid[row2 * col_num + column2].ip.upper = true; }
}

void Eps_Graph::delete_edge(indices ind1, indices ind2) {
	int row1 = ind1.row; int column1 = ind1.column;
	int row2 = ind2.row; int column2 = ind2.column;

	if (row1 == row2) { grid[row1 * col_num + column1].ip.right = false; grid[row2 * col_num + column2].ip.left = false; }
	else { grid[row1 * col_num + column1].ip.lower = false; grid[row2 * col_num + column2].ip.upper = false; }
}



bool Eps_Graph::cmpNadd(indices ind, bool direc) {

	Grid_Point A = grid[ind2num(ind)], B;
	if (direc == X) { B = grid[ind2num(ind) + 1]; }
	else { B = grid[ind2num(ind) + col_num]; }

	for (unsigned int ind1 = 0; ind1 < pols.size(); ind1++) { // pols.size() = A.cros.size() = B.cros.size()
		auto pol = pols[ind1];
		if (direc == X) {
			if (B.x < pol.x_max || pol.x_min < A.x || pol.y_max < A.y || A.y < pol.y_min) { continue; } // A.y = B.y
		}
		else {
			if (A.y < pol.y_max || pol.y_min < B.y || pol.x_max < A.x || A.x < pol.x_min) { continue; }
		}

		if (direc == X) { 
			if (pol.intersect(Point{A.x, A.y}, Point{B.x, B.y}, X)) { return false; } 
		}
		else {
			if (pol.intersect(Point{A.x, A.y}, Point{B.x, B.y}, Y)) { return false; }
		}

		if (A.cros[ind1] < 0 && B.cros[ind1] < 0) { 
			int mid_inter = pol.ray(Point((A.x + B.x / 2), (A.y, B.y) / 2));
			if (mid_inter % 2 == 1) { return false; }
		}
		else if (A.cros[ind1] < 0) {
			if (B.cros[ind1] % 2 == 1) { return false; }
		}
		else if (B.cros[ind1] < 0) {
			if (A.cros[ind1] % 2 == 1) { return false; }
		}
		else {
			if (A.cros[ind1] != B.cros[ind1]) { return false; }
			else {}
		}
	}
	
	return true;
}

bool Eps_Graph::cmpNadd_SinPol(indices ind, bool direc, int ord) {
	
	Polygon pol;
	vector<Polygon>::iterator it;

	for (it = pols.end() - 1;; it--) {
		if ((*it).ord == ord) {
			pol = *it;
			break;
		}
	}

	int index = distance(pols.begin(), it);

	Grid_Point A = grid[ind2num(ind)], B;
	if (direc == X) { B = grid[ind2num(ind) + 1]; }
	else { B = grid[ind2num(ind) + col_num]; }

	if (direc == X) {
		if (B.x < pol.x_max || pol.x_min < A.x || pol.y_max < A.y || A.y < pol.y_min) { return true; } // A.y = B.y
	}
	else {
		if (A.y < pol.y_max || pol.y_min < B.y || pol.x_max < A.x || A.x < pol.x_min) { return true; }
	}

	if (direc == X) {
		if (pol.intersect(Point{ A.x, A.y }, Point{ B.x, B.y }, X)) { return false; }
	}
	else {
		if (pol.intersect(Point{ A.x, A.y }, Point{ B.x, B.y }, Y)) { return false; }
	}

	int A_cro = A.cros[index], B_cro = B.cros[index];

	if (A_cro < 0 && B_cro < 0) {
		int mid_inter = pol.ray(Point((A.x + B.x / 2), (A.y, B.y) / 2));
		if (mid_inter % 2 == 1) { return false; }
	}
	else if (A_cro < 0) {
		if (B_cro % 2 == 1) { return false; }
	}
	else if (B_cro < 0) {
		if (A_cro % 2 == 1) { return false; }
	}
	else {
		if (A_cro != B_cro) { return false; }
	}

	return true;
}


void Eps_Graph::add_freepts(vector<Free_Point> p_vec) {
	for (auto p : p_vec) {
		fr_pts.push_back(p);

		Free_Point& pt = fr_pts.back();
		anchor(pt);

		for (Polygon& pol : pols) {
			int cro = pol.ray(pt);

			if (cro > 0 && cro % 2 == 1) {
				pt.encl = pol.ord;
				pol.encl_pts.push_back(pt);
			}
		}
	}
}

void Eps_Graph::delete_freept(int ind) {
	list<Free_Point>::iterator iter = fr_pts.begin();
	advance(iter, ind);
	Free_Point& p = *iter;

	for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
		if ((*(*it)).x == p.x && (*(*it)).y == p.y) {
			grid[p.host].anchored.erase(it);
			break;
		}
	}

	for (list<Free_Point>::iterator it = fr_pts.begin(); it != fr_pts.end(); ++it) {
		if ((*it).x == p.x && (*it).y == p.y) {
			fr_pts.erase(it);
			break;
		}
	}

}

void Eps_Graph::anchor(Free_Point& p) {

	if (p.encl != -1) { 
		p.host = -1; 
		return; 
	}
	// else

	if (p.host != -1) {
		vector<Free_Point*> vec = grid[p.host].anchored;
		for (vector<Free_Point*>::iterator it = vec.begin(); it != vec.end(); ++it) {
			if (p.x == (*(*it)).x && p.y == (*(*it)).y) {
				grid[p.host].anchored.erase(grid[p.host].anchored.begin() + distance(it, vec.begin()));
			}
		}
	}

	bool flag = false;
	int row; int col;
	row = int(ceil((upper_left.y - p.y) / eps - 0.5)); // 정 중앙에 있는 점은 위로 anchor됨
	col = int(ceil((p.x - upper_left.x) / eps - 0.5)); // 정 중앙에 있는 점은 왼쪽으로 anchor됨

	for (int step = 0; step < row_num + col_num; step++) {
		vector<Grid_Point> gr_pts = {};
		for (int x_step = 0; x_step <= step; x_step++) {
			int y_step = step - x_step;

			if (0 <= row + x_step && row + x_step < row_num && 0 <= col + y_step && col + y_step < col_num) {
				gr_pts.push_back(grid[ind2num(row + x_step, col + y_step)]); 
			}
			if (0 <= row + x_step && row + x_step < row_num && 0 <= col - y_step && col - y_step < col_num) {
				gr_pts.push_back(grid[ind2num(row + x_step, col - y_step)]);
			}
			if (0 <= row - x_step && row - x_step < row_num && 0 <= col + y_step && col + y_step < col_num) {
				gr_pts.push_back(grid[ind2num(row - x_step, col + y_step)]);
			}
			if (0 <= row - x_step && row - x_step < row_num && 0 <= col - y_step && col - y_step < col_num) {
				gr_pts.push_back(grid[ind2num(row - x_step, col - y_step)]);
			}
		}
		
		for (Grid_Point& gr_pt : gr_pts) {
		
		if (gr_pt.encl == -1) {
			p.host = gr_pt.num;
			Grid_Point& cur = grid[gr_pt.num];
			cur.anchored.push_back(&p);
			flag = true;
			break;
		}

		}
		if (flag) { break; }
	}
}

Grid_Point Eps_Graph::query_anchor(Free_Point p) {
	int row; int col;
	row = int(ceil((upper_left.y - p.y) / eps - 0.5)); // 정 중앙에 있는 점은 위로 anchor됨
	col = int(ceil((p.x - upper_left.x) / eps - 0.5)); // 정 중앙에 있는 점은 왼쪽으로 anchor됨
	return grid[ind2num(indices{ row, col })];
}


void Eps_Graph::add_pol(Polygon P) {
	pols.push_back(P);

	for (Grid_Point& gr_pt : grid) {
		int cro = P.ray(gr_pt);
		gr_pt.cros.push_back(cro);
		if (cro > 0 && cro % 2 == 1) { // assert(!gr_pt.encl)
			gr_pt.encl = P.ord;
		}
	}

	for (Free_Point& pt : fr_pts) {
		if (pt.encl == -1) {
			int cro = P.ray(pt);
			if (cro > 0 && cro % 2 == 1) { // assert(!gr_pt.encl)
				pt.encl = P.ord;
			}
		}
	}

	indices* diagonal = eff_region(P);

	int tm_row = diagonal[1].row;
	int bm_row = diagonal[0].row;

	int lm_col = diagonal[0].column;
	int rm_col = diagonal[1].column;

	std::vector<int> rows, cols;

	for (int i = tm_row; i < bm_row; i++) {
		for (int j = lm_col; j < rm_col; j++) {
			
			Grid_Point cur = grid[ind2num(i, j)];
			if (cur.encl != -1) { continue; }

			if ((i != row_num - 1) && (j != col_num - 1)) {
				if (cur.ip.lower && grid[ind2num(i + 1, j)].encl == -1) {
					if (!cmpNadd_SinPol(indices{ i, j }, Y, P.ord)) { delete_edge(indices{ i, j }, indices{ i + 1, j }); }
				}
				if (cur.ip.right && grid[ind2num(i, j + 1)].encl == -1) {
					if (!cmpNadd_SinPol(indices{ i, j }, X, P.ord)) { delete_edge(indices{ i, j }, indices{ i, j + 1 });}
				}
			}

			else if (i != row_num - 1) {
				if (cur.ip.lower && grid[ind2num(i + 1, j)].encl == -1) {
					if (!cmpNadd_SinPol(indices{ i, j }, Y, P.ord)) { delete_edge(indices{ i, j }, indices{ i + 1, j }); }
				}
			}

			else if (j != col_num - 1) {
				if (cur.ip.right && grid[ind2num(i, j + 1)].encl == -1) {
					if (!cmpNadd_SinPol(indices{ i, j }, X, P.ord)) { delete_edge(indices{ i, j }, indices{ i, j + 1 }); }
				}
			}

			else {}

		}
	}

	Grid_Point p1 = grid[ind2num(bm_row, lm_col)];
	Grid_Point p2 = grid[ind2num(tm_row, rm_col)];
	for (Free_Point& fr_pt : fr_pts) {
		if (p1.x <= fr_pt.x && fr_pt.x <= p2.x && p1.y <= fr_pt.y && fr_pt.y <= p2.y) 
		{ anchor(fr_pt); }
	}
}

void Eps_Graph::delete_pol(int ord) {

	vector<Polygon>::iterator it;

	for (it = pols.begin(); it != pols.end(); ++it) {
		if ((*it).ord == ord) {
			break;
		}
	}

	int ind = distance(pols.begin(), it);
	for (Grid_Point& gr_pt : grid) {
		gr_pt.cros.erase(gr_pt.cros.begin() + ind);
	}

	for (Grid_Point& gr_pt : grid) {
		if (gr_pt.encl == ord) {
			gr_pt.encl = -1;
		}
	}

	for (Free_Point& pt : fr_pts) {
		if (pt.encl == ord) {
			pt.encl = -1;
		}
	}

	indices* diagonal = eff_region(*it);

	int tm_row = diagonal[1].row;
	int bm_row = diagonal[0].row;

	int lm_col = diagonal[0].column;
	int rm_col = diagonal[1].column;

	std::vector<int> rows, cols;

	for (int i = tm_row; i < bm_row; i++) {
		for (int j = lm_col; j < rm_col; j++) {

			Grid_Point cur = grid[ind2num(i, j)];
			if (cur.encl != -1) { continue; }

			if ((i != row_num - 1) && (j != col_num - 1)) {
				if (!cur.ip.lower && grid[ind2num(i + 1, j)].encl == -1) {
					if (cmpNadd(indices{ i, j }, Y)) { add_edge(indices{ i, j }, indices{ i + 1, j }); }
				}
				if (!cur.ip.right && grid[ind2num(i, j + 1)].encl == -1) {
					if (cmpNadd(indices{ i, j }, X)) { add_edge(indices{ i, j }, indices{ i, j + 1 }); }
				}
			}

			else if (i != row_num - 1) {
				if (!cur.ip.lower && grid[ind2num(i + 1, j)].encl == -1) {
					if (cmpNadd(indices{ i, j }, Y)) { add_edge(indices{ i, j }, indices{ i + 1, j }); }
				}
			}

			else if (j != col_num - 1) {
				if (!cur.ip.right && grid[ind2num(i, j + 1)].encl == -1) {
					if (cmpNadd(indices{ i, j }, X)) { add_edge(indices{ i, j }, indices{ i, j + 1 }); }
				}
			}

			else {}

		}
	}

	pols.erase(it);

	for (Free_Point& fr_pt : fr_pts) {
		Grid_Point p1 = grid[ind2num(bm_row, lm_col)];
		Grid_Point p2 = grid[ind2num(tm_row, rm_col)];
		if (p1.x <= fr_pt.x && fr_pt.x <= p2.x && p1.y <= fr_pt.y && fr_pt.y <= p2.y) { anchor(fr_pt); }
	}
}

indices* Eps_Graph::eff_region(Polygon P) {
	static indices ret[2]; // ret[0] : lower left, ret[1] : upper right

	ret[0].row = min(row_num - 1, int(ceil((upper_left.y - P.y_min) / eps)));
	ret[0].column = max(0, int(floor((P.x_min - upper_left.x) / eps)));

	ret[1].row = max(0, int(floor((upper_left.y - P.y_max) / eps)));
	ret[1].column = min(col_num - 1, int(ceil((P.x_max - upper_left.x) / eps)));

	return ret;
}


void Eps_Graph::BFS(Grid_Point s) {
	
	for (int& elem : dist) { elem = INT_MAX; }
	for (unsigned int ind1 = 0; ind1 < visited.size(); ind1++) { visited[ind1] = false; }
	closest = {};

	queue<int> q;

	dist[s.num] = 0;
	q.push(s.num);

	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		if (visited[cur] == true) { continue; }
		if (cur != s.num) {
			closest.push_back(cur);
		}
		visited[cur] = true;

		if (grid[cur].ip.right && visited[cur + 1] == false) { dist[cur + 1] = dist[cur] + 1; q.push(cur + 1); }
		if (grid[cur].ip.lower && visited[cur + col_num] == false) { dist[cur + col_num] = dist[cur] + 1; q.push(cur + col_num); }
		if (grid[cur].ip.left && visited[cur - 1] == false) { dist[cur - 1] = dist[cur] + 1; q.push(cur - 1); }
		if (grid[cur].ip.upper && visited[cur - col_num] == false) { dist[cur - col_num] = dist[cur] + 1; q.push(cur - col_num); }
	}

}

vector<Free_Point> Eps_Graph::kNN(Free_Point p, int k) { // returns k nearest neighbors of p

	vector<Free_Point> ret = {};

	BFS(query_anchor(p));

	while (k > 0) {
		if (closest.empty()) {
			break;
		}

		int start = 0, end = 0;
		int grid_dist = dist[closest[start]];

		while (grid_dist == dist[closest[end]]) {
			if (end == int(closest.size()) - 1) { break; }
			else { end += 1; }
		}

		// start에서 end까지 애들 모아서.
		vector<Free_Point> temp = {};

		for (int ind1 = start; ind1 <= end; ind1++) {
			vector<Free_Point> added_pts = {};

			for (auto pt_pointer : grid[closest[ind1]].anchored) {
				added_pts.push_back(*pt_pointer);
			}
			{ vector<Free_Point>::iterator it = temp.insert(temp.end(), added_pts.begin(), added_pts.end()); }
		}

		/*
		for (auto elem : temp) {
			cout << elem.x << ' ' <<  elem.y << ' ' << elem.encl << endl;
		}
		*/

		sort(temp.begin(), temp.end(), [=](Free_Point first, Free_Point second)
		{
			return pow(first.x - p.x, 2) + pow(first.y - p.y, 2) < pow(second.x - p.x, 2) + pow(second.y - p.y, 2);
		});

		vector<Free_Point> pts = {};

		for (auto pt : temp) {
			if (pt.encl == -1) { 
				bool flag = true;
				for (auto ret_pt : ret) {
					if (pt.x == ret_pt.x && pt.y == ret_pt.y) { flag = false; break;}
				}
				if (flag) { ret.push_back(pt); }
			}
		}

		int sz = int(pts.size());

		if (sz <= k) {
			k -= sz;
			ret.insert(ret.end(), pts.begin(), pts.end());
		}
		else {
			ret.insert(ret.end(), pts.begin(), pts.begin() + k);
			k = 0;
		}
		closest.erase(closest.begin(), closest.begin() + end + 1);
	}

	return ret;
} 


void Eps_Graph::print_grid() {
	for (unsigned int i = 0; i < grid.size(); i++) {
		cout << grid[i].ind.row << grid[i].ind.column << ' ' << '|' << grid[i].ip.right << ' ' << '|' << grid[i].ip.lower << ' ' << '|';
		if (num2ind(i).column == col_num - 1) { cout << endl; }
	}
}

void Eps_Graph::print_edges() {
	for (auto gp : grid) {	// gridpt
		if (gp.ip.right == true) {
			cout << gp.ind.row << ' ' << gp.ind.column << '|' << gp.ind.row << ' ' << gp.ind.column + 1 << endl;
		}
		if (gp.ip.lower == true) {
			cout << gp.ind.row << ' ' << gp.ind.column << '|' << gp.ind.row + 1 << ' ' << gp.ind.column << endl;
		}
	}
}

void Eps_Graph::print_dist() {
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (dist[i * col_num + j] == INT_MAX) { printf(" INF "); }
			else { printf("%4.1d ", dist[i * col_num + j]); }
		}
		printf("\n");
	}
}

void Eps_Graph::print_kNN(Free_Point p, int k) { 
	vector<Free_Point> nbhd = kNN(p, k);
	for (auto nb : nbhd) {
		cout << nb.x << ' ' << nb.y << endl;
	}
}


Eps_Graph::Eps_Graph() { row_num = col_num = 0; x_min = x_max = y_min = y_max = eps = 0; }

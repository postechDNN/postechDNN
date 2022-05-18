#include "EpsGraph3D.h"
#include <queue>
#include <assert.h>

#define X true // ray direction
#define Y false

using namespace std;

Eps_Graph_3D::Eps_Graph_3D(list<Free_Point> _fr_pts, vector<Polytope> _pols, double _eps) {

	fr_pts = _fr_pts;
	pols = _pols;
	eps = _eps;
	NN_dist = {};

	x_min = y_min = z_min = DBL_MAX;
	x_max = y_max = z_max = DBL_MIN;

	// among all points and polygons vertices, find the minimum/maximum x- and y-coordinate.
	// this is to find a reasonably small bounding box for P \setunion O.

	for (auto pol : pols) {
		if (pol.x_max > this->x_max) { this->x_max = pol.x_max; }
		if (pol.y_max > this->y_max) { this->y_max = pol.y_max; }
		if (pol.z_max > this->z_max) { this->z_max = pol.z_max; }

		if (pol.x_min < this->x_min) { this->x_min = pol.x_min; }
		if (pol.y_min < this->y_min) { this->y_min = pol.y_min; }
		if (pol.z_min < this->z_min) { this->z_min = pol.z_min; }
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

void Eps_Graph_3D::init_grid() {

	int x_ind = int(floor(x_min / eps)) - 1, y_ind = int(floor(y_min / eps)) - 1, z_ind = int(floor(z_min / eps)) - 1;

	row_num = 2 + int(ceil(y_max / eps)) - y_ind;
	col_num = 2 + int(ceil(x_max / eps)) - x_ind;
	layer_num = 2 + int(ceil(z_max / eps)) - z_ind;

	upper_left = Point(x_ind * eps, int(ceil(y_max / eps + 1)) * eps, z_ind * eps);
	//need to check why y is different!!

	// initialization step for BFS
	for (int i = 0; i < row_num * col_num * layer_num; i++) {
		dist.push_back(INT_MAX);
		visited.push_back(false);
	}

	for (int i = 0; i < row_num * col_num * layer_num; i++)
	{
		grid.push_back(Grid_Point(num2ind(i).row, num2ind(i).column, num2ind(i).layer, upper_left.x, upper_left.y, upper_left.z, eps, col_num));
		//grid data need to add layer
	}

	// for each grid & free point, count # of crossings of the rightward ray with each polytope
	for (Grid_Point& pt : grid) {
		for (_Polytope& pol : pols) {
			int cro = pol.ray(pt);
			pt.cros.push_back(cro);

			if (cro > 0 && cro % 2 == 1) {
				pt.encl = pol.ord;
				pol.encl_pts.push_back(pt);
			}
		}
	}

	for (Free_Point& pt : fr_pts) {
		for (Polytope& pol : pols) {
			int cro = pol.ray(pt);

			if (cro > 0 && cro % 2 == 1) {
				pt.encl = pol.ord;
				pol.encl_pts.push_back(pt);
			}
		}
	}

	// draw grid edges
	//for (int i = 0; i < row_num; i++) {
	//	for (int j = 0; j < col_num; j++) {
	//		if (grid[ind2num(i, j)].encl != -1) { continue; }

	//		if ((i != row_num - 1) && (j != col_num - 1)) {
	//			if (grid[ind2num(i + 1, j)].encl == -1) {
	//				if (cmpNadd(indices{ i, j }, Y)) { add_edge(indices{ i, j }, indices{ i + 1, j }); }
	//			}
	//			if (grid[ind2num(i, j + 1)].encl == -1) {
	//				if (cmpNadd(indices{ i, j }, X)) { add_edge(indices{ i, j }, indices{ i, j + 1 }); }
	//			}
	//		}
	//		else if (i != row_num - 1) {
	//			if (grid[ind2num(i + 1, j)].encl == -1) {
	//				if (cmpNadd(indices{ i, j }, Y)) { add_edge(indices{ i, j }, indices{ i + 1, j }); }
	//			}
	//		}
	//		else if (j != col_num - 1) {
	//			if (grid[ind2num(i, j + 1)].encl == -1) {
	//				if (cmpNadd(indices{ i, j }, X)) { add_edge(indices{ i, j }, indices{ i, j + 1 }); }
	//			}
	//		}
	//		else {}
	//	}
	//}
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			for (int k = 0; k < layer_num; k++) {
				if (grid[ind2num(i, j. k)].encl != -1) { continue; }

				if (i != row_num - 1) {
					if (grid[ind2num(i + 1, j, k)].encl == -1) {
						if (cmpNadd(indices{ i, j, k }, 0)) { add_edge(indices{ i, j, k }, indices{ i + 1, j, k }); }
					}
				}
				if (j != col_num - 1) {
					if (grid[ind2num(i, j + 1, k)].encl == -1) {
						if (cmpNadd(indices{ i, j, k }, 1)) { add_edge(indices{ i, j, k }, indices{ i, j + 1, k }); }
					}
				}
				if (k != layer_num - 1) {
					if (grid[ind2num(i, j, k + 1 )].encl == -1) {
						if (cmpNadd(indices{ i, j, k }, 2)) { add_edge(indices{ i, j, k }, indices{ i, j, k + 1 }); }
					}
				}
			}
		}
	}
}

Grid_Point Eps_Graph_3D::get_gridpt(indices ind) {
	return grid[ind2num(ind)];
}

// one-to-one functions between 2-d indices and numbers
int Eps_Graph_3D::ind2num(indices ind) {
	return ind.row * col_num * layer_num + ind.column * layer_num + ind.layer;
}

int Eps_Graph_3D::ind2num(int row, int column, int layer) {
	return row * col_num * layer_num + column * layer_num + layer;
}

indices Eps_Graph_3D::num2ind(int num) {
	return indices{ num / (col_num * layer_num), (num % col_num) / layer_num, (num% col_num) % layer_num};
}

// adds/deletes a grid edge
void Eps_Graph_3D::add_edge(indices ind1, indices ind2) {
	int row1 = ind1.row; int column1 = ind1.column;
	int row2 = ind2.row; int column2 = ind2.column;

	if (row1 == row2) { grid[row1 * col_num + column1].ip.right = true; grid[row2 * col_num + column2].ip.left = true; }
	else { grid[row1 * col_num + column1].ip.lower = true; grid[row2 * col_num + column2].ip.upper = true; }
}

void Eps_Graph_3D::add_edge(int row, int column, bool direc) {
	int row1 = row; int column1 = column;
	int row2, column2;

	if (direc == X) { row2 = row1; column2 = column1 + 1; }
	else { row2 = row1 + 1; column2 = column1; }

	if (row1 == row2) { grid[row1 * col_num + column1].ip.right = true; grid[row2 * col_num + column2].ip.left = true; }
	else { grid[row1 * col_num + column1].ip.lower = true; grid[row2 * col_num + column2].ip.upper = true; }
}

void Eps_Graph_3D::delete_edge(indices ind1, indices ind2) {
	int row1 = ind1.row; int column1 = ind1.column;
	int row2 = ind2.row; int column2 = ind2.column;

	if (row1 == row2) { grid[row1 * col_num + column1].ip.right = false; grid[row2 * col_num + column2].ip.left = false; }
	else { grid[row1 * col_num + column1].ip.lower = false; grid[row2 * col_num + column2].ip.upper = false; }
}


bool Eps_Graph_3D::cmpNadd(indices ind, bool direc) { // checks if the line connecting the gridpoint and its neighboring one is blocked by any polygon. if is not, add an edge between them.

	Grid_Point A = grid[ind2num(ind)], B;
	if (direc == X) { B = grid[ind2num(ind) + 1]; }
	else { B = grid[ind2num(ind) + col_num]; }

	for (unsigned int ind1 = 0; ind1 < pols.size(); ind1++) { // assert(pols.size() = A.cros.size() = B.cros.size())
		auto pol = pols[ind1];

		// if one of the two gridpoints lie outside the bounding box of a polygon, then the edge between them may be present
		if (direc == X) {
			if (B.x < pol.x_max || pol.x_min < A.x || pol.y_max < A.y || A.y < pol.y_min) { continue; } // assert(A.y = B.y)
		}
		else {
			if (A.y < pol.y_max || pol.y_min < B.y || pol.x_max < A.x || A.x < pol.x_min) { continue; }
		}

		// if a polygon edge crosses the line connecting two gridpoints, then the edge between them is not present
		if (direc == X) {
			if (pol.intersect(Point{ A.x, A.y }, Point{ B.x, B.y }, X)) { return false; }
		}
		else {
			if (pol.intersect(Point{ A.x, A.y }, Point{ B.x, B.y }, Y)) { return false; }
		}

		// else, compare the # of intersections with the ray.
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

bool Eps_Graph_3D::cmpNadd_SinPol(indices ind, bool direc, int ord) { // do the same with a specific polygon.

	Polytope pol;
	vector<Polytope>::iterator it;

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
		if (B.x < pol.x_max || pol.x_min < A.x || pol.y_max < A.y || A.y < pol.y_min) { return true; }
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


void Eps_Graph_3D::add_freepts(vector<Free_Point> p_vec) { // add points to the query point set P
	for (auto p : p_vec) {
		for (Polytope& pol : pols) {
			bool in = pol.isIn(pt);
			if (in) {
				//error description
				return;
			}
		}
		fr_pts.push_back(p);
		anchor(pt); //
	}
}

void Eps_Graph_3D::delete_freept(int ind) { // delete a point from P, specified by its index
	if (ind < 0 || fr_pts.size() - 1 < ind) { return; }

	list<Free_Point>::iterator iter = fr_pts.begin();
	advance(iter, ind);
	Free_Point& p = *iter;
	//fr_pts.remove(ind)
	if (p.host != -1) {
		for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
			if ((*(*it)).x == p.x && (*(*it)).y == p.y) {
				grid[p.host].anchored.erase(it);
				break;
			}
		}
	}

	fr_pts.erase(iter);
}

void Eps_Graph_3D::anchor(Free_Point& p) { // cast anchor onto a grid point from a free point

	if (p.encl != -1) {
		p.host = -1;
		return;
	}
	// else

	if (p.host != -1) {
		assert(0 <= p.host && p.host < grid.size());
		for (vector<Free_Point*>::iterator it = grid[p.host].anchored.begin(); it != grid[p.host].anchored.end(); ++it) {
			if (p.x == (*(*it)).x && p.y == (*(*it)).y) {
				grid[p.host].anchored.erase(grid[p.host].anchored.begin() + distance(it, grid[p.host].anchored.begin()));
			}
		}
	}

	bool flag = false;
	int row; int col;
	row = int(ceil((upper_left.y - p.y) / eps - 0.5)); // points on the midline anchors upward
	col = int(ceil((p.x - upper_left.x) / eps - 0.5)); // points on the midline anchors leftward 


	// find a nearest gridpoint not enclosed by any polygon
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

		for (Grid_Point gr_pt : gr_pts) {

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

Grid_Point Eps_Graph_3D::query_anchor(Free_Point p) {
	int row; int col;
	row = int(ceil((upper_left.y - p.y) / eps - 0.5)); // Á¤ Áß¾Ó¿¡ ÀÖ´Â Á¡Àº À§·Î anchorµÊ
	col = int(ceil((p.x - upper_left.x) / eps - 0.5)); // Á¤ Áß¾Ó¿¡ ÀÖ´Â Á¡Àº ¿ÞÂÊÀ¸·Î anchorµÊ

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

		for (Grid_Point gr_pt : gr_pts) {

			if (gr_pt.encl == -1) {
				return gr_pt;
			}

		}
	}
	return Grid_Point();
}


void Eps_Graph_3D::add_pol(Polytope P) { // add a polygon to the set of obstacles O
	pols.push_back(P);

	for (Grid_Point& gr_pt : grid) {
		int cro = P.ray(gr_pt);
		gr_pt.cros.push_back(cro);
		if (cro > 0 && cro % 2 == 1) { // assert(!gr_pt.encl)
			gr_pt.encl = P.ord;
			for (vector<Free_Point*>::iterator it = gr_pt.anchored.begin(); it != gr_pt.anchored.end(); ++it) {
				anchor(*(*it));
			}
			vector<Free_Point*>().swap(gr_pt.anchored);
		}
	}

	for (Free_Point& pt : fr_pts) {
		if (pt.encl != -1) { continue; }

		int cro = P.ray(pt);
		if (cro > 0 && cro % 2 == 1) {
			pt.encl = P.ord;
			for (vector<Free_Point*>::iterator it = grid[pt.host].anchored.begin(); it != grid[pt.host].anchored.end(); ++it) {
				if ((*(*it)).x == pt.x && (*(*it)).y == pt.y) {
					grid[pt.host].anchored.erase(it);
					break;
				}
			}
			pt.host = -1; // anchor(pt);
		}
	}

	indices* diagonal = eff_region(P);

	int tm_row = diagonal[1].row;
	int bm_row = diagonal[0].row;

	int lm_col = diagonal[0].column;
	int rm_col = diagonal[1].column;

	// update grid edges among gridpoints in the effective region
	for (int i = tm_row; i < bm_row; i++) {
		for (int j = lm_col; j < rm_col; j++) {

			Grid_Point cur = grid[ind2num(i, j)];
			if (cur.encl != -1) { continue; }

			if ((i != row_num - 1) && (j != col_num - 1)) {
				if (cur.ip.lower && grid[ind2num(i + 1, j)].encl == -1) {
					if (!cmpNadd_SinPol(indices{ i, j }, Y, P.ord)) { delete_edge(indices{ i, j }, indices{ i + 1, j }); }
				}
				if (cur.ip.right && grid[ind2num(i, j + 1)].encl == -1) {
					if (!cmpNadd_SinPol(indices{ i, j }, X, P.ord)) { delete_edge(indices{ i, j }, indices{ i, j + 1 }); }
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


}

void Eps_Graph_3D::delete_pol(int ord) { // delete a polygon from O, specified by its index

	vector<Polytope>::iterator it;

	for (it = pols.begin(); it != pols.end(); ++it) {
		if ((*it).ord == ord) {
			break;
		}
	}

	if (it == pols.end()) { return; }

	// release them free; for gridpoints and free points that was enclosed by the polygon
	int ind = distance(pols.begin(), it);
	for (Grid_Point& gr_pt : grid) {
		gr_pt.cros.erase(gr_pt.cros.begin() + ind);
		if (gr_pt.encl == ord) {
			gr_pt.encl = -1;
		}
	}

	for (Free_Point& pt : fr_pts) {
		if (pt.encl == ord) {
			pt.encl = -1;
			anchor(pt);
		}
	}

	indices* diagonal = eff_region(*it);

	int tm_row = diagonal[1].row;
	int bm_row = diagonal[0].row;

	int lm_col = diagonal[0].column;
	int rm_col = diagonal[1].column;

	// update grid edges among gridpoints in the effective region
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
}

indices* Eps_Graph_3D::eff_region(Polytope P) { // returns a range indicating orthogonal rectangle bounding the polygon (effective region)
	static indices ret[2]; // ret[0] : lower left, ret[1] : upper right

	ret[0].row = min(row_num - 1, int(ceil((upper_left.y - P.y_min) / eps)));
	ret[0].column = max(0, int(floor((P.x_min - upper_left.x) / eps)));

	ret[1].row = max(0, int(floor((upper_left.y - P.y_max) / eps)));
	ret[1].column = min(col_num - 1, int(ceil((P.x_max - upper_left.x) / eps)));

	return ret;
}

vector<Free_Point> Eps_Graph_3D::kNN(Free_Point p, int k) { // returns k approximate nearest neighbors of p

	for (Polytope& pol : pols) {
		int cro = pol.ray(p);

		if (cro > 0 && cro % 2 == 1) {
			return {};
		}
	}

	vector<Free_Point> ret = {};
	vector<int>().swap(NN_dist);

	Grid_Point s = query_anchor(p);
	for (int& elem : dist) { elem = INT_MAX; }
	for (unsigned int ind1 = 0; ind1 < visited.size(); ind1++) { visited[ind1] = false; }

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
			if (grid[cur].ip.right && visited[cur + 1] == false) { dist[cur + 1] = dist[cur] + 1; q.push(cur + 1); }
			if (grid[cur].ip.lower && visited[cur + col_num] == false) { dist[cur + col_num] = dist[cur] + 1; q.push(cur + col_num); }
			if (grid[cur].ip.left && visited[cur - 1] == false) { dist[cur - 1] = dist[cur] + 1; q.push(cur - 1); }
			if (grid[cur].ip.upper && visited[cur - col_num] == false) { dist[cur - col_num] = dist[cur] + 1; q.push(cur - col_num); }

			for (auto FP : grid[q.front()].anchored) {
				FPs_temp.push_back(*FP);
			}
			q.pop();
		}

		for (auto pt : FPs_temp) {
			if (pt.encl == -1) {
				FPs.push_back(pt);
			}
		}

		sort(FPs.begin(), FPs.end(), [=](Free_Point first, Free_Point second)
			{
				return pow(first.x - p.x, 2) + pow(first.y - p.y, 2) < pow(second.x - p.x, 2) + pow(second.y - p.y, 2);
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

	return ret;
}

void Eps_Graph_3D::print_grid() {
	for (unsigned int i = 0; i < grid.size(); i++) {
		cout << grid[i].ind.row << grid[i].ind.column << ' ' << '|' << grid[i].ip.right << ' ' << '|' << grid[i].ip.lower << ' ' << '|';
		if (num2ind(i).column == col_num - 1) { cout << endl; }
	}
}

void Eps_Graph_3D::print_edges() {
	for (auto gp : grid) {
		if (gp.ip.right == true) {
			cout << gp.ind.row << ' ' << gp.ind.column << '|' << gp.ind.row << ' ' << gp.ind.column + 1 << endl;
		}
		if (gp.ip.lower == true) {
			cout << gp.ind.row << ' ' << gp.ind.column << '|' << gp.ind.row + 1 << ' ' << gp.ind.column << endl;
		}
	}
}

void Eps_Graph_3D::print_dist() {
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (dist[i * col_num + j] == INT_MAX) { printf(" INF "); }
			else { printf("%4.1d ", dist[i * col_num + j]); }
		}
		printf("\n");
	}
}

void Eps_Graph_3D::print_kNN(Free_Point p, int k) {
	vector<Free_Point> nbhd = kNN(p, k);
	for (auto nb : nbhd) {
		cout << nb.x << ' ' << nb.y << endl;
	}
}


Eps_Graph_3D::Eps_Graph_3D() { row_num = col_num = 0; x_min = x_max = y_min = y_max = eps = 0; }

Free_Point Eps_Graph_3D::get_free_point(int index) {
	list<Free_Point>::iterator iter = fr_pts.begin();
	std::advance(iter, index);
	return *iter;
}
Polytope Eps_Graph_3D::get_Polytope(int index) {
	for (auto pol : pols) {
		if (index == pol.ord) {
			return pol;
		}
	}
}
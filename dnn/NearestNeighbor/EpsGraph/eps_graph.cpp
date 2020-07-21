#include "eps_graph.h"

#define X true // ray direction
#define Y false

using namespace std;

Eps_graph::Eps_graph(vector<Point> grid_points, vector<Free_Point> _free_points, vector<Polygon> figures) {

	make_grid(grid_points);
	
	free_points = _free_points;
	for (auto free_point : free_points) {
		anchor(free_point);
	}

	polygons = figures;
	for (auto polygon : polygons) {
		add_pol(polygon);
	}
		// polygons.push_back(figure);
	
	for (unsigned int i = 0; i < grid.size(); i++) {
		dist_coll.push_back({});
		visited.push_back(false);
	}
}

void Eps_graph::make_grid(std::vector<Point> grid_points) {
	sort(grid_points.begin(), grid_points.end(), [](Point first, Point second)
	{
		return first.x < second.x;
	});
	Point u_l = grid_points[0]; // upper left point
	Point s_f = grid_points[1]; // one step further(to the right)
	Point l_r = grid_points[2]; // lower right point

	x_min = std::min({ u_l.x, s_f.x, l_r.x }); x_max = std::max({ u_l.x, s_f.x, l_r.x });
	y_min = std::min({ u_l.y, s_f.y, l_r.y }); y_max = std::max({ u_l.y, s_f.y, l_r.y });

	eps = s_f.x - u_l.x;
	row_num = 1 + int((y_max - y_min) / eps);
	col_num = 1 + int((x_max - x_min) / eps);

	for (int i = 0; i < row_num * col_num; i++) {
		grid.push_back(Grid_Point(num2ind(i).row, num2ind(i).column, x_min, y_max, eps, col_num));
	}

	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if ((i != row_num - 1) && (j != col_num - 1)) {
				add_edge(indices{ i, j }, indices{ i + 1, j }, 1); // 이미 add_edge 함수에서 양쪽 directed edge를 건드리므로, 여기서는 한 방향만 보면 됨.
				add_edge(indices{ i, j }, indices{ i, j + 1 }, 1);
			}
			else if (i != row_num - 1) {
				add_edge(indices{ i, j }, indices{ i + 1,j }, 1);
			}
			else if (j != col_num - 1) {
				add_edge(indices{ i, j }, indices{ i, j + 1 }, 1);
			}
			else {}
		}
	}
}

Grid_Point Eps_graph::get_gridpt(indices ind) {
	return grid[ind2num(ind)];
}


int Eps_graph::ind2num(indices ind) {
	return ind.row * col_num + ind.column;
}

indices Eps_graph::num2ind(int num) {
	return indices{ num / col_num, num % col_num };
}


bool Eps_graph::is_edge(indices ind1, indices ind2) {
	Point p1 = get_gridpt(ind1), p2 = get_gridpt(ind2);
	if (std::min({ p1.x, p1.y, p2.x, p2.y }) < 0 || std::max(p1.x, p2.x) > eps * (double(col_num) - 1)	// any of the two input points is outside from the epsilon graph
		|| std::max(p1.y, p2.y) > eps * (double(row_num) - 1)) {
		return false;
	}
	else if (abs(p1.x - p2.x) + abs(p1.y - p2.y) != eps) { return false; }	// two input points are not adjacent
	else {
		bool flag;
		if (abs(p1.x - p2.x) == eps) { flag = X; }	// their x-coordinates are the same
		else { flag = Y; }	// their y-coordinates are the same
		
		for (unsigned int i = 0; i < polygons.size(); i++)
		{
			if (polygons[i].intersect(p1, p2, flag) == true) { return false; }
			else {
				int inter1 = polygons[i].ray(p1), inter2 = polygons[i].ray(p2);
				if (inter1 == -2 && inter2 == -2) {
					int mid_inter = polygons[i].ray(Point((p1.x + p2.x / 2), (p1.y, p2.y) / 2));
					if (mid_inter % 2 == 1) { return false; }
				}
				else if (inter1 == -2) {
					if (inter2 % 2 == 1) { return false; }
				}
				else if (inter2 == -2) {
					if (inter1 % 2 == 1) { return false; }
				}
				else {
					if (inter1 % 2 == 1 || inter2 % 2 == 1) { return false; }
				}
			}

		}
		return true;
	}
}

void Eps_graph::add_edge(indices ind1, indices ind2, double weight) {
	int row1 = ind1.row; int column1 = ind1.column;
	int row2 = ind2.row; int column2 = ind2.column;

	if (row1 == row2) { grid[row1 * col_num + column1].ip.right = true; grid[row2 * col_num + column2].ip.left = true; }
	else { grid[row1 * col_num + column1].ip.lower = true; grid[row2 * col_num + column2].ip.upper = true; }

	/*
	auto it = adj_list[row1 * col_num + column1].begin();
	for (it = adj_list[row1 * col_num + column1].begin(); it != adj_list[row1 * col_num + column1].end(); it++) {
		if (std::get<0>(*it) == row2 * col_num + column2) { break; }
	}

	if (it == adj_list[row1 * col_num + column1].end()) {
	adj_list[row1 * col_num + column1].push_back(std::make_tuple(row2 * col_num + column2, weight));
	}
	*/
}

void Eps_graph::delete_edge(indices ind1, indices ind2) {
	int row1 = ind1.row; int column1 = ind1.column;
	int row2 = ind2.row; int column2 = ind2.column;

	if (row1 == row2) { grid[row1 * col_num + column1].ip.right = false; grid[row2 * col_num + column2].ip.left = false; }
	else { grid[row1 * col_num + column1].ip.lower = false; grid[row2 * col_num + column2].ip.upper = false; }

	/*
	for (auto it = adj_list[row1 * col_num + column1].begin(); it != adj_list[row1 * col_num + column1].end(); it++) {
		if (std::get<0>(*it) == row2 * col_num + column2) {
			adj_list[row1 * col_num + column1].erase(it);
			break;
		}
	}
	*/
}

void Eps_graph::update_grid(indices ind1, indices ind2) {
	int lowermost_row = ind1.row;
	int uppermost_row = ind2.row;

	int leftmost_column = ind1.column;
	int rightmost_column = ind2.column;

	std::vector<int> rows, cols;
	// for (int i = lower_bound; i < upper_bound + 1; i++) { // this for loop checks if the grid edges are still adjacent/not adjacent
	// 	for (int j = left_bound; j < right_bound + 1; j++) {
	for (int i = lowermost_row; i < uppermost_row; i++) {
		for (int j = leftmost_column; j < rightmost_column; j++) {

			if ((i != row_num - 1) && (j != col_num - 1)) {
				if (is_edge(indices{ i, j }, indices{ i + 1, j })) { add_edge(indices{ i, j }, indices{ i + 1, j }, 1);}
				else {delete_edge(indices{ i, j }, indices{ i + 1, j });}

				if (is_edge(indices{ i, j }, indices{ i, j + 1 })) {add_edge(indices{ i, j }, indices{ i, j + 1 }, 1);}
				else {delete_edge(indices{ i, j }, indices{ i, j + 1 });}
			}

			else if (i != row_num - 1) {
				if (is_edge(indices{ i, j }, indices{ i + 1, j })) {add_edge(indices{ i, j }, indices{ i + 1, j }, 1);}
				else {delete_edge(indices{ i, j }, indices{ i + 1, j });}
			}

			else if (j != col_num - 1) {
				if (is_edge(indices{ i, j }, indices{ i, j + 1 })) {add_edge(indices{ i, j }, indices{ i, j + 1 }, 1);}
				else {delete_edge(indices{ i, j }, indices{ i, j + 1 });}
			}

			else {}

		}
	}
}


void Eps_graph::add_freept(Free_Point p) {
	free_points.push_back(p);
}

void Eps_graph::delete_freept(Free_Point p) {
	for (vector<Free_Point>::iterator it = free_points.begin(); it != free_points.end(); ++it) {
		if ((*it).x == p.x && (*it).y == p.y) {
			free_points.erase(it);
			break;
		}
	}
}

void Eps_graph::delete_freept(int i) {
	free_points.erase(free_points.begin() + i); // begin()은 이터레이터를 반환
}


void Eps_graph::anchor(Free_Point p) {
	int row; int col;
	row = int(ceil((y_max - p.y) / eps - 0.5)); // 정 중앙에 있는 점은 위로 anchor됨
	col = int(ceil((p.x - x_min) / eps - 0.5)); // 정 중앙에 있는 점은 왼쪽으로 anchor됨
	Grid_Point& cur = grid[ind2num(indices{ row, col })];
	cur.pq.push(p);
	// p.host = cur;
	p.dist = sqrt(pow(cur.x - p.x, 2) + pow(cur.y - p.y, 2));
}

Grid_Point Eps_graph::query_anchor(Free_Point p) {
	int row; int col;
	row = int(ceil((y_max - p.y) / eps - 0.5)); // 정 중앙에 있는 점은 위로 anchor됨
	col = int(ceil((p.x - x_min) / eps - 0.5)); // 정 중앙에 있는 점은 왼쪽으로 anchor됨
	return grid[ind2num(indices{ row, col })];
}


void Eps_graph::add_pol(Polygon P) {
	polygons.push_back(P);

	indices* diagonal = eff_region(P);
	update_grid(diagonal[0], diagonal[1]);
}

void Eps_graph::delete_pol(int i) {
	Polygon P = *(polygons.begin() + i);
	polygons.erase(polygons.begin() + i);

	indices* diagonal = eff_region(P); 
	update_grid(diagonal[0], diagonal[1]); // erase한 뒤에 update하는 게 맞음. 순서 바꾸면 안됨
}

indices* Eps_graph::eff_region(Polygon P) {
	indices ret[2];

	ret[0].row = max(0, int(floor((y_max - P.y_max) / eps))); // lowermost row. 평면 상에서는 위쪽 부분임
	ret[0].column = max(0, int(floor((P.x_min - x_min) / eps))); // 다 floor인 게 맞음.
	
	ret[1].row = min(row_num - 1, row_num - 1 - int(floor((P.y_min - y_min) / eps)));
	ret[1].column = min(col_num - 1, col_num - 1 - int(floor((x_max - P.x_max) / eps)));

	return ret;
}

void Eps_graph::dijkstra(Grid_Point p) {
	for (int i = 0; i < row_num * col_num; i++) {
		dist_coll[i] = {INT_MAX, i};
		visited[i] = false;
	}
	closest.clear();

	// priority_queue<int, vector<int>, greater<int>> pq;

	priority_queue<tuple<int, int>, vector<tuple<int, int>>, greater<tuple<int, int>>> pq;
	dist_coll[p.num] = {0, p.num};
	pq.push(dist_coll[p.num]);

	while (!pq.empty()) {
		int distance = get<0>(pq.top());
		int node_num = get<1>(pq.top());
		pq.pop();

		if (!visited[node_num]) {
			if (node_num != p.num) {
				closest.push_back(node_num);
			}
			visited[node_num] = true;
			dist_coll[node_num] = {distance, node_num};
			Grid_Point gp = get_gridpt(num2ind(node_num));
			if (gp.ip.right) { pq.push(std::make_tuple(distance + 1, node_num + 1)); }
			if (gp.ip.lower) { pq.push(std::make_tuple(distance + 1, node_num + col_num)); }
			if (gp.ip.left) { pq.push(std::make_tuple(distance + 1, node_num - 1)); }
			if (gp.ip.upper) { pq.push(std::make_tuple(distance + 1, node_num - col_num)); }
		}
	}
}

vector<Free_Point> Eps_graph::kNN(Free_Point p, int k) {

	vector<Free_Point> whole = free_points;

	/*
	int sz = whole.size();
	if (1 + k >= sz) {
		for (vector<Free_Point>::iterator it = whole.begin(); it != whole.end(); ++it) {
			if (*it == p) {
				whole.erase(it);
			}
		}
		return free_points;
	}	// if k exceeds the number of free points, return all of them.
	*/

	vector<Free_Point> ret;

	dijkstra(query_anchor(p));
	while (k > 0) {
		if (closest.empty()) {
			break;
		}
		int cur = closest[0];
		// vector<Free_Point> pts = grid[cur].acd_pts;
		priority_queue <Free_Point, vector<Free_Point>, cmp> pts_pq = grid[cur].pq;
		int sz = pts_pq.size();
		if (sz < k) {
			k -= sz;
			// ret.insert(ret.end(), grid[cur].acd_pts.begin(), grid[cur].acd_pts.end());
			while (!pts_pq.empty()) {
				ret.push_back(pts_pq.top());
				pts_pq.pop();
			}
		}
		else {
			for (int i = 0; i < k; i++) {
				ret.push_back(pts_pq.top());
				// ret.insert(ret.begin(), pts_pq.top());
				pts_pq.pop();
			}
			k = 0;
		}
		closest.erase(closest.begin());
	}

	return ret;
}


void Eps_graph::print_grid() {
	for (unsigned int i = 0; i < grid.size(); i++) {
		cout << grid[i].ind.row << grid[i].ind.column << ' ' << '|' << grid[i].ip.right << ' ' << '|' << grid[i].ip.lower << ' ' << '|';
		if (num2ind(i).column == col_num - 1) { cout << endl; }

	}
}

void Eps_graph::print_edges() {
	for (auto gp : grid) {	// gridpt
		if (gp.ip.right == true) {
			cout << gp.ind.row << ' ' << gp.ind.column << '|' << gp.ind.row << ' ' << gp.ind.column + 1 << endl;
		}
		if (gp.ip.lower == true) {
			cout << gp.ind.row << ' ' << gp.ind.column << '|' << gp.ind.row + 1 << ' ' << gp.ind.column << endl;
		}
	}
	/*
	for (auto edge : edges) {
		cout << edge.p1.x << edge.p1.y << '|' << edge.p2.x << edge.p2.y << endl;
	}
	*/
}

void Eps_graph::print_dist() {
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (get<0>(dist_coll[i * col_num + j]) == INT_MAX) { printf(" INF "); }
			else { printf("%4.1d ", get<0>(dist_coll[i * col_num + j])); }
		}
		printf("\n");
	}
}

void Eps_graph::print_kNN(Free_Point p, int k) {
	vector<Free_Point> nbhd = kNN(p, k);
	for (auto nb : nbhd) {
		cout << nb.x << ' ' << nb.y << endl;
	}
}
// remaining issue : 


Eps_graph::Eps_graph() { row_num = col_num = 0; x_min = x_max = y_min = y_max = eps = 0; }
/*

driver code

int size[2] = { 6, 8 };
Eps_graph G = Eps_graph(size, 0.5);

std::vector <Point> vers = { Point(0.2, 2.3), Point(0.7, 2.3), Point(0.7, 1.8), Point(0.2, 1.8) };
G.add_pol(Polygon(vers, 4)); vers.clear();
std::vector <Point> vers2 = { Point(1.3, 1.6), Point(1.6, 1.1), Point(0.9, 0.9), Point(0.9, 1.3) };
G.add_pol(Polygon(vers2, 4)); vers2.clear();
std::vector <Point> vers3 = { Point(2, 2.5), Point(3, 2.5), Point(3, 1), Point(2, 1)};
G.add_pol(Polygon(vers3, 4)); vers3.clear();

*/

/*

	while (1)
	{
		int p_r, p_c;
		std::cin >> p_r >> p_c;
		int p_ind[2] = { p_r, p_c };

		G.dijkstra(p_ind);
		G.print_dist();
	}

*/

/*
	long double eps; // grid gap
	int row_num, col_num; // # of points in each row/column of the bounding box
	int pol_num; // polygon number
	std::cin >> eps;
	std::cin >> row_num >> col_num;
	int size[2] = { row_num, col_num };
	std::cin >> pol_num;
	Eps_graph G = Eps_graph(size, eps);

	std::vector <Point> vers;
	for (int i = 0; i < pol_num; i++) {
		int ver_num; // vertex number
		std::cin >> ver_num;
		long double x, y;
		for (int j = 0; j < ver_num; j++) {
			std::cin >> x >> y;
			vers.push_back(Point(x, y));
		}
		G.add_pol(Polygon(vers, ver_num));
		vers.clear();
	}
*/

/*
for (int i = 0; i < row_num * col_num; i++) {
	std::vector<std::tuple<int, long double>> empty_vec;
	adj_list.push_back(empty_vec);	// initialize the adj. list
}
*/

/*
int Eps_graph::get_row_num()
{return row_num;}

int Eps_graph::get_col_num()
{return col_num;}

std::vector<edge> Eps_graph::get_edges() {
	std::vector<edge> tmp_edges;
	for (auto _gridpt : grid_mat) {
		if (_gridpt.right == true) { tmp_edges.push_back(edge{get_gridpt(_gridpt.ind.row, _gridpt.ind.column), get_gridpt(_gridpt.ind.row, _gridpt.ind.column + 1)}); }
		if (_gridpt.lower == true) { tmp_edges.push_back(edge{get_gridpt(_gridpt.ind.row, _gridpt.ind.column), get_gridpt(_gridpt.ind.row + 1, _gridpt.ind.column) });}
	}
	edges = tmp_edges;
	return edges;
}
*/

/*
Eps_graph::Eps_graph(int* _size, long double _eps) {
	row_num = _size[0]; col_num = _size[1];
	eps = _eps;
	pol_num = 0;

	for (int i = 0; i < row_num * col_num; i++) {
		std::vector<std::tuple<int, long double>> empty_vec;
		adj_list.push_back(empty_vec);	// initialize the adj. list
		long double dist = INT_MAX;	// for Dijkstra's
		dist_coll.push_back(dist);
		visited.push_back(false);
	}

	std::vector<int> rows, cols;
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			rows.clear();
			cols.clear();
			if (i == row_num - 1) {
				if (j == col_num - 1) {}
				else { rows.push_back(0); cols.push_back(1); }
			}
			else {
				if (j == col_num - 1) { rows.push_back(1); cols.push_back(0); }
				else { rows.push_back(0); rows.push_back(1); cols.push_back(1); cols.push_back(0); }
			}

			for (int k = 0; k < rows.size(); k++) {
				int ind1[2] = { i, j };
				int ind2[2] = { i + rows[k], j + cols[k] };
				add_edge(indices{ i, j }, indices{ i + rows[k], j + cols[k] }, 1); add_edge(indices{ i + rows[k], j + cols[k] }, indices{ i, j }, 1);	// forms grid edges
			}
		}
	}
}


typedef std::vector<std::vector<std::tuple<int, long double>>> adj;

void Eps_graph::adj_edges(Point p)
{
	int row = p.row_num(), col = p.col_num();
	int ind[2] = { row, col };
	int upper[2] = { row - 1, col }, left[2] = { row, col - 1 }, right[2] = { row, col + 1 }, lower[2] = { row + 1, col };

	printf("Current Location : (%lf, %lf)\n\n", p.x, p.y);
	printf("     %d\n", is_edge(ind, upper));
	printf("%d         %d\n", is_edge(ind, left), is_edge(ind, right));
	printf("     %d\n", is_edge(ind, lower));

}

std::vector<gridpt> Eps_graph::get_grid_mat() {
	return grid_mat;
}

std::vector<std::vector<std::tuple<int, long double>>> Eps_graph::get_adj_list()
{
	return adj_list;
}
*/

/*
typedef std::tuple<long double, int> dijk_elem;  // for heap elements in Dijkstra's

void Eps_graph::dijkstra(int* ind) {
	for (int i = 0; i < row_num * col_num; i++) {
		dist_coll[i] = INT_MAX;
		visited[i] = false;
	}

	std::priority_queue<dijk_elem, std::vector<dijk_elem>, std::greater<dijk_elem>> pq;
	pq.push(std::make_tuple(0, ind[0] * col_num + ind[1]));
	while (!pq.empty()) {
		long double distance = std::get<0>(pq.top());
		int node = std::get<1>(pq.top());
		pq.pop();

		if (!visited[node]) {
			visited[node] = true;
			dist_coll[node] = distance;
			for (int i = 0; i < adj_list[node].size(); i++) {
				pq.push(std::make_tuple(distance + get<1>(adj_list[node][i]), get<0>(adj_list[node][i])));
			}
		}
	}
}

*/
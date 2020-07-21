#include "eps_graph.h"

using namespace std;

int main() {
	vector<Point> sites;
	sites.push_back(Point(0, 5)); sites.push_back(Point(1, 5)); sites.push_back(Point(5, 0));

	vector<Free_Point> _free_points;
	_free_points.push_back(Free_Point(0.2, 4.8)); _free_points.push_back(Free_Point(1.5, 3.5));
	_free_points.push_back(Free_Point(2.5, 0.5)); _free_points.push_back(Free_Point(3.5, 1.5));

	vector <Point> vers; // polygon vertices
	vers.push_back(Point(0.5, 4.5)); vers.push_back(Point(0.5, 1.5)); 
	vers.push_back(Point(2.5, 1.5)); vers.push_back(Point(2.5, 4.5));

	vector<Polygon> figures;
	figures.push_back(Polygon(vers));

	Eps_graph G = Eps_graph(sites, _free_points, figures); // polygon ¾øÀÌ
	G.print_kNN(_free_points[0], 1);
	cout << endl;
	G.print_kNN(_free_points[0], 2);
	cout << endl;
	G.print_kNN(_free_points[0], 3);

	// G.print_kNN(_free_points[0], 2);
	// G.print_kNN(_free_points[0], 3);
	// G.print_kNN(_free_points[0], 4);


	// G.print_edges();

	/*
	G.dijkstra(G.query_anchor(Free_Point(0.2, 4.8)));
	G.print_dist();

	G.dijkstra(G.query_anchor(Free_Point(1.5, 3.5)));
	G.print_dist();
	*/

	return 0;
}

// typedef std::vector<std::vector<std::tuple<int, long double>>> adj;

/*

adj adj_list = G.get_adj_list();

int row_num = G.get_row_num();
int col_num = G.get_col_num();
int num;

for (int i = 0; i < row_num * col_num; i++) {
	for (int j = 0; j < adj_list[i].size(); j++) {
		num = std::get<int>(adj_list[i][j]);
		Point p1 = G.get_GridPt(i / col_num, i % col_num);
		Point p2 = G.get_GridPt(num / col_num, num % col_num);
		std::cout << p1.getx() << p1.gety() << "|" << p2.getx() << p2.gety() << std::endl;
	}
}
*/

/*
for (auto elem : adj_list)
	for (auto incid : elem)
		std::cout << std::get<0>(incid) << std::endl;
*/

// G.print_grid_mat();
// G.get_edges();
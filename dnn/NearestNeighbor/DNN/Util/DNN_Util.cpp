#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "../../../Examples/ShortestPathSimplePolygon/Util/NODE.h"
#include "../../../Examples/ShortestPathSimplePolygon/Util/Edge.h"
#include "../../../Examples/ShortestPathSimplePolygon/Util/Point.h"
#include "../../../Examples/ShortestPathSimplePolygon/Util/Tree.h"
#include "../../../Examples/ShortestPathSimplePolygon/Util/VertexID.h"
#include "../../../Examples/ShortestPathSimplePolygon/Util/PointS.h"
#include "../../../Examples/ShortestPathSimplePolygon/Util/SNode.h"
#include "../../../Examples/ShortestPathSimplePolygon/Util/polygon_operation.h"
#include "../../../Examples/ShortestPathSimplePolygon/Util/polygon_decomposition.h"
#include "../../../Examples/ShortestPathSimplePolygon/Util/hourglass_operation.h"

#include "DNN_Util.h"

#define NULL_HELPER -1
#define PI 3.1415926535897931
using namespace std;

int read_file(string filePath);
void print_result(int argc, char **argv);
void display();
void reshape(int w, int h);
vector<Point> test_points;
vector<Point> query_point;
vector<int> sequence_diagonal;
int d_size;
Hourglass final_hour;
PointS point_state;

Hourglass test_hourglass;

////////////////////
vector<Point> polygon_boundary;
////////////////////

Hourglass find_shortest_path(vector<Point> test_points);

int w_h=800, w_w=800;

void construct_hourglasses() {
	for (int i = 0; i < d_size; i++) {
		for (int j = 0; j < d_size; j++) {
			if (s_graph[i][j] == connected) {
				construct_hourglass(i, j);
			}
		}
	}
}
bool findPath(SNode * root, vector<SNode *> &path, SNode * k)
{
	// base case 
	if (root == NULL) return false;
	if (root == k)
		return true;

	// Store this node in path vector. The node will be removed if 
	// not in path from root to k 
	path.push_back(root);

	// See if the k is same as root's key 

	// Check if k is found in left or right sub-tree 
	if ((root->get_left_children() && findPath(root->get_left_children(), path, k)) ||
		(root->get_right_children() && findPath(root->get_right_children(), path, k)))
		return true;

	// If not present in subtree rooted with root, remove root from 
	// path[] and return false 
	path.pop_back();
	return false;
}
//find triangle path
bool findPath(Triangle * root, vector<Triangle *> &path, Triangle * k)
{
	// base case 
	if (root == NULL) return false;
	path.push_back(root);
	if (root == k)
		return true;
	bool * check = root->get_dual_check_children();
	int * ad_tr = root->get_adjacent_triangles();
	// Check if k is found in left or right sub-tree 
	for (int i = 0; i < 3; i++) {
		if (check[i]&&findPath(&t_list[ad_tr[i]],path,  k))
			return true;
	}
	path.pop_back();
	return false;
}
Triangle * find_common_triangle(Triangle *t1, Triangle *t2) {
	vector<Triangle *> path1, path2;
	if (!findPath(&t_list[t_head], path1, t1) || !findPath(&t_list[t_head], path2, t2))
		return NULL;

	int i;
	for (i = 0; i < (int)path1.size() && i < (int)path2.size(); i++)
		if (path1[i] != path2[i])
			break;
	return path1[i - 1];
}
SNode * find_common_ancestor(int _t1, int _t2) {//input: triangle where the test_points are located
	// to store paths to n1 and n2 from the root 
	vector<SNode *> path1, path2;

	// Find paths from root to n1 and root to n1. If either n1 or n2 
	// is not present, return -1 
	Triangle t1 = t_list[_t1], t2 = t_list[_t2];
	SNode* s1 = t1.get_node(), * s2 = t2.get_node();
	if (!findPath(s_head, path1, s1) || !findPath(s_head, path2, s2))
		return NULL;

	/* Compare the paths to get the first different value */
	int i;
	for (i = 0; i < (int)path1.size() && i < (int)path2.size(); i++)
		if (path1[i] != path2[i])
			break;
	SNode * common_ancestor = path1[i - 1];

	Triangle * common_low_triangle = find_common_triangle(&t_list[_t1], &t_list[_t2]);
	bool check = false;
	bool inclusive;
	std::vector<SNode*>::reverse_iterator rit;
	vector<Triangle *> dummy;
	for (rit = path2.rbegin(); rit != path2.rend(); rit++) {
		if (!check) {
			inclusive = true;
			int * t = edge_triangle[(*rit)->get_diagonal()];
			for (int i = 0; i < 2; i++) {
				if (t[i] != -1) {
					//Triangle &target = t_list[t[i]];
					bool b1 = findPath(&t_list[t[i]],dummy, &t_list[_t1]);
					bool b2 = findPath(&t_list[t[i]],dummy,&t_list[_t2]);
					if (((&t_list[t[i]])!=(&t_list[_t1]))&&((&t_list[t[i]]) != (&t_list[_t2]))&&(!b1 || b2) && (b1 || !b2) && (common_low_triangle != &t_list[t[i]]))
						inclusive = false;
				}
			}
			if(inclusive) sequence_diagonal.push_back((*rit)->get_diagonal());
			if (common_ancestor == (*rit)) {
				check = true;
			}
		}
	}

	vector<SNode*>::iterator it;
	check = false;
	for (it = path1.begin(); it != path1.end(); it++) {
		if (common_ancestor == *it) {
			check++;
		}
		else if (check) {
			inclusive = true;
			int * t = edge_triangle[(*it)->get_diagonal()];
			for (int i = 0; i < 2; i++) {
				if (t[i] != -1) {
					bool b1 = findPath(&t_list[t[i]], dummy, &t_list[_t1]);
					bool b2 = findPath(&t_list[t[i]], dummy, &t_list[_t2]);
					if (((&t_list[t[i]]) != (&t_list[_t1])) && ((&t_list[t[i]]) != (&t_list[_t2])) && (!b1 || b2) && (b1 || !b2) && (common_low_triangle != &t_list[t[i]]))
						inclusive = false;
				}
			}
			if(inclusive) sequence_diagonal.push_back((*it)->get_diagonal());
		}
	}
	
	return common_ancestor;
}
void free_data() {
	for (int i = 0; i < v_num; i++) {
		delete(edge_finder[i]);
	}
	delete(edge_finder);

	for (int i = 0; i < (int)(diagonal_list.size()); i++) {
		delete(edge_triangle[i]);
		delete(s_graph[i]);
	}
	delete(s_graph);
	delete(edge_triangle);

	delete_snodes(s_head);

	delete(t_list);
}

vector<Point> input_polygon;

void preprocess_polygon()
{
	vector<int> polygon = vector<int>(point_list.size());

	iota(polygon.begin(), polygon.end(), 0);
	polygon_list.push_back(polygon);
	make_big_triangle();

	for (int i = 0; i < (int)point_list.size(); i++) {
		outer_edge_list.push_back(Edge(i, (i + 1) % point_list.size()));
	}
	bool  inside = true;
	vector<Edge> new_d_list(find_monotone_polygons(polygon_list));//divides P into smaller polygons(not necessarily triangles) -> 아직 test point 안 잡음
	diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
	new_d_list = find_monotone_polygons(outer_polygon_list);
	outer_diagonal_list.insert(outer_diagonal_list.end(), new_d_list.begin(), new_d_list.end());

	new_d_list = triangulate_monotone_polygons(polygon_list);
	diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
	new_d_list = triangulate_monotone_polygons(outer_polygon_list);
	outer_diagonal_list.insert(outer_diagonal_list.end(), new_d_list.begin(), new_d_list.end());

	d_size = diagonal_list.size();
	t_num = int(polygon_list.size());
	dual_tree(v_num);

	construct_hourglasses();

	diagonal_list = vector<Edge>(diagonal_list.begin(), diagonal_list.begin() + d_size);
	
}

Hourglass find_shortest_path(vector<Point> test_points) //input : two test points , returns final hourglass(string) representing shortest path of the two points
{
	selected_triangle = vector<int>();
	sequence_diagonal = vector<int>();
	for (int i = 0; i < (int)test_points.size(); i++) {
		int found_triangle = point_state.find_triangle(test_points[i]);
		selected_triangle.push_back(found_triangle);
	}
	Hourglass final_hourglass;
	if (selected_triangle[0] == selected_triangle[1]) {
		final_hourglass.set_string(new String(point_list.size() - 1, point_list.size() - 2));
		final_hourglass.set_first_edge(Edge(point_list.size() - 1));
		final_hourglass.set_second_edge(Edge(point_list.size() - 2));
		return final_hourglass;
	}
	SNode * common_ancestor = find_common_ancestor(selected_triangle[0], selected_triangle[1]);
	Hourglass origin, dest;

	origin = construct_hourglass_point(point_list.size() - 1, sequence_diagonal.front());
	dest = construct_hourglass_point(point_list.size() - 2, sequence_diagonal.back());
	if (sequence_diagonal.size() == 1) {
		final_hourglass = concatenate_hourglasses(origin, dest);
	}
	else if (sequence_diagonal.size() >= 2) {
		int h_num = s_graph[sequence_diagonal[0]][sequence_diagonal[1]];
		h_num = (h_num == -1) ? s_graph[sequence_diagonal[1]][sequence_diagonal[0]] : h_num;
		final_hourglass = hourglass_list[h_num];

		Hourglass temp;
		for (int i = 2; i < (int)sequence_diagonal.size(); i++) {
			int d_1 = sequence_diagonal[i - 1];
			int d_2 = sequence_diagonal[i];
			int h_num = s_graph[d_1][d_2];
			h_num = (h_num == -1) ? s_graph[d_2][d_1] : h_num;
			temp = hourglass_list[h_num];
			final_hourglass = concatenate_hourglasses(temp, final_hourglass);
		}
		final_hourglass = concatenate_hourglasses(origin, final_hourglass);
		final_hourglass = concatenate_hourglasses(final_hourglass, dest);
	}
	for (int i = 0; i<2; i++)
		diagonal_list.pop_back();

	return final_hourglass;
}

DNN_Util::DNN_Util() {
	this->knn = 1;
	this->polygonInputDone = false;

	polygon_list = vector<vector<int>>();
	diagonal_list = vector<Edge>();
	outer_diagonal_list = vector<Edge>();
	selected_triangle = vector<int>();
	outer_edge_list = vector<Edge>();
	sequence_diagonal = vector<int>();
	null_edge_list = vector<Edge*>();
	init_hourglass_val();
}

DNN_Util::~DNN_Util() {

}

void DNN_Util::addVertexPolygon(int x, int y) {
	Point new_point(x, y);
	input_polygon.push_back(new_point);
}

void DNN_Util::deleteVertexPolygon() {
	if (input_polygon.empty() == false) {
		input_polygon.pop_back();
	}
}

int DNN_Util::addPoint(int x, int y) {
	if (this->polygonInputDone == false) {
		v_num = input_polygon.size();
		point_list = vector<Point>();
		point_list.reserve(v_num);
		int id = 0;
		for (size_t i = 0; i < input_polygon.size(); i++) {
			Point p = Point(id, input_polygon[i].get_x(), input_polygon[i].get_y());
			point_list.push_back(p);
			polygon_boundary.push_back(p);
			id++;
		}
		this->preprocessing();
	}

	Point new_point(-1, x, y);
	int test_tri = point_state.find_triangle(new_point);
	if (test_tri < (int)polygon_list.size() && test_tri != -1) {
		this->points_set.push_back(std::make_pair(x, y));
		//test_points.push_back(new_point);
		//point_list.push_back(new_point);
		return 1;
	}
	else {
		return 0;
	}
}

void DNN_Util::deletePoint() {
	if (this->points_set.empty() == false) {
		this->points_set.pop_back();
	}

	if (test_points.empty() == false) {
		test_points.pop_back();
		if (point_list.empty() == false) {
			point_list.pop_back();
		}
	}
}

void DNN_Util::printInputData(std::string fileName) {
	std::ofstream writeFile(fileName);
	if (writeFile.is_open()) {
		writeFile << input_polygon.size() << std::endl;
		for (size_t i = 0; i < input_polygon.size(); i++) {
			writeFile << input_polygon[i].get_x() << ' ' << input_polygon[i].get_y() << '\n';
		}
		writeFile << test_points.size() << std::endl;
		for (size_t i = 0; i < test_points.size(); i++) {
			writeFile << test_points[i].get_x() << ' ' << test_points[i].get_y() << '\n';
		}
		writeFile.close();
	}
}

int DNN_Util::readInputData(std::string fileName) {
	std::ifstream readFile(fileName);
	if (readFile.is_open()) {
		int vertex_num;
		readFile >> vertex_num;
		v_num = vertex_num;
		point_list = vector<Point>();
		point_list.reserve(vertex_num);

		input_polygon = vector<Point>();
		input_polygon.reserve(vertex_num);

		point_type x, y;
		int id = 0;
		for (int i = 0; i < vertex_num; i++) {
			readFile >> x >> y;
			Point p = Point(id, x, y);
			point_list.push_back(p);
			polygon_boundary.push_back(p);
			input_polygon.push_back(p);
			id++;
		}
		this->preprocessing();

		int point_num;
		readFile >> point_num;
		for (int i = 0; i < point_num; i++) {
			readFile >> x >> y;
			this->addPoint(x,y);
		}

		readFile.close();
		return 1;
	}
	return 0;
}


void DNN_Util::preprocessing() {
	this->polygonInputDone = true;
	vector<int> polygon = vector<int>(point_list.size());

	iota(polygon.begin(), polygon.end(), 0);
	polygon_list.push_back(polygon);
	make_big_triangle();

	for (int i = 0; i < (int)point_list.size(); i++) {
		outer_edge_list.push_back(Edge(i, (i + 1) % point_list.size()));
	}
	bool  inside = true;
	vector<Edge> new_d_list(find_monotone_polygons(polygon_list));//divides P into smaller polygons(not necessarily triangles) -> 아직 test point 안 잡음
	diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
	new_d_list = find_monotone_polygons(outer_polygon_list);
	outer_diagonal_list.insert(outer_diagonal_list.end(), new_d_list.begin(), new_d_list.end());

	new_d_list = triangulate_monotone_polygons(polygon_list);
	diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
	new_d_list = triangulate_monotone_polygons(outer_polygon_list);
	outer_diagonal_list.insert(outer_diagonal_list.end(), new_d_list.begin(), new_d_list.end());

	d_size = diagonal_list.size();
	t_num = int(polygon_list.size());
	dual_tree(v_num);

	construct_hourglasses();

	diagonal_list = vector<Edge>(diagonal_list.begin(), diagonal_list.begin() + d_size);


	point_state = PointS();
	while (point_state.step());
}


int DNN_Util::addQueryPoint(double x, double y) {
	Point p(-1, x, y);
	int test_tri = point_state.find_triangle(p);
	if (test_tri < (int)polygon_list.size()) {
		if (!query_point.empty()) this->deleteQueryPoint();
		query_point.push_back(p);
		point_list.push_back(p);
		for (size_t i = 0; i < this->points_set.size(); i++) {
			std::vector<Point> query_points_pair;
			query_points_pair.push_back(p);
			Point p2(-1, this->points_set[i].first, this->points_set[i].second);
			point_list.push_back(p2);
			query_points_pair.push_back(p2);
			final_hour = find_shortest_path(query_points_pair); // RETURNS SINGLE FINAL HOURGLASS FOR THE TWO POINTS IN THE INPUT VECTOR
				
			// Save result 
			this->Strings.push_back(this->getStringPoints());

			// clear for next test point
			point_list.pop_back();
		}
		point_list.pop_back();
		return 1;
	}
	else {
		return 0;
	}
}

void DNN_Util::deleteQueryPoint() {
	if (query_point.empty() == false) {
		query_point.pop_back();
		for (size_t i = 0; i < this->Strings.size(); i++) {
			this->Strings[i].clear();
		}
		this->Strings.clear();
	}
}

double DNN_Util::getPathLength() {
	double result = 0;
	std::vector<std::pair<double, double>> path_points = this->getStringPoints();
	for (size_t i = 0; i < path_points.size() - 1; i++) {
		result += std::sqrt(std::pow(path_points[i].first - path_points[i + 1].first, 2)
			+ std::pow(path_points[i].second - path_points[i + 1].second, 2));
	}
	return result;
}

double DNN_Util::getPathLength(std::vector<std::pair<double, double>> path_points) {
	double result = 0;
	for (size_t i = 0; i < path_points.size() - 1; i++) {
		result += std::sqrt(std::pow(path_points[i].first - path_points[i + 1].first, 2)
			+ std::pow(path_points[i].second - path_points[i + 1].second, 2));
	}
	return result;
}

void DNN_Util::printQuery(std::string fileName) {
	if (query_point.size() >= 1) {
		std::ofstream writeFile(fileName);
		if (writeFile.is_open()) {
			int idx = this->Strings.size();
			if (idx > this->knn) {
				idx = knn;
			}
			writeFile << idx << "\n";

			std::vector<std::pair<double, int>> query_result = this->getQueryResult();
			for (int i = 0; i < idx; i++) {
				this->points_set;
				writeFile << this->points_set[query_result[i].second].first << ' ' << this->points_set[query_result[i].second].second << ' ' << query_result[i].first << "\n";
				auto points = this->Strings[query_result[i].second];
				writeFile << points.size() << "\n";
				for (size_t i = 0; i < points.size(); i++) {
					writeFile << points[i].first << ' ' << points[i].second << "\n";
				}
			}
			writeFile.close();
		}
	}
}

std::vector<std::pair<double, double>> DNN_Util::getPolygonVertices() {
	std::vector<std::pair<double, double>> result;
	for (size_t i = 0; i < input_polygon.size(); i++) {
		result.push_back(std::make_pair(input_polygon[i].get_x(), input_polygon[i].get_y()));
	}
	return result;
}

std::pair<double, double> DNN_Util::getPolygonVerticesBack() {
	if (input_polygon.empty()) {
		return std::make_pair(0.0,0.0);
	}
	else {
		return std::make_pair(input_polygon.back().get_x(), input_polygon.back().get_y());
	}
}

int DNN_Util::getNumPolygonVertices() {
	return input_polygon.size();
}

std::vector<std::pair<double, double>> DNN_Util::getInputPoints() {
	/*
	std::vector<std::pair<double, double>> result;
	for (size_t i = 0; i < test_points.size(); i++) {
		result.push_back(std::make_pair(test_points[i].get_x(), test_points[i].get_y()));
	}
	return result;
	*/
	return this->points_set;
}

std::pair<double, double> DNN_Util::getInputPointsBack() {
	/*
	if (test_points.empty()) {
		return std::make_pair(0.0, 0.0);
	}
	else {
		return std::make_pair(test_points.back().get_x(), test_points.back().get_y());
	}
	*/
	if (this->points_set.empty()) {
		return std::make_pair(0.0, 0.0);
	}
	else {
		return std::make_pair(this->points_set.back().first, this->points_set.back().second);
	}
}

int DNN_Util::getNumInputPoints() {
	return this->points_set.size();
}

std::vector<std::pair<double, double>> DNN_Util::getQueryPoint() {
	std::vector<std::pair<double, double>> result;
	for (size_t i = 0; i < query_point.size(); i++) {
		result.push_back(std::make_pair(query_point[i].get_x(), query_point[i].get_y()));
	}
	return result;
}

int DNN_Util::getNumQueryPoint() {
	return query_point.size();
}

std::vector<std::pair<double, double>> DNN_Util::getPathPoints() {
	std::vector<std::pair<double, double>> result;
	if (test_points.size() >= 2) {
		Chain** first_chain = final_hour.get_first_chain();
		for (int i = 0; i < 2; i++) {
			if (first_chain[i] == NULL) continue;
			vector<int> p0_list = first_chain[i]->get_point_list();
			for (size_t j = 0; j < p0_list.size(); j++) {
				result.push_back(std::make_pair(point_list[p0_list[j]].get_x(), point_list[p0_list[j]].get_y()));
			}
		}
		Chain** second_chain = final_hour.get_second_chain();
		for (int i = 0; i < 2; i++) {
			if (second_chain[i] == NULL) continue;
			vector<int> p0_list = second_chain[i]->get_point_list();
			for (size_t j = 0; j < p0_list.size(); j++) {
				result.push_back(std::make_pair(point_list[p0_list[j]].get_x(), point_list[p0_list[j]].get_y()));
			}
		}
	}
	return result;
}

std::vector<std::pair<double, double>> DNN_Util::getStringPoints() {
	std::vector<std::pair<double, double>> result;
	//if (test_points.size() >= 2) {
		String* s = final_hour.get_string();
		if (s != NULL) {
			vector<int> p0_list = s->get_chain()->get_point_list();
			for (size_t j = 0; j < p0_list.size(); j++) {
				result.push_back(std::make_pair(point_list[p0_list[j]].get_x(), point_list[p0_list[j]].get_y()));
			}
		}
	//}
	return result;
}

std::vector<std::pair<double, int>> DNN_Util::getQueryResult() {
	std::vector<std::pair<double, int>> result;
	for (size_t i = 0; i < this->Strings.size(); i++) {
		result.push_back(std::make_pair(this->getPathLength(this->Strings[i]), i));
	}
	std::sort(result.begin(),result.end());
	return result;
}
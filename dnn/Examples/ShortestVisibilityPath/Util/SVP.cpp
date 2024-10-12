#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "SVP.h"
#include "NODE.h"
#include "Edge.h"
#include "Point.h"
#include "Tree.h"
#include "VertexID.h"
#include "PointS.h"
#include "SNode.h"
#include "polygon_operation.h"
#include "polygon_decomposition.h"
#include "hourglass_operation.h"
#include "ShortestPathTree.h"
#include "event_computation.h"
#include <time.h>

#define NULL_HELPER -1
#define PI 3.1415926535897931
using namespace std;

//Hourglass find_shortest_path_test_points(vector<Point> test_points);
Hourglass find_shortest_path(int p1, int p2);
Hourglass find_shortest_path_test_points();
vector<Point> test_points;
vector<int> sequence_diagonal;
int d_size;
Hourglass final_hour;

int test_point_index;
bool displayAllEvents = false;

Hourglass test_hourglass;

////////////////////
vector<Point> polygon_boundary;
////////////////////

//global variables for display using openGL functions
vector<Point> shortest_path;
vector<Point> shortest_path_to_line[2];
int firstIdx = 0;
int secondIdx = 0;
EVENTS Events;
double length = 0;
enum MODE {
	MIN_MAX,
	MIN_SUM
};

MODE mode = MIN_MAX;
Hourglass construct_hourglass_point_line(int p, Edge e);
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
	SNode* s1 = t1.get_node(), *s2 = t2.get_node();
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
					bool b1 = findPath(&t_list[t[i]], dummy, &t_list[_t1]);
					bool b2 = findPath(&t_list[t[i]], dummy, &t_list[_t2]);
					if (((&t_list[t[i]]) != (&t_list[_t1])) && ((&t_list[t[i]]) != (&t_list[_t2])) && (!b1 || b2) && (b1 || !b2) && (common_low_triangle != &t_list[t[i]]))
						inclusive = false;
				}
			}
			if (inclusive) sequence_diagonal.push_back((*rit)->get_diagonal());
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
			if (inclusive) sequence_diagonal.push_back((*it)->get_diagonal());
		}
	}

	return common_ancestor;
}

SNode * find_common_ancestor(int _t1, int _t2, vector<int>& diagonal_list) {//input: triangle where the test_points are located
	// to store paths to n1 and n2 from the root 
	vector<SNode *> path1, path2;
	diagonal_list = vector<int>();

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
					bool b1 = findPath(&t_list[t[i]],dummy, &t_list[_t1]);
					bool b2 = findPath(&t_list[t[i]],dummy,&t_list[_t2]);
					if (((&t_list[t[i]])!=(&t_list[_t1]))&&((&t_list[t[i]]) != (&t_list[_t2]))&&(!b1 || b2) && (b1 || !b2) && (common_low_triangle != &t_list[t[i]]))
						inclusive = false;
				}
			}

			if(inclusive) diagonal_list.push_back((*rit)->get_diagonal());
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
			if(inclusive) diagonal_list.push_back((*it)->get_diagonal());
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
//sets the first & second chain / apex / string of the hourglass
Hourglass construct_hourglass_point_line(int p, Edge e) 
{
	int origin = e.get_origin();
	int dest = e.get_dest();
	Hourglass hourglass;
	hourglass.set_first_edge(Edge(p,p));
	hourglass.set_second_edge(e);
	vector<int> string;

	//origin and dest must be in the point_list
	Hourglass p_to_origin = find_shortest_path(p,origin);
	Hourglass p_to_dest = find_shortest_path(p,dest); 

	vector<int> origin_chain = p_to_origin.get_string_point_list();
	vector<int> dest_chain = p_to_dest.get_string_point_list();
	//vector<int> origin_chain = p_to_origin.get_string()->get_chain()->get_point_list();
	
	//vector<int> dest_chain = p_to_dest.get_string()->get_chain()->get_point_list();

	if(origin_chain.front()!=p)
		reverse(origin_chain.begin(), origin_chain.end());
	if(dest_chain.front()!=p)
		reverse(dest_chain.begin(), dest_chain.end());

	int i;
	for (i = 0; i < origin_chain.size() && i < dest_chain.size(); i++)
	{
		if (origin_chain[i] != dest_chain[i])//i holds the index of the next element following the apex
			break;
		string.push_back(origin_chain[i]);
	}
	int apex = origin_chain[i - 1];

	Chain* first_chain[2];
	first_chain[0] = new Chain(p, p);
	first_chain[1] = new Chain(p, p);
	hourglass.set_first_chain(first_chain);

	Chain* second_chain[2];
	vector<int> apex_to_origin;
	vector<int> apex_to_dest;

	apex_to_origin.push_back(apex);
	apex_to_dest.push_back(apex);
	apex_to_origin.insert(apex_to_origin.end(), origin_chain.begin() + i, origin_chain.end());
	apex_to_dest.insert(apex_to_dest.end(), dest_chain.begin() + i, dest_chain.end());
	second_chain[0] = new Chain(apex_to_origin);
	second_chain[1] = new Chain(apex_to_dest);
	hourglass.set_second_chain(second_chain);

	hourglass.set_apex(p, 0);
	hourglass.set_apex(apex, 1);

	if (string.size() > 1) //-> we can't have an empty string
	//must call the function get_chain later on
	hourglass.set_string(new String(string));

	return hourglass;
}

Hourglass find_shortest_path(int p1, int p2) //input : two test points , returns final hourglass(string) representing shortest path of the two points
{
	selected_triangle = vector<int>();
	Point P1 = point_list[p1];
	Point P2 = point_list[p2];

	int found_triangle = point_state.find_triangle(P1);
	selected_triangle.push_back(found_triangle);
	found_triangle = point_state.find_triangle(P2);
	selected_triangle.push_back(found_triangle);

	Hourglass final_hourglass;
	if (selected_triangle[0] == selected_triangle[1]) {
		final_hourglass.set_string(new String(p2,p1));
		final_hourglass.set_first_edge(Edge(p2));
		final_hourglass.set_second_edge(Edge(p1));
		return final_hourglass;
	}

	vector<int> sequence_diagonal_local;
	SNode * common_ancestor = find_common_ancestor(selected_triangle[0], selected_triangle[1], sequence_diagonal_local);
	Hourglass origin, dest;

	origin = construct_hourglass_point(p2, sequence_diagonal_local.front());
	dest = construct_hourglass_point(p1, sequence_diagonal_local.back());
	if (sequence_diagonal_local.size() == 1) {
		final_hourglass = concatenate_hourglasses(origin, dest);
	}
	else if (sequence_diagonal_local.size() >= 2) {
		int h_num = s_graph[sequence_diagonal_local[0]][sequence_diagonal_local[1]];
		h_num = (h_num == -1) ? s_graph[sequence_diagonal_local[1]][sequence_diagonal_local[0]] : h_num;
		final_hourglass = hourglass_list[h_num];

		Hourglass temp;
		for (int i = 2; i < (int)sequence_diagonal_local.size(); i++) {
			int d_1 = sequence_diagonal_local[i - 1];
			int d_2 = sequence_diagonal_local[i];
			int h_num = s_graph[d_1][d_2];
			h_num = (h_num == -1) ? s_graph[d_2][d_1] : h_num;
			temp = hourglass_list[h_num];
			final_hourglass = concatenate_hourglasses(temp, final_hourglass);
		}
		final_hourglass = concatenate_hourglasses(origin, final_hourglass);
		final_hourglass = concatenate_hourglasses(final_hourglass, dest);
	}

	//running this function adds two diagonals in diagonal_list
	//the following part deletes the recently added two
	for (int i = 0; i<2; i++)
		diagonal_list.pop_back();

	return final_hourglass;
}
/*input : two test points , returns final hourglass(string) representing shortest path of the two points*/
Hourglass find_shortest_path_test_points()
{
	selected_triangle = vector<int>();
	sequence_diagonal = vector<int>();
	for (int i = 0; i < (int)test_points.size(); i++) {
		int found_triangle = point_state.find_triangle(test_points[i]);
		selected_triangle.push_back(found_triangle);
	}
	Hourglass final_hourglass;
	if (selected_triangle[0] == selected_triangle[1]) {
		final_hourglass.set_string(new String(test_point_index+1, test_point_index));
		final_hourglass.set_first_edge(Edge(test_point_index+1));
		final_hourglass.set_second_edge(Edge(test_point_index));
		return final_hourglass;
	}
	SNode * common_ancestor = find_common_ancestor(selected_triangle[0], selected_triangle[1]);
	Hourglass origin, dest;

	origin = construct_hourglass_point(test_point_index+1, sequence_diagonal.front());
	dest = construct_hourglass_point(test_point_index, sequence_diagonal.back());
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

SVP::SVP() {
	polygon_list = vector<vector<int>>();
	diagonal_list = vector<Edge>();
	outer_diagonal_list = vector<Edge>();
	selected_triangle = vector<int>();
	outer_edge_list = vector<Edge>();
	sequence_diagonal = vector<int>();
	null_edge_list = vector<Edge*>();
	init_hourglass_val();
}

SVP::~SVP() {

}

void SVP::clearData() {
	polygon_list = vector<vector<int>>();
	diagonal_list = vector<Edge>();
	outer_diagonal_list = vector<Edge>();
	selected_triangle = vector<int>();
	outer_edge_list = vector<Edge>();
	sequence_diagonal = vector<int>();
	null_edge_list = vector<Edge*>();
	init_hourglass_val();

	input_polygon.clear();
	point_list.clear();
	polygon_boundary.clear();
	outer_polygon_list.clear();

	vertex_triangle_list.clear();
	triangle_edge_list.clear();

	id_num = 0;
	common_edge = NULL;

	this->clearTestPoint();
}

void SVP::setMode(int m) {
	mode = MODE(m);
}

int SVP::getMode() {
	return int(mode);
}

void SVP::addVertex(int x, int y) {
	Point new_point(x, y);
	input_polygon.push_back(new_point);
}

void SVP::deleteVertex() {
	if (input_polygon.empty() == false) {
		input_polygon.pop_back();
	}
}

void SVP::printInputPolygon(std::string fileName) {
	std::ofstream writeFile(fileName);
	if (writeFile.is_open()) {
		writeFile << input_polygon.size() << std::endl;
		for (size_t i = 0; i < input_polygon.size(); i++) {
			writeFile << input_polygon[i].get_x() << ' ' << input_polygon[i].get_y() << '\n';
		}
		writeFile.close();
	}
}

int SVP::readPolygon(std::string fileName) {
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
		readFile.close();
		return 1;
	}
	return 0;
}

void SVP::preprocessing() {
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

	diagonal_with_edge_list = diagonal_list;
	diagonal_with_edge_list[d_size + v_num - 1].set_dest(0);
	diagonal_list = vector<Edge>(diagonal_list.begin(), diagonal_list.begin() + d_size);

	/*initializing vertex list*/
	for (int i = 0; i < v_num; i++)
	{
		vector<int> newV;
		vertex_triangle_list.push_back(newV);
		triangle_edge_list.push_back(newV);
	}
	for (int i = 0; i < polygon_list.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertex_triangle_list[polygon_list[i][j]].push_back(i);
		}
	}
	for (int i = 0; i < diagonal_list.size(); i++)
	{
		int* tri_list = diagonal_list[i].get_triangle();
		for (int j = 0; j < 2; j++)
			triangle_edge_list[tri_list[j]].push_back(i);
	}
	for (int i = 0; i < v_num; i++)
	{
		int j = (i + 1) % v_num;

		vector<int> list1 = vertex_triangle_list[i];
		vector<int> list2 = vertex_triangle_list[j];
		vector<int> res(1);
		set_intersection(list1.begin(), list1.end(), list2.begin(), list2.end(), res.begin());

		triangle_edge_list[res[0]].push_back(i + d_size);
		//common element in list 1 and list2
	}

	point_state = PointS();
	while (point_state.step());
}

int SVP::addQueryPoint(double x, double y) {
	Point p(-1, x, y);
	int test_tri = point_state.find_triangle(p);
	if (test_tri < (int)polygon_list.size() && (int)test_points.size() < 2) {
		test_points.push_back(p);
		point_list.push_back(p);

		if (test_points.size() == 2) {
			SPT* spt_s = new SPT(point_list.size() - 2, point_list.size() - 1);
			SPT* spt_t = new SPT(point_list.size() - 1, point_list.size() - 2);
			spt_s->compute_shortest_path_tree();
			spt_t->compute_shortest_path_tree();

			vector<int> tpath = spt_t->retrieve_shortest_path_default();
			vector<int> spath = spt_s->retrieve_shortest_path_default();

			shortest_path = vector<Point>();
			for (int i = 0; i < spath.size(); i++)
				shortest_path.push_back(point_list[spath[i]]);

			//make sure the shortest path tree is computed for each spt
			EVENTS* events = new EVENTS(spath, spt_s, spt_t);
			events->compute_path_events();
			events->compute_boundary_events();
			events->compute_bend_events();
			//events->compute_min_sum();
			//events->compute_min_max();
			events->compute_min_line();
			std::pair<int, int> minmaxIdx = events->compute_minmax_idx();
			std::pair<int, int> minsumIdx = events->compute_minsum_idx();

			Events = *events;

			// Save paths
			/*
			vector<vector<LINE*>> Queue = Events.getQueue();
			if (!Queue.empty() && !Queue.front().empty()) {
				shortest_path_to_line[0] = Queue[firstIdx][secondIdx]->getShortestPath(0);
				shortest_path_to_line[1] = Queue[firstIdx][secondIdx]->getShortestPath(1);
			}
			*/
			vector<vector<LINE*>> Queue = Events.getQueue();
			shortest_path_to_line[0] = Queue[minmaxIdx.first][minmaxIdx.second]->getShortestPath(0);
			shortest_path_to_line[1] = Queue[minmaxIdx.first][minmaxIdx.second]->getShortestPath(1);
			std::vector<std::pair<double, double>> temp1;
			for (size_t i = 0; i < shortest_path_to_line[0].size(); i++) {
				temp1.push_back(std::make_pair(shortest_path_to_line[0][i].get_x(),
					shortest_path_to_line[0][i].get_y()));
			}
			this->minmaxStrings.push_back(temp1);
			std::vector<std::pair<double, double>> temp2;
			for (size_t i = 0; i < shortest_path_to_line[1].size(); i++) {
				temp2.push_back(std::make_pair(shortest_path_to_line[1][i].get_x(),
					shortest_path_to_line[1][i].get_y()));
			}
			this->minmaxStrings.push_back(temp2);
			
			shortest_path_to_line[0] = Queue[minsumIdx.first][minsumIdx.second]->getShortestPath(0);
			shortest_path_to_line[1] = Queue[minsumIdx.first][minsumIdx.second]->getShortestPath(1);
			std::vector<std::pair<double, double>> temp3;
			for (size_t i = 0; i < shortest_path_to_line[0].size(); i++) {
				temp3.push_back(std::make_pair(shortest_path_to_line[0][i].get_x(),
					shortest_path_to_line[0][i].get_y()));
			}
			this->minsumStrings.push_back(temp3);
			std::vector<std::pair<double, double>> temp4;
			for (size_t i = 0; i < shortest_path_to_line[1].size(); i++) {
				temp4.push_back(std::make_pair(shortest_path_to_line[1][i].get_x(),
					shortest_path_to_line[1][i].get_y()));
			}
			this->minsumStrings.push_back(temp4);


			if (minMaxLine != nullptr) {
				Point* EndP = minMaxLine->getEndpoints();
				Point p1 = EndP[0], p2 = EndP[1];
				this->minmaxLine.push_back(std::make_pair(p1.get_x(), p1.get_y()));
				this->minmaxLine.push_back(std::make_pair(p2.get_x(), p2.get_y()));
			}
			if (minSumLine != nullptr) {
				Point* EndP = minSumLine->getEndpoints();
				Point p1 = EndP[0], p2 = EndP[1];
				this->minsumLine.push_back(std::make_pair(p1.get_x(), p1.get_y()));
				this->minsumLine.push_back(std::make_pair(p2.get_x(), p2.get_y()));
			}
		}
		return 1;
	}
	else return 0;
}

void SVP::clearTestPoint() {
	shortest_path = vector<Point>();
	shortest_path_to_line[0] = vector<Point>();
	shortest_path_to_line[1] = vector<Point>();
	minSumLine = NULL;
	minMaxLine = NULL;
	displayAllEvents = false;
	Events = *(new EVENTS());
	firstIdx = 0;
	secondIdx = 0;

	for (int i = 0; i < (int)test_points.size(); i++)
		point_list.pop_back();
	test_points = vector<Point>();
	selected_triangle = vector<int>();
	sequence_diagonal = vector<int>();
	for (size_t i = 0; i < this->minmaxStrings.size(); i++) {
		this->minmaxStrings[i].clear();
	}
	this->minmaxStrings.clear();
	for (size_t i = 0; i < this->minsumStrings.size(); i++) {
		this->minsumStrings[i].clear();
	}
	this->minsumStrings.clear();
}


double SVP::getPathLength() {
	double result = 0;
	if (test_points.size() > 1) {
		// TODO
		if (mode == MIN_MAX) {
			for (size_t i = 0; i < this->minmaxStrings.size(); i++) {
				double temp = 0;
				for (size_t j = 0; j < this->minmaxStrings[i].size() - 1; j++) {
					temp += std::sqrt(std::pow(this->minmaxStrings[i][j].first - this->minmaxStrings[i][j + 1].first, 2)
						+ std::pow(this->minmaxStrings[i][j].second - this->minmaxStrings[i][j + 1].second, 2));
				}
				if (result < temp) result = temp;
			}			
		}
		else if (mode == MIN_SUM) {
			double temp = 0;
			for (size_t i = 0; i < this->minsumStrings.size(); i++) {
				for (size_t j = 0; j < this->minsumStrings[i].size() - 1; j++) {
					temp += std::sqrt(std::pow(this->minsumStrings[i][j].first - this->minsumStrings[i][j + 1].first, 2)
						+ std::pow(this->minsumStrings[i][j].second - this->minsumStrings[i][j + 1].second, 2));
				}
			}
			result += temp;
		}
		return result;
	}
	else {
		return result;
	}
}

double SVP::getSourcePathLength() {
	double result = 0;
	if (test_points.size() > 1) {
		// TODO
		if (mode == MIN_MAX) {
			if (this->minmaxStrings.size() > 0) {
				for (size_t j = 0; j < this->minmaxStrings[0].size() - 1; j++) {
					result += std::sqrt(std::pow(this->minmaxStrings[0][j].first - this->minmaxStrings[0][j + 1].first, 2)
						+ std::pow(this->minmaxStrings[0][j].second - this->minmaxStrings[0][j + 1].second, 2));
				}
			}
		}
		else if (mode == MIN_SUM) {
			if (this->minsumStrings.size() > 0) {
				for (size_t j = 0; j < this->minsumStrings[0].size() - 1; j++) {
					result += std::sqrt(std::pow(this->minsumStrings[0][j].first - this->minsumStrings[0][j + 1].first, 2)
						+ std::pow(this->minsumStrings[0][j].second - this->minsumStrings[0][j + 1].second, 2));
				}
			}
		}
		return result;
	}
	else {
		return result;
	}
}

double SVP::getTargetPathLength() {
	double result = 0;
	if (test_points.size() > 1) {
		// TODO
		if (mode == MIN_MAX) {
			if (this->minmaxStrings.size() > 1) {
				for (size_t j = 0; j < this->minmaxStrings[1].size() - 1; j++) {
					result += std::sqrt(std::pow(this->minmaxStrings[1][j].first - this->minmaxStrings[1][j + 1].first, 2)
						+ std::pow(this->minmaxStrings[1][j].second - this->minmaxStrings[1][j + 1].second, 2));
				}
			}
		}
		else if (mode == MIN_SUM) {
			if (this->minsumStrings.size() > 1) {
				for (size_t j = 0; j < this->minsumStrings[1].size() - 1; j++) {
					result += std::sqrt(std::pow(this->minsumStrings[1][j].first - this->minsumStrings[1][j + 1].first, 2)
						+ std::pow(this->minsumStrings[1][j].second - this->minsumStrings[1][j + 1].second, 2));
				}
			}
		}
		return result;
	}
	else {
		return result;
	}
}

void SVP::printQuery(std::string fileName) {
	if (test_points.size() >= 2) {
		std::ofstream writeFile(fileName);
		if (writeFile.is_open()) {
			if (mode == MIN_MAX) {
				writeFile << "Source point path\n";
				if (this->minmaxStrings.size() > 0) {
					for (size_t i = 0; i < this->minmaxStrings[0].size(); i++) {
						writeFile << this->minmaxStrings[0][i].first << ' ' <<
							this->minmaxStrings[0][i].second << '\n';
					}
				}
				writeFile << "Target point path\n";
				if (this->minmaxStrings.size() > 1) {
					for (size_t i = 0; i < this->minmaxStrings[1].size(); i++) {
						writeFile << this->minmaxStrings[1][i].first << ' ' <<
							this->minmaxStrings[1][i].second << '\n';
					}
				}
			}
			
			else if (mode == MIN_SUM) {
				writeFile << "Source point path\n";
				if (this->minsumStrings.size() > 0) {
					for (size_t i = 0; i < this->minsumStrings[0].size(); i++) {
						writeFile << this->minsumStrings[0][i].first << ' ' <<
							this->minsumStrings[0][i].second << '\n';
					}
				}
				writeFile << "Target point path\n";
				if (this->minsumStrings.size() > 1) {
					for (size_t i = 0; i < this->minsumStrings[1].size(); i++) {
						writeFile << this->minsumStrings[1][i].first << ' ' <<
							this->minsumStrings[1][i].second << '\n';
					}
				}
			}
			writeFile.close();
		}
	}
}

std::vector<std::pair<double, double>> SVP::getPolygonPoints() {
	std::vector<std::pair<double, double>> result;
	for (size_t i = 0; i < input_polygon.size(); i++) {
		result.push_back(std::make_pair(input_polygon[i].get_x(), input_polygon[i].get_y()));
	}
	return result;
}

int SVP::getNumPolygonVertices() {
	return input_polygon.size();
}

std::vector<std::pair<double, double>> SVP::getQueryPoints() {
	std::vector<std::pair<double, double>> result;
	for (size_t i = 0; i < test_points.size(); i++) {
		result.push_back(std::make_pair(test_points[i].get_x(), test_points[i].get_y()));
	}
	return result;
}

int SVP::getNumQueryPoint() {
	return test_points.size();
}

#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Edge.h"
#include "Tree.h"
#include "VertexID.h"
#define NULL_HELPER -1
#define PI 3.1415926535897931
using namespace std;

//vector<Point> point_list;
vector<vector<int>> polygon_list;
vector<vector<int>> outer_polygon_list;
vector<vector<int>> triangle_list;
vector<Edge> diagonal_list;
vector<Edge> outer_edge_list;
vector<Edge> outer_diagonal_list;
vector<Edge *> null_edge_list;
vector<int> selected_triangle;
int v_num;
int bigT[3];

TRIANGLE_TREE tree;
point_type dist(int p1, int p2) {
	point_type diff_x = (point_list[p1].get_x() - point_list[p2].get_x());
	point_type diff_y = (point_list[p1].get_y() - point_list[p2].get_y());
	return sqrt(pow(diff_x, 2) + pow(diff_y, 2));
}

double calculate_angle(int origin, int target) {
	point_type x = point_list[target].get_x() - point_list[origin].get_x();
	point_type y = point_list[target].get_y() - point_list[origin].get_y();
	return atan2(y, x);
}
double calculate_angle(int origin, int target, int reverse) {
	point_type x = point_list[target].get_x(reverse) - point_list[origin].get_x(reverse);
	point_type y = point_list[target].get_y(reverse) - point_list[origin].get_y(reverse);
	return atan2(y, x);
}
int get_left_vertex(vector<int>& polygon, int polygonID) {
	return polygon[(polygonID - 1 + polygon.size()) % (polygon.size())];
}
int get_right_vertex(vector<int>& polygon, int polygonID) {
	return polygon[(polygonID + 1 + polygon.size()) % (polygon.size())];
}
int get_left_vertex(vector<vector<int>>& polygon_list, int polygonID, int polygon_id) {
	return polygon_list[polygon_id][(polygonID - 1 + polygon_list[polygon_id].size()) % (polygon_list[polygon_id].size())];
}
int get_right_vertex(vector<vector<int>>& polygon_list, int polygonID, int polygon_id) {
	return  polygon_list[polygon_id][(polygonID + 1) % (polygon_list[polygon_id].size())];
}
bool y_DOWNUP_comp(VertexID a, VertexID b) {
	if (point_list[a.get_pointID()].get_y() < point_list[b.get_pointID()].get_y()) {
		return true;
	}
	else if (point_list[a.get_pointID()].get_y() > point_list[b.get_pointID()].get_y()) {
		return false;
	}
	else {
		if (point_list[a.get_pointID()].get_x() <= point_list[b.get_pointID()].get_x())
			return false;
		else return true;
	}
	return true;
}
bool y_UPDOWN_comp(VertexID a, VertexID b) {

	if (point_list[a.get_pointID()].get_y() > point_list[b.get_pointID()].get_y()) {
		return true;
	}
	else if (point_list[a.get_pointID()].get_y() < point_list[b.get_pointID()].get_y()) {
		return false;
	}
	else {
		if (point_list[a.get_pointID()].get_x() >= point_list[b.get_pointID()].get_x())
			return false;
		else return true;
	}
	return true;
}
Edge get_edge(int d_num) {
	int d_size = diagonal_list.size();
	return d_num >= d_size ? outer_edge_list[d_num - d_size] : diagonal_list[d_num];
}
bool check_line_intersection(int point1, int point2, int point3, int point4) {
	Point p1 = point_list[point1];
	Point p2 = point_list[point2];
	Point p3 = point_list[point3];
	Point p4 = point_list[point4];

	point_type ua = (p4.get_x() - p3.get_x()) * (p1.get_y() - p3.get_y()) - (p4.get_y() - p3.get_y()) * (p1.get_x() - p3.get_x());
	point_type ub = (p2.get_x() - p1.get_x()) * (p1.get_y() - p3.get_y()) - (p2.get_y() - p1.get_y()) * (p1.get_x() - p3.get_x());
	point_type denominator = (p4.get_y() - p3.get_y()) * (p2.get_x() - p1.get_x()) - (p4.get_x() - p3.get_x()) * (p2.get_y() - p1.get_y());

	bool intersection = false;

	if ((point_type)abs(denominator) >= 0.00001f)
	{
		ua /= denominator;
		ub /= denominator;

		if (ua > 0.0 && ua < 1.0 && ub > 0.0 && ub < 1.0)
		{
			intersection = true;
			//intersectionPoint.X = point1.X + ua * (point2.X - point1.X);
			//intersectionPoint.Y = point1.Y + ua * (point2.Y - point1.Y);
		}
	}
	return intersection;
}
// isLeft(): test if a point is Left|On|Right of an infinite line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 and P1
//            =0 for P2 on the line
//            <0 for P2 right of the line
//
inline point_type
isLeft(int p0, int p1, int p2)
{
	Point P0 = point_list[p0];
	Point P1 = point_list[p1];
	Point P2 = point_list[p2];

	return (P1.get_x() - P0.get_x())*(P2.get_y() - P0.get_y()) - (P2.get_x() - P0.get_x())*(P1.get_y() - P0.get_y());
}

bool is_left(int p0, int p1, int p2)
{
	Point P0 = point_list[p0];
	Point P1 = point_list[p1];
	Point P2 = point_list[p2];

	if ((P1.get_x() - P0.get_x())*(P2.get_y() - P0.get_y()) - (P2.get_x() - P0.get_x())*(P1.get_y() - P0.get_y())>=0)
		return true;
	else return false;
}
bool is_right(int p0, int p1, int p2)
{
	Point P0 = point_list[p0];
	Point P1 = point_list[p1];
	Point P2 = point_list[p2];

	if ((P1.get_x() - P0.get_x())*(P2.get_y() - P0.get_y()) - (P2.get_x() - P0.get_x())*(P1.get_y() - P0.get_y()) <= 0)
		return true;
	else return false;
}



// tests for polygon vertex ordering relative to a fixed point P
#define above(P,Vi,Vj)  (isLeft(P,Vi,Vj) >= 0)   // true if Vi is above Vj
#define below(P,Vi,Vj)  (isLeft(P,Vi,Vj) <= 0)   // true if Vi is below Vj
//===================================================================
//고쳐야 함
void seperate_polygon_with_edge(vector<vector<int>>& polygon_list_with_edge, int e_num) {
	Edge e = diagonal_list[e_num];
	for (int j = 0; j < int(polygon_list_with_edge.size()); j++) {
		int check = 0;
		vector<int>::iterator first, second;
		int checked_point[2] = { -1,-1 };
		//Edge e1, e2, e3, e4;
		first = second = polygon_list_with_edge[j].begin();
		for (vector<int>::iterator iter = polygon_list_with_edge[j].begin(); iter != polygon_list_with_edge[j].end(); iter++) {
			Edge check_edge = get_edge(*iter);
			int same_point = check_edge.check_same_point(e);
			if (same_point != -1 && checked_point[0]!=same_point && checked_point[1] != same_point) {
				if (check == 0) {
					first = iter;
					checked_point[0] = same_point;
				}
				else {
					second = iter;
					checked_point[1] = same_point;
				}
				check++;
			}
		}
		if (check == 2) {
			Edge e2 = get_edge(*(first + 1));
			vector<int> new_v;
			if (first == polygon_list_with_edge[j].begin() && e2.check_same_point(checked_point[0]) == -1) {
				new_v = vector<int>(first, second+1);
				new_v.push_back(e_num);
				polygon_list_with_edge[j].erase(first, second+1);
				polygon_list_with_edge[j].insert(polygon_list_with_edge[j].begin(),e_num);
				
			}
			else {
				new_v = vector<int>(first+1, second + 1);
				new_v.push_back(e_num);
				vector<int> temp = vector<int>(second+1, polygon_list_with_edge[j].end());
				polygon_list_with_edge[j].erase(first+1, polygon_list_with_edge[j].end());
				polygon_list_with_edge[j].push_back(e_num);
				polygon_list_with_edge[j].insert(polygon_list_with_edge[j].end(), temp.begin(), temp.end());
			}
			polygon_list_with_edge.push_back(new_v);
			j = int(polygon_list_with_edge.size());
		}
	}
}
void seperate_polygon(vector<vector<int>>& polygon_list, Edge e) {
	for (int j = 0; j < int(polygon_list.size()); j++) {
		int check = 0;
		vector<int>::iterator first, second;
		first = second = polygon_list[j].begin();
		for (vector<int>::iterator iter = polygon_list[j].begin(); iter != polygon_list[j].end(); iter++) {
			if ((*iter) == e.get_origin() || (*iter) == e.get_dest()) {
				if (check == 0) first = iter;
				else second = iter;
				check++;
			}
		}
		if (check == 2) {
			vector<int> new_v = vector<int>(first, second + 1);
			polygon_list[j].erase(first + 1, second);
			polygon_list.push_back(new_v);
			j = int(polygon_list.size());
		}
	}
}
//should be modified
void seperate_polygon(vector<vector<int>>& polygon_list, vector<Edge> & diagonal_list) {
	for (int i = 0; i < int(diagonal_list.size()); i++) {
		Edge e = diagonal_list[i];
		for (int j = 0; j < int(polygon_list.size()); j++) {
			int check = 0;
			vector<int>::iterator first, second;
			first = second = polygon_list[j].begin();
			for (vector<int>::iterator iter = polygon_list[j].begin(); iter != polygon_list[j].end(); iter++) {
				if ((*iter) == e.get_origin() || (*iter) == e.get_dest()) {
					if (check == 0) first = iter;
					else second = iter;
					check++;
				}
			}
			if (check == 2) {
				vector<int> new_v = vector<int>(first, second + 1);
				polygon_list[j].erase(first + 1, second);
				polygon_list.push_back(new_v);
				j = int(polygon_list.size());
			}
		}
	}
}
vector<vector<int>> seperate_polygon(vector<int>& polygon, vector<Edge> & diagonal_list) {
	
	vector<vector<int>> polygon_list;
	polygon_list.push_back(polygon);
	for (int i = 0; i < int(diagonal_list.size()); i++) {
		Edge e = diagonal_list[i];
		for (int j = 0; j < int(polygon_list.size()); j++) {
			int check = 0;
			vector<int>::iterator first, second;
			first = second = polygon_list[j].begin();
			for (vector<int>::iterator iter = polygon_list[j].begin(); iter != polygon_list[j].end(); iter++) {
				if ((*iter) == e.get_origin() || (*iter) == e.get_dest()) {
					if (check == 0) first = iter;
					else second = iter;
					check++;
				}
			}
			if (check == 2) {
				vector<int> new_v = vector<int>(first, second + 1);
				polygon_list[j].erase(first + 1, second);
				polygon_list.push_back(new_v);
				break;
			}
		}
	}
	return polygon_list;
}
void seperate_polygon(vector<vector<int>>& polygon_list, vector<Edge> & diagonal_list, int& read_diagonal) {
	for (int i = read_diagonal; i < int(diagonal_list.size()); i++) {
		Edge e = diagonal_list[i];
		for (int j = 0; j < int(polygon_list.size()); j++) {
			int check = 0;
			vector<int>::iterator first, second;
			first = second = polygon_list[j].begin();
			for (vector<int>::iterator iter = polygon_list[j].begin(); iter != polygon_list[j].end(); iter++) {
				if ((*iter) == e.get_origin() || (*iter) == e.get_dest()) {
					if (check == 0) first = iter;
					else second = iter;
					check++;
				}
			}
			if (check == 2) {
				vector<int> new_v = vector<int>(first, second + 1);
				polygon_list[j].erase(first + 1, second);
				polygon_list.push_back(new_v);
				j = int(polygon_list.size());
			}
		}
	}
	read_diagonal = int(diagonal_list.size());
}
vector<Edge> triangulate_monotone_polygon(vector<int>& polygon) {
	vector<Edge> diagonal_list;
	//make point_id_list
	int poly_size = int(polygon.size());
	vector<VertexID> point_id_list = vector<VertexID>(poly_size);
	for (int j = 0; j < poly_size; j++)
		point_id_list[j] = VertexID(polygon[j], j);
	sort(point_id_list.begin(), point_id_list.end(), y_UPDOWN_comp);

	int cur_point = point_id_list[0].get_pointID();
	int cur_poly = point_id_list[0].get_polygonID();

	//set the left chain
	int direction = -1;
	if (calculate_angle(cur_point, get_left_vertex(polygon, cur_poly)) > calculate_angle(cur_point, get_right_vertex(polygon, cur_poly)))
		direction = 1;

	point_type cur_y = point_list[cur_point].get_y();
	point_type min_y = point_list[point_id_list[poly_size - 1].get_pointID()].get_y();
	point_type temp_y;

	int id = 0;

	do{
		cur_poly = (cur_poly + direction + poly_size) % poly_size;
		while (id < int(point_id_list.size()) && point_id_list[id].get_polygonID() != cur_poly)
			id++;
		if (id >= int(point_id_list.size())) break;
		temp_y = point_list[point_id_list[id].get_pointID()].get_y();
		if (cur_y == min_y && temp_y > min_y) break;
		point_id_list[id].set_left_chain(true);
		cur_y = temp_y;
	} while (1);
	/*while (cur_y > min_y) {
		cur_poly = (cur_poly + direction + poly_size) % poly_size;
		while (point_id_list[id].get_polygonID() != cur_poly)
			id++;
		point_id_list[id].set_left_chain(true);
		cur_y = point_list[point_id_list[id].get_pointID()].get_y();
	}*/

	//stack initialization
	vector<int> stack = vector<int>();
	stack.push_back(0);
	stack.push_back(1);

	//iterate from 3 to n-1 
	for (int i = 2; i < poly_size - 1; i++) {
		cur_point = point_id_list[i].get_pointID();
		bool left = point_id_list[i].get_left_chain();
		//different chain
		if (point_id_list[stack.back()].get_left_chain() != left) {
			while (stack.size() != 1) {
				Edge e = Edge(cur_point, point_id_list[stack.back()].get_pointID());
				diagonal_list.push_back(e);
				stack.pop_back();
			}
			stack.pop_back();
			stack.push_back(i - 1);
			stack.push_back(i);

		}//same chain
		else {

			double small_angle, big_angle;
			small_angle = left ? -1 : calculate_angle(cur_point, point_id_list[stack.back()].get_pointID());
			big_angle = left ? calculate_angle(cur_point, point_id_list[stack.back()].get_pointID()) : PI + 1;
			int last_vertex = stack.back();
			stack.pop_back();

			// as long as the diagoanls are inside
			while (stack.size() != 0) {
				double cur_angle = calculate_angle(cur_point, point_id_list[stack.back()].get_pointID());
				if (small_angle >= cur_angle || cur_angle >= big_angle) break;

				//update slope
				if (point_id_list[stack.back()].get_left_chain()) big_angle = cur_angle;
				else small_angle = cur_angle;

				last_vertex = stack.back();
				stack.pop_back();
				Edge e = Edge(cur_point, point_id_list[last_vertex].get_pointID());
				diagonal_list.push_back(e);

			}
			stack.push_back(last_vertex);
			stack.push_back(i);
		}
	}

	stack.pop_back();
	while (stack.size() != 1) {
		Edge e = Edge(point_id_list[point_id_list.size() - 1].get_pointID(), point_id_list[stack.back()].get_pointID());
		diagonal_list.push_back(e);
		stack.pop_back();
	}

	return diagonal_list;
}
vector<Edge> triangulate_monotone_polygons(vector<vector<int>>& polygon_list) {
	vector<vector<int>> new_polygon_list;
	vector<Edge> new_d_list;
	vector<Edge> diagonal_list;
	vector<vector<int>> new_p_list;
	for (int i = 0; i < (int)polygon_list.size(); i++) {
		new_d_list = triangulate_monotone_polygon(polygon_list[i]);
		vector<vector<int>> new_p_list = seperate_polygon(polygon_list[i], new_d_list);
		new_polygon_list.insert(new_polygon_list.end(), new_p_list.begin(), new_p_list.end());
		diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
	}
	polygon_list.swap(new_polygon_list);
	return diagonal_list;
}
vector<Edge> find_monotone_polygon(vector<int>& polygon, bool reverse) {

	bool(*fp)(VertexID, VertexID);
	fp = reverse ? y_DOWNUP_comp : y_UPDOWN_comp;
	vector<VertexID> point_set = vector<VertexID>(polygon.size());
	for (int j = 0; j < (int)polygon.size(); j++)
		point_set[j] = VertexID(polygon[j], j);
	sort(point_set.begin(), point_set.end(), fp);

	vector<Edge> diagonal_list;
	if (point_set.size() <= 3) return vector<Edge>();
	Tree tree = Tree();

	for (int i = 0; i < (int)point_set.size(); i++) {

		int pointID = point_set[i].get_pointID();
		int polygonID = point_set[i].get_polygonID();

		int left_PointID = get_left_vertex(polygon, polygonID);
		int right_PointID = get_right_vertex(polygon, polygonID);

		Point vertex = point_list[pointID];
		Point lVertex = point_list[left_PointID];
		Point rVertex = point_list[right_PointID];

		Node * left_node = tree.find_left_edge(vertex.get_x(reverse), vertex.get_y(reverse));
		point_type point_y = vertex.get_y(reverse);
		point_type e1 = calculate_angle(pointID, left_PointID, reverse);
		point_type e2 = calculate_angle(pointID, right_PointID, reverse);
		if (e1 < 0) e1 = e1 + 2 * PI;
		if (e2 < 0) e2 = e2 + 2 * PI;
		if (e1 > e2) swap(e1, e2);

		//straight line
		if (abs(e1 - 0) < absTolerance && abs(e2 - PI) < absTolerance) break;
		//one side is straight
		else if ((abs(e1 - 0) < absTolerance && e2 > PI) ||(abs(e1 - PI) < absTolerance)) {
			if (left_node != NULL && left_node->get_helper() != NULL_HELPER) {
				//right is straight
				if (abs(e1 - 0) < absTolerance && e2 > PI) {
					Edge e = Edge(left_node->get_helper(), pointID);
					diagonal_list.push_back(e);
					left_node->set_helper(pointID);
				}
				tree.insert_node(NULL_HELPER, e2, point_list[pointID].get_reverse(reverse), vertex.get_y(reverse));
			}
			else tree.insert_node(pointID, e2, point_list[pointID].get_reverse(reverse), vertex.get_y(reverse));
			
		}//both lower
		else if (e1 > PI) {
			if (left_node != NULL && left_node->get_helper() != NULL_HELPER) {
				Edge e = Edge(left_node->get_helper(), pointID);
				diagonal_list.push_back(e);
				left_node->set_helper(pointID);
				
				tree.insert_node(NULL_HELPER, e1, point_list[pointID].get_reverse(reverse), vertex.get_y(reverse));
				tree.insert_node(pointID, e2, point_list[pointID].get_reverse(reverse), vertex.get_y(reverse));
			}
			else {
				tree.insert_node(pointID, e1, point_list[pointID].get_reverse(reverse), vertex.get_y(reverse));
				tree.insert_node(NULL_HELPER, e2, point_list[pointID].get_reverse(reverse), vertex.get_y(reverse));
			}
		}//both upper
		else if (abs(e2 - PI) < absTolerance || e2 < PI) {
			if (abs(e1 - PI) > absTolerance && abs(e1 - 0) > absTolerance) tree.delete_node(vertex.get_x(reverse), vertex.get_y(reverse));
			if (abs(e2 - PI) > absTolerance && abs(e2 - 0) > absTolerance) tree.delete_node(vertex.get_x(reverse), vertex.get_y(reverse));
			if (abs(e2 - PI) > absTolerance && left_node != NULL && left_node->get_helper() != NULL_HELPER) left_node->set_helper(pointID);
		}//one upper one lower
		else {
			tree.delete_node(vertex.get_x(reverse), vertex.get_y(reverse));
			if (left_node != NULL && left_node->get_helper() != NULL_HELPER) {
				left_node->set_helper(pointID);
				tree.insert_node(NULL_HELPER, e2, point_list[pointID].get_reverse(reverse), vertex.get_y(reverse));
			}
			else {
				tree.insert_node(pointID, e2, point_list[pointID].get_reverse(reverse), vertex.get_y(reverse));
			}
		}
	}

	return diagonal_list;
}
vector<Edge> find_monotone_polygons(vector<vector<int>>& polygon_list)
{
	vector<Edge> diagonal_list;
	vector<vector<int>> new_polygon_list;
	bool seq[2] = { false, true };
	for (int k = 0; k < 2; k++) {
		new_polygon_list = vector<vector<int>>();
		for (int i = 0; i < (int)polygon_list.size(); i++) {
			vector<Edge> new_d_list = find_monotone_polygon(polygon_list[i], seq[k]);
			vector<vector<int>> new_p_list = seperate_polygon(polygon_list[i], new_d_list);
			diagonal_list.insert(diagonal_list.end(), new_d_list.begin(), new_d_list.end());
			new_polygon_list.insert(new_polygon_list.end(), new_p_list.begin(), new_p_list.end());
		}
		polygon_list.swap(new_polygon_list);
	}
	return diagonal_list;
}
void make_big_triangle() {

	vector<VertexID> point_id_list = vector<VertexID>(point_list.size());
	for (int i = 0; i < int(point_list.size()); i++)
		point_id_list[i] = VertexID(i, i);
	sort(point_id_list.begin(), point_id_list.end(), y_UPDOWN_comp);

	int top = point_id_list.begin()->get_pointID();
	int bottom = (point_id_list.end() - 1)->get_pointID();
	int poly_size = int(polygon_list[0].size());

	int direction = -1;
	if (calculate_angle(top, get_left_vertex(polygon_list[0], top)) > calculate_angle(top, get_right_vertex(polygon_list[0], top)))
		direction = 1;

	point_type min_y = point_list[bottom].get_y();
	int cur_point = top;
	int margin = int((point_list[top].get_y() - point_list[bottom].get_y()) * (float)0.1 + 1);

	Point vtop(v_num, point_list[top].get_x(), point_list[top].get_y() + margin);
	point_list.push_back(vtop);
	for (int i = 0; i < 2; i++) {
		cur_point = top;
		int point = -1;
		double limit_angle = 1;

		vector<int> chain;
		chain.push_back(v_num);
		while (cur_point != bottom) {
			chain.push_back((cur_point + poly_size) % poly_size);
			cur_point = (cur_point + direction + poly_size) % poly_size;
			double cur_angle = calculate_angle(v_num, cur_point);
			if (point == -1 || (i == 0 && cur_angle < limit_angle) || (i == 1 && cur_angle > limit_angle)) {
				limit_angle = cur_angle;
				point = cur_point;
			}
		}
		chain.push_back(bottom);
		chain.push_back(v_num + 1 + i);
		outer_polygon_list.push_back(chain);

		Point new_point;
		if (limit_angle == PI / 2) new_point = Point(v_num + 1 + i, point_list[top].get_x() + (2 * i - 1) *margin, min_y - margin);
		else new_point = Point(v_num + 1 + i, float((min_y - margin - (point_list[v_num].get_y() - tan(limit_angle)*point_list[v_num].get_x())) / tan(limit_angle) + (2 * i - 1) *margin), min_y - margin);
		point_list.push_back(new_point);

		direction *= -1;
	}

	vector<int> bottom_triangle;
	bottom_triangle.push_back(bottom);
	bottom_triangle.push_back(v_num + 1);
	bottom_triangle.push_back(v_num + 2);
	outer_polygon_list.push_back(bottom_triangle);

	point_list[v_num] = Point(v_num, point_list[top].get_x(), point_list[top].get_y() + 3 * margin);
	for (int i = 0; i < 3; i++)
		bigT[i] = v_num + i;

	Edge e = Edge(v_num, top);
	outer_diagonal_list.push_back(e);
	e = Edge(v_num + 1, bottom);
	outer_diagonal_list.push_back(e);
	e = Edge(v_num + 2, bottom);
	outer_diagonal_list.push_back(e);

}
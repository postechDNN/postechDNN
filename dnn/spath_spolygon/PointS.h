#pragma once
#include <iostream>
#include <vector>
#include "Point.h"
#include <cmath>
#include <map>
#include "TNODE.h"
#include "Edge.h"
#include "polygon_operation.h"
#define PI 3.1415926535897931
using namespace std;

struct angle_comp {
	bool operator() (const point_type& lhs, const point_type& rhs) const
	{
		return lhs<rhs;
	}
};

class PointS {
private:
	//pair info <int,int> == <origin_point, triangle_num>
	vector<map<double, pair<int, int>>> vertex_triangle;
	vector<bool> visited;
	
	int unvisited_vertex;
public:
	PointS() {
		tree =TRIANGLE_TREE();
		triangle_list = vector<vector<int>>(polygon_list);
		triangle_list.insert(triangle_list.end(), outer_polygon_list.begin(), outer_polygon_list.end());
		unvisited_vertex = int(point_list.size());
		vertex_triangle = vector<map<double, pair<int, int>>>(point_list.size(), map<double, pair<int, int>>());
		for (int i = 0; i< (int)triangle_list.size(); i++) {
			vector<int> triangle = triangle_list[i];
			tree.push(i);
			for (int j = 0; j < 3; j++) {
				push_triangle(i, triangle[j % 3], triangle[(j + 1) % 3], triangle[(j + 2) % 3]);
			}
		}
		visited = vector<bool>(point_list.size(), false);
	}
	bool step() {
		if (unvisited_vertex <= 4) return false;
		vector<int> for_debug;
		for_debug = tree.get_current_layer(tree.get_current_height());
		tree.add_layer();
		vector<bool> semi_visited(visited);
 		for (int i = 0; i < (int)point_list.size()-3; i++) {
			if (semi_visited[i]) continue;
			vector<vector<int>> new_polygons;
			vector<int> outer_vertices = get_outer_polygon(i, semi_visited);
			map<double, pair<int, int>>::iterator it;
			vector<int> outer_poly;
			for (it = vertex_triangle[i].begin(); it != vertex_triangle[i].end(); it++) {
				outer_poly.push_back(it->second.second);
			}
			new_polygons.push_back(outer_vertices);
			visited[i] = true;
			unvisited_vertex--;
			semi_visited[i] = true;
			tree.delete_triangles(outer_poly);
			for_debug = tree.get_current_layer(tree.get_current_height());

			for (int k = 0; k < (int)outer_vertices.size(); k++) {
				int v_num = outer_vertices[k];
				bool next;
				for (it = vertex_triangle[v_num].begin(); it != vertex_triangle[v_num].end(); ) {
					next = true;
					for (int j = 0; j < 3; j++) {
						if (triangle_list[it->second.second][j] == i) {
							it = vertex_triangle[v_num].erase(it);
							next = false;
							break;
						}
					}
					if (next) {
						it++;
					}
				}
			}

			vector<Edge> diagonal_list;
			diagonal_list = find_monotone_polygons(new_polygons);
			diagonal_list = triangulate_monotone_polygons(new_polygons);

			for (int p = 0; p< (int)new_polygons.size(); p++) {
				int t_num = push_triangle(new_polygons[p]);
				vector<int> parents = check_overlapping(t_num, outer_poly);
				tree.push(t_num, parents);
			}
		}
		for_debug = tree.get_current_layer(tree.get_current_height());
		return true;

	}
	int find_triangle(Point & target) {
		int current_height = tree.get_current_height();
		vector<int> candidates = tree.get_current_layer(current_height);
		vector<int> ret_candidates;
		for (int i = 0; i < (int)candidates.size(); i++) {
			if (check_in_polygon(triangle_list[candidates[i]], target) != 0) {
				ret_candidates.push_back(candidates[i]);
				break;
			}
		}
		
		while (current_height > 0) {
			candidates = vector<int>();
			for (int i = 0; i < (int)ret_candidates.size(); i++) {
				vector<int> parents = tree.get_parents(current_height, ret_candidates[i]);
				candidates.insert(candidates.end(), parents.begin(), parents.end());
			}
			ret_candidates = vector<int>();
			for (int i = 0; i < (int)candidates.size(); i++) {
				if (check_in_polygon(triangle_list[candidates[i]], target) != 0) {
					ret_candidates.push_back(candidates[i]);
				}
			}
			if (ret_candidates.size() == 0) return -1;
			current_height--;
		}

		return *min_element(ret_candidates.begin(), ret_candidates.end());;
		//return triangle_list[ret];
	}
	void find_independent_list() {
		vector<bool> dependent_vertex(visited);
		bool independent;
		for (int i = 0; i < (int)point_list.size(); i++) {
			independent = true;
			vector<int> triangle = triangle_list[i];
			for (int i = 0; i < 3; i++) {
				if (dependent_vertex[triangle[i]]) {
					independent = false;
					break;
				}
			}
			dependent_vertex[i] = true;
			//return ret;
		}
	}
	int push_triangle(vector<int>& triangle) {
		int t_num = int(triangle_list.size());
		triangle_list.push_back(triangle);
		for (int j = 0; j < 3; j++) {
			push_triangle(t_num, triangle[j % 3], triangle[(j + 1) % 3], triangle[(j + 2) % 3]);
		}
		return t_num;
	}
	void push_triangle(int t_num, int origin, int target1, int target2) {
		Point middle = Point((point_list[target1].get_x() + point_list[target2].get_x()) / 2, (point_list[target1].get_y() + point_list[target2].get_y()) / 2);
		double angle = calculate_angle(point_list[origin], middle);
		map<double, pair<int, int>>& adjecent_traingle = vertex_triangle[origin];
		adjecent_traingle.insert(make_pair(angle, make_pair(origin, t_num)));
	}
	double calculate_angle(Point& origin, Point& target) {
		point_type x = target.get_x() - origin.get_x();
		point_type y = target.get_y() - origin.get_y();
		double ret = atan2(y, x);
		return ret >= 0 ? ret : ret + 2 * PI;
	}
	int find_counterclockwise_vertex(pair<int, int> & triangle_pair) {
		int origin = triangle_pair.first;
		int ret = -1;
		double ret_angle = -1;
		vector<int> triangle = triangle_list[triangle_pair.second];
		for (int i = 0; i < 3; i++) {
			if (triangle[i] != origin) {
				double new_angle = calculate_angle(point_list[origin], point_list[triangle[i]]);
				if (ret == -1 || (ret_angle < new_angle && new_angle - ret_angle < PI) || (ret_angle > new_angle && ret_angle-new_angle > PI)){
					ret = triangle[i];
					ret_angle = new_angle;
				}
			}
		}
		return ret;
	}
	
	bool check_intersection(int origin, int target) {
		vector<int>& t_origin = triangle_list[origin];
		vector<int>& t_target = triangle_list[target];
		bool ret = false;
		for (int i = 0; i<3; i++)
			for (int j = 0; j < 3; j++) {
				ret = ret || check_line_intersection(t_origin[i % 3], t_origin[(i + 1) % 3], t_target[j % 3], t_target[(j + 1) % 3]);
				if (ret) return ret;
			}

		return ret;
	}
	double dot(pair<double, double> & v1, pair<double, double> & v2) {
		return v1.first*v2.first + v1.second * v2.second;
	}
	
	int check_in_polygon(vector<int> & triangle, int p_num) {
		pair<point_type, point_type> v0, v1, v2;
		v0 = make_pair<double, double>(point_list[triangle[2]].get_x() - point_list[triangle[0]].get_x(), point_list[triangle[2]].get_y() - point_list[triangle[0]].get_y());
		v1 = make_pair<double, double>(point_list[triangle[1]].get_x() - point_list[triangle[0]].get_x(), point_list[triangle[1]].get_y() - point_list[triangle[0]].get_y());
		v2 = make_pair<double, double>(point_list[p_num].get_x() - point_list[triangle[0]].get_x(), point_list[p_num].get_y() - point_list[triangle[0]].get_y());

		//compute dot product
		double dot00, dot01, dot02, dot11, dot12;
		dot00 = dot(v0, v0);
		dot01 = dot(v0, v1);
		dot02 = dot(v0, v2);
		dot11 = dot(v1, v1);
		dot12 = dot(v1, v2);

		// Compute barycentric coordinates
		double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		if (abs(u) < absTolerance) return 1;
		if (abs(v) < absTolerance) return 1;
		if (abs(u + v - 1) < absTolerance) return 2;
		if (u > 0 && v > 0 && u+v <1 )return 4;
		return 0;
	}
	int check_in_polygon(vector<int> & triangle, Point& p) {
		pair<point_type, point_type> v0, v1, v2;
		v0 = make_pair<point_type, point_type>(point_list[triangle[2]].get_x() - point_list[triangle[0]].get_x(), point_list[triangle[2]].get_y() - point_list[triangle[0]].get_y());
		v1 = make_pair<point_type, point_type>(point_list[triangle[1]].get_x() - point_list[triangle[0]].get_x(), point_list[triangle[1]].get_y() - point_list[triangle[0]].get_y());
		v2 = make_pair<point_type, point_type>(p.get_x() - point_list[triangle[0]].get_x(), p.get_y() - point_list[triangle[0]].get_y());



		//compute dot product
		point_type dot00, dot01, dot02, dot11, dot12;
		dot00 = dot(v0, v0);
		dot01 = dot(v0, v1);
		dot02 = dot(v0, v2);
		dot11 = dot(v1, v1);
		dot12 = dot(v1, v2);

		// Compute barycentric coordinates
		point_type invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		point_type u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		point_type v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		if (abs(v2.first) < absTolerance && abs(v2.second) < absTolerance) return 1;
		if (abs(u) < absTolerance && abs(v-1)<absTolerance) return 1;
		if (abs(v) < absTolerance && abs(u-1)<absTolerance) return 1;
		if ((abs(u) < absTolerance || abs(v) < absTolerance) && u+v >0 && u+v <1) return 2;
		if (v > 0 && u > 0 && abs(u + v - 1) < absTolerance) return 2;
		if (u > 0 && v > 0 && u+v >0 && u + v <1)return 4;
		return 0;
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
	vector<int> check_overlapping(int t_num, vector<int>& adjacent_triangle_list) {
		vector<int> parents;
		vector<int> triangle = triangle_list[t_num];
		for (int i = 0; i < (int)adjacent_triangle_list.size(); i++) {
			if (check_intersection(t_num, adjacent_triangle_list[i])) {
				parents.push_back(adjacent_triangle_list[i]);
				continue; 
			}
			else {
				int check = 0;
				for (int j = 0; j < 3; j++) {
					check += check_in_polygon(triangle, triangle_list[adjacent_triangle_list[i]][j] );
				}
				if(check>=3) parents.push_back(adjacent_triangle_list[i]);//디버깅중 고침
			}
		}
		return parents;

	}
	vector<int> get_adjacent_triangles(int target) {
		vector<int> ret;
		map<double, pair<int, int>>& adjacent_traingle = vertex_triangle[target];
		for (map<double, pair<int, int>>::iterator it = adjacent_traingle.begin(); it != adjacent_traingle.end(); it++) {
			ret.push_back(it->second.second);
		}
		return ret;
	}
	vector<int> get_outer_polygon(int target) {
		vector<int> ret;
		map<double, pair<int, int>>& adjacent_traingle = vertex_triangle[target];
		for (map<double, pair<int, int>>::iterator it = adjacent_traingle.begin(); it != adjacent_traingle.end(); it++) {
			int v = find_counterclockwise_vertex(it->second);
			ret.push_back(v);
		}
		return ret;
	}
	vector<int> get_outer_polygon(int target, vector<bool>& visited) {
		vector<int> ret;
		map<double, pair<int, int>>& adjacent_traingle = vertex_triangle[target];
		for (map<double, pair<int, int>>::iterator it = adjacent_traingle.begin(); it != adjacent_traingle.end(); it++) {
			int v = find_counterclockwise_vertex(it->second);
			visited[v] = true;
			ret.push_back(v);
		}
		return ret;
	}
};
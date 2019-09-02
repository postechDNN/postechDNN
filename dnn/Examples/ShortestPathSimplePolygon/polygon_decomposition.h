#pragma once
#include <iostream>
#include <vector>
#include "Edge.h"
#include "polygon_operation.h"
#include "SNode.h"
using namespace std;

#define connected -2
int t_num;
int t_head;
int ** edge_finder;
int ** edge_triangle;
int ** s_graph;
vector<SNode *> edge_SNode;
SNode * s_head;

class Triangle {
private:
	int p_list[3];
	int d_list[3];
	int adjacent_triangles[3];
	bool check_children[3];
	int d_size;
	int t_size;
	int tree_size;
	int parent;
	bool dual_check_children[3];
	SNode * node;
public:
	void init() {
		memset(d_list, -1, 3 * sizeof(int));
		memset(adjacent_triangles, -1, 3 * sizeof(int));
		t_size = 0;
		d_size = 0;
		tree_size = 0;
		for (int i = 0; i < 3; i++) {
			check_children[i] = false;
			dual_check_children[i] = false;
		}
			
	}
	Triangle() {
		memset(p_list, -1, 3 * sizeof(int));
		init();
	}
	Triangle(vector<int> & _p_list) {
		for (int i = 0; i < 3; i++)
			p_list[i] = _p_list[i];
		init();
	}
	void add_diagonal(int _d) {
		if (_d != -1) {
			d_list[d_size] = _d;
			d_size++;
		}
	}
	void add_adjacent_triangle(int t_num) {
		adjacent_triangles[t_size] = t_num;
		t_size++;
	}
	int get_t_size() {
		return t_size;
	}
	int * get_adjacent_triangles() {
		return adjacent_triangles;
	}
	void set_tree_size(int _tree_size) {
		tree_size = _tree_size;
	}
	void set_parent(int _parent) {
		parent = _parent;
	}
	int get_parent() {
		return parent;
	}
	void set_check_children_index(int i, bool _check_children) {
		check_children[i] = _check_children;
	}
	void set_dual_check_children_index(int i, bool _check_children) {
		dual_check_children[i] = _check_children;
	}
	void set_check_children_t_num(int t_num, bool _check_children) {
		for (int i = 0; i < 3; i++) {
			if (adjacent_triangles[i] == t_num) {
				set_check_children_index(i, _check_children);
				return;
			}
		}
	}
	bool * get_check_children() {
		return check_children;
	}
	bool * get_dual_check_children() {
		return dual_check_children;
	}
	int * get_d_list() {
		return d_list;
	}
	int get_tree_size() {
		return tree_size;
	}
	void deduct_children_size(int removed_children) {
		tree_size -= removed_children;
	}
	int get_not_cutting_point(int cutting_edge) {
		Edge e = diagonal_list[cutting_edge];
		int p1, p2;
		p1 = e.get_origin();
		p2 = e.get_dest();
		for (int i = 0; i < 3; i++) {
			if (p_list[i] != p1 && p_list[i] != p2)
				return p_list[i];
		}
		return -1;
	}
	int get_not_cutting_edge(int cutting_edge) {
		for (int i = 0; i < 3; i++) {
			int edge_num = edge_finder[p_list[i]][p_list[(i + 1) % 3]];
			if (edge_num != cutting_edge)
				return edge_num;
		}
		return -1;
	}
	void set_node(SNode * _node) {
		node = _node;
		return;
	}
	SNode * get_node() {
		return node;
	}
};
Triangle * t_list;

int find_cutting_triangle(int head, int tree_size) {
	bool * check_children = t_list[head].get_check_children();
	int * adjacent_triangles = t_list[head].get_adjacent_triangles();
	for (int i = 0; i < 3; i++) {
		if (check_children[i]) {
			int children_size = t_list[adjacent_triangles[i]].get_tree_size();
			if (children_size >(int) tree_size / 3) {
				if (children_size < (int)(tree_size * 2) / 3)
					return adjacent_triangles[i];
				else return find_cutting_triangle(adjacent_triangles[i], tree_size);
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		if (check_children[i]) {
			return adjacent_triangles[i];
		}
	}
	return head;
}
int find_cutting_edge(int cutting_triangle) {
	Triangle t = t_list[cutting_triangle];
	int parent = t.get_parent();
	int * d_list = t.get_d_list();
	for (int i = 0; i < 3; i++) {
		if (d_list[i] == -1) break;
		if (edge_triangle[d_list[i]][0] == parent || edge_triangle[d_list[i]][1] == parent)
			return d_list[i];
	}
	return -1;
}
void update_children_state(int triangle, int removed_children_size) {
	if (triangle == -1) return;
	t_list[triangle].deduct_children_size(removed_children_size);
	update_children_state(t_list[triangle].get_parent(), removed_children_size);
	return;
}
SNode * create_STree(int head, vector<int> polygon) {
	int tree_size = t_list[head].get_tree_size();
	if (tree_size == 1) {
		SNode * new_node = new SNode(polygon);
		t_list[head].set_node(new_node);
		return new_node;
	}
	int cutting_triangle = find_cutting_triangle(head, tree_size);
	int cutting_edge = find_cutting_edge(cutting_triangle);

	int parent = t_list[cutting_triangle].get_parent();
	t_list[parent].set_check_children_t_num(cutting_triangle, false);
	update_children_state(parent, t_list[cutting_triangle].get_tree_size());
	t_list[cutting_triangle].set_parent(-1);

//	int not_cutting_point = t_list[cutting_triangle].get_not_cutting_point(cutting_edge);
	int not_cutting_edge = t_list[cutting_triangle].get_not_cutting_edge(cutting_edge);

	if (cutting_edge == 0) {
		cout << "??" << endl;

	}
	SNode* node = new SNode(cutting_edge, polygon);
	diagonal_list[cutting_edge].set_SNode(node);

	vector<vector<int>> polygon_list;
	polygon_list.push_back(polygon);
	seperate_polygon_with_edge(polygon_list, cutting_edge);
	int p[2];
	if (find(polygon_list[0].begin(), polygon_list[0].end(), not_cutting_edge) != polygon_list[0].end()) {
		p[0] = cutting_triangle;
		p[1] = head;
		/*node->set_children(create_STree(cutting_triangle, polygon_list[0]));
		node->set_children(create_STree(head, polygon_list[1]));*/
	}
	else {
		p[0] = head;
		p[1] = cutting_triangle;
		/*node->set_children(create_STree(cutting_triangle, polygon_list[1]));
		node->set_children(create_STree(head, polygon_list[0]));*/
	}
	for (int i = 0; i < 2; i++) {
		SNode * temp = create_STree(p[i], polygon_list[i]);
		node->set_children(temp);
		temp->set_parent(node);
	}
	return node;
}
int set_tree_number(int head, int parent) {
	int * adjacent_triangles = t_list[head].get_adjacent_triangles();
	int sum = 1;
	for (int i = 0; i < 3; i++) {
		if (adjacent_triangles[i] != parent && adjacent_triangles[i] != -1) {
			t_list[head].set_check_children_index(i, true);
			t_list[head].set_dual_check_children_index(i, true);
			sum += set_tree_number(adjacent_triangles[i], head);
		}
	}
	t_list[head].set_tree_size(sum);
	t_list[head].set_parent(parent);
	return sum;
}
int add_edge_triangle(int p1, int p2, int t_num) {
	int edge_num = p1 < p2 ? edge_finder[p1][p2] : edge_finder[p2][p1];
	if (edge_num != -1) {
		if (edge_triangle[edge_num][0] == -1) {
			edge_triangle[edge_num][0] = t_num;
		}
		else {
			edge_triangle[edge_num][1] = t_num;
		}
	}
	return edge_num;
}
void recursive_make_star_graph(SNode * head) {
	int d_size = diagonal_list.size();
	SNode * parent = head->get_parent();
	if (parent != NULL) {
		int p_diagonal_num = parent->get_diagonal();
		vector<int> polygon_with_edge = head->get_polygon_with_edge();
		for (vector<int>::iterator iter = polygon_with_edge.begin();
			iter != polygon_with_edge.end(); iter++) {
			if ((*iter) < d_size && (*iter) != p_diagonal_num) {
				s_graph[p_diagonal_num][*iter] = connected;
				//s_graph[*iter][p_diagonal_num] = 1;
			}
		}
	}
	SNode ** children = head->get_children();
	for (int i = 0; i < 2; i++) {
		if (children[i] != NULL) recursive_make_star_graph(children[i]);
	}
	return;
}
void make_star_graph(SNode* head) {
	int d_size = diagonal_list.size();
	s_graph = new int *[d_size];
	for (int i = 0; i < d_size; i++) {
		s_graph[i] = new int[d_size];
		memset(s_graph[i], -1, d_size * sizeof(int));
	}
	recursive_make_star_graph(head);

	return;
}
void dual_tree(int v_num) {
	edge_SNode = vector<SNode *>(diagonal_list.size(), NULL);
	edge_finder = new int*[v_num];
	for (int i = 0; i < v_num; i++) {
		edge_finder[i] = new int[v_num];
		memset(edge_finder[i], -1, v_num * sizeof(int));
	}
	edge_triangle = new int*[(int)diagonal_list.size()];
	for (int i = 0; i < (int)diagonal_list.size(); i++) {
		Edge e = diagonal_list[i];
		int p1 = e.get_origin();
		int p2 = e.get_dest();
		edge_finder[p1][p2] = i;
		edge_triangle[i] = new int[2];
		memset(edge_triangle[i], -1, 2 * sizeof(int));
	}
	t_list = new Triangle[polygon_list.size()];
	for (int i = 0; i < (int)polygon_list.size(); i++) {
		vector<int> v = polygon_list[i];
		//sort(v.begin(), v.end());
		t_list[i] = Triangle(v);
		for (int j = 0; j < 3; j++) {
			int e = add_edge_triangle(v[j], v[(j + 1) % 3], i);
			if (e != -1) t_list[i].add_diagonal(e);
		}
	}
	for (int i = 0; i < (int)diagonal_list.size(); i++) {
		int * two_triangles = edge_triangle[i];
		int t1 = two_triangles[0];
		int t2 = two_triangles[1];
		t_list[t1].add_adjacent_triangle(t2);
		t_list[t2].add_adjacent_triangle(t1);
	}
	int head = -1;
	for (int i = 0; i < (int)polygon_list.size(); i++) {
		if (t_list[i].get_t_size() != 3) {
			head = i;
			break;
		}
	}
	t_head = head;

	set_tree_number(head, -1);
	vector<int> polygon_with_edge = vector<int>(v_num);
	iota(polygon_with_edge.begin(), polygon_with_edge.end(), diagonal_list.size());
	diagonal_list.insert(diagonal_list.end(), outer_edge_list.begin(), outer_edge_list.end());
	for (int i = 0; i < v_num; i++) {
		int p1 = i % v_num;
		int p2 = (i + 1) % v_num;
		edge_finder[p1][p2] = edge_finder[p2][p1] = i + diagonal_list.size();
	}
	s_head = create_STree(head, polygon_with_edge);
	make_star_graph(s_head);

	return;
}
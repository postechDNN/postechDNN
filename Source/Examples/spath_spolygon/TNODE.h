#pragma once
#include<iostream>
#include<vector>
#include<map>
using namespace std;

class TNODE {
private:
	//int t_num;
	vector<int>parents;
public:
	TNODE() {
	}
	TNODE(vector<int>& _parents) {
		parents.assign(_parents.begin(), _parents.end());
	}
	void clear_parents() {
		parents = vector<int>();
	}
	void add_parents(int _parent) {
		parents.push_back(_parent);
	}
	vector<int> get_parents() {
		return parents;
	}

};
class TRIANGLE_TREE {
private:
	int tree_height;
	vector<map<int,TNODE>> triangle_tree;
public:
	TRIANGLE_TREE() {
		tree_height = 0;
		triangle_tree.push_back(map<int,TNODE>());
	}
	void add_layer() {
		map<int,TNODE> new_layer(triangle_tree[tree_height]);
		map<int, TNODE>::iterator it;
		for (it = triangle_tree[tree_height].begin(); it != triangle_tree[tree_height].end();it++){
			int t_num = it->first;
			new_layer[t_num].clear_parents();
			new_layer[t_num].add_parents(t_num);
		}
		tree_height++;
		triangle_tree.push_back(new_layer);
	}
	void delete_triangles(vector<int>& outer_poly) {
		for (int i = 0; i < int(outer_poly.size()); i++)
			delete_poly(outer_poly[i]);
	}
	void delete_poly(int poly_num) {
		std::map<int, TNODE>::iterator it;
		it = triangle_tree[tree_height].find(poly_num);
		if (it != triangle_tree[tree_height].end())
			triangle_tree[tree_height].erase(it);
		return;
	}
	void push(int _t_num) {
		triangle_tree[tree_height].insert(make_pair(_t_num,TNODE()));
	}
	void push(int _t_num,vector<int> parents) {
		triangle_tree[tree_height].insert(make_pair(_t_num, TNODE(parents)));
	}
	int get_current_height() {
		return tree_height;
	}
	vector<int> get_current_layer(int current_height) {
		vector<int> ret;
		map<int, TNODE>::iterator it;
		for (it = triangle_tree[current_height].begin(); it != triangle_tree[current_height].end(); it++) {
			ret.push_back(it->first);
		}
		return ret;
	}
	vector<int> get_parents(int current_height, int t_num) {
		return triangle_tree[current_height][t_num].get_parents();
	
	}
};
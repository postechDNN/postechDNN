#pragma once
#include<iostream>
#include<vector>
#include<cmath>
#include "Tree.h"
#include "PointS.h"
#include "hourglass_operation.h"
#include "SPTnode.h"

using namespace std;

class SPT
{
	SPTnode* root;
	vector<int> components;	//maintains all the point IDs in the tree
	int s;
	int t;
	int t_tri_num;
	vector<int> t_tri_vertices;
	int t_pred;
	bool set;
public:
	SPT()
	{
		root = NULL;
		components = vector<int>();
		set = false;
	}
	SPT(int _s,int _t) //must be called after decomposition is complete
	{
		root = new SPTnode(_s, NULL);
		components.push_back(_s);
		t = _t;
		s = _s;
		Point point_t = point_list[t];
		t_tri_num = point_state.find_triangle(point_t);
		t_tri_vertices = polygon_list[t_tri_num];
		t_pred = -1;
		set = false;
	}
	int set_pred(int leaf, int parent);
	/* Returns the SPT node pointer with the vertex ID */
	SPTnode* get_node(int vertex)
	{
		if (find(components.begin(), components.end(), vertex) == components.end())
			return NULL; //NO CORRESPONDING NODE IN THE TREE SO FAR

		return root->find_node(vertex);	
	}
	/* Returns the SPT node pointer of the parent node of node in question*/
	SPTnode* get_pred(int vertex)
	{
		//find the parent of the node representing 'vertex' in the tree!
		SPTnode* node = get_node(vertex);

		//will return null for the root
		return node->get_parent();	
	}
	void set_root(int _root)
	{
		root = new SPTnode(_root);
		components.push_back(_root);
	}
	int get_root()
	{
		return root->get_id();
	}
	bool is_set(int id)
	{
		if (find(components.begin(), components.end(), id) == components.end())
			return false;
		else
			return true;
	}
	bool get_size() {
		return components.size();
	}
	bool compute_shortest_path_tree();
	vector<int> retrieve_shortest_path_default();
	vector<int> retrieve_shortest_path(int _t);
	void split_funnel(Funnel* funnel);
	int choose_v(Funnel* funnel);
};

/*
int compute_pred(Funnel* funnel, int v)
{
	int apex = funnel->get_apex();
	int pred;
	vector<int> vertex_list = funnel->get_vertex_list();

	if (vertex_list.empty())
	{
		printf("funnel's vertex list should not be empty!\n");
		exit(234);
	}
	else if (vertex_list.size() == 2)
		return apex;

	vector<int>::iterator apex_ptr = find(vertex_list.begin(), vertex_list.end(), apex);
	vector<int> alpha_list, beta_list;

	//always start with alpha!!
	alpha_list.insert(alpha_list.end(), vertex_list.begin(), apex_ptr);
	alpha_list.push_back(apex);
	reverse(alpha_list.begin(), alpha_list.end());
	beta_list.insert(beta_list.end(), apex_ptr, vertex_list.end());

	float angle_alpha = calculate_angle_between(apex, v, alpha_list[1]);
	float angle_beta = calculate_angle_between(apex, v, beta_list[1]);

	//the case that the apex is the predecessor -> different signature (or zero)
	if (angle_alpha*angle_beta <= 0)
		return apex;
	
	//not the case that the apex is the predecessor
	vector<int> chain;

	//select alpha / beta so that the chain is closer to v
	chain = (abs(angle_alpha) > abs(angle_beta)) ? beta_list : alpha_list;

	vector<int>::iterator predecessor = chain.begin();
	predecessor++;

	while ((predecessor + 1) != chain.end())
	{
		if (in_between(*(predecessor-1), *predecessor, *(predecessor + 1), v))
			return *(predecessor);
		predecessor++;
	}
	
	//when the predecessor is the last element
	return *predecessor;
}
*/
/*  Add the leaf as a member of the tree and link it to the parent node and return the vertex ID of the leaf
If the parent is not part of the tree in the first place, return -1 */
int SPT::set_pred(int leaf, int parent)
{
	//if parent is not in the tree, then it's an error
	if (find(components.begin(), components.end(), parent) == components.end())
	{
		return -1;
	}

	if (find(components.begin(), components.end(), leaf) != components.end())
		return -1;
	//now we can be sure that the parent is a node in the tree!!

	//add the leaf to components list and set the parent

	SPTnode* parent_node = get_node(parent);
	SPTnode* new_leaf = new SPTnode(leaf, parent_node);
	parent_node->add_child(new_leaf);

	components.push_back(leaf);

	return leaf;
}

/* chooses the next vertex to expand
also calculates the predecessor of t if possible*/
int SPT::choose_v(Funnel* funnel)
{
	int alpha = funnel->get_alpha();
	int beta = funnel->get_beta();
	int diag = funnel->get_diag();

	if (diag == -1)
		return -1;

	//array of two indexes of triangles in polygon_list that are adjacent to the diagonal
	int* triangles = diagonal_list[diag].get_triangle();
	int selected_tri_index = -1;
	int v = -1;

	for (int i = 0; i < 2; i++)
	{
		int v_cand;
		selected_tri_index = i;
		for (int j = 0; j < 3; j++)
		{
			int vertex = polygon_list[triangles[i]][j];
			if (vertex != alpha && vertex != beta)
			{
				v_cand = vertex;
				break;
			}
		}

		if (is_set(v_cand) == false)
		{
			v = v_cand;
			break;
		}
	}

	if (triangles[selected_tri_index] == t_tri_num)
	{
		t_pred = funnel->compute_pred(t);//compute_pred(funnel, t);
		set_pred(t, t_pred);
	}
	return v;
}


void SPT::split_funnel(Funnel* funnel)
{
	int alpha = funnel->get_alpha();
	int beta = funnel->get_beta();
	int apex = funnel->get_apex();

	//if alpha and beta are adjacent numbers (meaning the diagonal is a polygon edge), return
	if (abs(alpha - beta) == 1 || abs(alpha - beta) == (v_num - 1))
		return;

	int v = choose_v(funnel);
	if (v == -1)//cannot choose v
		return;

	//int pred = compute_pred(funnel, v);
	int pred = funnel->compute_pred(v);
	//add pred info to tree
	set_pred(v, pred);

	vector<int> chain = funnel->get_vertex_list();
	vector<int>::iterator pred_ptr = find(chain.begin(), chain.end(), pred);
	vector<int> first_chain, second_chain;

	if (find(chain.begin(), pred_ptr, apex) != pred_ptr)
	{
		reverse(chain.begin(), chain.end());
		pred_ptr = find(chain.begin(), chain.end(), pred);
	}
	first_chain.insert(first_chain.end(), chain.begin(), pred_ptr);
	first_chain.push_back(pred);
	first_chain.push_back(v);
	second_chain.push_back(v);
	second_chain.insert(second_chain.end(), pred_ptr, chain.end());

	Funnel* first = new Funnel(pred, first_chain);
	Funnel* second = new Funnel(apex, second_chain);

	if (pred == alpha || pred == beta)
	{
		if (first_chain.size() == 2)//second_chain should have the origin apex
		{
			first = new Funnel(pred, first_chain);
			second = new Funnel(apex, second_chain);
		}
		else if (second_chain.size() == 2)//first_chain should have the original apex
		{
			first = new Funnel(apex, first_chain);
			second = new Funnel(pred, second_chain);
		}
		else {
			printf("this doesn't make sense\n");
			exit(-10);
		}
	}

	split_funnel(first);
	split_funnel(second);
	return;
}

vector<int> SPT::retrieve_shortest_path_default()
{
	return retrieve_shortest_path(t);
}
vector<int> SPT::retrieve_shortest_path(int _t)
{
	if (!set)
	{
		printf("shortest path tree is not computed yet!\n");
		exit(92);
	}

	vector<int> path;

	if (is_set(_t)) // t is a polygon vertex, not a test point
	{
		path = root->find_node_save_path(path, _t);
	}
	else
	{
		printf("no such t in the shortest path tree\n");
		exit(39);
	}
	//basic validity check
	if (path.front() == s && path.back() == _t)
		return path;
	else
	{
		printf("error! not a valid shortest path\n");
		exit(103);
	}
}

bool SPT::compute_shortest_path_tree()
{
	//Point root = point_list[s];//probably index point_list.size()-2
	//set_root(s);
	int triangle_with_s_id = point_state.find_triangle(point_list[s]);
	vector<int> triangle_with_s = polygon_list[triangle_with_s_id];

	//set predecessor of each vertex!
	set_pred(triangle_with_s[0], s);
	set_pred(triangle_with_s[1], s);
	set_pred(triangle_with_s[2], s);

	//divide the triangle into three
	for (int i = 0; i < 3; i++)
	{
		Funnel* temp = new Funnel(s, triangle_with_s[i % 3], triangle_with_s[(i + 1) % 3]);
		//call split
		split_funnel(temp);
	}
	set = true;
	return components.size();
}
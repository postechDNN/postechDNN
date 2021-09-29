#pragma once
#include <iostream>
#include "polygon_operation.h"

using namespace std;

int find_diagonal_index(int first, int second);
bool in_between(int prev, int curr, int next, int v);

class SPTnode
{
	int vertexID; //the id of the vertex the node represents
	SPTnode* parent;
	vector<SPTnode*> children;

public:
	SPTnode()
	{
		vertexID = -1;
		parent = NULL;
		children = vector<SPTnode*>();
	}
	SPTnode(int point_num)
	{
		vertexID = point_num;
		parent = NULL;
		children = vector<SPTnode*>();
	}
	SPTnode(int _vertexID, SPTnode* _parent)
	{
		vertexID = _vertexID;
		parent = _parent;
		children = vector<SPTnode*>();
	}
	/* Searches all its descendents and returns a pointer to the SPT node with the corresponding vertex ID */
	SPTnode* find_node(int ID)
	{
		if (vertexID == ID)
			return this;
		else
		{
			for (int i = 0; i < children.size(); i++)
			{
				SPTnode* node = children[i]->find_node(ID);
				if (node != NULL)
					return node;
			}
			return NULL;
		}
	}
	vector<int> find_node_save_path(vector<int> path, int ID)
	{
		path.push_back(vertexID);
		if (vertexID == ID)
		{
			return path;
		}
		else
		{
			vector<int> child;
			//if all the children have no match, return an empty vector
			for (int i = 0; i < children.size(); i++)
			{
				child = children[i]->find_node_save_path(path, ID);
				if (!child.empty())
				{
					return child;
				}
			}

			return vector<int>();
		}
	}
	int get_id()
	{
		return vertexID;
	}
	/* Returns a pointer to the parent node of the current node */
	SPTnode* get_parent()
	{
		return parent;
	}
	vector<SPTnode*> get_children()
	{
		return children;
	}
	/* Adds the pointer to the leaf SPT node to in the children list of the current node */
	void add_child(SPTnode* new_child)
	{
		children.push_back(new_child);
	}
};

class Funnel
{
	int apex;	// apex's vertex id in point_list
	int diag;
	int alpha;	// an edge's endpoint id
	int beta;	//other point of the edge
	vector<int> vertex_list; //starts with alpha and ends with beta

public:
	Funnel()
	{
		apex = -1;
		diag = -1;
		alpha = -1;
		beta = -1;
	}
	Funnel(int _apex, int _diag)
	{
		apex = _apex;
		diag = _diag;
		if (_diag < diagonal_with_edge_list.size())
		{
			//treats the origin of the diagonal as alpha
			alpha = diagonal_with_edge_list[_diag].get_origin();
			beta = diagonal_with_edge_list[_diag].get_dest();
			vertex_list.push_back(alpha);
			vertex_list.push_back(apex);
			vertex_list.push_back(beta);
		}
		else//error case
		{
			alpha = -1;
			beta = -1;
		}
	}
	Funnel(int _apex, int _alpha, int _beta)
	{
		//diag = -1;
		apex = _apex;
		alpha = _alpha;
		beta = _beta;
		//need to set the diagonal number if it exists!!
		diag = find_diagonal_index(_alpha, _beta);
		vertex_list.push_back(_alpha);
		vertex_list.push_back(apex);
		vertex_list.push_back(_beta);
	}
	Funnel(int _apex, vector<int> _vertex_list)
	{
		apex = _apex;
		alpha = _vertex_list.front();
		beta = _vertex_list.back();
		diag = find_diagonal_index(alpha, beta);
		vertex_list = _vertex_list;
	}
	int get_apex() { return apex; }
	int get_alpha() { return alpha; }
	int get_beta() { return beta; }
	int get_diag() { return diag; }
	vector<int> get_vertex_list() { return vertex_list; }
	int compute_pred(int v);
};

int Funnel::compute_pred(int v)
{
	int pred;
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
		if (in_between(*(predecessor - 1), *predecessor, *(predecessor + 1), v))
			return *(predecessor);
		predecessor++;
	}

	//when the predecessor is the last element
	return *predecessor;
}

int find_diagonal_index(int first, int second)
{
	for (int i = 0; i < diagonal_list.size(); i++) {
		if (diagonal_list[i].check_same_edge(first, second))
			return i;
	}

	return -1;
}

/* returns true iff vector (curr, v) lies between vectors (prev, curr) and (curr, next)
(the smaller side of the pie)
*/
bool in_between(int prev, int curr, int next, int v)
{
	float prev_curr = calculate_angle(prev, curr);
	float curr_next = calculate_angle(curr, next);
	float curr_v = calculate_angle(curr, v);

	float diff_first = calculate_angle_between(curr, next, v);
	float diff_second = curr_v - prev_curr;

	if (diff_second > PI)
		diff_second -= 2 * PI;
	else if (diff_second < -PI)
		diff_second += 2 * PI;

	if (diff_first*diff_second >= 0)
		return true;
	else return false;
}

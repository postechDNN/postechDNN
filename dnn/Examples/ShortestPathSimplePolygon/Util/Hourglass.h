#pragma once
#include<iostream>
#include "Chain.h"

using namespace std;
int id_num = 0;
Edge * common_edge = NULL;

class String {
private:
	Chain * s;
	//String * left;
	//String * middle;
	//String * right; 
	point_type len;
public:

	String() {
	}
	String(vector<int> v) {
		init();
		s = new Chain(v);
		len = s->get_len();
	}
	String(int p1, int p2) {
		init();
		if (p1 == p2)
			s = new Chain(p1);
		else
			s = new Chain(p1, p2);
		len = s->get_len();
	}
	String(String* _left, String* _middle) {
		init();
		s = new Chain(_left->get_chain()->get_point_list());
		s->add_point_list(_middle->get_chain()->get_point_list(), _middle->get_length());
		len = _left->get_length() + _middle->get_length();
		/*if (_left->get_point_num() <= 1) {
		s = _middle->get_chain();
		left = _middle->get_left_string();
		middle = _middle->get_middle_string();
		right = _middle->get_right_string();
		len = _middle->get_length();
		//delete(_left);
		}
		else {
		left = _left;
		middle = _middle;
		len = left->get_length() + middle->get_length();
		}*/

	}
	String(String* _left, String* _middle, String* _right) {
		init();
		s = new Chain(_left->get_chain()->get_point_list());
		s->add_point_list(_middle->get_chain()->get_point_list(), _middle->get_length());
		s->add_point_list(_right->get_chain()->get_point_list(), _right->get_length());

		len = _left->get_length() + _middle->get_length();
		/*		left = _left;
		middle = _middle;
		right = _right;
		len = left->get_length() + middle->get_length() + right->get_length();*/
	}

	void init() {
		s = NULL;
		//	left = middle = right = NULL;
		len = 0;
	}
	Chain * get_chain() {
		return s;
	}/*
	 String * get_left_string() {
	 return left;
	 }
	 String * get_middle_string() {
	 return middle;
	 }
	 String * get_right_string() {
	 return right;
	 }
	 int get_children_number() {
	 if (right != NULL) return 3;
	 else if (middle != NULL) return 2;
	 else if (left != NULL) return 1;
	 else return 0;;
	 }*/
	void set_length(int _length) {
		len = _length;
	}
	point_type get_length() {
		return len;
	}
	/*int get_point_num() {
	if (s == NULL) {
	int ret = 0;
	if (left != NULL)
	ret += left->get_point_num();
	if (middle != NULL)
	ret += middle->get_point_num();
	if (right != NULL)
	ret += right->get_point_num();
	return ret;
	}
	else return s->get_point_num();
	}*/
};

class Return_val {
public:
	String * new_string;
	int apax;
	Chain * chain[2];
	Return_val() {
		new_string = new String();
		apax = -1;
		chain[0] = NULL;
		chain[1] = NULL;
	}
	Return_val(String * _new_string, int _apax, Chain ** _chain) {
		new_string = _new_string;
		apax = _apax;
		chain[0] = _chain[0];
		chain[1] = _chain[1];
	}
};



class Hourglass {
private:
	int id;
	Edge edge_list[2];
	//Edge first;
	//Edge second;
	Chain * first_chain[2];
	Chain * second_chain[2];
	int apex[2];
	String * s;
public:
	Hourglass & operator=(const Hourglass& rhs) {
		id = rhs.id;

		for (int i = 0; i < 2; i++)
		{
			edge_list[i] = rhs.edge_list[i];
			first_chain[i] = rhs.first_chain[i];
			second_chain[i] = (rhs.second_chain[i]);
			apex[i] = rhs.apex[i];
		}
		s = (rhs.s);
		return *this;
	}

	Hourglass() {
		init();
	}
	Hourglass(int low_point, int high) {
		init();
		edge_list[0] = diagonal_list[low_point];
		edge_list[1] = diagonal_list[high];

		first_chain[0] = new Chain(edge_list[0].get_origin(), edge_list[1].get_origin());
		first_chain[1] = new Chain(edge_list[0].get_origin(), edge_list[1].get_dest());

	}
	Hourglass(int low, int high, SNode * middle) {
		init();
		int middle_diagonal = middle->get_diagonal();
		SNode * node = diagonal_list[low].get_SNode();
		if (middle_diagonal == -1) {
			edge_list[0] = diagonal_list[low];
			edge_list[1] = diagonal_list[high];
			//first = diagonal_list[low];
			//second = diagonal_list[high];

			first_chain[0] = new Chain(&diagonal_list[middle->other_edge(low, high)]);

			//null edge
			int common_point = diagonal_list[low].get_origin();
			if (common_point != diagonal_list[high].get_origin() && common_point != diagonal_list[high].get_dest())
			{
				common_point = diagonal_list[low].get_dest();
			}
			//common_point = diagonal_list[high].get_dest();
			first_chain[1] = new Chain(new Edge(common_point));
		}
	}
	void set_id() {
		id = id_num++;
	}
	Edge * get_edge_list() {
		return edge_list;
	}
	void set_first_edge(Edge _first) {
		edge_list[0] = _first;
	}
	void set_second_edge(Edge _second) {
		edge_list[1] = _second;
	}
	void init() {
		id = -1;
		//id = id_num++;
		for (int i = 0; i < 2; i++) {
			first_chain[i] = NULL;
			second_chain[i] = NULL;
			apex[i] = 0;
		}
		s = NULL;
	}
	int get_id() {
		return id;
	}
	bool check_openess() {
		if (s == NULL) return true;
		return false;
	}
	void set_first_chain(Chain ** _chain) {
		for (int i = 0; i < 2; i++)
			first_chain[i] = _chain[i];
	}
	void set_second_chain(Chain ** _chain) {
		for (int i = 0; i < 2; i++)
			second_chain[i] = _chain[i];
	}
	void set_first_chain(Chain *first, Chain * second) {
		first_chain[0] = first;
		first_chain[1] = second;
	}
	void set_second_chain(Chain *first, Chain * second) {
		second_chain[0] = first;
		second_chain[1] = second;
	}
	void set_string(String * _string) {
		s = _string;
	}
	Chain ** get_first_chain() {
		return first_chain;
	}
	Chain ** get_second_chain() {
		return second_chain;
	}
	String* get_string() {
		return s;
	}
	void set_apex(int _point, int _index) {
		apex[_index] = _point;
	}
	int * get_apaxes() {
		return apex;
	}
	point_type get_len() {
		point_type len = 0;
		if (first_chain[0] != NULL) {
			len += first_chain[0]->get_len();
			len += first_chain[1]->get_len();
		}
		if (second_chain[0] != NULL) {
			len += second_chain[0]->get_len();
			len += second_chain[1]->get_len();
		}
		if (s != NULL) {
			len += 2 * s->get_length();
		}
		return len;
	}
	void duplicate_strings();
	Edge get_first_edge() {
		return edge_list[0];
	}
	Edge get_second_edge() {
		return edge_list[1];
	}
	void clear_chains() {
		for (int i = 0; i < 2; i++)
			first_chain[i] = NULL;
		for (int i = 0; i < 2; i++)
			second_chain[i] = NULL;
	}
	void clear_string() {
		s = NULL;
	}
	void clear_apaxes() {
		apex[0] = -1;
		apex[1] = -1;
	}
	bool check_valid() {
		for (int i = 0; i < 2; i++) {
			if (!first_chain[i]->check_inclusive(edge_list[0].get_origin()) && !first_chain[i]->check_inclusive(edge_list[0].get_dest())) {
				return false;
			}
			if (!first_chain[i]->check_inclusive(s->get_chain()->get_point(0)) && !first_chain[i]->check_inclusive(s->get_chain()->get_last_point())) {
				return false;
			}
			if (!second_chain[i]->check_inclusive(edge_list[1].get_origin()) && !second_chain[i]->check_inclusive(edge_list[1].get_dest())) {
				return false;
			}
			if (!second_chain[i]->check_inclusive(s->get_chain()->get_point(0)) && !second_chain[i]->check_inclusive(s->get_chain()->get_last_point())) {
				return false;
			}
		}
		return true;
	}
};
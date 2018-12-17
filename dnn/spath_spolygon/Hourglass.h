#pragma once
#include <iostream>
#include <algorithm>
#include "Point.h"
#include "Edge.h"
#include "polygon_operation.h"
#include "tangent.h"
#include "Chain.h"
using namespace std;
int id_num=0;
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
	String (String* _left, String* _middle) {
		init();
		s = new Chain(_left->get_chain()->get_point_list());
		s->add_point_list(_middle->get_chain()->get_point_list(),_middle->get_length());
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
class Return_val{
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
	Return_val(String * _new_string,int _apax, Chain ** _chain) {
		new_string = _new_string;
		apax = _apax;
		chain[0] = _chain[0];
		chain[1] = _chain[1];
	}
};
SNode * find_middle_diagonal(SNode * root, int e1, int e2) {
	if (root->get_diagonal() == -1)
		return root;

	SNode ** children = root->get_children();
	bool check = true;
	for (int i = 0; i < 2; i++) {
		check = children[i]->check_inclusive(e1);
		check = check && children[i]->check_inclusive(e2);
		if (check) {
			return find_middle_diagonal(children[i], e1, e2);
		}
	}
	return root;
}

String * duplicate_string(String *s, Chain ** c, int apax) {
	vector<int> p_1 = c[0]->get_point_list();
	vector<int> p_2 = c[1]->get_point_list();

	int it1, it2, diff_it1, diff_it2;
	if (apax == p_1.front()) {
		it1 = 0;
		diff_it1 = 1;
	}
	else {
		it1 = p_1.size() - 1;
		diff_it1 = -1;
	}

	if (apax == p_2.front()) {
		it2 = 0;
		diff_it2 = 1;
	}
	else {
		it2 = p_2.size() - 1;
		diff_it2 = -1;
	}

	it1 = it1 + diff_it1;
	it2 = it2 + diff_it2;
	while (it1 >= 0 && it1 < (int)p_1.size() && it2 >= 0 && it2 < (int)p_2.size()) {
		if (p_1[it1] != p_2[it2]) {
			break;
		}
		it1 = it1 + diff_it1;
		it2 = it2 + diff_it2;
	}
	it1 = it1 - diff_it1;
	it2 = it2 - diff_it2;
	apax = p_1[it1];
	delete(c[0]);
	vector<int> new_s;
	if (diff_it1 == 1) {
		c[0] = new Chain(vector<int>(p_1.begin() + it1, p_1.end()));
		new_s = vector<int>(p_1.begin(), p_1.begin() + it1 + 1);
	}
	else {
		int t1 = p_1.size() - it1;
		c[0] = new Chain(vector<int>(p_1.begin(), p_1.end() - t1+1));
		new_s = vector<int>(p_1.end() - t1, p_1.end());
	}
	delete(c[1]);
	if (diff_it2 == 1) {
		c[1] = new Chain(vector<int>(p_2.begin() + it2, p_2.end()));
	}
	else {
		int t2 = p_2.size() - it2;
		c[1] = new Chain(vector<int>(p_2.begin(), p_2.end() - t2+1));
	}
	String * ret = NULL;
	if (new_s.size() >  1) {
		if (s == NULL)
			ret = new String(new_s);
		else ret = ret = new String(s, new String(new_s));
	}
	return ret;
}
class Hourglass {
private:
	int id;
	Edge edge_list[2];
	//Edge first;
	//Edge second;
	Chain * first_chain[2];
	Chain * second_chain[2];
	int apax[2];
	String * s;
public:
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

			first_chain[0] = new Chain(&diagonal_list[middle ->other_edge(low, high)]);
			//null edge
			int common_point = diagonal_list[low].get_origin();
			if (common_point != diagonal_list[high].get_origin() && common_point != diagonal_list[high].get_dest())
				common_point = diagonal_list[high].get_dest();
			first_chain[1] = new Chain(new Edge(common_point));
		}
	}
	void set_id() {
		id = id_num++;
	}
	Edge * get_edge_list() {
		return edge_list;
	}
	/*Edge get_first_edge() {
		return first;
	}
	Edge get_second_edge() {
		return second;
	}
	void set_first_edge(int _first) {
		first = diagonal_list[_first];
	}
	void set_second_edge(int _second) {
		second = diagonal_list[_second];
	}*/
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
			apax[i] = 0;
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
	void set_first_chain(Chain *first,Chain * second) {
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
	void set_apax(int _point, int _index) {
		apax[_index] = _point;
	}
	int * get_apaxes() {
		return apax;
	}
	point_type get_len() {
		point_type len =0;
		if (first_chain[0] != NULL) {
			len += first_chain[0]->get_len();
			len += first_chain[1]->get_len();
		}
		if (second_chain[0] != NULL) {
			len += second_chain[0]->get_len();
			len += second_chain[1]->get_len();
		}
		if ( s != NULL) {
			len += 2 *s -> get_length();
		}
		return len;
	}
	void duplicate_strings() {
		String * new_s = NULL;
		if (first_chain[0] != NULL) {
			if (s == NULL) {
				int f00 = first_chain[0]->get_point(0), f01 = first_chain[0]->get_last_point();
				int f10 = first_chain[1]->get_point(0), f11 = first_chain[1]->get_last_point();
				int f_a, s_a = -1;
				if (edge_list[0].check_same_point(f00)==-1 && (f00 == f10 || f00 == f11)) {
					s_a = f00;
					f_a = f00;
					new_s = duplicate_string(s, first_chain, f_a);
				}
				else if (edge_list[0].check_same_point(f01)==-1 && (f01 == f10 || f01 == f11)) {
					f_a = f01;
					s_a = f01;
					new_s = duplicate_string(s, first_chain, f_a);
				}

				if (new_s != NULL) {
					s = new_s;
					apax[0] = f_a;
					apax[1] = s_a;
					second_chain[0] = new Chain(s_a);
					second_chain[1] = new Chain(s_a);
				}
			}
			else {
				new_s = duplicate_string(s, first_chain, apax[0]);
				if (new_s != NULL) {
					s = new_s;
				}
			}
		}
		if(second_chain[0] !=NULL) new_s = duplicate_string(s, second_chain, apax[1]);
		if (new_s != NULL) s = new_s;

		return;
	}
	Edge get_first_edge() {
		return edge_list[0];
	}
	Edge get_second_edge() {
		return edge_list[1];
	}
	void clear_chains(){
		for (int i = 0; i < 2; i++)
			first_chain[i] = NULL;
		for (int i = 0; i < 2; i++)
			second_chain[i] = NULL;
	}
	void clear_string() {
		s = NULL;
	}
	void clear_apaxes() {
		apax[0] = -1;
		apax[1] = -1;
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
vector<Hourglass> hourglass_list;
Hourglass concatenate_hourglasses(Hourglass& left, Hourglass& right);
Hourglass concatenate_hourglasses(int h1, int h2);
Hourglass construct_hourglass_point(int p, int high) {
	Edge e(p);
	diagonal_list.push_back(e);
	//hourglass_list.push_back(Hourglass(diagonal_list.size()-1, high));
	return Hourglass(diagonal_list.size() - 1, high);
}
int construct_hourglass(int low, int high) {
	
	SNode * node = diagonal_list[low].get_SNode();
	//-1 return -> triangle
	SNode * middle = find_middle_diagonal(node, low, high);
	Hourglass new_h;
	int middle_diagonal = middle->get_diagonal();
	if (middle_diagonal == -1) {
		new_h = Hourglass(low, high,middle);
	}
	else {
		int h1 = s_graph[middle_diagonal][low];
		if (h1 == -2) h1= construct_hourglass(middle_diagonal,low);

		int h2 = s_graph[middle_diagonal][high];
		if (h2 == -2) h2 = construct_hourglass(middle_diagonal, high);

		new_h = concatenate_hourglasses(h1, h2);
	}
	new_h.set_id();
	hourglass_list.push_back(new_h);
	s_graph[low][high] = new_h.get_id();
	return new_h.get_id();
}
void init_hourglass_val() {
	id_num = 0;
	hourglass_list = vector<Hourglass>();
}
String* concatenate_two_funnels_cc(Chain** left, int apax1, Chain** right, int apax2, Edge * common_edge) {

	Hourglass h_left;
	h_left.set_first_edge(Edge(apax1));
	h_left.set_second_edge(*common_edge);
	h_left.set_first_chain(left);

	Hourglass h_right;
	h_right.set_first_edge(Edge(apax2));
	h_right.set_second_edge(*common_edge);
	h_right.set_first_chain(right);

	Edge * temp = common_edge;
	Hourglass new_h = concatenate_hourglasses(h_right, h_left);
	return new_h.get_string();
	/*String * min = NULL;
	int t1, t2;
	for(int i=0;i<2;i++)
		for (int j = 0; j < 2; j++) {
			 compute_tangent(left[i], right[j],&t1,&t2);
			 vector<int> v_left = left[i]->get_point_list();
			 vector<int> v_right = right[j]->get_point_list();
			 vector<int> new_v;
			 if (v_left[0] != common_edge->get_origin() && v_left[0] != common_edge->get_dest()) {
				 new_v.insert(new_v.end(), v_left.begin(), v_left.begin() + t1 +1);
			 }
			 else {
				 new_v.insert(new_v.end(), v_left.begin()+t1,v_left.end());
				 reverse(new_v.begin(), new_v.end());
			 }

			 if (v_right[0] != common_edge->get_origin() && v_right[0] != common_edge->get_dest()) {
				 if (new_v.back() == *(v_right.begin() + t2)) {
					 vector<int> temp(v_right.begin(), v_right.begin() + t2);
					 reverse(temp.begin(), temp.end());
					 new_v.insert(new_v.end(), temp.begin(), temp.end());
				 }
				 else {
					 vector<int> temp(v_right.begin(), v_right.begin() + t2+1);
					 reverse(temp.begin(), temp.end());
					 new_v.insert(new_v.end(), temp.begin(), temp.end());
				 }
			 }
			 else {
				 if (new_v.back() == *(v_right.begin() + t2)) {
					 new_v.insert(new_v.end(), v_right.begin() + t2, v_right.end());
				 }
				 else {
					 new_v.insert(new_v.end(), v_right.begin() + t2 + 1, v_right.end());
				 }				
			 }
			 
			 String * new_string = new String(new_v);

			 if (min == NULL || min->get_length() > new_string->get_length())
				min = new_string;
			 else delete(new_string);
		}
	return min;*/
}
Return_val concatenate_two_funnels_oc(Chain** left, Chain** right, Edge * common_edge,int apax, Edge right_edge) {
	Hourglass h_left;
	h_left.set_first_edge(Edge(apax));
	h_left.set_second_edge(*common_edge);
	h_left.set_first_chain(left);

	Hourglass h_right;
	h_right.set_first_edge(right_edge);
	h_right.set_second_edge(*common_edge);
	h_right.set_first_chain(right);

	Edge * temp = common_edge;
	Hourglass new_h = concatenate_hourglasses(h_right, h_left);
	common_edge = temp;
	return Return_val(new_h.get_string(), new_h.get_apaxes()[0], new_h.get_first_chain());
}
Return_val concatenate_two_funnels_oc(Chain** left, Chain** right, Edge * common_edge) {
	String * min = NULL;
	int t1, t2;
	String * s_list[2][2];
	Chain * c_list[2][2][2];
	int apax[2][2];
	bool valid[2][2] = { true, true, true, true };
	for (int i = 0; i<2; i++)
		for (int j = 0; j < 2; j++) {
			vector<int> v_left = left[i]->get_point_list();
			vector<int> v_right = right[j]->get_point_list();
			int v_l0 = v_left.front(), v_l1 = v_left.back(), v_r0 = v_right.front(), v_r1 = v_right.back();
			//outer
			if (v_l0 == v_r0 || v_l0 == v_r1 || v_l1 == v_r0 || v_l1 == v_r1) {
				bool check = compute_outer_tangent(left[i], right[j], &t1, &t2,common_edge,left[(i+1)%2],right[(j+1)%2]);
				if(check == false){
					valid[i][j] = false;
					continue;
				}
			}//inner
			else {
				bool check = compute_inner_tangent(left[i], right[j], &t1, &t2, common_edge, left[(i + 1) % 2], right[(j + 1) % 2]);
				if(check == false) {
					valid[i][j] = false;
					continue;
				}
			}

			vector<int> new_v;
			if (common_edge->check_same_point(v_left[0]) == -1) {
				new_v.insert(new_v.end(), v_left.begin(), v_left.begin() + t1 + 1);
			}
			else {
				new_v.insert(new_v.end(), v_left.begin() + t1, v_left.end());
				reverse(new_v.begin(), new_v.end());
			}
			s_list[i][j] = new String(new_v);

			apax[i][j] = v_left[t1];
			c_list[i][j][0] = right[j]->cutting_chain(common_edge, apax[i][j], right[j]);
			c_list[i][j][1] = right[(j + 1) % 2]->cutting_chain(common_edge, apax[i][j], right[(j + 1) % 2]);
		}
	int min_i, min_j;
	point_type min_val = -1;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			if (valid[i][j] == false) continue;
			String * s = duplicate_string(s_list[i][j], c_list[i][j], apax[i][j]);
			if (s != NULL) s_list[i][j] = s;
			point_type len_val = 2 * s_list[i][j]->get_length() + c_list[i][j][0]->get_len() + c_list[i][j][1]->get_len();
			if (min_val == -1 || min_val > len_val) {
				min_val = len_val;
				min_i = i;
				min_j = j;
			}
		}
	}
	return Return_val(s_list[min_i][min_j], apax[min_i][min_j], c_list[min_i][min_j]);
}
Hourglass concatenate_hourglasses(int h1, int h2) {
	return concatenate_hourglasses(hourglass_list[h1], hourglass_list[h2]);
}
Hourglass concatenate_hourglasses(Hourglass& _left, Hourglass& _right) {
	 
	bool left_openess = _left.check_openess();
	bool right_openess = _right.check_openess();
	Edge* left_edge_list = _left.get_edge_list();
	Edge* right_edge_list = _right.get_edge_list();
	
	int common_edge_check[2];

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			if (left_edge_list[i] == right_edge_list[j]) {
				common_edge_check[0] = i;
				common_edge_check[1] = j;
				common_edge = &left_edge_list[i];
			}
		}
	}
	Hourglass new_hourglass;
	
	if (!left_openess && !right_openess) {
		//set left
		Chain ** lc, **rc;
		int la, ra;
		if (common_edge_check[0] == 0) {
			new_hourglass.set_first_edge(left_edge_list[1]);
			new_hourglass.set_first_chain(_left.get_second_chain());
			new_hourglass.set_apax(_left.get_apaxes()[1],0);
			lc = _left.get_first_chain();
			la = _left.get_apaxes()[0];
		}
		else {
			new_hourglass.set_first_edge(left_edge_list[0]);
			new_hourglass.set_first_chain(_left.get_first_chain());
			new_hourglass.set_apax(_left.get_apaxes()[0], 0);
			lc = _left.get_second_chain();
			la = _left.get_apaxes()[1];
		}
		//set right
		if (common_edge_check[1] == 0) {
			new_hourglass.set_second_edge(right_edge_list[1]);
			new_hourglass.set_second_chain(_right.get_second_chain());
			new_hourglass.set_apax(_right.get_apaxes()[1], 0);
			rc = _right.get_first_chain();
			ra = _right.get_apaxes()[0];
		}
		else {
			new_hourglass.set_second_edge(right_edge_list[0]);
			new_hourglass.set_second_chain(_right.get_first_chain());
			new_hourglass.set_apax(_right.get_apaxes()[0], 0);
			rc = _right.get_second_chain();
			ra = _right.get_apaxes()[1];
		}

		String* middle_string = concatenate_two_funnels_cc(lc, la,rc, ra, common_edge);
		String* new_string;
		if (middle_string != NULL) {
			new_string = new String(_left.get_string(), middle_string);
			new_string = new String(new_string, _right.get_string());
			new_hourglass.set_string(new_string);
		}
		else {
			new_string = new String(_left.get_string(), _right.get_string());
			new_hourglass.set_string(new_string);
		}
	}//left : close, right : open
	else if (!left_openess && right_openess) {
		
		Return_val r_val;
		new_hourglass.set_string(_left.get_string());
		if (common_edge_check[1] == 0) {
			new_hourglass.set_second_edge(right_edge_list[1]);
		}
		else {
			new_hourglass.set_second_edge(right_edge_list[0]);
		}
		//set left
		if (common_edge_check[0] == 0) {
			new_hourglass.set_first_edge(left_edge_list[1]);
			new_hourglass.set_first_chain(_left.get_second_chain());
			new_hourglass.set_apax(_left.get_apaxes()[1], 0);
			r_val = concatenate_two_funnels_oc(_left.get_first_chain(), _right.get_first_chain(), common_edge, _left.get_apaxes()[0], new_hourglass.get_second_edge());
			new_hourglass.set_apax(_left.get_apaxes()[0], 1);
		}
		else {
			new_hourglass.set_first_edge(left_edge_list[0]);
			new_hourglass.set_first_chain(_left.get_first_chain());
			new_hourglass.set_apax(_left.get_apaxes()[0], 0);
			r_val = concatenate_two_funnels_oc(_left.get_second_chain(), _right.get_first_chain(), common_edge, _left.get_apaxes()[1], new_hourglass.get_second_edge());
			new_hourglass.set_apax(_left.get_apaxes()[1], 1);
		}
		if (r_val.new_string != NULL) {
			new_hourglass.set_string(new String(_left.get_string(), r_val.new_string));
			new_hourglass.set_apax(r_val.apax, 1);
		}
		new_hourglass.set_second_chain(r_val.chain);
		
	}else if (left_openess && !right_openess) {
		new_hourglass.set_string(_right.get_string());
		if (common_edge_check[0] == 0) {
			new_hourglass.set_first_edge(left_edge_list[1]);
		}
		else {
			new_hourglass.set_first_edge(left_edge_list[0]);
		}
		Return_val r_val;

		if (common_edge_check[1] == 0) {
			new_hourglass.set_second_edge(right_edge_list[1]);
			new_hourglass.set_second_chain(_right.get_second_chain());
			new_hourglass.set_apax(_right.get_apaxes()[1], 1);
			r_val = concatenate_two_funnels_oc(_right.get_first_chain(), _left.get_first_chain(), common_edge, _right.get_apaxes()[0], new_hourglass.get_first_edge());
			new_hourglass.set_apax(_right.get_apaxes()[0], 0);
		}
		else {
			new_hourglass.set_second_edge(right_edge_list[0]);
			new_hourglass.set_second_chain(_right.get_first_chain());
			new_hourglass.set_apax(_right.get_apaxes()[0], 1);
			r_val = concatenate_two_funnels_oc(_right.get_second_chain(), _left.get_first_chain(), common_edge, _right.get_apaxes()[1], new_hourglass.get_first_edge());
			new_hourglass.set_apax(_right.get_apaxes()[1], 0);
		}
		if (r_val.new_string != NULL) {
			new_hourglass.set_string(new String(r_val.new_string, _right.get_string()));
			new_hourglass.set_apax(r_val.apax, 0);
		}
		new_hourglass.set_first_chain(r_val.chain);
	}else {
		//set first_edge and second edge
		if (common_edge_check[0] == 0) {
			new_hourglass.set_first_edge(left_edge_list[1]);
		}
		else {
			new_hourglass.set_first_edge(left_edge_list[0]);
		}
		if (common_edge_check[1] == 0) {
			new_hourglass.set_second_edge(right_edge_list[1]);
		}
		else {
			new_hourglass.set_second_edge(right_edge_list[0]);
		}

		//set outer chain
		Chain** left = _left.get_first_chain(), **right = _right.get_first_chain();
		int outer_chain[2][2];
		int p1 = common_edge->get_origin();
		
		if ((left[0]->check_inclusive(p1) && left[1]->check_inclusive(p1)) || (right[0]->check_inclusive(p1) && right[1]->check_inclusive(p1))){
			p1 = common_edge->get_dest();
		}
		vector<int> left_0_point_list = left[0]->get_point_list();
		if (left_0_point_list.front() == p1 || left_0_point_list.back() == p1) {
			outer_chain[0][0] = 0;//= left[0]
			outer_chain[1][0] = 1;// left[1];
		}
		else {
			outer_chain[1][0] = 0;// left[0];
			outer_chain[0][0] = 1;// left[1];
		}
		vector<int> right_0_point_list = right[0]->get_point_list();
		if (right_0_point_list.front() == p1 || right_0_point_list.back() == p1) {
			outer_chain[0][1] = 0;// right[0];
			outer_chain[1][1] = 1;// right[1];
		}
		else {
			outer_chain[1][1] = 0;// right[0];
			outer_chain[0][1] = 1;// right[1];
		}

		//compute outer_chain
		Hourglass outer_hour;
		bool check = true;
		Chain *left_c_list[2];
		Chain *right_c_list[2];
		int apax[2][2];
		for (int i = 0; i < 2; i++) {
			Chain * left_upper_chain = left[outer_chain[i][0]];
			Chain * right_upper_chain = right[outer_chain[i][1]];
			Chain * left_down_chain = left[outer_chain[(i+1)%2][0]];
			Chain * right_down_chain = right[outer_chain[(i + 1) % 2][1]];
			int t1, t2;
			bool valid = compute_outer_tangent(left_upper_chain, right_upper_chain, &t1, &t2, common_edge, left_down_chain, right_down_chain);
			
			if (valid) {
				apax[i][0] = left_upper_chain->get_point(t1);
				left_c_list[i] = left_upper_chain->cutting_chain(common_edge, apax[i][0],left_upper_chain);
				apax[i][1] = right_upper_chain->get_point(t2);
				right_c_list[i] = right_upper_chain->cutting_chain(common_edge, apax[i][1], right_upper_chain);
			}
			else {
				check = false;
			}
		}
		if (check) {
			Chain * new_chain[2];
			for (int i = 0; i < 2; i++) {
				Chain * c1 = left_c_list[i];
				Chain * c2 = right_c_list[i];
				int p1 = apax[i][0];
				int p2 = apax[i][1];
				new_chain[i] = new Chain(c1, c2, p1, p2);
			}
			new_hourglass.set_first_chain(new_chain);
			new_hourglass.duplicate_strings();
			outer_hour = new_hourglass;
			//return outer_hour;
		}
		//compute inner chain
		Hourglass h[2];
		bool h_valid[2] = { true, true };
		for (int i = 0; i < 2; i++) {

			Chain * left_upper_chain = left[outer_chain[i][0]];
			Chain * right_upper_chain = right[outer_chain[i][1]];
			Chain * left_down_chain = left[outer_chain[(i + 1) % 2][0]];
			Chain * right_down_chain = right[outer_chain[(i + 1) % 2][1]];
			int t1, t2;
			
			bool check = compute_inner_tangent(left_upper_chain, right_down_chain, &t1, &t2, common_edge, left_down_chain, right_upper_chain);
			if (check == false) {
				h_valid[i] = false;
				continue;
			}
			int apax1 = left_upper_chain->get_point(t1), apax2 = right_down_chain->get_point(t2);

			int apax[2] = { apax1, apax2 };
			Hourglass hh[2];
			bool hh_valid[2] = { true, true };
			for (int k = 0; k < 2; k++) {
				new_hourglass.clear_chains();
				new_hourglass.clear_apaxes();
				new_hourglass.clear_string();
				apax1 = apax[0];
				apax2 = apax[1];
				//&&side_check(left_upper_chain, right_down_chain, left_down_chain, right_upper_chain, apx)
				if (k == 0 && right_upper_chain->check_inclusive(apax1)) {
					int edge_point = right_upper_chain->get_point(0);
					if (common_edge->check_same_point(edge_point) != -1) {
						edge_point = right_upper_chain->get_last_point();
					}
					if (right_upper_chain->check_sequence(apax1, edge_point)) {
						apax2 = apax1;
					}					
				}					
				if (k == 1 && left_down_chain->check_inclusive(apax2)) {
					int edge_point = left_down_chain->get_point(0);
					if (common_edge->check_same_point(edge_point) != -1) {
						edge_point = left_down_chain->get_last_point();
					}
					if (left_down_chain->check_sequence(apax2, edge_point)) {
						apax1 = apax2;
					}
				}
				Chain * cutting_chain_u;
				Chain * cutting_chain_d;
				Chain * ret[2];
				cutting_chain_u = left_upper_chain->cutting_chain(common_edge, apax1, left_upper_chain);
				cutting_chain_d = left_down_chain->cutting_chain(common_edge, apax1, left_upper_chain);
				ret[0] = cutting_chain_u;
				ret[1] = cutting_chain_d;
				new_hourglass.set_first_chain(ret);
				new_hourglass.set_apax(apax1, 0);
				new_hourglass.set_string(new String(apax1, apax2));

				cutting_chain_u = right_upper_chain->cutting_chain(common_edge, apax2, right_down_chain);
				cutting_chain_d = right_down_chain->cutting_chain(common_edge, apax2, right_down_chain);
				ret[0] = cutting_chain_u;
				ret[1] = cutting_chain_d;
				new_hourglass.set_second_chain(ret);
				new_hourglass.set_apax(apax2, 1);
				
				if (new_hourglass.check_valid()) {
					new_hourglass.duplicate_strings();
					hh[k] = new_hourglass;
				}
				else {
					hh_valid[k] = false;
				}
			}
			point_type min_len = -1;
			for (int k = 0; k < 2; k++) {
				if (hh_valid[k] && (min_len == -1 || min_len > hh[k].get_len())) {
					h[i] = hh[k];
					min_len = hh[k].get_len();
				}
			}
			if (min_len == -1) {
				h_valid[i] = false;
			}
			
			/*if (right_upper_chain->check_inclusive(apax1)) {
				apax2 = apax1;
			}

			if (left_down_chain->check_inclusive(apax2)) {
				apax1 = apax2;
			}
 
			Chain * cutting_chain_u;
			Chain * cutting_chain_d;
			Chain * ret[2];
			cutting_chain_u = left_upper_chain->cutting_chain(common_edge, apax1, left_upper_chain);
			cutting_chain_d = left_down_chain->cutting_chain(common_edge, apax1, left_upper_chain);
			ret[0] = cutting_chain_u;
			ret[1] = cutting_chain_d;
			new_hourglass.set_first_chain(ret);
			new_hourglass.set_apax(apax1, 0);
			new_hourglass.set_string(new String(apax1, apax2));

			cutting_chain_u = right_upper_chain->cutting_chain(common_edge, apax2, right_down_chain);
			cutting_chain_d = right_down_chain->cutting_chain(common_edge, apax2, right_down_chain);
			ret[0] = cutting_chain_u;
			ret[1] = cutting_chain_d;
			new_hourglass.set_second_chain(ret);
			new_hourglass.set_apax(apax2, 1);
			
			if (new_hourglass.check_valid()) {
				new_hourglass.duplicate_strings();
				h[i] = new_hourglass;
			}
			else {
				h_valid[i] = false;
			}*/
			
		}
		Hourglass min;
		point_type min_len =-1;
		for (int i = 0; i < 2; i++) {
			if (h_valid[i] && (min_len == -1 || min_len > h[i].get_len())) {
				min = h[i];
				min_len = h[i].get_len();
			}
		}

		if (check && (min_len == -1 || min_len > outer_hour.get_len())) {
			min = outer_hour;
			min_len = outer_hour.get_len();
		}
		if (min_len == -1) {
			cout << "Somthings wrong" << endl;
		}
		return min;
	}
	return new_hourglass;
}
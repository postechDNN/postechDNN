#pragma once
#include <iostream>
#include <vector>
#include "Point.h"
#include "Edge.h"
#include "polygon_operation.h"
using namespace std;
class Chain {
private:
	vector<int> c_point_list;
	point_type len;
public:
	Chain() {
		len = 0;
	}
	Chain(int p) {
		len = 0;
		c_point_list.push_back(p);
	}
	Chain(int p1, int p2) {
		if (p1 == p2) {
			len = 0;
			c_point_list.push_back(p1);
		}
		else {
			len = dist(p1, p2);
			c_point_list.push_back(p1);
			c_point_list.push_back(p2);
		}
	}
	Chain(vector<int> _point_list) {
		len = 0;
		int prev = -1;
		int c_index = 0;
		for (int i = 0; i < (int)_point_list.size(); i++) {
			if (prev == _point_list[i]) continue;
			prev = i;
			c_point_list.push_back(_point_list[i]);
			if (i == 0) continue;
			c_index++;
			len += dist(c_point_list[c_index - 1], c_point_list[c_index]);
		}
	}
	Chain(Chain * c1, Chain * c2, int p1, int p2) {
		vector<int> v_c1 = c1->get_point_list();
		vector<int> v_c2 = c2->get_point_list();

		c_point_list = v_c1;
		if (p1 == v_c1[0]) {
			reverse(c_point_list.begin(), c_point_list.end());
		}
		if (p1 == p2) {
			c_point_list.pop_back();
		}

		vector<int> temp;
		temp = v_c2;
		if (p2 != v_c2[0]) {
			reverse(temp.begin(), temp.end());
		}
		c_point_list.insert(c_point_list.end(), temp.begin(), temp.end());

		len = 0;
		for (int i = 1; i < (int)c_point_list.size(); i++) {
			len += dist(c_point_list[i - 1], c_point_list[i]);
		}
	}
	Chain(Edge* _tan) {
		//edge_list.push_back(_tan);
		if (_tan->is_point()) {
			c_point_list.push_back(_tan->get_origin());
		}
		else {
			c_point_list.push_back(_tan->get_origin());
			c_point_list.push_back(_tan->get_dest());
		}
		len = _tan->get_len();
	}
	int get_point(int index) {
		return c_point_list[index];
	}
	void set_point_list(vector<int> _point_list) {
		c_point_list = _point_list;
	}
	void add_point_list(vector<int> _point_list, point_type length = 0) {
		if (_point_list[0] == c_point_list[0]) {
			reverse(_point_list.begin(), _point_list.end());
			_point_list.pop_back();
			c_point_list.insert(c_point_list.begin(), _point_list.begin(), _point_list.end());
		}
		else if (_point_list[0] == c_point_list.back()) {
			c_point_list.pop_back();
			c_point_list.insert(c_point_list.end(), _point_list.begin(), _point_list.end());
		}
		else if (_point_list.back() == c_point_list.front()) {
			_point_list.pop_back();
			c_point_list.insert(c_point_list.begin(), _point_list.begin(), _point_list.end());
		}
		else if (_point_list.back() == c_point_list.back()) {
			_point_list.pop_back();
			reverse(_point_list.begin(), _point_list.end());
			c_point_list.insert(c_point_list.end(), _point_list.begin(), _point_list.end());
		}
		else {
			c_point_list.insert(c_point_list.end(), _point_list.begin(), _point_list.end());
		}
		len += length;
	}
	void set_len(point_type _len) {
		len = _len;
	}
	point_type get_len() {
		return len;
	}
	vector<int> get_point_list() {
		return c_point_list;
	}
	int check_one_side(int inner_point, int outter_point, Edge * common_edge) {
	
		int p1 = -1;
		if (check_inclusive(inner_point)) {
			p1 = inner_point;
		}
		else if(check_inclusive(outter_point)){
			p1 = outter_point;
		}
		int idx = -1, diff_idx = -1;
		if (common_edge->get_origin() == c_point_list[0] || common_edge->get_dest() == c_point_list[0]) {
			idx = 0;
			diff_idx = 1;
		}
		else {
			idx = c_point_list.size() - 1;
			diff_idx = -1;
		}
		int n = 0, m = 0;
		int num = 0;
		while (idx >= 0 && idx < (int)c_point_list.size() && c_point_list[idx] != p1) {
			num++;
			if (is_left(c_point_list[idx], inner_point, outter_point))
				n++;
			if (is_right(c_point_list[idx], inner_point, outter_point))
				m++;
			idx += diff_idx;
		}
		int r1 = -1;
		if (m == 0 && n == 0) {
			r1 = 3;
		}
		if (m == num && n == num) {
			r1 = 3;
		}
		else if (m == num) {
			r1 = 2;
		}
		else if (n == num) {
			r1 = 1;
		}
		else r1 = 0;

		num = n = m = 0;
		while (idx >= 0 && idx < (int)c_point_list.size()) {
			num++;
			if (is_left(c_point_list[idx], inner_point, outter_point))
				n++;
			if (is_right(c_point_list[idx], inner_point, outter_point))
				m++;
			idx += diff_idx;
		}
		int r2 = -1;
		if (m == 0 && n == 0) {
			r2 = 3;
		}
		else if (m == num && n == num) {
			r2 = 3;
		}
		else if (m == num) {
			r2 = 2;
		}
		else if (n == num) {
			r2 = 1;
		}
		else r2 = 0;

		if (r1 != 0 && r2 != 0) {
			return r1;
			/*if (r1 == 3 && r2 == 3) return 3;
			else if (r1 == 3 && r2 != 3) return r2;
			else if (r1 != 3 && r2 == 3) return r1;
			else return -1;*/
		}
		else return 0;
	}
	int check_one_side(int p1, int p2) {
		int n = 0;
		int m = 0;
		for (int i = 0; i < (int)c_point_list.size(); i++) {
			if (is_left(c_point_list[i], p1, p2))
				n++;
			if (is_right(c_point_list[i], p1, p2))
				m++;
		}
		if (m == (int)c_point_list.size() && n == (int)c_point_list.size()) {
			return 3;
		}
		else if (m == (int)c_point_list.size()) {
			return 2;
		}
		else if (n == (int)c_point_list.size()) {
			return 1;
		}
		else return 0;
	}
	vector<int> cutting_c_point_list(Edge * common_chain, int idx, bool first) {
		vector<int> new_v;
		if (c_point_list[0] == common_chain->get_dest() || c_point_list[0] == common_chain->get_origin()) {
			new_v = vector<int>(c_point_list.begin() + idx, c_point_list.end());
			reverse(new_v.begin(), new_v.end());
		}
		else {
			new_v = vector<int>(c_point_list.begin(), c_point_list.begin() + idx + 1);
		}
		if (first) {
			return new_v;
		}
		else {
			reverse(new_v.begin(), new_v.end());
			return new_v;
		}
	}

	Chain* cutting_chain(Edge * common_chain, int idx, bool first) {
		vector<int> new_v = cutting_c_point_list(common_chain, idx, first);
		return new Chain(new_v);
	}
	Chain* cutting_chain(Edge * common_chain, int idx, bool first, int last_point) {
		vector<int> new_v = cutting_c_point_list(common_chain, idx, first);
		if (new_v.size() != 0 && new_v.back() != last_point)
			new_v.push_back(last_point);
		return new Chain(new_v);
	}
	bool check_enable_line(int p1, int p2, Edge * common_edge, Chain * c1) {

		int check_side = c1->check_one_side(p1, p2, common_edge);
		if (check_side == 0) return false;
		
		return true;
		bool check = check_line_intersection(p1, p2, common_edge->get_origin(), common_edge->get_dest());
		if (check) return true;
		
		/*int check1 = common_edge->check_same_point(p1);
		int check2 = common_edge->check_same_point(p2);
		bool seq1 = c1->check_sequence(p1,p2);
		bool seq2 = check_sequence(p1,p2);
		if ((check1 != -1 || check2 != -1) && (seq1 || seq2))
			return true;
		return false;*/
	}
	Chain* cutting_chain(Edge * common_edge, int apax, Chain * c1) {
		int index = -1;
		if (common_edge->check_same_point(c_point_list.front()) == -1) {
			for (int k = 0; k < (int)c_point_list.size(); k++) {
				bool check1 = true;
				check1 = check_enable_line(apax, c_point_list[k], common_edge, c1);//c1->check_one_side(apax, c_point_list[k], common_edge);
				if (check1){// != 0) {
					index = k;
					break;
				}
			}
		}
		else {
			for (int k = (int)c_point_list.size() - 1; k >= 0; k--) {
				bool check1 = true;
				check1 = check_enable_line(apax, c_point_list[k], common_edge, c1);//c1->check_one_side(apax, c_point_list[k], common_edge);
				if (check1){// != 0) {
					index = k;
					break;
				}
			}
		}
		return (this->cutting_chain(common_edge, index, true, apax));
	}
	//고쳐야함
	/*Chain* cutting_chain(Edge * common_edge, int apax, Chain * c1, Chain* c2, bool reverse = false) {
		int index = -1;
		if (reverse == false) {

		}
		else {

		}
		if (common_edge->check_same_point(c_point_list.front()) == -1) {
			for (int k = 0; k < (int)c_point_list.size(); k++) {
				int check1 = 0, check2;
				check1 = c1->check_one_side(apax, c_point_list[k],common_edge);
				check2 = c2->check_one_side(apax, c_point_list[k],common_edge);
				if ((check1 == 3 && check2 == 3) || (check1 != 0 && check2 != 0)) {
					index = k;
					break;
				}
			}
		}
		else {
			for (int k = (int)c_point_list.size() - 1; k >= 0; k--) {
				int check1 = 0, check2;
				check1 = c1->check_one_side(apax, c_point_list[k],common_edge);
				check2 = c2->check_one_side(apax, c_point_list[k],common_edge);
				if ((check1 == 3 && check2 == 3) || (check1 != 0 && check2 != 0)) {
					index = k;
					break;
				}
			}
		}
		return (this->cutting_chain(common_edge, index, true, apax));
	}*/
	bool check_inclusive(int p) {
		if (find(c_point_list.begin(), c_point_list.end(), p) != c_point_list.end())
			return true;
		else return false;
	}
	int get_point_num() {
		return c_point_list.size();
	}
	int get_last_point() {
		return c_point_list.back();
	}
	bool check_intersection(int p1, int p2) {
		for (int i = 1; i < c_point_list.size(); i++) {
			if (check_line_intersection(c_point_list[i - 1], c_point_list[i], p1, p2)) {
				return true;
			}
		}
		return false;
	}
	bool check_sequence(int p1, int p2) {
		vector<int>::iterator it1 =  find(c_point_list.begin(), c_point_list.end(), p1);
		vector<int>::iterator it2 = find(c_point_list.begin(), c_point_list.end(), p2);
		if (it1 == c_point_list.end() || it2 == c_point_list.end())
			return false;
		if (abs(it1 - it2) == 1)
			return true;
		else return false;
	}
};
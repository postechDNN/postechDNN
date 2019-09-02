#pragma once
#include "Hourglass.h"
#include <algorithm>
#include <set>
#include "Point.h"
#include "Edge.h"
#include "polygon_decomposition.h"
#include "DebugPrint.h"
#include "tangent.h"

#define HORIZONTAL_MAX 10000 //used in check_inclusive function

enum state {
	neutral, //none of the edge points share the same y-value
	pending, //both of the edge points share the same y-value + both are to the right of the test point
	upward, //one of the edge points share the same y-value + the other one has a bigger y-value
	downward, //one of the edge points share the same y-value + the other one has a smaller y-value
};

Edge compute_tangent(vector<int>, int, bool);
bool set_current_state(state&, point_type, point_type, Point, Point);
bool check_inclusive(vector<int>, int);
int check_inclusive(vector<int>, int, int);

vector<int> inspect_intruders(Edge tangent, vector<int> other_side, vector<int> same_side, bool upper);



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
		c[0] = new Chain(vector<int>(p_1.begin(), p_1.end() - t1 + 1));
		new_s = vector<int>(p_1.end() - t1, p_1.end());
	}
	delete(c[1]);
	if (diff_it2 == 1) {
		c[1] = new Chain(vector<int>(p_2.begin() + it2, p_2.end()));
	}
	else {
		int t2 = p_2.size() - it2;
		c[1] = new Chain(vector<int>(p_2.begin(), p_2.end() - t2 + 1));
	}
	String * ret = NULL;
	if (new_s.size() >  1) {
		if (s == NULL)
			ret = new String(new_s);
		else ret = ret = new String(s, new String(new_s));
	}
	return ret;
}
void Hourglass::duplicate_strings() {
	String * new_s = NULL;
	if (first_chain[0] != NULL) {
		if (s == NULL) {
			int f00 = first_chain[0]->get_point(0), f01 = first_chain[0]->get_last_point();
			int f10 = first_chain[1]->get_point(0), f11 = first_chain[1]->get_last_point();
			int f_a, s_a = -1;
			if (edge_list[0].check_same_point(f00) == -1 && (f00 == f10 || f00 == f11)) {
				s_a = f00;
				f_a = f00;
				new_s = duplicate_string(s, first_chain, f_a);
			}
			else if (edge_list[0].check_same_point(f01) == -1 && (f01 == f10 || f01 == f11)) {
				f_a = f01;
				s_a = f01;
				new_s = duplicate_string(s, first_chain, f_a);
			}

			if (new_s != NULL) {
				s = new_s;
				apex[0] = f_a;
				apex[1] = s_a;
				second_chain[0] = new Chain(s_a);
				second_chain[1] = new Chain(s_a);
			}
		}
		else {
			new_s = duplicate_string(s, first_chain, apex[0]);
			if (new_s != NULL) {
				s = new_s;
			}
		}
	}
	if (second_chain[0] != NULL) new_s = duplicate_string(s, second_chain, apex[1]);
	if (new_s != NULL) s = new_s;

	return;
}
vector<Hourglass> hourglass_list;
Hourglass concatenate_hourglasses(Hourglass& left, Hourglass& right);
Hourglass concatenate_hourglasses(int h1, int h2);
Hourglass construct_hourglass_point(int p, int high) {
	Edge e(p);
	diagonal_list.push_back(e);
	//hourglass_list.push_back(Hourglass(diagonal_list.size()-1, high));
	return Hourglass(diagonal_list.size() - 1, high);//방금 추가한 point edge 로 funnel? 만드는 셈
}
int construct_hourglass(int low, int high) {
	low %= v_num;
	high %= v_num;
	SNode * node = diagonal_list[low].get_SNode();
	//-1 return -> triangle
	SNode * middle = find_middle_diagonal(node, low, high);
	Hourglass new_h;
	int middle_diagonal = middle->get_diagonal();
	if (middle_diagonal == -1) {
		new_h = Hourglass(low, high, middle);
	}
	else {
		int h1 = s_graph[middle_diagonal][low];
		if (h1 == -2) h1 = construct_hourglass(middle_diagonal, low);

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
Return_val concatenate_two_funnels_oc(Chain** left, Chain** right, Edge * common_edge, int apax, Edge right_edge) {
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
				bool check = compute_outer_tangent(left[i], right[j], &t1, &t2, common_edge, left[(i + 1) % 2], right[(j + 1) % 2]);
				if (check == false) {
					valid[i][j] = false;
					continue;
				}
			}//inner
			else {
				bool check = compute_inner_tangent(left[i], right[j], &t1, &t2, common_edge, left[(i + 1) % 2], right[(j + 1) % 2]);
				if (check == false) {
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



Edge compute_tangent( vector<int> concatenated_point_list, int common_point_index, bool upper) {
	/*
	@brief returns the tangent connecting the left and right chains

	@param concatenated_point_list : the connected chain of _left and _right
	@param common_point_index : the index(within the concatenated_point_list) of the common point that the left and right share
	@param upper : whether the chain is an upper chain or a lower chain
	*/
	int samePoint = concatenated_point_list[common_point_index];
	bool point_tangent = true;

	bool(*one_sided_test)(vector<int>, int, int) = upper ? all_left : all_right;

	if (concatenated_point_list.size() == 2)
		return Edge(concatenated_point_list[0], concatenated_point_list[1]);

	for (int i = 0; i < concatenated_point_list.size() - 1; i++)
	{
		if (!one_sided_test(concatenated_point_list, concatenated_point_list[i], concatenated_point_list[i + 1]))
		{
			point_tangent = false;
			break;
		}
	}

	if (point_tangent) //tangent is a single point
	{
		return Edge(samePoint, samePoint);
	}
	else {
		for (int i = 0; i <= common_point_index; i++) {
			for (int j = common_point_index; j < concatenated_point_list.size(); j++) {
				int left = concatenated_point_list[i];
				int right = concatenated_point_list[j];
				if (one_sided_test(concatenated_point_list, left, right)) {
					if (left==right)
						samePoint = left;
					else//generally, the tangent should contain a point from the left and a point from the right
						return Edge(left,right);
				}
			}
		}
		return Edge(samePoint, samePoint);
	}
}
bool set_current_state(state& current_state, point_type x, point_type y, Point p1, Point p2)
{
	/*
	@brief: sets the current state depending on the x,y values of the test point and the two edge points

	@param left_point_list, right_point_list : vectors that represent each chain's point list
	@param sum : the connected chain of _left and _right
	@param common : the point where the _left and _right chains meet ("common")
	@param upper : whether the chain is an upper chain or a lower chain
	*/

	//state meaning explained in the above enum declaration
	current_state = neutral;

	point_type p1_y = p1.get_y();
	point_type p2_y = p2.get_y();
	point_type p1_x = p1.get_x();
	point_type p2_x = p2.get_x();

	if (p1_y == y && p1_x > x)
	{
		if (p1_y > p2_y)
			current_state = downward;
		else if (p1_y < p2_y)
			current_state = upward;
		else
			current_state = pending;

		return true;
	}
	else if (p2_y == y && p2_x>x)
	{
		if (p1_y > p2_y)
			current_state = upward;
		else if (p1_y < p2_y)
			current_state = downward;
		else
			current_state = pending;
		return true;
	}
	else
		return false;
}
bool check_inclusive(vector<int> chain, int test_point)
{
	/*
	@brief: returns whether test_point is included in the area bounded by points in the chain

	@param chain : the points that make up the boundary
	@param test_point : the point we are checking

	for more information on the algorithm, check out the link below
	https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
	*/
	Point test = point_list[test_point];
	point_type test_x = test.get_x();
	point_type test_y = test.get_y();

	Point horizontal(HORIZONTAL_MAX, test_y);

	//trivial case - chain is a single edge - > return true only when the test_point is on the edge
	if (chain.size() == 2)
		return check_line_intersection_open(test, test, point_list[chain[0]], point_list[chain[1]]);

	chain.push_back(chain.front());

	int count = 0;
	Point from = point_list[chain[0]];
	Point to;
	state previous_state = neutral;
	state current_state = neutral;

	//we test each edge bounding the area and count the intersections it has with the horizontal line starting at the test point
	for (int i = 0; i < chain.size() - 1; i++)
	{
		to = point_list[chain[i + 1]];

		if (set_current_state(current_state, test_x, test_y, from, to)) //special case when the test point and edge points have the same y value
		{
			if (previous_state == upward)
			{
				previous_state = neutral;
				if (current_state == pending)
					previous_state = upward;
				else if (current_state == downward)
					count++;
			}
			else if (previous_state == downward)
			{
				previous_state = neutral;
				if (current_state == pending)
					previous_state = downward;
				else if (current_state == upward)
					count++;
			}
			else
			{
				if (previous_state == neutral && current_state == pending)
					count++;
				previous_state = current_state;
			}
		}
		else {//the general case
			if (check_line_intersection_open(test, horizontal, from, to))
				count++;
		}
		from = to;
	}
	if (count % 2 == 0)
		return false;
	else
		return true;
}
int check_inclusive(vector<int> chain, int test_point, int neighbor)
{
	/*
	DERIVED FROM bool check_inclusive(....)
	
	@brief : when the test_point is included in the chain, returns index of the edge that the test_point penetrates through

	@param chain, test_point : same as the ones in bool check_inclusive(....)
	@param neighbor : the neighboring point of test_point that has an edge in the current hourglass chain
	*/

	Point test = point_list[test_point];
	point_type test_x = test.get_x();
	point_type test_y = test.get_y();

	Point horizontal(HORIZONTAL_MAX, test_y);

	//trivial case - chain is a single edge - > return true only when the test_point is on the edge
	if (chain.size() == 2)
		return check_line_intersection_open(test, test, point_list[chain[0]], point_list[chain[1]]);

	chain.push_back(chain.front());

	int count = 0;
	Point from = point_list[chain[0]];
	Point to;
	state previous_state = neutral;
	state current_state = neutral;

	int edge_num = -1;
	if (neighbor == -1)
	{
		printf("not a valid neighbor!\n");
		exit(12);
	}

	for (int i = 0; i < chain.size() - 1; i++)
	{
		to = point_list[chain[i + 1]];

		//the added part
		if (i<chain.size() - 2 && check_line_intersection(test_point, neighbor, chain[i], chain[i + 1],true))
			edge_num = i;

		if (set_current_state(current_state, test_x, test_y, from, to))
		{
			if (previous_state == upward)
			{
				previous_state = neutral;
				if (current_state == pending)
					previous_state = upward;
				else if (current_state == downward)
					count++;
			}
			else if (previous_state == downward)
			{
				previous_state = neutral;
				if (current_state == pending)
					previous_state = downward;
				else if (current_state == upward)
					count++;
			}
			else
			{
				if (previous_state == neutral && current_state == pending)
					count++;
				previous_state = current_state;
			}
		}
		else {//general case
			if (check_line_intersection_open(test, horizontal, from, to))
				count++;
		}

		from = to;
	}

	if (count % 2 == 0)
		return -1;
	else
	{
		if (edge_num == -1)
			exit(11);
		return edge_num;
	}
}
vector<int> inspect_intruders(Edge tangent, vector<int> other_side, vector<int> same_side, bool upper)
{
	/*
	@brief: given boundary (tangent & same_side) , return any points from the 'other_side' that are included in the area => they will become outliers, making the tangent INVALID
	
	@param tangent: edge on the same side, same_side : the subsequence starting and ending with the points of the tangent will make a 'boundary'
	@param other_side: all the points on the other side that make up the chain
	@param upper: whether the chain is upper or lower
	*/

	bool(*which_side)(int, int, int) = upper ? is_left : is_right;
	vector<int> included;
	vector<int> boundary;

	if (tangent.is_point())
		return included;

	int from = (tangent.is_reverse()) ? tangent.get_dest() : tangent.get_origin();
	int to = (tangent.is_reverse()) ? tangent.get_origin() : tangent.get_dest();

	vector<int>::iterator from_it = find(same_side.begin(), same_side.end(), from);
	vector<int>::iterator to_it = find(same_side.begin(), same_side.end(), to);
	
	boundary.insert(boundary.end(), from_it, to_it);
	boundary.push_back(to);

	//With the boundary set, we now check every point in the other side to see if they're included in the area (boundary included)
	for (int i = 0; i < other_side.size(); i++)
	{
		if (find(boundary.begin(), boundary.end(), other_side[i]) == boundary.end()) {
			if (check_inclusive(boundary, other_side[i]))
				included.push_back(other_side[i]);
		}
	}

	return included;
}
vector<int> final_tangent_chain(Edge tangent, vector<int> outliers, vector<int> same_side, bool upper)
{
	/*
	@brief: considering the original tangent, outliers and same_side points, return the ultimate chain
	
	@param tangent : the edge that has the start & end points
	@param outliers : the intruders from the other side that we need to include in our final chain
	@param same_side : the points from the same side(upper or lower) that we need to include in our final chain
	*/

	vector<int> temp_boundary;
	vector<int> boundary;
	vector<pair<int, int>> same_side_outliers;
	vector<int> final_chain;

	int edge_num;
	int start = tangent.is_reverse() ? tangent.get_dest() : tangent.get_origin();
	int end = tangent.is_reverse() ? tangent.get_origin() : tangent.get_dest();

	bool(*valid_tangent_test)(vector<int>, int, int) = upper ? all_right : all_left;
	temp_boundary.push_back(start);
	temp_boundary.insert(temp_boundary.end(), outliers.begin(), outliers.end());
	temp_boundary.push_back(end);

	vector<int>::iterator from = temp_boundary.begin();
	vector<int>::iterator to;

	//set the boundary first, considering only the tangent & outliers from the other side (not the same_side)
	boundary.push_back(start);
	while (*from != end)
	{
		for (to = from + 1; to != temp_boundary.end(); to++)
		{
			if (valid_tangent_test(temp_boundary, *from, *to))
			{
				boundary.push_back(*to);
				from = to;
				break;
			}
		}
	}

	int start_idx = -1, end_idx = -1;
	//consider the outliers from the same side that intrude the boundary area
	for (int i = 0; i < same_side.size(); i++)
	{
		if (same_side[i] == start)
			start_idx = i;
		else if (same_side[i] == end)
			end_idx = i;
	}

	//we only need to consider the subsequence in same_side bounded by the starting/ending points in the tangent
	for (int idx = start_idx + 1; idx < end_idx; idx++)
	{
		edge_num = check_inclusive(boundary, same_side[idx], same_side[idx - 1]);

		if (edge_num != -1)
			same_side_outliers.push_back(make_pair(edge_num, same_side[idx]));
	}

	int boundary_index = -1;
	for (int i = 0; i < same_side_outliers.size(); i++)
	{
		edge_num = same_side_outliers[i].first;
		for (int j = boundary_index + 1; j <= edge_num; j++)
			final_chain.push_back(boundary[j]);

		final_chain.push_back(same_side_outliers[i].second);
		boundary_index = edge_num;
	}

	for (int i = boundary_index + 1; i < boundary.size(); i++)
		final_chain.push_back(boundary[i]);

	return final_chain;
}
Chain* invalid_outer_chains(Edge tangent, vector<int> concatenated_chain, vector<int> outliers, bool upper)
{
	int left_tangent_point = tangent.is_reverse() ? tangent.get_dest() : tangent.get_origin();
	int right_tangent_point = tangent.is_reverse() ? tangent.get_origin() : tangent.get_dest();

	vector<int> piAB;
	vector<int> tanBC;
	vector<int> piCD;

	Chain* result = new Chain();

	//set path from chain start to tangent start
	vector<int>::iterator it = find(concatenated_chain.begin(), concatenated_chain.end(), left_tangent_point);
	if (it == concatenated_chain.end())
		exit(1);
	piAB.insert(piAB.begin(), concatenated_chain.begin(), it);

	//set path from tangent end to chain end
	it = find(it, concatenated_chain.end(), right_tangent_point);
	if (it == concatenated_chain.end())
		exit(2);
	piCD.insert(piCD.begin(), it + 1, concatenated_chain.end());

	//set the tangent
	tanBC = final_tangent_chain(tangent, outliers, concatenated_chain, upper);

	result->append_points(piAB);
	result->append_points(tanBC);
	result->append_points(piCD);

	return result;
}
Chain* valid_outer_chains(Edge tangent, vector<int> concatenated_chain)
{
	int left_tangent_point = tangent.is_reverse() ? tangent.get_dest() : tangent.get_origin();
	int right_tangent_point = tangent.is_reverse() ? tangent.get_origin() : tangent.get_dest();

	vector<int> piAB;
	vector<int> tanBC;
	vector<int> piCD;

	Chain* result = new Chain();

	vector<int>::iterator it = find(concatenated_chain.begin(), concatenated_chain.end(), left_tangent_point);
	if (it == concatenated_chain.end())
		exit(1);
	piAB.insert(piAB.begin(), concatenated_chain.begin(), it);

	it = find(it, concatenated_chain.end(), right_tangent_point);
	if (it == concatenated_chain.end())
		exit(2);
	piCD.insert(piCD.begin(), it + 1, concatenated_chain.end());
	
	tanBC.push_back(left_tangent_point);
	if (!tangent.is_point())
		tanBC.push_back(right_tangent_point);

	result->append_points(piAB);
	result->append_points(tanBC);
	result->append_points(piCD);
	return result;
}

int nearest_point_to_common(Chain* chain, Edge common)
{
	vector<int> points = chain->get_point_list();

	//check origin
	if (points.front() == common.get_origin() || points.front() == common.get_dest())
		return points[1];//second to first
	else
		return points[points.size() - 2];//second to last
}

bool check_sharks_fin_case(Edge common, Edge other)
{
	if (is_right(other.get_origin(), common.get_origin(), common.get_dest()) && is_right(other.get_dest(), common.get_origin(), common.get_dest()))
		return false;
	else if (is_left(other.get_origin(), common.get_origin(), common.get_dest()) && is_left(other.get_dest(), common.get_origin(), common.get_dest()))
		return false;
	else
		return true;
}
Hourglass concatenateOpenOpen(Hourglass& _left, Hourglass& _right)
{
	Hourglass _new;

	Edge* leftEdgeList = _left.get_edge_list();
	Edge* rightEdgeList = _right.get_edge_list();
	Chain** leftChainList = _left.get_first_chain();
	Chain** rightChainList = _right.get_first_chain();

	Edge commonEdge;
	int leftCommonEdgeIndex = 0, rightCommonEdgeIndex = 0;

	for (int i = 0; i < 2; i++) //두 edge 모두 같은 경우는 없다고 가정함니다.
	{
		for (int j = 0; j < 2; j++)
		{
			if (leftEdgeList[i] == rightEdgeList[j]) {
				leftCommonEdgeIndex = i;
				rightCommonEdgeIndex = j;
				commonEdge = Edge(leftEdgeList[i].get_origin(), leftEdgeList[i].get_dest());
				break;
			}
		}
	}

	Edge leftEdge = leftEdgeList[!leftCommonEdgeIndex];
	Edge rightEdge = rightEdgeList[!rightCommonEdgeIndex];

	if (leftEdge == rightEdge)
		return _left;

	//left hourglass의 common하지 않은 edge를 first edge로 set함!! -> 이 순서가 중요한가?

	_new.set_first_edge(leftEdge);
	_new.set_second_edge(rightEdge);

	bool left_sharks_fin = check_sharks_fin_case(commonEdge, leftEdge);
	bool right_sharks_fin = check_sharks_fin_case(commonEdge, rightEdge);

	int common_upper_point = commonEdge.get_origin();
	int common_lower_point = commonEdge.get_dest(); //if this doesn't work, we'll swap the two

	int which_chain_to_test = 0;
	bool found = false;
	bool left = true;
	Chain* chain_to_check;

	Chain* left_upper_chain;
	Chain* left_lower_chain;
	Chain* right_upper_chain;
	Chain* right_lower_chain;

	if (!left_sharks_fin && !right_sharks_fin)
	{
		if (leftEdge.is_point() && commonEdge.check_same_point(leftEdge.get_dest()) != -1)//the special case 이게 true 이면 right은 그럴 수 없음!
		{
			which_chain_to_test = 2;
		}
		while (!found)
		{
			switch (which_chain_to_test) {
			case 0:
			case 1:
				chain_to_check = leftChainList[which_chain_to_test];
				break;
			case 2:
			case 3:
				left = false;
				chain_to_check = rightChainList[which_chain_to_test - 2];
				break;
			default:
				printf("this shouldn't be happening\n");
				exit(10);
			}

			if (chain_to_check->get_point_list_size() != 1)
				found = true;
			else
				which_chain_to_test++;
		}

		int nearest = nearest_point_to_common(chain_to_check, commonEdge);
		bool(*side)(int, int, int) = left ? is_left : is_right;

		if (!side(nearest, common_lower_point, common_upper_point))
			swap(common_upper_point, common_lower_point);

		//the common Edge's upper and lower point should be correctly set.....
	}
	else//둘 중 하나는 샥스핀 일때
	{
		if (left_sharks_fin)
		{
			Chain* chain_with_origin = leftChainList[0]->check_inclusive(leftEdge.get_origin()) ? leftChainList[0] : leftChainList[1];
			if (is_right(leftEdge.get_dest(), leftEdge.get_origin(), commonEdge.get_origin()) && is_right(leftEdge.get_dest(), leftEdge.get_origin(), commonEdge.get_dest()))
			{
				if (chain_with_origin->check_inclusive(commonEdge.get_origin()))
				{
					common_upper_point = commonEdge.get_origin();
					common_lower_point = commonEdge.get_dest();
				}
				else
				{
					common_upper_point = commonEdge.get_dest();
					common_lower_point = commonEdge.get_origin();
				}
			}
			else
			{
				if (chain_with_origin->check_inclusive(commonEdge.get_origin()))
				{
					common_upper_point = commonEdge.get_dest();
					common_lower_point = commonEdge.get_origin();
				}
				else
				{
					common_upper_point = commonEdge.get_origin();
					common_lower_point = commonEdge.get_dest();
				}
			}
		}
		else
		{
			Chain* chain_with_origin = rightChainList[0]->check_inclusive(rightEdge.get_origin()) ? rightChainList[0] : rightChainList[1];
			if (is_left(rightEdge.get_dest(), rightEdge.get_origin(), commonEdge.get_origin()) && is_left(rightEdge.get_dest(), rightEdge.get_origin(), commonEdge.get_dest()))
			{
				if (chain_with_origin->check_inclusive(commonEdge.get_origin()))
				{
					common_upper_point = commonEdge.get_origin();
					common_lower_point = commonEdge.get_dest();
				}
				else
				{
					common_upper_point = commonEdge.get_dest();
					common_lower_point = commonEdge.get_origin();
				}
			}
			else
			{
				if (chain_with_origin->check_inclusive(commonEdge.get_origin()))
				{
					common_upper_point = commonEdge.get_dest();
					common_lower_point = commonEdge.get_origin();
				}
				else
				{
					common_upper_point = commonEdge.get_origin();
					common_lower_point = commonEdge.get_dest();
				}
			}
		}
	}

	if (leftChainList[0]->check_inclusive(common_upper_point) && leftChainList[1]->check_inclusive(common_lower_point))
	{
		left_upper_chain = leftChainList[0];
		left_lower_chain = leftChainList[1];
	}
	else
	{
		left_upper_chain = leftChainList[1];
		left_lower_chain = leftChainList[0];
	}
	if (rightChainList[0]->check_inclusive(common_upper_point) && rightChainList[1]->check_inclusive(common_lower_point))
	{
		right_upper_chain = rightChainList[0];
		right_lower_chain = rightChainList[1];
	}
	else
	{
		right_upper_chain = rightChainList[1];
		right_lower_chain = rightChainList[0];
	}








	vector<int> left_upper_points = left_upper_chain->get_point_list();
	vector<int> left_lower_points = left_lower_chain->get_point_list();
	vector<int> right_lower_points = right_lower_chain->get_point_list();
	vector<int> right_upper_points = right_upper_chain->get_point_list();
	//assign left&right / upper&lower chains ^^^^^^^^^^

	//connect them!
	vector<int> upper_points;
	vector<int> lower_points;

	//concatnate the left and right, and sort them independently
	int common_upper_index = connect_vectors(left_upper_points, right_upper_points, upper_points);
	int common_lower_index = connect_vectors(left_lower_points, right_lower_points, lower_points);

	if (common_upper_index == -1)
	{
		printf("failed to connect the two vectors");
		exit(10);
	}
	if (common_lower_index == -1)
	{
		printf("failed to connect the two vectors");
		exit(10);
	}

	Edge upperT = compute_tangent(upper_points, common_upper_index, true);
	Edge lowerT = compute_tangent(lower_points, common_lower_index, false);

	vector<int> UpperOutliers = inspect_intruders(upperT, lower_points, upper_points,  true);
	vector<int> LowerOutliers = inspect_intruders(lowerT, upper_points, lower_points, false);

	Chain* upperChain = new Chain();
	Chain* lowerChain = new Chain();

	Chain* first[2];
	Chain* second[2];

	upperChain = (UpperOutliers.empty()) ? valid_outer_chains(upperT, upper_points) : invalid_outer_chains(upperT, upper_points, UpperOutliers, true);
	lowerChain = (LowerOutliers.empty()) ? valid_outer_chains(lowerT, lower_points) : invalid_outer_chains(lowerT, lower_points, LowerOutliers, false);

	bool open = (!UpperOutliers.empty() || !LowerOutliers.empty()) ? false : true;

	if (upperChain->get_point_list() == lowerChain->get_point_list()) //for the point-point edge case -> should be closed
		open = false;

	vector<int> string;
	std::set<int> upperS;
	std::set<int> lowerS;
	vector<int> final_upper_list = upperChain->get_point_list();
	vector<int> final_lower_list = lowerChain->get_point_list();
	vector<int>::iterator up;
	vector<int>::iterator low;
	vector<int> upper;
	vector<int> lower;
	int start, end;

	if (open)//open case~
	{
		first[0] = upperChain;
		first[1] = lowerChain;
		_new.set_first_chain(first);
	}
	else {
		up = final_upper_list.begin();
		low = final_lower_list.begin();
		lowerS.insert(*low);

		while (up != final_upper_list.end() || low != final_lower_list.end())
		{
			if (up != final_upper_list.end()) {
				if (lowerS.find(*up) != lowerS.end())
				{
					start = *up;
					break;
				}
				else
					upperS.insert(*up);
			}
			if (low != final_lower_list.end()) {
				if (upperS.find(*low) != upperS.end())
				{
					start = *low;
					break;
				}
				else
					lowerS.insert(*low);
			}

			if (up != final_upper_list.end())
				up++;
			if (low != final_lower_list.end())
				low++;
		}//setting the start of the string

		up = find(final_upper_list.begin(), final_upper_list.end(), start);
		low = find(final_lower_list.begin(), final_lower_list.end(), start);

		upper.insert(upper.begin(), final_upper_list.begin(), up);
		lower.insert(lower.begin(), final_lower_list.begin(), low);
		upper.push_back(start);
		lower.push_back(start);
		first[0] = new Chain(upper);
		first[1] = new Chain(lower);

		while (up != final_upper_list.end() && low != final_lower_list.end() && *up == *low)
		{
			end = *up;
			string.push_back(end);
			up++;
			low++;
		}

		upper.clear();
		lower.clear();
		upper.push_back(end);
		lower.push_back(end);
		upper.insert(upper.end(), up, final_upper_list.end());
		lower.insert(lower.end(), low, final_lower_list.end());
		second[0] = new Chain(upper);
		second[1] = new Chain(lower);

		_new.set_string(new String(string));
		_new.set_first_chain(first);
		_new.set_second_chain(second);
		_new.set_apex(start, 0);
		_new.set_apex(end, 1);
	}

	//printSummary(_left, _right, _new, open);

	return _new;
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
			new_hourglass.set_apex(_left.get_apaxes()[1], 0);
			lc = _left.get_first_chain();
			la = _left.get_apaxes()[0];
		}
		else {
			new_hourglass.set_first_edge(left_edge_list[0]);
			new_hourglass.set_first_chain(_left.get_first_chain());
			new_hourglass.set_apex(_left.get_apaxes()[0], 0);
			lc = _left.get_second_chain();
			la = _left.get_apaxes()[1];
		}
		//set right
		if (common_edge_check[1] == 0) {
			new_hourglass.set_second_edge(right_edge_list[1]);
			new_hourglass.set_second_chain(_right.get_second_chain());
			new_hourglass.set_apex(_right.get_apaxes()[1], 1);
			rc = _right.get_first_chain();
			ra = _right.get_apaxes()[0];
		}
		else {
			new_hourglass.set_second_edge(right_edge_list[0]);
			new_hourglass.set_second_chain(_right.get_first_chain());
			new_hourglass.set_apex(_right.get_apaxes()[0], 1);
			rc = _right.get_second_chain();
			ra = _right.get_apaxes()[1];
		}

		String* middle_string = concatenate_two_funnels_cc(lc, la, rc, ra, common_edge);
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
			new_hourglass.set_apex(_left.get_apaxes()[1], 0);
			r_val = concatenate_two_funnels_oc(_left.get_first_chain(), _right.get_first_chain(), common_edge, _left.get_apaxes()[0], new_hourglass.get_second_edge());
			new_hourglass.set_apex(_left.get_apaxes()[0], 1);
		}
		else {
			new_hourglass.set_first_edge(left_edge_list[0]);
			new_hourglass.set_first_chain(_left.get_first_chain());
			new_hourglass.set_apex(_left.get_apaxes()[0], 0);
			r_val = concatenate_two_funnels_oc(_left.get_second_chain(), _right.get_first_chain(), common_edge, _left.get_apaxes()[1], new_hourglass.get_second_edge());
			new_hourglass.set_apex(_left.get_apaxes()[1], 1);
		}
		if (r_val.new_string != NULL) {
			new_hourglass.set_string(new String(_left.get_string(), r_val.new_string));
			new_hourglass.set_apex(r_val.apax, 1);
		}
		new_hourglass.set_second_chain(r_val.chain);

	}
	else if (left_openess && !right_openess) {
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
			new_hourglass.set_apex(_right.get_apaxes()[1], 1);
			r_val = concatenate_two_funnels_oc(_right.get_first_chain(), _left.get_first_chain(), common_edge, _right.get_apaxes()[0], new_hourglass.get_first_edge());
			new_hourglass.set_apex(_right.get_apaxes()[0], 0);
		}
		else {
			new_hourglass.set_second_edge(right_edge_list[0]);
			new_hourglass.set_second_chain(_right.get_first_chain());
			new_hourglass.set_apex(_right.get_apaxes()[0], 1);
			r_val = concatenate_two_funnels_oc(_right.get_second_chain(), _left.get_first_chain(), common_edge, _right.get_apaxes()[1], new_hourglass.get_first_edge());
			new_hourglass.set_apex(_right.get_apaxes()[1], 0);
		}
		if (r_val.new_string != NULL) {
			new_hourglass.set_string(new String(r_val.new_string, _right.get_string()));
			new_hourglass.set_apex(r_val.apax, 0);
		}
		new_hourglass.set_first_chain(r_val.chain);
	}
	else {//open open case
		new_hourglass = concatenateOpenOpen(_left, _right);
	}
	return new_hourglass;

}

#pragma once
#include<iostream>
#include"Point.h"
#include<vector>
#include "Chain.h"
using namespace std;




// tangent_PointPoly(): find any polygon's exterior tangents
//    Input:  P = a 2D point (exterior to the polygon)
//            n = number of polygon vertices
//            V = array of vertices for any 2D polygon with V[n]=V[0]
//    Output: *rtan = index of rightmost tangent point V[*rtan]
//            *ltan = index of leftmost tangent point V[*ltan]
/*void
tangent_PointPoly(Point P, int n, vector<Point> V, int* rtan, int* ltan)
{
	float  eprev, enext;        // V[i] previous and next edge turn direction

	*rtan = *ltan = 0;          // initially assume V[0] = both tangents
	eprev = isLeft(V[0], V[1], P);
	for (int i = 1; i<n; i++) {
		enext = isLeft(V[i], V[i + 1], P);
		if ((eprev <= 0) && (enext > 0)) {
			if (!below(P, V[i], V[*rtan]))
				*rtan = i;
		}
		else if ((eprev > 0) && (enext <= 0)) {
			if (!above(P, V[i], V[*ltan]))
				*ltan = i;
		}
		eprev = enext;
	}
	return;
}*/
//===================================================================



// tangent_PointPolyC(): fast binary search for tangents to a convex polygon
//    Input:  P = a 2D point (exterior to the polygon)
//            n = number of polygon vertices
//            V = array of vertices for a 2D convex polygon with V[n] = V[0]
//    Output: *rtan = index of rightmost tangent point V[*rtan]
//            *ltan = index of leftmost tangent point V[*ltan]

/*void
tangent_PointPolyC(Point P, int n, vector<Point> V, int* rtan, int* ltan)
{
	*rtan = Rtangent_PointPolyC(P, n, V);
	*ltan = Ltangent_PointPolyC(P, n, V);
}*/


// Rtangent_PointPolyC(): binary search for convex polygon right tangent
//    Input:  P = a 2D point (exterior to the polygon)
//            n = number of polygon vertices
//            V = array of vertices for a 2D convex polygon with V[n] = V[0]
//    Return: index "i" of rightmost tangent point V[i]
int
Rtangent_PointPolyC(int p, int n, vector<int> V)
{
	Point P = point_list[p];
	// use binary search for large convex polygons
	int     a, b, c;            // indices for edge chain endpoints
	int     upA, dnC;           // test for up direction of edges a and c

								// rightmost tangent = maximum for the isLeft() ordering
								// test if V[0] is a local maximum
	if (below(p, V[1], V[0]) && !above(p, V[n - 1], V[0]))
		return 0;               // V[0] is the maximum tangent point

	for (a = 0, b = n;;) {          // start chain = [0,n] with V[n]=V[0]
		c = (a + b) / 2;        // midpoint of [a,b], and 0<c<n
		dnC = below(p, V[c + 1], V[c]);
		if (c==0 || c== n || a==b||(dnC && !above(p, V[c - 1], V[c])))
			return c;          // V[c] is the maximum tangent point

							   // no max yet, so continue with the binary search
							   // pick one of the two subchains [a,c] or [c,b]
		upA = above(p, V[a + 1], V[a]);
		if (upA) {                       // edge a points up
			if (dnC)                         // edge c points down
				b = c;                           // select [a,c]
			else {                           // edge c points up
				if (above(p, V[a], V[c]))     // V[a] above V[c]
					b = c;                       // select [a,c]
				else                          // V[a] below V[c]
					a = c;                       // select [c,b]
			}
		}
		else {                           // edge a points down
			if (!dnC)                        // edge c points up
				a = c;                           // select [c,b]
			else {                           // edge c points down
				if (below(p, V[a], V[c]))     // V[a] below V[c]
					b = c;                       // select [a,c]
				else                          // V[a] above V[c]
					a = c;                       // select [c,b]
			}
		}
	}
}



// Ltangent_PointPolyC(): binary search for convex polygon left tangent
//    Input:  P = a 2D point (exterior to the polygon)
//            n = number of polygon vertices
//            V = array of vertices for a 2D convex polygon with V[n]=V[0]
//    Return: index "i" of leftmost tangent point V[i]
int
Ltangent_PointPolyC(int p, int n, vector<int> V)
{
	Point P = point_list[p];
	// use binary search for large convex polygons
	int     a, b, c;            // indices for edge chain endpoints
	int     dnA, dnC;           // test for down direction of edges a and c

								// leftmost tangent = minimum for the isLeft() ordering
								// test if V[0] is a local minimum
	if (above(p, V[n - 1], V[0]) && !below(p, V[1], V[0]))
		return 0;               // V[0] is the minimum tangent point

	for (a = 0, b = n;;) {          // start chain = [0,n] with V[n] = V[0]
		c = (a + b) / 2;        // midpoint of [a,b], and 0<c<n
		dnC = below(p, V[c + 1], V[c]);
		if (c == 0 || c == n || a==b||(above(p, V[c - 1], V[c]) && !dnC))
			return c;          // V[c] is the minimum tangent point

							   // no min yet, so continue with the binary search
							   // pick one of the two subchains [a,c] or [c,b]
		dnA = below(p, V[a + 1], V[a]);
		if (dnA) {                       // edge a points down
			if (!dnC)                        // edge c points up
				b = c;                           // select [a,c]
			else {                           // edge c points down
				if (below(p, V[a], V[c]))     // V[a] below V[c]
					b = c;                       // select [a,c]
				else                          // V[a] above V[c]
					a = c;                       // select [c,b]
			}
		}
		else {                           // edge a points up
			if (dnC)                         // edge c points down
				a = c;                           // select [c,b]
			else {                           // edge c points up
				if (above(p, V[a], V[c]))     // V[a] above V[c]
					b = c;                       // select [a,c]
				else                          // V[a] below V[c]
					a = c;                       // select [c,b]
			}
		}
	}
}
//===================================================================



// RLtangent_PolyPolyC(): get the RL tangent between two convex polygons
//    Input:  m = number of vertices in polygon 1
//            V = array of vertices for convex polygon 1 with V[m]=V[0]
//            n = number of vertices in polygon 2
//            W = array of vertices for convex polygon 2 with W[n]=W[0]
//    Output: *t1 = index of tangent point V[t1] for polygon 1
//            *t2 = index of tangent point W[t2] for polygon 2
void
RLtangent_PolyPolyC(vector<int> V, vector<int> W, int* t1, int* t2)
{
	int m = V.size() - 1;
	int n = W.size() - 1;

	int ix1, ix2;      // search indices for polygons 1 and 2

	// first get the initial vertex on each polygon
	ix1 = Rtangent_PointPolyC(W[0], m, V);   // right tangent from W[0] to V
	ix2 = Ltangent_PointPolyC(V[ix1], n, W); // left tangent from V[ix1] to W

	// ping-pong linear search until it stabilizes
	int done = false;                    // flag when done
	while ((ix1!=m)&&(ix2!=0)&&done == false) {
		done = true;                     // assume done until...
		while ((ix1 != m) && (ix2 != 0) &&isLeft(W[ix2], V[ix1], V[ix1 + 1]) <= 0) {
			++ix1;                       // get Rtangent from W[ix2] to V
		}
		while ((ix1 != m) && (ix2 != 0) &&isLeft(V[ix1], W[ix2], W[ix2 - 1]) >= 0) {
			--ix2;                       // get Ltangent from V[ix1] to W
			done = false;                // not done if had to adjust this
		}
	}
	*t1 = ix1;
	*t2 = ix2;
	return;
}
bool side_check(Chain * left_upper_chain, Chain * right_upper_chain, Chain * left_down_chain, Chain * right_down_chain, int apax1, int apax2, Edge * common_edge) {

	//int l1 = left_down_chain->check_one_side(apax1, apax2, common_edge);
	//int r1 = right_down_chain->check_one_side(apax1, apax2, common_edge);
	int l2 = left_upper_chain->check_one_side(apax1, apax2, common_edge);
	int r2 = right_upper_chain->check_one_side(apax1, apax2, common_edge);
	
	if (l2 == 0 || r2 == 0)// || (l1 == l2 && l1 != 3) || (r1 == r2 && r2 != 3))
		return false;
	bool check = check_line_intersection(apax1, apax2, common_edge->get_origin(), common_edge->get_dest());
	
	if (check)
		return true;
	int check1 = common_edge->check_same_point(apax1);
	int check2 = common_edge->check_same_point(apax2);
	bool seq1 = left_upper_chain->check_sequence(apax1, apax2);
	bool seq2 = right_upper_chain->check_sequence(apax1, apax2);
	if ((check1 != -1 || check2 != -1) && (seq1 || seq2))
		return true;
	return false;
	/*int check_side[4] = { -1, -1, -1, -1 };
	check_side[0] = left_down_chain->check_one_side(apax1, apax2,common_edge);
	check_side[1] = right_down_chain->check_one_side(apax1, apax2, common_edge);
	check_side[2] = left_upper_chain->check_one_side(apax1, apax2,common_edge);
	check_side[3] = right_upper_chain->check_one_side(apax1, apax2, common_edge);
*/
	/*int num = 0;
	bool check_3 = false;
	int prev = -1;
	for (int i = 0; i < 4; i++) {
		if (check_side[i] == 3) {
			check_3 = true;
			continue;
		}
		if (prev == -1 || prev != check_side[i]) {
			prev = check_side[i];
			num++;
		}
	}
	if (num == 1 && !check_3)return false;
	else return true;
	*/
	//Àû±â
	
	
}

bool valid_check(Chain * left_upper_chain, Chain * right_upper_chain, Chain * left_down_chain, Chain * right_down_chain, int apax1, int apax2, Edge * common_edge) {
	
/*	if (!side_check(left_upper_chain, right_upper_chain, left_down_chain, right_down_chain, apax1, apax2, common_edge))
		return false;
		*/
	bool bl1 = left_down_chain->check_intersection(apax1, apax2);
	bool br1 = right_down_chain->check_intersection(apax1, apax2);
	bool bl2 = left_upper_chain->check_intersection(apax1, apax2);
	bool br2 = right_upper_chain->check_intersection(apax1, apax2);

	if (bl1 || br1 || bl2 || br2) {
		return false;
	}

	return true;
}
bool compute_outer_tangent(Chain* lu, Chain *ru, int * t1, int *t2, Edge * common_edge, Chain * ld = NULL, Chain * rd = NULL) {
	vector<int> p1_list = lu->get_point_list();
	vector<int> p2_list = ru->get_point_list();
	point_type min = -1;
	if (p1_list.size() == 1 && p2_list.size() == 1) {
		*t1 = 0;
		*t2 = 0;
	}else if (p1_list.size() == 1) {
		int idx = -1, idx_diff = -1;
		if (common_edge->check_same_point(p2_list.front()) == -1) {
			idx = 0;
			idx_diff = 1;
		}
		else {
			idx = p2_list.size() - 1;
			idx_diff = -1;
		}
		while (idx >= 0 && idx < (int)p2_list.size()) {
			if(side_check(lu,ru,ld,rd, p1_list[0],p2_list[idx], common_edge)){
				*t1 = 0;
				*t2 = idx;
				break;
			}
			idx += idx_diff;
		}
		/**t1 = 0;
		*t2 = (idx - idx_diff);*/
	}else if (p2_list.size() == 1) {
		int idx = -1, idx_diff = -1;
		if (common_edge->check_same_point(p1_list.front()) == -1) {
			idx = 0;
			idx_diff = 1;
		}
		else {
			idx = p1_list.size() - 1;
			idx_diff = -1;
		}
		while (idx >= 0 && idx < (int)p1_list.size()) {
			if(side_check(lu,ru,ld,rd,p1_list[idx],p2_list[0],common_edge)){
				*t1 = idx;
				*t2 = 0;
				break;
			}
			idx += idx_diff;
		}
		/**t1 = (idx - idx_diff);
		*t2 = 0;*/
	}/*else if (p1_list.size() == 2 && (int)p2_list.size() == 2) {
		int p1_com = -1, p2_com = -1;
		int p1_diff = -1, p2_diff = -1;
		if (p1_list.front() == p2_list.front() || p1_list.front() == p2_list.back()) {
			p1_com = 0;
		}
		else {
			p1_com = 1;
		}
		p1_diff = (p1_com + 1) % 2;

		if (p2_list.front() == p1_list.front() || p2_list.front() == p1_list.back()) {
			p2_com = 0;
		}
		else {
			p2_com = 1;
		}
		p2_diff = (p2_com + 1) % 2;
		
		*t1 = p1_diff;
		*t2 = p2_diff;

		if (!side_check(lu, ru, ld, rd, p1_list[*t1], p2_list[*t2],common_edge)){
			*t1 = p1_com;
			*t2 = p2_com;			
		}
	}*/else if (p1_list.size() == 2) {
		if (p1_list.front() == p2_list.front() || p1_list.front() == p2_list.back()) {
			*t1 = 1;
		}
		else if (p1_list.back() == p2_list.front() || p1_list.back() == p2_list.back()) {
			*t1 = 0;
		}
		bool break_check = false;
		for(int i=0 ;i<2;i++){
			int idx = -1, idx_diff = -1;
			if (common_edge->check_same_point(p2_list.front()) == -1) {
				idx = 0;
				idx_diff = 1;
			}
			else {
				idx = p2_list.size() - 1;
				idx_diff = -1;
			}
			while (idx >= 0 && idx < (int)p2_list.size()) {
				if (side_check(lu, ru, ld, rd, p1_list[*t1], p2_list[idx],common_edge)) {
					if (common_edge->check_same_point(p2_list[idx])!=-1) {
						*t1 = (*t1 + 1) % 2;
					}
					*t2 = idx;
					break_check = true;
					break;
				}
				*t2 = idx;
				idx += idx_diff;
			}
			if (break_check == true) {
				break;
			}
			*t1 = (*t1 + 1) % 2;
		}
	}else if(p2_list.size() == 2) {
		if (p2_list.front() == p1_list.front() || p2_list.front() == p1_list.back()) {
			*t2 = 1;
		}
		else if (p2_list.back() == p1_list.front() || p2_list.back() == p1_list.back()) {
			*t2 = 0;
		}
		bool break_check = false;
		for(int i=0 ;i<2; i++) {
			int idx = -1, idx_diff = -1;
			if (common_edge->check_same_point(p1_list.front()) == -1) {
				idx = 0;
				idx_diff = 1;
			}
			else {
				idx = p1_list.size() - 1;
				idx_diff = -1;
			}

			while (idx >= 0 && idx < (int)p1_list.size()) {
				if (side_check(lu,ru,ld,rd,p1_list[idx],p2_list[*t2],common_edge)){
					if (common_edge->check_same_point(p1_list[idx])!=-1) {
						*t2 = (*t2 + 1) % 2;
					}
					*t1 = idx;
					break_check = true;
					break;
				}
				*t1 = idx;
				idx += idx_diff;
			}
			if (break_check == true) {
				break;
			}
			*t2 = (*t2 + 1) % 2;
		}
	}
	else {
		p1_list.push_back(p1_list[0]);
		p2_list.push_back(p2_list[0]);
		RLtangent_PolyPolyC(p1_list, p2_list, t1, t2);

		if (*t1 == p1_list.size() - 1) *t1 = 0;
		if (*t2 == p2_list.size() - 1) *t2 = 0;
		if (!side_check(lu, rd, ld, ru, p1_list[*t1], p2_list[*t2], common_edge))
			return false;
	}
	return valid_check(lu, ru, ld, rd, p1_list[*t1], p2_list[*t2], common_edge);
}
bool compute_inner_tangent(Chain* lu, Chain *rd, int * t1, int *t2, Edge * common_edge, Chain * ld = NULL, Chain * ru = NULL) {
	
	vector<int> p1_list = lu->get_point_list();
	vector<int> p2_list = rd->get_point_list();
	point_type min = -1;
	if (p1_list.size() == 1 && p2_list.size() == 1) {
		*t1 = 0;
		*t2 = 0;
	}else if (p1_list.size() == 1) {
		int idx = -1, idx_diff = -1;
		if (common_edge->check_same_point(p2_list.front()) == -1) {
			idx = 0;
			idx_diff = 1;
		}
		else {
			idx = p2_list.size() - 1;
			idx_diff = -1;
		}
		while (idx >= 0 && idx < (int)p2_list.size()) {
			if (side_check(lu, rd, ld, ru, p1_list[0], p2_list[idx], common_edge)) {
				*t1 = 0;
				*t2 = idx;
				break;
			}
			*t2 = idx;
			idx += idx_diff;
		}
		*t1 = 0;
	}
	else if (p2_list.size() == 1) {
		int idx = -1, idx_diff = -1;
		if (common_edge->check_same_point(p1_list.front()) == -1) {
			idx = 0;
			idx_diff = 1;
		}
		else {
			idx = p1_list.size() - 1;
			idx_diff = -1;
		}
		while (idx >= 0 && idx < (int)p1_list.size()) {
			if (side_check(lu, rd, ld, ru, p1_list[idx], p2_list[0], common_edge)) {
				*t1 = idx;
				*t2 = 0;
				break;
			}
			*t1 = idx;
			idx += idx_diff;
		}
		*t2 = 0;
	}/*else if (p1_list.size() == 2 && (int)p2_list.size() == 2) {
		int p1_com = -1, p2_com = -1;
		int p1_diff = -1, p2_diff = -1;
		if (p1_list.front() == common_edge->get_origin() || p1_list.front() == common_edge->get_dest()) {
			p1_com = 0;
		}
		else {
			p1_com = 1;
		}
		p1_diff = (p1_com + 1) % 2;

		if (p2_list.front() == common_edge->get_origin() || p2_list.front() == common_edge->get_dest()) {
			p2_com = 0;
		}
		else {
			p2_com = 1;
		}
		p2_diff = (p2_com + 1) % 2;

		*t1 = p1_diff;
		*t2 = p2_diff;

		if (!side_check(lu, rd, ld, ru, p1_list[*t1], p2_list[*t2], common_edge)) {
			*t1 = p1_com;
			*t2 = p2_com;
		}
	}*/
	else if (p1_list.size() == 2) {
		if (p1_list.front() == common_edge->get_origin() || p1_list.front() == common_edge->get_dest()) {
			*t1 = 1;
		}
		else if (p1_list.back() == common_edge->get_origin() || p1_list.back() == common_edge->get_dest()) {
			*t1 = 0;
		}
		bool break_check = false;
		for (int i = 0; i<2; i++) {
			int idx = -1, idx_diff = -1;
			if (common_edge->check_same_point(p2_list.front()) == -1) {
				idx = 0;
				idx_diff = 1;
			}
			else {
				idx = p2_list.size() - 1;
				idx_diff = -1;
			}
			while (idx >= 0 && idx < (int)p2_list.size()) {
				if (side_check(lu, rd, ld, ru, p1_list[*t1], p2_list[idx], common_edge)) {
					if (common_edge->check_same_point(p2_list[idx])!=-1) {
						*t1 = (*t1 + 1) % 2;
					}
					*t2 = idx;
					break_check = true;
					break;
				}
				*t2 = idx;
				idx += idx_diff;
			}
			if (break_check == true) {
				break;
			}
			*t1 = (*t1 + 1) % 2;
		}
	}
	else if (p2_list.size() == 2) {
		if (p2_list.front() == common_edge->get_origin() || p2_list.front() == common_edge->get_dest()) {
			*t2 = 1;
		}
		else if (p2_list.back() == common_edge->get_origin() || p2_list.back() == common_edge->get_dest()) {
			*t2 = 0;
		}
		bool break_check = false;
		for (int i = 0; i<2; i++) {
			int idx = -1, idx_diff = -1;
			if (common_edge->check_same_point(p1_list.front()) == -1) {
				idx = 0;
				idx_diff = 1;
			}
			else {
				idx = p1_list.size() - 1;
				idx_diff = -1;
			}

			while (idx >= 0 && idx < (int)p1_list.size()) {
				if (side_check(lu, rd, ld, ru, p1_list[idx], p2_list[*t2], common_edge)) {
					if (common_edge->check_same_point(p1_list[idx])!=-1) {
						*t2 = (*t2 + 1) % 2;
					}
					*t1 = idx;
					break_check = true;
					break;
				}
				*t1 = idx;
				idx += idx_diff;
			}
			if (break_check == true) {
				break;
			}
			*t2 = (*t2 + 1) % 2;
		}
	}
	else {
		p1_list.push_back(p1_list[0]);
		p2_list.push_back(p2_list[0]);
		RLtangent_PolyPolyC(p1_list, p2_list, t1, t2);

		if (*t1 == p1_list.size() - 1) *t1 = 0;
		if (*t2 == p2_list.size() - 1) *t2 = 0;
		if (!side_check(lu, rd, ld, ru, p1_list[*t1], p2_list[*t2], common_edge))
			return false;
	}
	return valid_check(lu, rd, ld, ru, p1_list[*t1], p2_list[*t2], common_edge);
	/*vector<int> p1_list = lu->get_point_list();
	vector<int> p2_list = rd->get_point_list();
	point_type min = -1;
	if (p1_list.size() == 1) {
		int idx = -1, idx_diff = -1;
		if (common_edge->check_same_point(p2_list.front()) != -1) {
			idx = 0;
			idx_diff = 1;
		}
		else {
			idx = p2_list.size() - 1;
			idx_diff = -1;
		}
		while (idx >= 0 && idx < (int)p2_list.size()) {
			if (valid_check(lu, ru, ld, rd, p1_list[0], p2_list[idx], common_edge)) {
				*t1 = 0;
				*t2 = idx;
				return true;
			}
			idx += idx_diff;
		}
		return false;
	}
	if (p2_list.size() == 1) {
		int idx = -1, idx_diff = -1;
		if (common_edge->check_same_point(p1_list.front()) != -1) {
			idx = 0;
			idx_diff = 1;
		}
		else {
			idx = p1_list.size() - 1;
			idx_diff = -1;
		}
		while (idx >= 0 && idx < (int)p1_list.size()) {
			if (valid_check(lu, ru, ld, rd, p1_list[idx], p2_list[0], common_edge)) {
				*t1 = idx;
				*t2 = 0;
				return true;
			}
			idx += idx_diff;
		}
		return false;
	}
	if (p1_list.size() == 2 && (int)p2_list.size() == 2) {
		if (common_edge->check_same_point(p1_list[0]) != -1)
			*t1 = 0;
		else *t1 = 1;

		if (common_edge->check_same_point(p2_list[0]) != -1)
			*t2 = 0;
		else *t2 = 1;
		return true;
	}


	
	if (p1_list.size() == 2) {
		if (p1_list.front() == common_edge->get_origin() || p1_list.front() == common_edge->get_dest()) {
			*t1 = 0;// 0;
		}
		else if (p1_list.back() == common_edge->get_origin() || p1_list.back() == common_edge->get_dest()) {
			*t1 = 1;// 1;
		}
		while (1) {
			int idx = -1, idx_diff = -1;
			if (common_edge->check_same_point(p2_list.front()) == -1) {
				idx = 0;
				idx_diff = 1;
			}
			else {
				idx = p2_list.size() - 1;
				idx_diff = -1;
			}
			while (idx >= 0 && idx < (int)p2_list.size()) {
				if (valid_check(lu, ru, ld, rd, p1_list[*t1], p2_list[idx], common_edge)) {
					*t2 = idx;
					return true;
				}
				idx += idx_diff;
			}
			*t1 = (*t1 + 1) % 2;
		}
		return true;
	}
	if (p2_list.size() == 2) {
		if (p2_list.front() == common_edge->get_origin() || p2_list.front() == common_edge->get_dest()) {
			*t2 = 0;// 0;
		}
		else if (p2_list.back() == common_edge->get_origin() || p2_list.back() == common_edge->get_dest()) {
			*t2 = 1;// 1;
		}
		while (1) {
			int idx = -1, idx_diff = -1;
			if (common_edge->check_same_point(p2_list.front()) == -1) {
				idx = 0;
				idx_diff = 1;
			}
			else {
				idx = p1_list.size() - 1;
				idx_diff = -1;
			}
			while (idx >= 0 && idx < (int)p2_list.size()) {
				if (valid_check(lu, ru, ld, rd, p2_list[*t2], p1_list[idx], common_edge)) {
					*t1 = idx;
					return true;
				}
				idx += idx_diff;
			}
			*t2 = (*t2 + 1) % 2;
		}
		return true;
	}

	p1_list.push_back(p1_list[0]);
	p2_list.push_back(p2_list[0]);
	RLtangent_PolyPolyC(p1_list, p2_list, t1, t2);

	if (*t1 == p1_list.size() - 1) *t1 = 0;
	if (*t2 == p2_list.size() - 1) *t2 = 0;
	return valid_check(lu, ru, ld, rd, p1_list[*t1], p2_list[*t2], common_edge);
	*/

}
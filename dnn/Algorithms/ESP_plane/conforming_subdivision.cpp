#pragma warning(disable : 4996)

#include <cmath>
#include <algorithm>
#include <assert.h>
#include "conforming_subdivision.h"

// S : set of i-quads
// N : set of trees
// Q(i) : set of i-quads

// 우선은 point 좌표들이 잘 들어온다고 가정하겠음. 추후 translation&scaling 함수 만들 예정

bool operator==(i_box a, i_box b) {
	return a.i == b.i && a.k == b.k && a.l == b.l;
}

bool operator==(i_quad a, i_quad b) {
	return a.i == b.i && a.lower_left == b.lower_left;
}

conforming_subdivision::conforming_subdivision(vector<Point> _pts) {
	pts = _pts;
}

core conforming_subdivision::get_core(i_quad q) {
	return core{i_box{q.lower_left.k + 1, q.lower_left.l + 1, q.i}, i_box{q.upper_right.k - 1, q.upper_right.l - 1, q.i}, q};
}

bool conforming_subdivision::contained(i_quad q1, i_quad q2) { // check if (q1 set_union q2) can be contained in a 2 * 2 array of (i+2) boxes
	int k_min = int(floor(min(q1.lower_left.k, q2.lower_left.k) / 4)); // left bounding edge
	int l_min = int(floor(min(q1.lower_left.l, q2.lower_left.k) / 4)); // lower bounding edge
	
	if (double(k_min) + 2 >= ceil(max(q1.upper_right.k, q2.upper_right.k) / 4) && 
	double(l_min) + 2 >= ceil(max(q1.upper_right.l, q2.upper_right.l) / 4)) {return true;}
	else {return false;}
}

i_quad conforming_subdivision::bigger_quad(i_quad q1, i_quad q2) {
	assert(contained(q1, q2)); // assumption : contained(q1, q2) == true 
	
	int k_min = int(floor(min(q1.lower_left.k, q2.lower_left.k) / 4));
	int l_min = int(floor(min(q1.lower_left.l, q2.lower_left.k) / 4));

	int i = q1.i; // level
	return i_quad{i+2, i_box{k_min-1, l_min-1}, i_box{k_min+2, l_min+2}};
}

i_quad conforming_subdivision::bigger_quad(i_quad q) {
	int k_min = int(floor(min(q.lower_left.k, q.lower_left.k) / 4));
	int l_min = int(floor(min(q.lower_left.l, q.lower_left.k) / 4));
	
	int i = q.i;
	return i_quad{i+2, i_box{k_min-1, l_min-1}, i_box{k_min+2, l_min+2}};
}

vector<vector<int>> conforming_subdivision::maximal_matching(vector<vector<int>> adj_list) {
	vector<vector<int>> ret = {};
	vector<bool> visited = {};
	for (unsigned int ind1 = 0; ind1 < adj_list.size(); ind1++) {
		visited.push_back(false);
	}

	for (unsigned int ind1 = 0; ind1 < adj_list.size(); ind1++) {
		if (visited[ind1]) { continue; }
		for (auto nb : adj_list[ind1]) { // neighbor
			if (visited[nb]) {}
			else {
				visited[ind1] = true;
				visited[nb] = true;
				vector<int> v{int(ind1), nb};
				ret.push_back(v);
				break;
			}
		}
	}

	for (unsigned int ind1 = 0; ind1 < adj_list.size(); ind1++) {
		if (!visited[ind1]) {
			visited[ind1] = true;
			vector<int> v{int(ind1)};
			ret.push_back(v);
		}
	}

	return ret;
}

vector<i_quad> conforming_subdivision::growth(vector<i_quad> S) {
	vector<i_quad> ret = {};

	vector<vector<int>> adj_list;
	for (auto s : S) {
		adj_list.push_back({});
	}

	for (unsigned int ind1 = 0; ind1 < S.size(); ind1++) {
		for (unsigned int ind2 = ind1 + 1; ind2 < S.size(); ind2++) {
			if (contained(S[ind1], S[ind2])) {
				adj_list[ind1].push_back(ind2);
				adj_list[ind2].push_back(ind1);
			}
		}
	}

	vector<vector<int>> mm = maximal_matching(adj_list);

	for (vector<int> m : mm) {
		int sz = int(m.size());

		if (sz == 2) {
			i_quad q1 = S[m[0]], q2 = S[m[1]];
			ret.push_back(bigger_quad(q1, q2));
			// i_quad q_bar;	
			q1.G = q2.G = &ret.back(); 
		}

		else { // sz == 1
			i_quad q = S[m[0]];
			ret.push_back(bigger_quad(q));
			q.G = &ret.back();
		}
		
		 // (* add q_bar to growth(S) *)
	}

	return ret;
}
// if -1 : break; 

vector<i_quad> conforming_subdivision::ind2quads(vector<int> S, vector<i_quad> Q) {
	vector<i_quad> ret = {};
	for (auto num : S) {
		ret.push_back(Q[num]);
	}

	return ret; 
}

bool conforming_subdivision::overlap(i_quad q1, i_quad q2) {
	if ((q1.lower_left.k - q2.lower_left.k) < 4 && (q1.lower_left.l - q2.lower_left.l) < 4) {return true;}
	else {return false;}
}

vector<vector<int>> conforming_subdivision::matmul(vector<vector<int>> mat1, vector<vector<int>> mat2) {
	// int인 이유는 graph니까. // O(V^3) 짜리
	if (mat1[0].size() != mat2.size()) return {};

	vector<vector<int>> ret;


	for (unsigned int ind1 = 0; ind1 < mat1.size(); ind1++) {
		vector<int> row = {};
		for (unsigned int ind2 = ind1 + 1; ind2 < mat2[0].size(); ind2++) {
			int sum = 0;
			for (unsigned int ind3 = ind1 + 1; ind3 < mat1[0].size(); ind3++) {
				sum += mat1[ind1][ind3] * mat2[ind3][ind2];
			}
			row.push_back(sum);
		}
		ret.push_back(row);
	}

	return ret;
}

vector<vector<int>> conforming_subdivision::power(vector<vector<int>> mat, int k) {

	if (k == 1) { return mat; }

	if (k % 2 == 1) { return matmul(mat, power(mat, k / 2)); }
	else {
		vector<vector<int>> half = power(mat, k / 2);
		return matmul(half, half);
	}

	/*
	vector<vector<int>> ret = {};
	return ret;
	*/
	
	return {};
}

vector<vector<int>> conforming_subdivision::trans_closure(vector<vector<int>> adj_mat) { 
	// input과 return값의 자료형이 같지만, input은 adj_list같은 느낌이고 output은 equivalence class들을 모아놓은 것임.
	vector<vector<int>> res = power(adj_mat, int(adj_mat.size()));
	vector<vector<int>> ret = {};

	vector<bool> visited = {};
	for (unsigned int ind1 = 0; ind1 < adj_mat.size(); ind1++) {
		visited.push_back(false);
	}


	while (1) {
		vector<bool>::iterator it = find(visited.begin(), visited.end(), false);

		if (it == visited.end()) {break;}
		// 아직 방문하지 않은 node가 있다면,
		vector<int> a_class = {};
		int first_one = int(it - visited.begin());
		a_class.push_back(first_one);
		visited[first_one] = true;

		for (unsigned int ind2 = first_one; ind2 < a_class.size(); ind2++) {
			if (res[first_one][ind2] != 0) {
				a_class.push_back(ind2);
				visited[ind2] = true;
			}
		}
		ret.push_back(a_class);
	}

	return ret;
}

vector<vector<int>> conforming_subdivision::equiv_classes(vector<i_quad> S) {
	
	vector<int> elem = {};
	vector<vector<int>> adj_mat = {};

	for (unsigned int ind1 = 0; ind1 < S.size(); ind1++) {
		elem.push_back(0);
	}

	for (unsigned int ind1 = 0; ind1 < S.size(); ind1++) {
		adj_mat.push_back(elem);
	}

	for (unsigned int ind1 = 0; ind1 < S.size(); ind1++) {
		for (unsigned int ind2 = ind1 + 1; ind2 < S.size(); ind2++) {
			if (overlap(S[ind1], S[ind2])) {
				adj_mat[ind1][ind2] = 1;
				adj_mat[ind2][ind1] = 1;
			}
		}
	}

	vector<vector<int>> ret = trans_closure(adj_mat);

	return ret;
}

int conforming_subdivision::interior(i_quad q, vector<i_quad> Q_prev, vector<i_quad> Q_curr) { // 지금은 모두 돌면서 다 체크하므로 비효율적이고, 나중에 고쳐야 함.

	// vector<int> S_prime = {};
	// S_prime.push_back(*it);
	int num = 0;
	for (i_quad q_prime : Q_prev) {
		if (pow(2, q.i) * q.lower_left.k <= pow(2, q_prime.i) * q_prime.lower_left.k &&
		pow(2, q.i) * q.lower_left.l <= pow(2, q_prime.i) * q_prime.lower_left.l &&
		pow(2, q_prime.i) * q_prime.upper_right.k <= pow(2, q.i) * q.upper_right.k &&
		pow(2, q_prime.i) * q_prime.upper_right.l <= pow(2, q.i) * q.upper_right.l) { num += 1; }
		// q.i = q_prime.i + 2
	}

	return num;
}

	// bool isSimple(i_quad q, Q Q_prev, Q Q_curr) {
bool conforming_subdivision::isSimple(i_quad q, vector<i_quad> Q_prev, vector<i_quad> Q_curr) { // simple은 component에 대해서 정의되는 개념임
// build-subdivision step 4에서 체크하는 것은 충분조건. 이 함수 내에서는 시키는대로 simple의 필요조건 모두 체크 필요.
	if (q.i == -2) { return true; }
	
	if (Q_curr.size() == 1 && interior(q, Q_prev, Q_curr) == 1) {return true;}
	else {return false;}
}
// q랑 q_bar는 i-quad이므로 outer_boundary 조건은 그냥 만족됨

vector<line_segment> conforming_subdivision::draw_SPBD(i_quad q) { // draw simple boundary
	vector<line_segment> ret = {};
	double unit_length = pow(2, q.i);
	double x_min = q.lower_left.k * unit_length, y_min = q.lower_left.l * unit_length;
	
	for (int i = 0; i < 4; i++) {
		ret.push_back(line_segment{ Point{x_min + i, y_min} , Point{x_min + i + 1, y_min}}); // 아래쪽
		ret.push_back(line_segment{ Point{x_min + i, y_min + 3}, Point{x_min + i + 1, y_min + 3} }); // 위쪽
		ret.push_back(line_segment{ Point{x_min, y_min + i}, Point{x_min, y_min + i + 1} }); // 왼쪽
		ret.push_back(line_segment{ Point{x_min + 3, y_min + i}, Point{x_min + 3, y_min + i + 1}}); // 오른쪽
	}

	return ret;
}

bool conforming_subdivision::contained(i_box B, vector<core> R1) { // box size가 큰 경우와 작은 경우 모두 커버 가능.
	for (core C : R1) {
		double x_box = pow(2, B.i) * B.k, y_box = pow(2, B.i) * B.l, x_C = pow(2, C.i) * C.lower_left.k, y_C = pow(2, C.i) * C.lower_left.l;
		if (x_C <= x_box && x_box <= x_C + 2 * pow(2, C.i) && y_C <= y_box && y_box <= y_C + 2 * pow(2, C.i)) { return true; }
	}
	return false;
}

bool conforming_subdivision::contained(i_box B, vector<i_quad> vec) {
	for (i_quad Q : vec) {
		double x_box = pow(2, B.i) * B.k, y_box = pow(2, B.i) * B.l, x_Q = pow(2, Q.i) * Q.lower_left.k, y_Q = pow(2, Q.i) * Q.lower_left.l;
		if (x_Q <= x_box && x_box <= x_Q + 4 * pow(2, Q.i) && y_Q <= y_box && y_box <= y_Q + 4 * pow(2, Q.i)) { return true; }
	}
	return false;
}

vector<line_segment> conforming_subdivision::draw_CXBD(i_box B) { // draw complex boundary
	vector<line_segment> ret = {};
	double unit_length = pow(2, B.i);
	ret.push_back(line_segment{ Point{unit_length * B.k, unit_length * B.l}, Point{unit_length * (double(B.k) + 1), unit_length * B.l}}); // 아래쪽
	ret.push_back(line_segment{ Point{unit_length * B.k, unit_length * (double(B.l) + 1)}, Point{unit_length * (double(B.k) + 1), unit_length * (double(B.l) + 1)} }); // 위쪽
	ret.push_back(line_segment{ Point{unit_length * B.k, unit_length * B.l}, Point{unit_length * B.k, unit_length * (double(B.l) + 1)} }); // 왼쪽
	ret.push_back(line_segment{ Point{unit_length * (double(B.k) + 1), unit_length * B.l}, Point{unit_length * (double(B.k) + 1), unit_length * (double(B.l) + 1)} }); // 오른쪽
	
	return ret;
}

vector<line_segment> conforming_subdivision::fill_R1R2(vector<core> R1, vector<i_quad> R2) {
	vector<line_segment> ret = {};

	double unit_length = pow(2, R1[0].i - 2);
	for (core r1 : R1) {
		for (int ind1 = 0; ind1 < 8; ind1++) {
			for (int ind2 = 0; ind2 < 8; ind2++) {
				i_box B = { int(4 * pow(2, r1.i) * r1.lower_left.k + unit_length * ind1), int(4 * pow(2, r1.i) * r1.lower_left.l + unit_length * ind2), r1.i - 2 };
				if (!contained(B, R2)) {
					// draw edges. 4개의 edge를 추가함으로써 반영.
					vector<line_segment> temp = draw_CXBD(B);
					ret.insert(ret.end(), temp.begin(), temp.end());
				}
			}
		}
	}

	for (i_quad q : R2) {
		for (int ind1 = 0; ind1 < 4; ind1++) {
			for (int ind2 = 0; ind2 < 4; ind2++) {
				i_box B = { int(pow(2, q.i) * q.lower_left.k + unit_length * ind1), int(pow(2, q.i) * q.lower_left.l + unit_length * ind1), q.i - 2 };
				if (!contained(B, R1)) {
					vector<line_segment> temp = draw_CXBD(B);
					ret.insert(ret.end(), temp.begin(), temp.end());
				}
			}
		}
	}

	return ret;
}

vector<line_segment> conforming_subdivision::fill_R1S(vector<core> R1, vector<i_quad> S_region) {
	vector<line_segment> ret = {};

	double unit_length = pow(2, R1[0].lower_left.i);
	for (core r1 : R1) {
		for (int ind1 = 0; ind1 < 2; ind1++) {
			for (int ind2 = 0; ind2 < 2; ind2++) {
				i_box B = { int(pow(2, r1.i) * r1.lower_left.k + unit_length * ind1), int(pow(2, r1.i) * r1.lower_left.l + unit_length * ind2), r1.i };
				if (!contained(B, S_region)) {
					vector<line_segment> temp = draw_CXBD(B);
					ret.insert(ret.end(), temp.begin(), temp.end());
				}
			}
		}
	}

	for (i_quad q : S_region) {
		for (int ind1 = 0; ind1 < 4; ind1++) {
			for (int ind2 = 0; ind2 < 4; ind2++) {
				i_box B = { int(pow(2, q.i) * q.lower_left.k + unit_length * ind1), int(pow(2, q.i) * q.lower_left.l + unit_length * ind1), q.i };
				if (!contained(B, R1)) {
					vector<line_segment> temp = draw_CXBD(B);
					ret.insert(ret.end(), temp.begin(), temp.end());
				}
			}
		}
	}
	
	/* break each cell boundary with an endpoint incident to R1 into four edges of length 2^(i-2), to satisfy Invariant 1. */
	// core가 겹칠 수도 있지만 우선은 다 쪼개는 것으로 생각.
	for (core r1 : R1) {
		double unit_length = pow(2, r1.i - 2);
		double x_min = r1.lower_left.k * pow(2, r1.i), y_min = r1.lower_left.l * pow(2, r1.i);
		for (int ind1 = 0; ind1 < 4; ind1++) {
			ret.push_back(line_segment{ Point{x_min, y_min}, Point{x_min, y_min + unit_length * ind1}});
			ret.push_back(line_segment{ Point{x_min + pow(2, r1.i), y_min}, Point{x_min + pow(2, r1.i), y_min + unit_length * ind1} });
			ret.push_back(line_segment{ Point{x_min, y_min}, Point{x_min + unit_length * ind1, y_min} });
			ret.push_back(line_segment{ Point{x_min, y_min + pow(2, r1.i)}, Point{x_min + unit_length * ind1, y_min + pow(2, r1.i)} });
		}
	}

	return ret;
}

vector<line_segment> conforming_subdivision::build_subdivision() {	// inefficient
	vector<i_quad> Q_init; // initialization step to construct Q_init := Q(-2)
	int i = -2; // level
	for (auto pt : pts) {
		Q_init.push_back(i_quad{-2, i_box{int(floor(pt.x / 0.25) - 1), int(floor(pt.y / 0.25) - 2)}, i_box{int(floor(pt.x / 0.25) + 2), int(floor(pt.y / 0.25) + 1)}});
	}
	
	vector<line_segment> ret = {};

	vector<i_quad> Q_oldest = {};
	vector<i_quad> Q_prev = Q_init; // Q : family of quads
	vector<i_quad> Q_curr = {}; // Q_prev <=> Q(i-2), Q_curr <=> Q(i)

	while (Q_prev.size() > 1) {
		// (* 1. Increment i: i = i + 2)
		i += 2;

		// (* 2. Compute Q(i) from Q(i-2) *)

		// (a) Initialize Q(i) = empty_set;
		
		Q_curr.clear(); // vector<i_quad>().swap(Q_curr);

		// (b) for each equivalence class S of Q(i-2) do ...

		// vector<vector<int>> EC = equiv_classes(Q_prev);
		// for (unsigned int ind1 = 0; ind1 < EC.size(); ind1++) {
		// 	vector<int> S = EC[ind1];
		
		for (vector<int> S : equiv_classes(Q_prev)) {
			vector<i_quad> G_curr = growth(ind2quads(S, Q_prev));
			// 우선은 union 대신 concatenate 하는 걸로.
			Q_curr.insert(Q_curr.end(), G_curr.begin(), G_curr.end());
		}

		// (c) for every pair of i_quads q, q' in Q(i) do ...
		// (d) Extend =_i to an equivalence relation to... and compute the equivalence classes.
		
		// (* 3. Process simple components of ≡_i-2 that are about to merge with some other components. *)
		for (i_quad q : Q_prev) {
			i_quad q_bar = *q.G;
			if (isSimple(q,	Q_oldest, Q_prev) && !isSimple(q_bar, Q_prev, Q_curr)) {
				// (* draw the boundary box of q and subdivide each of its sides into four edges at the (i-2)-order grid lines. *)
				// 실제로 boundary box를 그리지 않아도, subdivide한 작은 edge들만 line segment set에 넣어 주면 됨.
				// 기존에 있던 큰 edge를 삭제해야 함.
				vector<line_segment> temp = draw_SPBD(q);
				ret.insert(ret.end(), temp.begin(), temp.end());
			}
		}

		// Invariant 1: For any edge e and cell c of the subdivision, 
		// c has an interior point within distance |e| if and only if c and e are incident ( their closures intersect).
		// Thus there are at most six cells within distance |e| of any edge e.
		

		// (* 4. Process complex components. *)

		
		for (vector<int> S : equiv_classes(Q_curr)) {
			vector<i_quad> S_region = {};
			for (int num : S) {
				S_region.push_back(Q_curr[num]);
			} // 아래 R2 계산에서도 쓰임.

			vector<i_quad> S_prime = {};
			for (i_quad q : Q_prev) {
				vector<i_quad>::iterator it = find(S_region.begin(), S_region.end(), *q.G);
				if (it != S_region.end()) {S_prime.push_back(q);}
			}
			
			if (S_prime.size() > 1) { // (* S is complex *)

				// 아래 R1과 (특히) R2의 자료형은 vector<i_quad>로 두고, 실제로는 union처럼 다룸.
				
				// (* R1 := union of cores of growth(q) *)
				vector<core> R1 = {}; 
				for (i_quad q : S_prime) {
					R1.push_back(get_core(*q.G));
				}

				// (* R2 := the region covered by q. *)
				vector<i_quad> R2 = S_prime;
				
				vector<line_segment> temp = fill_R1R2(R1, R2);
				ret.insert(ret.end(), temp.begin(), temp.end());

				temp = fill_R1S(R1, S_region);
				ret.insert(ret.end(), temp.begin(), temp.end());
			}
		}

		

		Q_oldest.resize(Q_prev.size());
		std::copy(Q_prev.begin(), Q_prev.end(), Q_oldest.begin());
		// Q_oldest = Q_prev;

		Q_prev.resize(Q_curr.size());
		std::copy(Q_curr.begin(), Q_curr.end(), Q_prev.begin());
		// Q_prev = Q_curr;
	}

	return ret;
	// 뒤에서부터 거둬들이면서 중복된 line segment 무시
}

// for (vector<int> S : equiv_classes(Q_prev)) {

/*
void add_edge(i_quad q1, i_quad q2) {

}
*/

/*
double crit_dist = 2 * pow(2.0, i + 2); // = 8 * pow(2.0, i+2)
if (abs(pow(2.0, i) * (q1.lower_left.k - q2.upper_right.k)) <= crit_dist && abs(q1.lower_left.l - q2.upper_right.l) <= crit_dist)
{
	return true;
}
*/

// if (abs(q1.lower_left.x - q2.upper_right.x) <= crit_dist && abs(q1.lower_left.y - q2.upper_right.y) <= crit_dist) {return true;}

// int i = q1.i; // level

// int level = q1.lower_left.i;
/*
if (min(q1.upper_right.k + 1, q2.upper_right.k + 1) > max(q1.lower_left.k, q2.lower_left.k) &&
min(q1.upper_right.l + 1, q2.upper_right.l + 1) > max()) { return true; } // 좌표 자체는 pow(2.0, level)
*/

/*
vector<i_quad> conforming_subdivision::growth(vector<vector<int>> S) {

}
*/

/*
bool conforming_subdivision::outer_boundary(vector<int> S) { // boundary가 하나의 i-quad이면 true를 리턴.
	if (S.size() == 1) {return true;}
	else {return false;}
}
*/

// Given an i-quad q, growth(q) is an (i+2)-quad containing q inside its core.
// For a family S of i-quads, growth(S) is a minimal set of (i+2)-quads satisfying the following :
// i-box는 (k, l, i)로 표현. 여기서 k는 row number, l은 column number
// i-quad는 (i, ul, lr)로 표현. ul는 upper-left i-box, lr은 lower-right i-box

/*
for (vector<int> S : equiv_classes(Q_prev)) {
	vector<i_quad> G_curr = growth(ind2quads(S, Q_prev));
	// 우선은 union 대신 concatenate 하는 걸로.
	vector<i_quad> U = {};
	vector<i_quad>::iterator it = set_union(Q_curr.begin(), Q_curr.end(), G_curr.begin(), G_curr.end(), U.begin());
	Q_curr = U; // U 안 써도 되나?
}
*/

// References
// https://m.blog.naver.com/PostView.nhn?blogId=sipzirala&logNo=220628463127&proxyReferer=https:%2F%2Fwww.google.com%2F
// C++ STL 집합 연산
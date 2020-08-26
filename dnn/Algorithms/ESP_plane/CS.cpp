#pragma warning(disable : 4996)

#include <cmath>
#include <algorithm>
#include <assert.h>
#include <stdexcept>
#include "CS.h"

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
	Q_oldest = Q_prev = Q_curr = {};
}

core conforming_subdivision::get_core(i_quad q) {
	return core{ i_box{q.lower_left.k + 1, q.lower_left.l + 1, q.i}, i_box{q.upper_right.k - 1, q.upper_right.l - 1, q.i}, q };
}

bool conforming_subdivision::contained(i_quad q1, i_quad q2) { // check if (q1 set_union q2) can be contained in a 2 * 2 array of (i+2) boxes
	int k_min = int(floor(min(q1.lower_left.k, q2.lower_left.k) / 4.0)); // left bounding edge
	int l_min = int(floor(min(q1.lower_left.l, q2.lower_left.k) / 4.0)); // lower bounding edge

	if (double(k_min) + 2 >= ceil(max(q1.upper_right.k + 1, q2.upper_right.k + 1) / 4.0) &&
		double(l_min) + 2 >= ceil(max(q1.upper_right.l + 1, q2.upper_right.l + 1) / 4.0)) {
		return true;
	}
	else { return false; }
}

i_quad conforming_subdivision::bigger_quad(i_quad q1, i_quad q2) {
	assert(contained(q1, q2)); // assumption : contained(q1, q2) == true 

	int k_min = int(floor(min(q1.lower_left.k, q2.lower_left.k) / 4.0));
	int l_min = int(floor(min(q1.lower_left.l, q2.lower_left.l) / 4.0));

	int i = q1.i; // level
	return i_quad{ i+2, i_box{k_min-1, l_min-1, i+2}, i_box{k_min+2, l_min+2, i+2} };
}

i_quad conforming_subdivision::bigger_quad(i_quad q) {
	int k_min = int(floor(q.lower_left.k / 4.0));
	int l_min = int(floor(q.lower_left.l / 4.0));

	int i = q.i;
	return i_quad{ i+2, i_box{k_min-1, l_min-1, i+2}, i_box{k_min+2, l_min+2, i+2} };
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
				vector<int> v{ int(ind1), nb };
				ret.push_back(v);
				break;
			}
		}
	}

	for (unsigned int ind1 = 0; ind1 < adj_list.size(); ind1++) {
		if (!visited[ind1]) {
			visited[ind1] = true;
			vector<int> v{ int(ind1) };
			ret.push_back(v);
		}
	}

	return ret;
}

void conforming_subdivision::growth(vector<int> mask) { // vector<int> mask, 

	vector<vector<int>> adj_list;
	for (unsigned int ind1 = 0; ind1 < mask.size(); ind1++) {
		adj_list.push_back({});	// mask의 size와 같은 크기의 adj. list를 만듬
	}

	for (unsigned int ind1 = 0; ind1 < mask.size(); ind1++) {
		for (unsigned int ind2 = ind1 + 1; ind2 < mask.size(); ind2++) {
			if (contained(Q_prev[mask[ind1]], Q_prev[mask[ind2]])) {
				adj_list[ind1].push_back(ind2);
				adj_list[ind2].push_back(ind1);
			}
		}
	}

	vector<vector<int>> mm = maximal_matching(adj_list);

	for (vector<int> m : mm) {
		int sz = int(m.size());
		vector<i_quad>::iterator it;

		if (sz == 2) {
			i_quad bq = bigger_quad(Q_prev[mask[m[0]]], Q_prev[mask[m[1]]]);

			vector<i_quad>::iterator it = find(Q_curr.begin(), Q_curr.end(), bq);

			if (it == Q_curr.end()) {
				Q_curr.push_back(bq); Q_pointer.push_back(bq);
				Q_prev[mask[m[0]]].G = Q_prev[mask[m[1]]].G = &Q_pointer.back();
			}
			else {
				Q_prev[mask[m[0]]].G = Q_prev[mask[m[1]]].G = &(*it);
			}

		}

		else { // sz == 1
			i_quad bq = bigger_quad(Q_prev[mask[m[0]]]);

			vector<i_quad>::iterator it = find(Q_curr.begin(), Q_curr.end(), bq);

			if (it == Q_curr.end()) {
				Q_curr.push_back(bq); Q_pointer.push_back(bq);
				Q_prev[mask[m[0]]].G = &Q_pointer.back();
			}
			else {
				Q_prev[mask[m[0]]].G = &(*it);
			}
		}
	}

	return;
}
/*
for (unsigned int ind1 = 0; ind1 < S.size(); ind1++) {
	for (unsigned int ind2 = ind1 + 1; ind2 < S.size(); ind2++) {
		if (contained(S[ind1], S[ind2])) {
			adj_list[ind1].push_back(ind2);
			adj_list[ind2].push_back(ind1);
		}
	}
}
*/
// Q_prev[mask[m[0]]].G = Q_prev[mask[m[1]]].G = &Q_curr[back];
// &Q_curr.back();으로 두면 오류 발생

bool conforming_subdivision::overlap(i_quad q1, i_quad q2) {
	if (abs((q1.lower_left.k - q2.lower_left.k)) < 4 && abs((q1.lower_left.l - q2.lower_left.l)) < 4) { return true; }
	else { return false; }

}

vector<vector<int>> conforming_subdivision::trans_closure(vector<vector<bool>> adj_mat) {
	// input과 return값의 자료형이 같지만, input은 adj_list같은 느낌이고 output은 equivalence class들을 모아놓은 것임.
	// vector<vector<int>> res = power(adj_mat, int(adj_mat.size()));
	vector<vector<bool>> reach = adj_mat; // reachability

	unsigned int sz = adj_mat.size();

	for (unsigned int ind1 = 0; ind1 < sz; ind1++) {
		for (unsigned int ind2 = 0; ind2 < sz; ind2++) {
			for (unsigned int ind3 = 0; ind3 < sz; ind3++) {
				reach[ind2][ind3] = reach[ind2][ind3] || (reach[ind2][ind1] && reach[ind1][ind3]);
			}
		}
	}

	vector<vector<int>> ret = {};

	vector<bool> visited = {};
	for (unsigned int ind1 = 0; ind1 < sz; ind1++) {
		visited.push_back(false);
	}


	while (1) {
		vector<bool>::iterator it = find(visited.begin(), visited.end(), false);

		if (it == visited.end()) { break; }
		// 아직 방문하지 않은 node가 있다면,
		vector<int> a_class = {}; // a_class는 한 equivalence class를 나타냄
		int first_one = int(it - visited.begin());
		a_class.push_back(first_one); 
		visited[first_one] = true;

		for (unsigned int ind2 = first_one + 1; ind2 < sz; ind2++) {
			if (reach[first_one][ind2] != 0) {
				a_class.push_back(ind2);
				visited[ind2] = true;
			}
		}
		ret.push_back(a_class);
	}

	return ret;
}

vector<vector<int>> conforming_subdivision::equiv_classes(vector<i_quad> S) {

	vector<bool> elem = {};
	vector<vector<bool>> adj_mat = {};

	for (unsigned int ind1 = 0; ind1 < S.size(); ind1++) {
		elem.push_back(false);
	}

	for (unsigned int ind1 = 0; ind1 < S.size(); ind1++) {
		adj_mat.push_back(elem);
	}

	for (unsigned int ind1 = 0; ind1 < S.size(); ind1++) {
		for (unsigned int ind2 = ind1 + 1; ind2 < S.size(); ind2++) {
			if (overlap(S[ind1], S[ind2])) {
				adj_mat[ind1][ind2] = true;
				adj_mat[ind2][ind1] = true;
			}
		}
	}

	return trans_closure(adj_mat);
}

int conforming_subdivision::interior(i_quad q, char ch) {
	// 그냥 pointer q에 대한 것 아닌가?
	// 지금은 모두 돌면서 다 체크하므로 비효율적이고, 나중에 고쳐야 함.

		// vector<int> S_prime = {};
		// S_prime.push_back(*it);

	assert(ch == 'o' || ch == 'p');
	vector<i_quad> Q;
	if (ch == 'o') { Q = Q_oldest; }
	else { Q = Q_prev; }

	int num = 0;
	for (i_quad q_prime : Q) {
		if (pow(2, q.i) * q.lower_left.k <= pow(2, q_prime.i) * q_prime.lower_left.k &&
			pow(2, q.i) * q.lower_left.l <= pow(2, q_prime.i) * q_prime.lower_left.l &&
			pow(2, q_prime.i) * q_prime.upper_right.k <= pow(2, q.i) * q.upper_right.k &&
			pow(2, q_prime.i) * q_prime.upper_right.l <= pow(2, q.i) * q.upper_right.l) {
			num += 1;
		}
		// q.i = q_prime.i + 2
	}

	return num;
}

bool conforming_subdivision::ATM(i_quad q, i_quad q_bar) { // simple은 component에 대해서 정의되는 개념임
// build-subdivision step 4에서 체크하는 것은 충분조건. 이 함수 내에서는 시키는대로 simple의 필요조건 모두 체크 필요.
	bool flag1, flag2;
	if (q.i == -2) { flag1 = true; }
	else {
		if (q.alone == true && interior(q, 'o') == 1) { flag1 = true; }
		else { flag1 = false; }
	}

	if (q_bar.alone == true && interior(q, 'p') == 1) { flag2 = true; }
	else { flag2 = false; }

	if (flag1 && !flag2) { return true; }
	else { return false; }
}
// q랑 q_bar는 i-quad이므로 outer_boundary 조건은 그냥 만족됨

void conforming_subdivision::draw_SPBD(i_quad q) { // draw simple boundary
	double unit_length = pow(2, q.i);
	double x_min = q.lower_left.k * unit_length, y_min = q.lower_left.l * unit_length;

	for (__int64 i = 0; i < 4; i++) {
		BD.push_back(line_segment{ Point{x_min + i * unit_length, y_min} , Point{x_min + (i+1) * unit_length, y_min} }); // 아래쪽
		BD.push_back(line_segment{ Point{x_min + i * unit_length, y_min + 3 * unit_length}, Point{x_min + (i+1) * unit_length, y_min + 3 * unit_length} }); // 위쪽
		BD.push_back(line_segment{ Point{x_min, y_min + i * unit_length}, Point{x_min, y_min + (i+1) * unit_length} }); // 왼쪽
		BD.push_back(line_segment{ Point{x_min + 3 * unit_length, y_min + i * unit_length}, Point{x_min + 3 * unit_length, y_min + (i+1) * unit_length} }); // 오른쪽
	}
}

bool conforming_subdivision::contained(i_box B, vector<core> R1) { // box size가 큰 경우와 작은 경우 모두 커버 가능.
	for (core C : R1) {
		double x_box = pow(2, B.i) * B.k, y_box = pow(2, B.i) * B.l, x_C = pow(2, C.i) * C.lower_left.k, y_C = pow(2, C.i) * C.lower_left.l;
		if (x_C <= x_box && x_box <= x_C + 2 * pow(2, C.i) && y_C <= y_box && y_box <= y_C + 2 * pow(2, C.i)) { return true; }
	}
	return false;
}

bool conforming_subdivision::contained(i_box B, i_quad Q) {
	double x_box = pow(2, B.i) * B.k, y_box = pow(2, B.i) * B.l, x_Q = pow(2, Q.i) * Q.lower_left.k, y_Q = pow(2, Q.i) * Q.lower_left.l;
	if (x_Q <= x_box && x_box <= x_Q + 4 * pow(2, Q.i) && y_Q <= y_box && y_box <= y_Q + 4 * pow(2, Q.i)) { return true; }
	return false;
}

void conforming_subdivision::draw_CXBD(i_box B) { // draw complex boundary

	double unit_length = pow(2, B.i);

	BD.push_back(line_segment{ Point{unit_length * B.k, unit_length * B.l}, Point{unit_length * (double(B.k) + 1), unit_length * B.l} }); // 아래쪽
	BD.push_back(line_segment{ Point{unit_length * B.k, unit_length * (double(B.l) + 1)}, Point{unit_length * (double(B.k) + 1), unit_length * (double(B.l) + 1)} }); // 위쪽
	BD.push_back(line_segment{ Point{unit_length * B.k, unit_length * B.l}, Point{unit_length * B.k, unit_length * (double(B.l) + 1)} }); // 왼쪽
	BD.push_back(line_segment{ Point{unit_length * (double(B.k) + 1), unit_length * B.l}, Point{unit_length * (double(B.k) + 1), unit_length * (double(B.l) + 1)} }); // 오른쪽
}

void conforming_subdivision::fill_R1R2(vector<core> R1, vector<i_quad> R2) { 
	// R1의 level = R2의 level + 2
	// R1과 R2에서 같은 index의 원소들은 1대1 대응 관계임

	double unit_length = pow(2, R1[0].i - 2);

	for (unsigned int ind1 = 0; ind1 < R1.size(); ind1++) { // for each core
		for (int ind2 = 0; ind2 < 8; ind2++) {
			for (int ind3 = 0; ind2 < 8; ind2++) {
				i_box B = { int(4 * pow(2, R1[ind1].i) * R1[ind1].lower_left.k + unit_length * ind2), 
				int(4 * pow(2, R1[ind1].i) * R1[ind1].lower_left.l + unit_length * ind3), R1[ind1].i - 2 };
				if (!contained(B, R2[ind1])) { draw_CXBD(B); } // draw edges. 4개의 edge를 추가함으로써 반영.
			}
		}
	}
}

void conforming_subdivision::fill_R1S(vector<core> R1, vector<i_quad> S_region) {

	double unit_length = pow(2, R1[0].lower_left.i);

	for (i_quad q : S_region) {
		for (int ind1 = 0; ind1 < 4; ind1++) {
			for (int ind2 = 0; ind2 < 4; ind2++) {
				i_box B = { int(pow(2, q.i) * q.lower_left.k + unit_length * ind1), int(pow(2, q.i) * q.lower_left.l + unit_length * ind1), q.i };
				if (!contained(B, R1)) { draw_CXBD(B); } // intersection 여부를 모든 core에 대해 검사해야 함.
			}
		}
	}
}

void conforming_subdivision::build_subdivision() {	// inefficient
	BD = {};

	// Q : family of quads. Q_prev <=> Q(i-2), Q_curr <=> Q(i).
	int i = -2; // level
	for (auto pt : pts) { // initialization step to construct Q_prev = Q_init := Q(-2)
		Q_prev.push_back(i_quad{ -2, i_box{int(floor(pt.x / 0.25) - 1), int(floor(pt.y / 0.25) - 2), -2}, i_box{int(floor(pt.x / 0.25) + 2), int(floor(pt.y / 0.25) + 1), -2} });
	}

	for (auto q : Q_prev) {
		draw_SPBD(q);
	}

	while (Q_prev.size() > 1) {
		// (* 1. Increment i: i = i + 2)
		i += 2;

		// (* 2. Compute Q(i) from Q(i-2) *)

		// (a) Initialize Q(i) = empty_set;

		Q_curr.clear(); // vector<i_quad>().swap(Q_curr);
		Q_pointer.clear();

		// (b) for each equivalence class S of Q(i-2) do ...

		// vector<vector<int>> EC = equiv_classes(Q_prev);
		// for (unsigned int ind1 = 0; ind1 < EC.size(); ind1++) {
		// 	vector<int> S = EC[ind1];

		for (vector<int> S : equiv_classes(Q_prev)) {

			growth(S); // 이 growth 함수 내에서 Q_curr에 원소들이 추가됨.
			// Q_prev는 함수 내에서 자유롭게 접근 가능하므로, 우선은 union 대신 concatenate 하는 걸로.
			if (int(S.size()) > 1) {
				for (int elem : S) { Q_prev[elem].alone = false; }
			}
			else { Q_prev[S[0]].alone = true; }
		}

		// (c) for every pair of i_quads q, q' in Q(i) do ...
		// (d) Extend =_i to an equivalence relation to... and compute the equivalence classes.

		// (* 3. Process simple components of ≡_i-2 that are about to merge with some other components. *)
		for (i_quad q : Q_prev) {
			i_quad q_bar = *q.G;
			if (ATM(q, q_bar)) {
				// (* draw the boundary box of q and subdivide each of its sides into four edges at the (i-2)-order grid lines. *)
				// 실제로 boundary box를 그리지 않아도, subdivide한 작은 edge들만 line segment set에 넣어 주면 됨.
				// 기존에 있던 큰 edge를 삭제해야 함.
				draw_SPBD(q);
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
				if (it != S_region.end()) { S_prime.push_back(q); }
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

				fill_R1R2(R1, R2);

				fill_R1S(R1, S_region);
			}
		}



		Q_oldest.resize(Q_prev.size());
		std::copy(Q_prev.begin(), Q_prev.end(), Q_oldest.begin());
		// Q_oldest = Q_prev;

		Q_prev.resize(Q_curr.size());
		std::copy(Q_curr.begin(), Q_curr.end(), Q_prev.begin());
		// Q_prev = Q_curr;

		EC_prev.resize(Q_curr.size());
		std::copy(EC_curr.begin(), EC_curr.end(), EC_prev.begin());
	}

	dedup();
}

void conforming_subdivision::dedup() { // 앞에서부터 읽어나가면서 중복된 line segment 무시
	/* realizes breaking each cell boundary with an endpoint incident to R1 into four edges of length 2^(i-2), to satisfy Invariant 1. */


}
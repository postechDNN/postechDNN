#pragma once

#include <vector>
#include <list>

using namespace std;

typedef struct Point Point;
typedef struct i_box i_box;
typedef struct i_quad i_quad;
typedef struct core core;
// typedef struct Q Q;

struct Point {
	double x;
	double y;
};

struct i_box {
	int k; // its lower-left corner lies at a point (k2^i, l2^i)
	int l;
	int i; // = i_quad.i
};

bool operator==(i_box a, i_box b);

struct i_quad {
	int i; // level
	i_box lower_left; // upper-left
	i_box upper_right; // lower-right
	i_quad* G; // growth
	bool alone;
	// core는 get_core()를 통해서 필요할 때마다 i_quad로부터 뽑아내면 됨.
};

bool operator==(i_quad a, i_quad b);

struct core {
	i_box lower_left;
	i_box upper_right;
	i_quad parent_quad;
	int i; // = i_quad.i
};

struct line_segment { // represents a line segment. Either p1.x < p2.x or p1.y > p2.y.
	Point p1;
	Point p2;
	int i; // i_box의 단위 edge 길이짜리 line segment  
};

class conforming_subdivision {
public: // 변수
	list<i_quad> Q_pointer;
	vector<Point> pts;
	vector<i_quad> Q_oldest, Q_prev, Q_curr;
	vector<vector<int>> EC_prev, EC_curr;
	vector<line_segment> BD;

public: // 함수
	conforming_subdivision(vector<Point>);

	core get_core(i_quad);
	bool contained(i_quad, i_quad);
	i_quad bigger_quad(i_quad, i_quad);
	i_quad bigger_quad(i_quad);
	// vector<i_quad> ind2quads(vector<int>, vector<i_quad>);
	vector<vector<int>> maximal_matching(vector<vector<int>>);
	void growth(vector<int>); // growth(S)
	bool overlap(i_quad, i_quad);
	// vector<vector<int>> matmul(vector<vector<int>>, vector<vector<int>>);
	// vector<vector<int>> power(vector<vector<int>> mat, int k);
	vector<vector<int>> trans_closure(vector<vector<bool>>);
	vector<vector<int>> equiv_classes(vector<i_quad>);
	int interior(i_quad, char);
	bool ATM(i_quad, i_quad); // about to merge
	void draw_SPBD(i_quad);
	bool contained(i_box, vector<core>);
	bool contained(i_box, i_quad);
	void draw_CXBD(i_box);
	void fill_R1R2(vector<core>, vector<i_quad>);
	void fill_R1S(vector<core>, vector<i_quad>);
	void build_subdivision(); // subdivision : set of line segments. datatype : vector<line_segment>
	void dedup(); // deduplication
};

// void add_edge(i_quad, i_quad); // class 변수를 변경할 것이므로 void
// vector<i_quad> growth(i_quad); // growth(q)
// bool outer_boundary(vector<int>); // size가 1인 것과 iff인 것 같아서 우선 지움

/*
struct Q {
	int level;
	vector<i_quad> set;
};
*/

// vector set_union(vector<i_quad>, vector<i_quad>);
// vector<i_quad> ind2quads(vector<int>, vector<i_quad>); // 각 equivalence class를 의미하는 index 집합을 실제 i-quad들의 집합으로 바꾸는 함수
// bool isSimple(i_quad, Q, Q);
#pragma warning(disable : 4996)

#include <cmath>
#include <algorithm>
#include <assert.h>
#include <stdexcept>
#include <tuple>
#include <set>
#include "CS.h"
// #include "basic.h"

// #define EPS 0.000001
typedef struct Q_iT Q_iT;

// we assume that either the horizontal or the vertical distance between any two points in V is at least 1, and no point coordinate is a multiple of 1/4.
// translation&scaling functions needed

conforming_subdivision::conforming_subdivision() {}
conforming_subdivision::~conforming_subdivision() {}
conforming_subdivision::conforming_subdivision(Point* _src, Point* _dst, vector<Point*> _vs) {
	src = _src; dst = _dst; obstacle_vertices = _vs;
	src->setIndex(0);
	for (int i = 0; i < obstacle_vertices.size(); i++) { obstacle_vertices[i]->setIndex(i + 1); }
	dst->setIndex(obstacle_vertices.size() + 1);
	// Q_im4 = 
	Q_old = Q_i = {};
	MSF_old = MSF_i = {};
}

bool conforming_subdivision::contained(i_quad* q1, i_quad* q2) { // check if (q1 set_union q2) can be contained in a 2 * 2 array of (i+2) boxes
	int lv = q1->getLv();
	
	int k_min = int(floor(min(q1->getX(), q2->getX()) / myPow(2, lv + 2))); // left bounding edge
	int l_min = int(floor(min(q1->getY(), q2->getY()) / myPow(2, lv + 2))); // lower bounding edge

	if (myPow(2, lv+2) * (k_min+2) >= ceil(max(q1->getX() + 4 * myPow(2, lv), q2->getX() + 4 * myPow(2, lv))) &&
		myPow(2, lv + 2) * (l_min + 2) >= ceil(max(q1->getY() + 4 * myPow(2, lv), q2->getY() + 4 * myPow(2, lv)))) {
		return true;
	}
	else { return false; }
}

// choose an (i+2)-quad in the maximal matching
i_quad* conforming_subdivision::bigger_quad(i_quad* q1, i_quad* q2) {
	assert(contained(q1, q2)); // assumption : contained(q1, q2) == true 

	int lv = q1->getLv();

	int k_min = int(floor(min(q1->getX(), q2->getX()) / myPow(2, lv + 2))); // left bounding edge
	int l_min = int(floor(min(q1->getY(), q2->getY()) / myPow(2, lv + 2))); // lower bounding edge

	i_quad* bq = new i_quad{ lv + 2, (k_min-1) * myPow(2, lv + 2), (l_min-1) * myPow(2, lv + 2) };

	return bq;
}

i_quad* conforming_subdivision::bigger_quad(i_quad* q) {
	int lv = q->getLv();

	int k = int(floor(q->getX() / myPow(2, lv + 2))); // left bounding edge
	int l = int(floor(q->getY() / myPow(2, lv + 2))); // lower bounding edge

	i_quad* bq = new i_quad{ lv + 2, (k-1) * myPow(2, lv + 2), (l-1) * myPow(2, lv + 2) };

	return bq;
}

// pair<vector<vector<int>>, vector<i_quad*>>
vector<pair<i_quad*, i_quad*>> conforming_subdivision::construct_graph(vector<i_quad*> Qs) { // computes 'touching' i_quads for each i_quad. 

	vector<pair<i_quad*, i_quad*>> ret = {};
	// sort(Qs.begin(), Qs.end(), sortbyY);
	// stable_sort(Qs.begin(), Qs.end(), sortbyX);
	set<i_quad*> S;
	S.insert(Qs[0]);
	int sidelength = myPow(2, Qs[0]->getLv() + 2);

	for (unsigned int i = 1; i < Qs.size(); i++) {
		if (Qs[i]->getX() > Qs[i - 1]->getX() + sidelength) { // recall that each i-quad has sidelength 4 * 2^(lv)
			S.clear();
			S.insert(Qs[i]); // eliminate all nodes in S, and insert a single node.
		}
		else { // there may exist i_quads in S that are 'touching'
			auto it = S.lower_bound(Qs[i]);
			for (it; it != S.end(); ++it) {
				if ((*it)->getY() + sidelength < Qs[i]->getY()) { break; }
				else {
					if ((*it)->getX() + sidelength < Qs[i]->getX()) { S.erase(it); it--; }
					else {
						(*it)->addTouching(Qs[i]); Qs[i]->addTouching(*it);
						pair<i_quad*, i_quad*> p = make_pair(Qs[i], *it); ret.push_back(p);
					}
				}
			}
			S.insert(Qs[i]);
		}
	}
	return ret; // in total, it has linear size
}

pair<vector<pair<i_quad*, i_quad*>>, vector<i_quad*>> conforming_subdivision::maximal_matching(vector<i_quad*> Qs, vector<pair<i_quad*, i_quad*>> edges) {
	// this algorithm takes linear time (linear in the size of i_quads), because the size of 'touching' for each i_quad is constant
	vector<pair<i_quad*, i_quad*>> matchings = {};
	vector<i_quad*> alone = {};

	for (auto edge : edges) {
		i_quad* q1 = edge.first;
		i_quad* q2 = edge.second;
		if (!q1->getVisited() && !q2->getVisited()) { matchings.push_back(edge); q1->setVisited(true); q2->setVisited(true); }
	}

	for (auto Q : Qs) {
		if (!Q->getVisited()) { alone.push_back(Q); }
	}

	auto ret = make_pair(matchings, alone);
	return ret;
}

vector<i_quad*> conforming_subdivision::growth(vector<i_quad*> Qs) { // using plane sweep
	
	vector<i_quad*> ret = {};
	vector<pair<i_quad*, i_quad*>> edges = construct_graph(Qs);
	pair<vector<pair<i_quad*, i_quad*>>, vector<i_quad*>> mm = maximal_matching(Qs, edges);

	vector<pair<i_quad*, i_quad*>> matchings = mm.first;
	vector<i_quad*> alone = mm.second;

	for (auto m : matchings) {
		auto q1 = m.first, q2 = m.second;
		i_quad* bq = bigger_quad(q1, q2);
		ret.push_back(bq);
		q1->setG(bq); q2->setG(bq);
	}

	for (auto q : alone) {
		i_quad* bq = bigger_quad(q);
		ret.push_back(bq); 
		q->setG(bq);
	}

	return ret;
}

bool conforming_subdivision::overlap(i_quad* q1, i_quad* q2) {
	int lv = q1->getLv();
	if (abs(q1->getX() - q2->getX()) < 4 * myPow(2, lv) && abs((q1->getY() - q2->getY())) < 4 * myPow(2, lv)) { return true; }
	else { return false; }
}

vector<tuple<int, int, double>> conforming_subdivision::delaunay(Graph*) {
	return {};
}

bool conforming_subdivision::is_simple(i_quad* q) {
	/*
	if () { return true; }
	else { return false; }
	*/
	return true;
}

bool conforming_subdivision::about_to_merge(i_quad* q) { // simple�� component�� ���ؼ� ���ǵǴ� ������
// build-subdivision step 4���� üũ�ϴ� ���� �������. �� �Լ� �������� ��Ű�´�� simple�� �ʿ����� ��� üũ �ʿ�.
	if (is_simple(q) && !is_simple(q->getG())) { return true; }
	else { return false; }
}
// q�� q_bar�� i-quad�̹Ƿ� outer_boundary ������ �׳� ������

// �ϴ� union���� ������ �ִ��� �ƴ������� �Ǵ��ؾ� �ҵ�?
// ���� quad�� �����ϴ� �� �´� ��. pair<int*, vector<i_quad*>> 
// the first argument in pair gives, for each input i_quad, which equivalent classes it is contained, in order.
// the second argument in pair gives, 
pair<vector<vector<int>>, vector<i_quad*>> conforming_subdivision::equiv_classes(vector<i_quad*> Qs) { // plane sweep to compute the equivalence classes
	int Q_size = Qs.size();
	int* root; int* rank;
	root = new int[Q_size]; // for Kruskal's algorithm
	rank = new int[Q_size];
	for (int j = 0; j < Q_size; j++) {
		root[j] = j;
		rank[j] = 0;
	}

	// sort(Qs.begin(), Qs.end(), sortbyYDecr);
	// stable_sort(Qs.begin(), Qs.end(), sortbyXIncr);
	set<i_quad*> S;
	S.insert(Qs[0]);
	int sidelength = myPow(2, Qs[0]->getLv() + 2);

	for (unsigned int i = 1; i < Qs.size(); i++) {
		if (Qs[i]->getX() > Qs[i - 1]->getX() + sidelength) { // recall that each i-quad has sidelength 4 * 2^(lv)
			S.clear();
			S.insert(Qs[i]); // eliminate all nodes in S, and insert a single node.
		}
		else { // there may exist i_quads in S that are 'touching'
			auto it = S.lower_bound(Qs[i]);
			for (it; it != S.end(); ++it) {
				if ((*it)->getY() + sidelength < Qs[i]->getY()) { break; }
				else {
					if ((*it)->getX() + sidelength < Qs[i]->getX()) { S.erase(it); it--; }
					else {
						my_union((*it)->getIndex(), i, root, rank, -1); // -1 has no meaning
					}
				}
			}
			S.insert(Qs[i]);
		}
	}

	vector<vector<int>> EC = {}; EC.assign(Qs.size(), {});
	for (int i = 0; i < Qs.size(); i++) {
		int rt = root[i];
		EC[rt].push_back(i);
	}

	/*
	for (auto it = EC.begin(); it != EC.end(); ++it) {
		if ((*it).empty()) { EC.erase(it);  it--; }
	}
	*/

	// at this moment, Qs is sorted.
	pair<vector<vector<int>>, vector<i_quad*>> ret = make_pair(EC, Qs);

	return ret;
}
// vector<vector<int>> conforming_subdivision::trans_closure(vector<vector<bool>> adj_mat)

void conforming_subdivision::build_subdivision() {
	int i = -2;
	vector<Point*> srcNobs = obstacle_vertices;
	srcNobs.push_back(src);

	for (auto pt : srcNobs) { 
		i_quad* Q = new i_quad(-2, 0.25 * int(floor(pt->getX()/ 0.25) - 1), 0.25 * int(floor(pt->getY() / 0.25) - 1));
		Q_old.push_back(Q); 
		Graph* G = new Graph({ pt }, {}, { Q }, pt->getIndex());
		MSF_old.push_back(G);
	}

	set<int> N = {}; // set of trees with cardinality more than one. we store the indices of the trees.
	// vector<Graph*> N = {}; // set of trees with cardinality more than one
	// compute L_inf Delaunay triangulation DT, say, vector<tuple<int, int>> DT = delaunay(G).
	// assume that DT is given as edges (equivalently, vertex-vertex pairs).
	vector<tuple<int, int>> DT = {};
	vector<Edge*> edges = {};
	Graph* G = new Graph(srcNobs, {}, Q_old, -1);

	for (auto edge : DT) {
		Edge* E = new Edge(G->getVertex(get<0>(edge)), G->getVertex(get<1>(edge)));
		edges.push_back(E);
	}
	// sort(edges.begin(), edges.end(), sortbylength); // sort edges in increasing order of length

	int num_vertices = G->getNumVertices();
	int* root; int* rank;
	root = new int[num_vertices]; // union-and-find for Kruskal's algorithm
	rank = new int[num_vertices];

	for (int j = 0; j < num_vertices; j++) {
		root[j] = j;
		rank[j] = 0;
	}
	int j = 0;

	while (Q_i.size() > 1) { // until all components merge into one, process sorted edges one by one.
		vector<Edge*> new_edges = {};
		int i_old = i;
		double edge_length;
		int k, k_old;
		bool first = true;
		if (N.size() > 0) { i += 2; }
		else { // set i to the smallest even i' > i such that MSF(i') is not equal to MSF(i).
			// each time, if possible, we compute T_1, T_2, to avoid storing several edges connecting the same two trees as new edges.
			while (true) {
				int v1 = edges[j]->getP1()->getIndex(), v2 = edges[j]->getP2()->getIndex();
				int T1 = my_find(v1, root, num_vertices), T2 = my_find(v2, root, num_vertices); // T1 and T2 are indices of the corresponding graphs
				if (T1 == T2) { j += 1; }
				else {
					edge_length = edges[j]->getLength();

					if (first) { i = k_old = 2 * ceil(0.5 * log2(edge_length / 6)); }
					else { i = k = 2 * ceil(0.5 * log2(edge_length / 6)); }

					if (!first && (k != k_old)) { break; }

					int Ts[2] = { T1,T2 };
					for (auto Tx : Ts) {
						auto it = N.find(Tx);
						if (it != N.end()) { N.erase(it); }
						else {
							for (int m = 0; m < int((i - 2 - i_old) / 2); i++) {
								i_quad* temp = MSF_old[Tx]->getQ_iT(0);
								MSF_old[Tx]->setQ_ioldT({ bigger_quad(temp) });
							}
						}
					}
					// join T1 and T2 to get T'...
					int newroot = my_union(T1, T2, root, rank, -1); int nonroot;
					if (newroot != T1) { nonroot = T1; }
					else { nonroot = T2; }
					Graph* merged = MSF_old[newroot]->operator+(MSF_old[nonroot]);
					merged->addEdge(edges[j]);
					MSF_old[nonroot] = merged; 
					vector<i_quad*> temp = MSF_old[newroot]->getQ_ioldT();
					vector<i_quad*> temp2 = MSF_old[nonroot]->getQ_ioldT();
					temp.insert(temp.end(), temp2.begin(), temp2.end());
					MSF_old[newroot]->setQ_ioldT(temp);
					delete MSF_old[nonroot]; MSF_old[nonroot] = NULL;
					N.insert(newroot);

					first = false;
				}
			}
		}

		MSF_i = MSF_old;
		for (auto n : N) {
			auto T = MSF_i[n];
			T->setQ_iT({});
			auto temp = equiv_classes(T->getQ_ioldT());
			auto EC = temp.first;  auto Qs = temp.second;
			for (auto ec : EC) {
				vector<i_quad*> S = {};
				for (auto num : ec) { S.push_back(Qs[num]); }
				T->addQ_iT(growth(S));
			}

			// auto temp = equiv_classes(T->getQ_iT());
			// auto EC = temp.first;  auto Q_iT = temp.second; // Q_iT is a subset of the Qs just above.

			// (* 3. Process simple components of ��_i-2 that are about to merge with some other components. *)
			for (auto q : T->getQ_ioldT()) {
				// at this moment, q->G is correctly computed.
				if (about_to_merge(q)) { draw_SPBD(q); }
			}
			
			// (* 4. Process complex components. *)
			
			
			vector<vector<int>> equi_classes = get<0>(equiv_classes(Q_i));
			for (auto ec : equi_classes) {
				vector<i_quad*> Qs = {};
				for (auto i : ec) {
					Qs.push_back(Q_i[i]);
				}
			
				// vector<vector<double>> R1 = union_core(Qs);
				// vector<vector<double>> R2 = union_normal(Qs);
			}
			/*
			
			for (vector<int> S : equiv_classes(Q_i)) {
				vector<i_quad> S_region = {};
				for (int num : S) {
					S_region.push_back(Q_i[num]);
				} // �Ʒ� R2 ��꿡���� ����.

				vector<i_quad> S_prime = {};
				for (i_quad* q : Q_im2) {
					vector<i_quad>::iterator it = find(S_region.begin(), S_region.end(), *q.G);
					if (it != S_region.end()) { S_prime.push_back(q); }
				}

				if (S_prime.size() > 1) { // (* S is complex *)

				
					// �Ʒ� R1�� (Ư��) R2�� �ڷ����� vector<i_quad>�� �ΰ�, �����δ� unionó�� �ٷ�.

					// (* R1 := union of cores of growth(q) *)
					vector<core> R1 = {};
					for (i_quad q : S_prime) {
						core temp = get_core(*q.G);
						vector<core>::iterator it = find(R1.begin(), R1.end(), temp);
						if (it == R1.end()) { R1.push_back(temp); }
					}

					// (* R2 := the region covered by q. *)
					vector<i_quad> R2 = S_prime;

					fill_R1R2(R1, R2);

					fill_R1S(R1, S_region);
				}
				
			}
			*/
			
			if (T->getQ_iT().size() == 1) {
				auto it = N.find(T->getIndex());
				if (it != N.end()) { N.erase(it); }
				else { throw NULL; }
			}
		}

		Q_old.resize(Q_i.size());
		std::copy(Q_i.begin(), Q_i.end(), Q_old.begin());
	}
}

void conforming_subdivision::draw_SPBD(i_quad* q) { // draw simple boundary
		double unit_length = pow(2, q->getLv());
		// double x_min = q->ll.k * unit_length, y_min = q->ll.l * unit_length;

		for (__int64 i = 0; i < 4; i++) {
			// BD.push_back(line_segment{ Point{x_min + i * unit_length, y_min} , Point{x_min + (i + 1) * unit_length, y_min} }); // �Ʒ���
			// BD.push_back(line_segment{ Point{x_min + i * unit_length, y_min + 3 * unit_length}, Point{x_min + (i + 1) * unit_length, y_min + 3 * unit_length} }); // ����
			// BD.push_back(line_segment{ Point{x_min, y_min + i * unit_length}, Point{x_min, y_min + (i + 1) * unit_length} }); // ����
			// BD.push_back(line_segment{ Point{x_min + 3 * unit_length, y_min + i * unit_length}, Point{x_min + 3 * unit_length, y_min + (i + 1) * unit_length} }); // ������
		}
}

/*
	bool conforming_subdivision::contained(i_box* B, vector<core> R1) {
		assert(B.i == R1[0].i);
		for (core C : R1) {
			int x_diff = B.k - C.ll.k, y_diff = B.l - C.ll.l;
			if (0 <= x_diff && x_diff <= 1 && 0 <= y_diff && y_diff <= 1) { return true; }
		}
		return false;
	}

	bool conforming_subdivision::contained(i_box* B, vector<i_quad*> Qs) {
		assert(B.i == Qs[0].i);

		for (auto Q : Qs) {
			int x_diff = B.k - Q.ll.k, y_diff = B.l - Q.ll.l;
			if (0 <= x_diff && x_diff <= 3 && 0 <= y_diff && y_diff <= 3) { return true; }
		}

		return false;
	}

	void conforming_subdivision::draw_CXBD(i_box* B) { // draw complex boundary

		double unit_length = pow(2, B.i);

		// BD.push_back(line_segment{ Point{unit_length * B.k, unit_length * B.l}, Point{unit_length * (double(B.k) + 1), unit_length * B.l} }); // �Ʒ���
		// BD.push_back(line_segment{ Point{unit_length * B.k, unit_length * (double(B.l) + 1)}, Point{unit_length * (double(B.k) + 1), unit_length * (double(B.l) + 1)} }); // ����
		// BD.push_back(line_segment{ Point{unit_length * B.k, unit_length * B.l}, Point{unit_length * B.k, unit_length * (double(B.l) + 1)} }); // ����
		// BD.push_back(line_segment{ Point{unit_length * (double(B.k) + 1), unit_length * B.l}, Point{unit_length * (double(B.k) + 1), unit_length * (double(B.l) + 1)} }); // ������
	}

	void conforming_subdivision::fill_R1R2(vector<core> R1, vector<i_quad> R2) {
		// R1�� level = R2�� level + 2
		// R1�� R2���� ���� index�� ���ҵ��� 1��1 ���� ������
		for (core r1 : R1) {
			for (int j = 0; j < 8; j++) {
				for (int ind3 = 0; ind3 < 8; ind3++) {
					i_box B = { 4 * r1.ll.k + j, 4 * r1.ll.l + ind3, r1.i - 2 };
					if (!contained(B, R2)) { draw_CXBD(B); } // draw edges. 4���� edge�� �߰������ν� �ݿ�.
				}
			}
		}
	}

	
	void conforming_subdivision::fill_R1S(vector<core> R1, vector<i_quad> S_region) {

		for (i_quad q : S_region) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					i_box B = { q.ll.k + i, q.ll.l + j, q.i };
					if (!contained(B, R1)) { draw_CXBD(B); } // intersection ���θ� ��� core�� ���� �˻��ؾ� ��.
				}
			}
		}

	}
	*/

	// https://stackoverflow.com/questions/1505675/power-of-an-integer-in-c
int myPow(int x, unsigned int p)
{
	if (p == 0) return 1;
	if (p == 1) return x;

	int tmp = myPow(x, p / 2);
	if (p % 2 == 0) return tmp * tmp;
	else return x * tmp * tmp;
}

// https://gmlwjd9405.github.io/2018/08/31/algorithm-union-find.html
int my_find(int x, int* root, int count) {
	// ��Ʈ ���� �θ� ��� ��ȣ�� �ڱ� �ڽ��� ������.
	if (root[x] == x) {
		return x;
	}
	else {
		// "��� ����(Path Compression)"
		// find �ϸ鼭 ���� ��� ���� �θ� ��带 root�� �����.
		return root[x] = my_find(root[x], root, count);
	}
}

/* union1(x, y): union-by-rank ����ȭ */
int my_union(int x, int y, int* root, int* rank, int count) {
	x = my_find(x, root, count);
	y = my_find(y, root, count);

	// �� ���� root�� ������(�̹� ���� Ʈ��) ��ġ�� �ʴ´�.
	if (x == y)
		return -1; // debug �ʿ�. �³�?

	// "union-by-rank ����ȭ"
	// �׻� ���̰� �� ���� Ʈ���� ���̰� ���� Ʈ�� �ؿ� �ִ´�. ��, ���̰� �� ���� ���� root�� ����
	if (rank[x] < rank[y]) {
		root[x] = y; // x�� root�� y�� ����
		return y; // return the new root
	}
	else {
		root[y] = x; // y�� root�� x�� ����
		return x; // return the new root

		if (rank[x] == rank[y])
			rank[x]++; // ���� ���̰� ���ٸ� ��ģ �� (x�� ���� + 1)
	}
}

double my_log2(double x) { return log(x) / log(2); }



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
conforming_subdivision::conforming_subdivision(Point* _src, Point* _dst, vector<vector<Point*>> _vs) {
	int obver_id = 2;
	src = _src; dst = _dst;  obstacles = _vs;
	src->setIndex(0);
	for (int j = 0; j < obstacles.size(); j++) {
		for (int i = 0; i < obstacles[j].size(); i++) { obstacles[j][i]->setIndex(obver_id); obver_id++;}
	}
	dst->setIndex(1);
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

vector<i_quad*> conforming_subdivision::growth(vector<i_quad*> Qs) { // using plane sweep. need to debug once
	
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

bool conforming_subdivision::about_to_merge(i_quad* q) { // simple은 component에 대해서 정의되는 개념임
// build-subdivision step 4에서 체크하는 것은 충분조건. 이 함수 내에서는 시키는대로 simple의 필요조건 모두 체크 필요.
	if (q->is_simple && !q->G->is_simple) { return true; }
	else { return false; }
}
// q랑 q_bar는 i-quad이므로 outer_boundary 조건은 그냥 만족됨

// 일단 union으로 가지고 있는지 아닌지부터 판단해야 할듯?
// 개별 quad로 존재하는 건 맞는 듯. pair<int*, vector<i_quad*>> 
// the first argument in pair gives, for each input i_quad, which equivalent classes it is contained, in order.
// the second argument in pair gives, 
pair<vector<vector<int>>, vector<i_quad*>> conforming_subdivision::equiv_classes(vector<i_quad*> Qs) { 
// plane sweep to compute the equivalence classes
	int Q_size = Qs.size();
	int* root; int* rank;
	root = new int[Q_size]; // for Kruskal's algorithm
	rank = new int[Q_size];
	for (int j = 0; j < Q_size; j++) {
		root[j] = j;
		rank[j] = 0;
	}


	sort(Qs.begin(), Qs.end(), sortbyYDecr);
	stable_sort(Qs.begin(), Qs.end(), sortbyXIncr);
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

	for (int i = 0; i < EC.size(); i++) {
		if (EC[i].size() > 1) {
			for (auto index : EC[i]) {
				Qs[index]->is_simple = false;
			}
		}
		else if (EC[i].size() == 1) Qs[EC[i][0]]->is_simple = true;
	}
	pair<vector<vector<int>>, vector<i_quad*>> ret = make_pair(EC, Qs);

	return ret;
}
// vector<vector<int>> conforming_subdivision::trans_closure(vector<vector<bool>> adj_mat)

DCEL* conforming_subdivision::build_subdivision() {
	DCEL* D = new DCEL;

	int i = -2;

	vector<Point*> srcNobs; // source and obstacles.

	for (auto ob : obstacles) {
		for (auto pt : ob) {
			srcNobs.push_back(pt);
		}
	}
	srcNobs.push_back(src);

	// for each point in srcNobs, construct a graph (an isolated vertex) and transform it into an i_quad. 
	for (auto pt : srcNobs) { 
		i_quad* Q = new i_quad(-2, 0.25 * int(floor(pt->getX()/ 0.25) - 1), 0.25 * int(floor(pt->getY() / 0.25) - 1));
		Q_old.push_back(Q); 
		MSF_i.push_back(new Graph({ pt }, {}, { Q }, pt->getIndex()));
	}

	// set of trees with cardinality more than one. we store the indices of the trees.
	set<int> N = {}; 

	// 현재 Q_old에는 srcNobs에 해당하는 quad들이 들어 있음
	Graph* G = new Graph(srcNobs, {}, Q_old, -1);

	// compute delaunay triangulation
	// srcNobs에 해당하는 Point들을 받아서, 그것의 delaunay triangulation에 해당하는 Graph를 반환
	toDelaunay(srcNobs, G);

	// after computing Delaunay triangulation, we compute minimum spanning forest

	// sort edges in increasing order of length
	// sort(edges.begin(), edges.end(), sortbylength);
	sort(G->edges.begin(), G->edges.end(), sortbylength_inf); 

	// remove duplicated edges
	// *** G->edges와 edges 구분 필요. ***
	auto it = G->edges.begin();
	while (it != G->edges.end()) {
		auto it2 = it; it2++;
		if (it2 == G->edges.end()) break;
		
		if ((*it)->p1->index == (*it2)->p1->index && (*it)->p2->index == (*it2)->p2->index) {
			it = G->edges.erase(it);
			G->num_edges--;	
		}
		else it++;
	}

	//  Kruskal's algorithm
	int num_vertices = G->getNumVertices();
	int* root; int* rank;
	root = new int[num_vertices]; 
	rank = new int[num_vertices];

	// srcNobs의 size만큼 makeset operation을 진행. (initialization for Kruskal)
	for (int j = 0; j < num_vertices; j++) {
		root[j] = j;
		rank[j] = 0;
	}
	Q_i = Q_old;

	vector<Edge*> new_edges;
	// until all components merge into one, process sorted edges (G->edges) one by one.
	while (Q_i.size() > 1) { 
		vector<Edge*>().swap(new_edges);
		int i_old = i;
		// sorted된 G->edges에서 순서대로 꺼낼 edge의 (L_inf) 길이
		double edge_length;
		// for each edge length, we compute k (at k-th stage we stop and process)
		// k_old는 k와 비교하면서 간보려고. 지금처럼 매 while문마다 -1로 초기화해 주는 것이 좋은 듯.
		int k = -1, k_old = -1;
		// j는 현재 보고 있는 G->edges의 index
		int j = 0;
		bool first = true;

		if (N.size() > 0) { i += 2; }
		else { // set i to the smallest even i' > i such that MSF(i') is not equal to MSF(i).
			// each time, if possible, we compute T_1, T_2, to avoid storing several edges connecting the same two trees as new edges.

			while (true) {
				int v1 = G->edges[j]->p1->index, v2 = G->edges[j]->p2->index;
				int T1 = my_find(v1, root, num_vertices), T2 = my_find(v2, root, num_vertices); // T1 and T2 are indices of the corresponding graphs
				
				// edge의 양 끝점에 해당하는 두 tree가 이미 연결되어 있으면, Kruskal의 방식대로 이 edge를 넘어감
				if (T1 == T2) { j += 1; }
				else {
					edge_length = G->edges[j]->getLength_inf();

					if (first) { i = k_old = 2 * ceil(0.5 * log2(edge_length / 6)); }
					else { i = k = 2 * ceil(0.5 * log2(edge_length / 6)); }

					// first - MSF가 달라지기 위해 적어도 하나의 edge는 추가해야 하므로, 이와 관련한 변수. 
					// k와 k_old가 같으면 계속 j를 늘려야 함.
					if (!first && (k != k_old)) { break; }

					// pseudocode에서와 달리 tree를 계속 합치기. 두 tree의 index와 관련한.
					int Ts[2] = { T1,T2 };
					for (auto Tx : Ts) {
						auto it = N.find(Tx);
						// if Tx is in N, then remove Tx from N. 
						// 어차피 다시 들어가게 돼 있으므로 지금 빼는 것이 맞음
						if (it != N.end()) { N.erase(it); }
						// else, compute the singleton (i-2)-quad.
						// 다시 말해, vertex를 포함하는 quad를 만드는 과정.
						else {
							// 이 경우, |Q(i, Tx)| = 1이므로 Q_iT의 첫번째 원소가 해당하는 i-quad임
							// 직전 step까지 quad를 키움.
							for (int m = 0; m < int((i - 2 - i_old) / 2); m++) {
								i_quad* temp = MSF_i[Tx]->Q_iT[0]; // 매번 같은 애가 잡힘.
								MSF_i[Tx]->Q_iT.push_back(bigger_quad(temp));
								MSF_i[Tx]->Q_iT.erase(MSF_i[Tx]->Q_iT.begin());
							}
						}
					}
					// join T1 and T2 to get T'
					// my_union은 merge이후 tree의 root를 반환
					int newroot = my_union(T1, T2, root, rank, -1); 
					// nonroot는 기존에 root였으나 merge이후 root가 아니게된 애.
					int nonroot;
					if (newroot != T1) { nonroot = T1; } else { nonroot = T2; }
					// 두 그래프를 합치고 edge까지 추가해서, 기존 nonroot의 위치에 저장.
					MSF_i[newroot]->operator+(MSF_i[nonroot]);
					MSF_i[newroot]->addEdge(G->edges[j]);
					// Q_iT를 union
					MSF_i[newroot]->Q_iT.insert(MSF_i[newroot]->Q_iT.end(), MSF_i[nonroot]->Q_iT.begin(), MSF_i[nonroot]->Q_iT.end());
					// 합쳤으니까, 기존 것들은 삭제.
					delete MSF_i[nonroot]; MSF_i[nonroot] = NULL;
					// put T' in N.
					N.insert(newroot);

					j++;
					first = false;
				}
			}
		}
		MSF_old = MSF_i;

		// for each T in N do. 아래 auto까지 두 줄에 해당하는 내용임
		for (auto n : N) {
			auto T = MSF_i[n];
			
			T->Q_old = T->Q_iT;
			// initialize Q(i, T) = \emptyset
			T->setQ_iT({});

			// for each equivalence class S of Q(i-2, T)) do
			auto temp = equiv_classes(T->getQ_old());
			// EC stands for equivalence class
			auto EC = temp.first;  auto Qs = temp.second;
			for (auto ec : EC) {
				vector<i_quad*> S = {};
				for (auto num : ec) { S.push_back(Qs[num]); }
				// Q(i, T) = Q(i, T) \union growth(S)
				T->addQ_iT(growth(S));
			}

			// 이제 Q_iT의 lv은 i-2가 아니라 i가 됨.
			// 앞의 equivalence class와 다르지만 같은 변수명(temp, EC, Qs) 사용
			// compute the equivalence classes of Q(i, T) by plane sweep
			temp = equiv_classes(T->getQ_iT());
			EC = temp.first;  Qs = temp.second; // Q_iT is a subset of the Qs just above.

			// perform steps 3 and 4 of build-subdivision on Q(i, T).

			// (* 3. Process simple components of ≡_i-2 that are about to merge with some other components. *)
			for (auto q : T->getQ_old()) {
				// at this moment, q->G is correctly computed.
				if (about_to_merge(q)) { // *** need to check once more ***
					makeDCEL(quad2RP(q));
				} 
			}
			
			// (* 4. Process complex components. *)
			vector<vector<int>> equi_classes = get<0>(equiv_classes(Q_i));
			for (auto ec : equi_classes) {
				vector<i_quad*> Qs = {};
				for (auto i : ec) { // insert each element in each equivalence class, given with an index in Q_i, into Qs.
					Qs.push_back(Q_i[i]);
				}

			RP* R1 = Union(Qs, 2);
			DCEL* D1 = makeDCEL(R1);
			DCEL* temp = new DCEL(D->merge(*D1));
			D = temp;
			RP* R2 = Union(Qs, 4);
			DCEL* D2 = makeDCEL(R2);
			DCEL* temp2 = new DCEL(D->merge(*D2));
			D = temp2;

			// rectilinear polygon

			// Draw (i-2)-boxes to fill the region between the boundaries of R1 and R2.
			// Draw i-boxes to ll the region between the boundaries of R1 and S; 
			// break each cell boundary with an endpoint incident to R1 into four edges of length 2^(i-2), to satisfy Invariant 1.
			}

			// If |Q(i, T)| = 1 then delete T from N.
			if (T->getQ_iT().size() == 1) {
				auto it = N.find(T->getIndex());
				if (it != N.end()) { N.erase(it); }
				else { throw NULL; }
			}
		}

		Q_old.resize(Q_i.size());
		std::copy(Q_i.begin(), Q_i.end(), Q_old.begin());
	}

	return D;
}

DCEL* conforming_subdivision::build_ls_subdivision(DCEL* D) { // vertex conforming subdivision as input
	DCEL* S = new DCEL;
	
	// merge DCELs made from each obstacle 
	for (auto ob : obstacles) {
		vector<Vertex*> vers;
		for (auto pt : ob) {
			Vertex* now = new Vertex(*pt);
			vers.push_back(now);
		}
		DCEL* S1 = makeDCEL(vers, false);
		DCEL* temp = new DCEL(S->merge(*S1));
		S = temp;
	}

	vector<HEdge*> vec;
	vector<Vertex*> vec2;

	auto LC1 = Location(S);
	DCEL* S2 = new DCEL; DCEL* S3 = new DCEL;
	for (auto ob : obstacles) {
		for (auto pt : ob) {
			bool check = true; // determines when to stop
			Vertex* now = D->getVertex("v_" + std::to_string(pt->index));
			// conforming vertex 
			vec2.push_back(now);

			//check result of point location query
			while (check) {
				// traverse around 'now'
				vector<HEdge*> es = S->getOutgoingHEdges(now);
				for (HEdge* e : es) {
					Vertex* v = e->getTwin()->getOrigin();
					Face* F = LC1.locate(v);

					if (!F->isOutMost()) {
						for (auto ed : F->getInnerHEdges()) {
							Point* temp = ed->crossing(*e);
							Vertex* inter_p = new Vertex(*temp);
							HEdge* short_e = new HEdge(now, inter_p);
							vec.push_back(short_e);
							vec2.push_back(inter_p);
							check = false;
						}
					}
					else {
						vec.push_back(e);
						now = v;
					}
				}
			}
		}
	}
	S2->setHedges(vec);
	S2->setFaces(ConstructFaces(vec));
	S2->setVertices(vec2);


	//update S3;
	DCEL* temp = new DCEL(S->merge(*S2));
	S = temp;

	auto LC2 = Location(D);

	vector<HEdge*>().swap(vec);
	vector<Vertex*>().swap(vec2);

	// 각 obstacle vertex에 대해서 찾아 주고, 기존 cell에 포함돼 있다면 위/아래로 ray shooting 했을 때의 edge를 찾아야 함
	for (auto ob : obstacles) {
		for (auto pt : ob) {
			Vertex* now = D->getVertex("v_" + std::to_string(pt->index));
			vec2.push_back(now);
			Face* F = LC2.locate(now);

			Vertex* top_v = new Vertex(now->getx(), DBL_MAX);
			Vertex* bot_v = new Vertex(now->getx(), DBL_MIN);
			HEdge* top_e = new HEdge(now, top_v);
			HEdge* bot_e = new HEdge(now, bot_v);

			// for each edge
			for (auto e : F->getInnerHEdges()) {	
				Point* temp1 = e->crossing(*top_e);
				Point* temp2 = e->crossing(*bot_e);

				if (temp1 != NULL) {
					Vertex* v1 = new Vertex(*temp1);
					vec2.push_back(v1);
					vec.push_back(new HEdge(now, v1));
				}

				if (temp2 != NULL) {
					Vertex* v2 = new Vertex(*temp2);
					vec2.push_back(v2);
					vec.push_back(new HEdge(now, v2));
				}
			}
		}
	}
	S3->setHedges(vec);
	S3->setFaces(ConstructFaces(vec));
	S3->setVertices(vec2);


	DCEL* temp2 = new DCEL(S->merge(*S3));
	S = temp2;

	return S;
}

void conforming_subdivision::propagation(DCEL* D) { // ls_subdivision as input

	vector<HEdge*> he_vec;
	
	int te_num = 0;
	for (auto e : D->getHedges()) { // count # of transparent edges
		if (e->type) {
			te_num += 1;
			he_vec.push_back(e);
		}
	}

	// vector<double> c_times; c_times.assign(te_num, -1);
	int ori_num = te_num;

	while (te_num > 0) {// there is an unprocessed transparent edge
		double min_ct = DBL_MAX;
		int e_i = -1;
		for (int i = 0; i < ori_num; i++) {
			if (min_ct > he_vec[i]->covertime) min_ct = he_vec[i]->covertime;
			e_i = i;
		} // e_i is the hedge index with the minimum covertime

		auto e = he_vec[e_i];

		
		compute_aw(e); // compute the approximate wavefronts at e
		compute_sdist(e->getOrigin()); compute_sdist(e->getTwin()->getOrigin()); // compute d(v, s) for each endpoint v of e

		for (auto g : e->output) {
			double t_g = compute_eg() // computes the time s.t. the approximate wavefront from e first engulfs an endpoint of g
			g->covertime = min(g->covertime, t_g + g->length);
				
		}

		// compute approximate wavefronts at e based on the 
		for (auto te : he_vec) {
			
		}
		// minimum
		// select
	}
}

void conforming_subdivision::compute_aw(HEdge* e) { // compute approximate wavefront
	vector<HEdge*> vec;

	for (auto f : e->input) {
		if (f->covertime < e->covertime) vec.push_back(f);
	}

	// need to implement later
}

void conforming_subdivision::compute_sdist(Vertex* v) { // compute d(v,s), for source point s, exactly for an endpoint v of an edge e

}

// computes the time s.t. the approximate wavefront from e first engulfs an endpoint of g
double conforming_subdivision::compute_eg(Vertex*) { 

}

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
	// 루트 노드는 부모 노드 번호로 자기 자신을 가진다.
	if (root[x] == x) {
		return x;
	}
	else {
		// "경로 압축(Path Compression)"
		// find 하면서 만난 모든 값의 부모 노드를 root로 만든다.
		return root[x] = my_find(root[x], root, count);
	}
}

/* union1(x, y): union-by-rank 최적화 */
int my_union(int x, int y, int* root, int* rank, int count) {
	x = my_find(x, root, count);
	y = my_find(y, root, count);

	// 두 값의 root가 같으면(이미 같은 트리) 합치지 않는다.
	if (x == y)
		return -1; // debug 필요. 맞나?

	// "union-by-rank 최적화"
	// 항상 높이가 더 낮은 트리를 높이가 높은 트리 밑에 넣는다. 즉, 높이가 더 높은 쪽을 root로 삼음
	if (rank[x] < rank[y]) {
		root[x] = y; // x의 root를 y로 변경
		return y; // return the new root
	}
	else {
		root[y] = x; // y의 root를 x로 변경

		if (rank[x] == rank[y])
			rank[x]++; // 만약 높이가 같다면 합친 후 (x의 높이 + 1)

		return x; // return the new root
	}
}

double my_log2(double x) { return log(x) / log(2); }





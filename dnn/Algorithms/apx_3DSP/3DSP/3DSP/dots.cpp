#include <cmath>
#include <algorithm>
#include <iostream>
#include <float.h>
#include <vector>
#include "dots.h"
#include "propagation.h"

MyVec operator*(const double a, const MyVec& V)
{
	return MyVec(a * V.x, a * V.y, a * V.z);
}

double VecSize(MyVec V) {
	return sqrt(pow(V.getx(), 2) + pow(V.gety(), 2) + pow(V.getz(), 2));
}

double PointsDist(Point P1, Point P2) {
	return sqrt(pow(P1.getx() - P2.getx(), 2) + pow(P1.gety() - P2.gety(), 2) + pow(P1.getz() - P2.getz(), 2));
}

double InnerProd(MyVec V1, MyVec V2) {
	return V1.getx() * V2.getx() + V1.gety() * V2.gety() + V1.getz() * V2.getz();
}

MyVec Point2Vec(Point P1) {
	return MyVec{P1.getx(), P1.gety(), P1.getz(), P1.getindex()};
}

Point Vec2Point(MyVec V1) {
	return Point{ V1.getx(), V1.gety(), V1.getz(), V1.getindex() };
}

MyVec Points2Vec(Point P1, Point P2) {
	return MyVec(P2.getx() - P1.getx(), P2.gety() - P1.gety(), P2.getz() - P1.getz());
}

MyVec OuterProd(MyVec V1, MyVec V2) {
	double a1 = V1.getx(), a2 = V1.gety(), a3 = V1.getz(), b1 = V2.getx(), b2 = V2.gety(), b3 = V2.getz();
	return MyVec{ a2 * b3 - a3 * b2, -(a1 * b3 - a3 * b1), a1 * b2 - a2 * b1 };
}

double PlaneAngle(Plane PL1, Plane PL2) {
	MyVec V1{ PL1.geta(), PL1.getb(), PL1.getc() }, V2{ PL2.geta(), PL2.getb(), PL2.getc() };
	return abs(InnerProd(V1, V2)) / (VecSize(V1) * VecSize(V2));
}

double PointSegDist(Point P0p, Segment S) {
	Point P1 = Vec2Point(S.geta());
	Point P2 = Vec2Point(S.getb());

	MyVec R = OuterProd(OuterProd(Points2Vec(P0p, P2), Points2Vec(P0p, P1)), Points2Vec(P1, P2));
	double cos_gamma = InnerProd(Points2Vec(P0p, P1), R) / (PointsDist(P0p, P1) * VecSize(R));
	double P0p_P0pp_len = PointsDist(P0p, P1) * cos_gamma;
	MyVec P0p_P0pp = R * (P0p_P0pp_len) / VecSize(R);

	Point P0pp = P0p.operator+(P0p_P0pp);

	double t = VecSize(Points2Vec(P0pp, P1)) / VecSize(Points2Vec(P2, P1));

	if (0 <= t && t <= 1) { return P0p_P0pp_len; } // P0pp is between P1 and P2
	else if (t < 0) { return 2; } // P0 is closest to P1
	else { return 3; } // P0 is closest to P2

}

// 점과 triangle 사이의 거리. reference : 3D Distance from a Point to a Triangle
double PolyDomain::FacePointDist(Tri T, Point P0) {
	int p1 = T.getPoint(1), p2 = T.getPoint(2), p3 = T.getPoint(3);
	Point P1 = pts[p1], P2 = pts[p2], P3 = pts[p3];
	MyVec P1_P2 = Points2Vec(P1, P2), P1_P3 = Points2Vec(P1, P3);
	MyVec N_P = OuterProd(P1_P2, P1_P3);
	double cos_alpha = InnerProd(Points2Vec(P1, P0), N_P) / (PointsDist(P1, P0) * VecSize(N_P));
	double P0_P0p_len = PointsDist(P0, P1) * cos_alpha;
	MyVec P0_P0p = N_P * (-1 * P0_P0p_len / VecSize(N_P));
	Point P0p = P0 + P0_P0p;

	MyVec temp = Points2Vec(P3, P1) / PointsDist(P3, P1);
	MyVec V1 = MyVec(Points2Vec(P2, P1) / PointsDist(P2, P1)).operator+(temp);
	MyVec temp2 = Points2Vec(P1, P2) / PointsDist(P1, P2);
	MyVec V2 = MyVec(Points2Vec(P3, P2) / PointsDist(P3, P2)).operator+(temp2);
	MyVec temp3 = Points2Vec(P2, P3) / PointsDist(P2, P3);
	MyVec V3 = MyVec(Points2Vec(P1, P3) / PointsDist(P1, P3)).operator+(temp3);

	double f1 = InnerProd(OuterProd(V1, Points2Vec(P1, P0p)), N_P);
	double f2 = InnerProd(OuterProd(V2, Points2Vec(P2, P0p)), N_P);
	double f3 = InnerProd(OuterProd(V3, Points2Vec(P3, P0p)), N_P);

	int outside;

	if (f1 > 0 && f2 < 0) {
		if (InnerProd(OuterProd(Points2Vec(P0p, P1), Points2Vec(P0p, P2)), N_P)) { outside = 1; }
		else { outside = -1; }
	}
	else if (f2 > 0 && f3 < 0) {
		if (InnerProd(OuterProd(Points2Vec(P0p, P2), Points2Vec(P0p, P3)), N_P)) { outside = 2; }
		else { outside = -1; }
	}
	else {
		if (InnerProd(OuterProd(Points2Vec(P0p, P3), Points2Vec(P0p, P1)), N_P)) { outside = 3; }
		else { outside = -1; }
	}

	Segment S;
	switch (outside) {
	case -1:
		return abs(P0_P0p_len);
	case 1:
		S.seta(Point2Vec(P1)), S.setb(Point2Vec(P2));
	case 2:
		S.seta(Point2Vec(P2)), S.setb(Point2Vec(P3));
	case 3:
		S.seta(Point2Vec(P3)), S.setb(Point2Vec(P1));
	}

	double PointSegRelation = PointSegDist(P0p, S);
	if (PointSegRelation == 1) { // t < 0 in PointSegDistance()
		return VecSize(Points2Vec(Vec2Point(S.geta()), P0));
	}
	else if (PointSegRelation == 2) { // t > 1 in PointSegDistance()
		return VecSize(Points2Vec(Vec2Point(S.getb()), P0));
	}
	else //  0 <= t <= 1 in PointSegDistance()
	{
		return sqrt(pow(PointSegRelation, 2) + pow(P0_P0p_len, 2));
	}

	// still need to handle degenerate cases

}

Point PolyDomain::incircle_center(Tri T) {

	// 3차원에서 두 직선이 이루는 평면.
	// general position assumption for points in 'tri' : no two points have the same x-coordinate.
	int p1 = T.getPoint(1), p2 = T.getPoint(2), p3 = T.getPoint(3);
	Point A = pts[p1], B = pts[p2], C = pts[p3];

	// MyVec BA = {A.x - B.x, A.y - B.y, A.z - B.z}; // A - B
	// MyVec BC = {C.x - B.x, C.y - B.y, C.z - B.z };

	// ABC_sec (bisector) = 선분 BA + 선분 BC
	MyVec ABC_sec = { (A.getx() - B.getx()) + (C.getx() - B.getx()), (A.gety() - B.gety()) + (C.gety() - B.gety()),
	(A.getz() - B.getz()) + (C.getz() - B.getz()) };

	// MyVec CA = { A.x - C.x, A.y - C.y, A.z - C.z };
	// MyVec CB = { B.x - C.x, B.y - C.y, B.z - C.z };

	// BCA_sec (bisector) = 선분 CA + 선분 CB
	MyVec BCA_sec = { (A.getx() - C.getx()) + (B.getx() - C.getx()), (A.gety() - C.gety()) + (B.gety() - C.gety()),
	(A.getz() - C.getz()) + (B.getz() - C.getz()) };

	// 직선 
	double x1 = B.getx(), a1 = ABC_sec.getx(), y1 = B.gety(), b1 = ABC_sec.gety(), z1 = B.getz(), c1 = ABC_sec.getz();
	double x2 = C.getx(), a2 = BCA_sec.getx(), y2 = C.gety(), b2 = BCA_sec.gety(), z2 = C.getz(), c2 = BCA_sec.getz();
	double t = (x2 - x1) / (a1 - a2);

	return Point(a1 * t + x1, b1 * t + y1, c1 * t + z1);
}

void PolyDomain::init_neigh_sgs() {// initialize neighboring segments
	vector<int> NFI = { 0, 3, 0, 2, 3, 2, 0, 1, 3, 1, 2, 1}; // neighboring face indices
	// cout << neigh_face_indices.size() << endl;

	for (int j = 0; j < tets.size(); j++) {
	    Tetra& T = tets[j];
		vector<tuple<int, int, int>> temp = {};
		for (int i = 0; i < NFI.size(); i++) {
			// cout << t1.get_index() << " " << t2.get_index() << endl;
			// cout << T.getindex() << " " << T.getTri(neigh_face_indices[i]) << " " << fcs[T.getTri(neigh_face_indices[i])].get_index() << endl;
			
			// int ttt = T.getTri(NFI[i]);
			temp.push_back(make_tuple(j, i / 2, fcs[T.getTri(NFI[i])].get_index()));
			if (i % 2 == 1) {
				bi* B = new bi{ fcs[T.getTri(NFI[i - 1])].get_index(), fcs[T.getTri(NFI[i])].get_index(), temp, j, i / 2 };
				T.add_bis(B);
				temp.clear();
			}
		}
	}

	// vector<vector<bool>> adj_M(tets.size(), vector<bool>(tets.size(), false));

	for (int j = 0; j < tets.size(); j++) {
		Tetra& Tj = tets[j]; // index j
		vector<int> Tj_itets = Tj.get_itets();
		for (int temp = 0; temp < Tj_itets.size(); temp++) {
			int i = Tj_itets[temp]; // index i
			Tetra& Ti = tets[i];
			for (int k = 0; k < 6; k++) {
				for (int m = 0; m < 6; m++) {
					int cmp_num = -1;
					if (Tj.get_bi(k)->f1 == Ti.get_bi(m)->f1) {cmp_num = Tj.get_bi(k)->f1;}
					else if (Tj.get_bi(k)->f1 == Ti.get_bi(m)->f2) { cmp_num = Tj.get_bi(k)->f1; }
					else if (Tj.get_bi(k)->f2 == Ti.get_bi(m)->f1) { cmp_num = Tj.get_bi(k)->f2; }
					else if (Tj.get_bi(k)->f2 == Ti.get_bi(m)->f2) { cmp_num = Tj.get_bi(k)->f2; }
					else {continue;}
					Tj.update_bi(k, make_tuple(i, m, cmp_num)); // Ti.update_bi(m, make_tuple(j, k, cmp_num)); // adj_M[j][i] = true; adj_M[i][j] = true;
				}
			}
		}
	}
}

// all the 'dist' functions compute d(x) for a given point x, 
// the minimum euclidean distance from x to any point on the boundary of D(x).

double PolyDomain::dist(int pt_num) { // when the input point is a vertex

	Point P = pts[pt_num];

	vector<Tri> Tris;

	vector<int> P_ifcs = P.get_ifcs();

	for (int i : P.get_itets()) {
		for (int j : tets[i].get_fcs()) { // 이래도 되나
			// the face is not incident to P
			if (std::find(P_ifcs.begin(), P_ifcs.end(), j) == P_ifcs.end()) {
				Tris.push_back(fcs[j]);
			}
			// the face is incident to P
			else {}
		}
	}

	double val = DBL_MAX;
	for (Tri T : Tris) {
		double temp = FacePointDist(T, P);
		if (val > temp) { val = temp; }
	}

	return val;
}

// when the input point is not a vertex (it either lies on the interior of a segment or on the interior of a face)
double PolyDomain::dist(Point P, int num, bool onSeg) {

	if (onSeg) {
		Segment* S = sgs[num];
		// Segment S = *(PD.get_sg(num));

		vector<int> S_ifcs = S->get_ifcs();

		double val = DBL_MAX;
		for (int i : S->get_itets()) {
			if (std::find(S_ifcs.begin(), S_ifcs.end(), i) == S_ifcs.end()) {
				double temp = FacePointDist(fcs[i], P);
				if (val > temp) { val = temp; }
			}
		}

		return val;
	}
	else {
		double val = DBL_MAX;
		for (int i : tets[num].get_fcs()) {
			double temp = FacePointDist(fcs[i], P);
			if (val > temp) { val = temp; }
		}
		return val;
	}
}

double PolyDomain::radius(int pt_num) {
	return dist(pt_num) / 14;
}

double PolyDomain::radius(Point P, int sg_num, bool onSeg) {
	if (onSeg) { return dist(P, sg_num, true) / 24; }
	else { return dist(P, sg_num, false) / 24; }
}

double* PolyDomain::radius_e(Segment S) { // radius of an edge in the polygonal domain

	vector<double> As = {};
	vector<double> Bs = {};

	for (int i : S.get_itets()) { // for each tetrahedron that is incident to the segment
		for (int j : tets[i].get_fcs()) { // among the faces that are incident to the tetrahedron 
			// [c = b-a] for slope

			vector<int> vec = S.get_ifcs();
			if (std::find(vec.begin(), vec.end(), j) == vec.end()) { // if the face is not incident to the segment
				As.push_back(FacePointDist(fcs[j], Vec2Point(S.geta()))); // calculate the distance between the face and the left endpoint of the segment
				Bs.push_back(FacePointDist(fcs[j], Vec2Point(S.getb())));
			}


		}
	}

	vector<double> lines_inter = { 0, 1 }; // set of intersection points. 0 and 1 correspond to the endpoints of the segment
	for (unsigned int i = 0; i < As.size(); i++) {
		for (unsigned int j = i + 1; j < As.size(); j++) {
			if (As[i] > Bs[i] && As[i + 1] < Bs[i + 1] || As[i] < Bs[i] && As[i + 1] > Bs[i + 1]) {
				lines_inter.push_back((As[i + 1] - As[i]) / ((Bs[i] - As[i]) - (Bs[i + 1] - As[i + 1])));
			}
		}
	}

	double res_inter = -1;
	double res = DBL_MIN;
	for (double inter : lines_inter) {
		double temp = DBL_MAX;
		for (int i = 0; i < As.size(); i++) {
			double val = (Bs[i] - As[i]) * inter + As[i];
			if (temp > val) { temp = val; }
		}
		if (res < temp) { res = temp; res_inter = inter; }
	}

	res /= 24;
	double a_rad = radius(S.geta_ind()), b_rad = radius(S.getb_ind());
	if (res < a_rad) { res = a_rad; res_inter = 0; }
	if (res < b_rad) { res = b_rad; res_inter = 1; }

	double* ans = new double[2];
	ans[0] = res_inter;
	ans[1] = res;

	return ans;
}

void PolyDomain::MarkPoints_Bi(int i, int j) {
	Tetra& T = tets[i];
	int a = T.getPoint(perm[j][0]), b = T.getPoint(perm[j][1]), c = T.getPoint(perm[j][2]), d = T.getPoint(perm[j][3]); // perm is defined in dots.h
	Point A = pts[a]; Point B = pts[b]; Point C = pts[c]; Point D = pts[d];
	// Segment S = *(PD.get_sg(sg_num)); // represents the segment AB

	Point P = (C + D) / 2;

	Plane PL1(A, B, C), PL2(A, B, D);

	double e = sqrt(eps / 8) * sin(acos(PlaneAngle(PL1, PL2)) / 2);

	Point H = perpen(A, B, P);
	double dist_PH = PointsDist(P, H);
	double dist_AB = PointsDist(A, B);

	vector<Segment*> ans = {};

	vector<double> Pi_s = { e / (1.0 + e) };
	vector<int> ki_s = {};

	// int s_num = 0; // for test
	// int i1 = -1, i2p = -1, i2pp = -1;
	// bool i1_set = false;

	double rad_a = radius(a);
	double rad_b = radius(b);

	int sg_num = tets[i].getSg(j); // the index of the segment AB in the polygonal domain
	double rad_Seg = radius_e(*(sgs[sg_num]))[1];


	// Point M2p = 
	// Point M2pp = 

	// while (ans.empty() || !(*ans.back()).getX().empty()) { // if ans.empty() = true, then it should be iterated
	// while (ans.empty() || (*ans.back()).sizeX() > 1) {
	while (1) {
		// s_num += 1; // for test
		// cout << s_num << endl; // for test

		// First calculate P_{i+1}
		double Pi = Pi_s.back();
		double Pip1 = Pi + (1 - Pi) * e / (1.0 + e);
		Pi_s.push_back(Pip1);

		// Then calculate k_{i}
		// int kii = int(ceil((dist_AB * Pi) / (dist_PH * (Pip1 - Pi))));
		int ki = max(3, int(ceil((dist_AB * Pi) / (dist_PH * (Pip1 - Pi)))) / 10); // for test
		ki_s.push_back(ki);

		Point Ai = P * (1 - Pi) + A * Pi;
		Point Bi = P * (1 - Pi) + B * Pi;

		// Segment S(Ai, Bi, {}, num_seg, false, &(tets[i]), i, j);
		num_seg++;
		Segment* S = new Segment(Ai, Bi, {}, num_seg - 1, false, &(tets[i]), i, j);

		vector<double> Xi = {};

		// cout << endl;
		// cout << endl;
		// cout << endl;
		// cout << dist_PH * (1 - Pi) << " " << rad_Seg << endl; // for test
		// cout << endl;

		// 같은 segment 위에서는 어떤 점이든 AB까지의 직선 거리가 같으므로

		// if (dist_PH * (1 - Pi) >= rad_Seg) {i1 = s_num;}
		// if (dist_PH * (1 - Pi) > ) {i2p = s_num;}
		// if () {i2pp = s_num;}

		// if (i1 == -1) {

		if (dist_PH * (1 - Pi) >= rad_Seg) {
			for (int i = 0; i < ki + 2; i++) {

				// Point test1 = (Bi - Ai); // for test
				// Point test2 = (Bi - Ai) * (i / (ki + 1)); // for test

				Point temp_pt = Ai + ((Bi - Ai) * (i / (ki + 1.0))); // a steiner point candidate (on the segment)

				// cout << PointsDist(temp_pt, A) << " " << radius(PD, a) << endl; // for test
				// cout << PointsDist(temp_pt, B) << " " << radius(PD, b) << endl; // for test
				// cout << endl; // for test

				if (PointsDist(temp_pt, A) >= radius(a) && PointsDist(temp_pt, B) >= radius(b)) {
					Xi.push_back(i / (ki + 1.0));
				}
			}
		}
		// else {	
		// }
		
		/*Debug Start
		if (!Xi.empty())
		{
			S->setX(Xi);
		    ans.push_back(S);
		}
		*/
		//Debug End

		if (!Xi.empty()) {
			S->set_itets({ i });
			S->setX(Xi);
			sgs.push_back(S);
			ans.push_back(S);
		}
		else {
			num_seg--;
			delete S;
			break;
		}
	}
	ans.push_back(sgs[T.getSg(j)]);
	bi* Bi = T.get_bi(j);
	Bi->bi_sgs = ans;

	// Bi.bi_sgs = ans;
	// for (int i = 0; i < ans.size(); i++) { T.get_bi(j).bi_sgs.push_back(ans[i]); }
}

void PolyDomain::MarkPoints_Sg(int i) {
	Segment* S = sgs[i];

	vector<double> X = {};

	double er_va = eps * radius(S->geta_ind());
	double er_vb = eps * radius(S->getb_ind());

	MyVec A = S->geta(); MyVec B = S->getb();
	double sz = (A - B).size();

	X.push_back(er_va / sz); // A'
	X.push_back(1 - er_vb / sz); // B'

	double value;

	value = radius_e(*S)[1];
	while (value > er_va / sz) {
		X.push_back(value);
		value -= eps * radius(Vec2Point(value * A + (1 - value) * B), S->getind(), true);
	}

	value = radius_e(*S)[1];
	while (value < 1 - er_vb / sz) {
		X.push_back(value);
		value += eps * radius(Vec2Point(value * A + (1 - value) * B), S->getind(), true);
	}

	sgs[S->getind()]->setX(X);
}

void PolyDomain::MarkPoints() {

	// for test
	// int pt_num = 0;
	for (int i = 0; i < sgs.size(); i++) {
		MarkPoints_Sg(i);
	}

	for (int i = 0; i < tets.size(); i++) {
		Tetra Tet = tets[i];

		for (int j = 0; j < 6; j++) {
			// bi* B = Tet.get_bi(j); // the j-th bisector of the i-th tetrahedron
			MarkPoints_Bi(i, j);
		}
	}
}

bool sort_S(const pair<Segment*, int>& a, const pair<Segment*, int>& b) {
	return (get<0>(a)->getind() < get<0>(b)->getind());
}

bool sort_int(const pair<Segment*, int>& a, const pair<Segment*, int>& b) {
	return get<1>(a) < get<1>(b);
}

/*
bool sort_Adj(const pair<Segment*, int>& a, const pair<Segment*, int>& b) {
	if (get<0>(a)->getind() < get<0>(b)->getind() || (get<0>(a)->getind() == get<0>(b)->getind() || get<1>(a) < get<1>(b))) {return false;}
	else {return true;}
	// return ;
}
*/

vector<pair<Segment*, int>> Remove_Dup(vector<pair<Segment*, int>> ps) {
	sort(ps.begin(), ps.end(), sort_int);
	sort(ps.begin(), ps.end(), sort_S);
	Segment* prev_S = NULL; int prev_int;
	vector<pair<Segment*, int>>::iterator it = ps.begin();
	for (; it != ps.end(); it++) {
		while (prev_S == get<0>(*it) && prev_int == get<1>(*it)) { 
			it = ps.erase(it); 
		}
		prev_S = get<0>(*it); prev_int = get<1>(*it);
		
		/*
		if (prev_S == get<0>(*it) && prev_int == get<1>(*it)) {
			ps.erase(it); it--;
		}
		else {prev_S = get<0>(*it); prev_int = get<1>(*it);}
		*/
	}

	return ps;
}

int PolyDomain::Ln_Search(int i, pair<Segment*, int> pr) {
	int j = get<0>(pr)->getind();
	Segment* Sj = sgs[j];
	return Sj->Ln_Search(i, get<1>(pr));
}

void PolyDomain::SetSgs()
{
	for (Segment* S : sgs) {
		S->initDist();
		S->SetSbar();
		S->initVoronoi();
		S->SetAdjDiagram(*this);
		S->SetNear();
		S->initPrevs();
	}
}

bool PolyDomain::inTet(int i, Point p)
{
	MyVec v = Point2Vec(p);
	vector<MyVec> vecs;
	for (size_t j = 0;  j< 4; j++)
	{
		vecs.push_back(Point2Vec(pts[tets[i].getPoint(j)]));
	}
	for (size_t j = 0; j < 4; j++)
	{
		MyVec n = OuterProd(vecs[(j+1)%4] - vecs[j], vecs[(j+2)%4] - vecs[j]);
		if (InnerProd(vecs[(j + 3) % 4] - vecs[j], n) * InnerProd(v - vecs[j], n) < 0)
			return false;
	}
	return true;
}

void PolyDomain::ConnectSgs() {
	for (int i = 0; i < tets.size(); i++) {
		Tetra& T = tets[i];
		for (int j = 0; j < 6; j++) {
			bi* B = T.get_bi(j);
			for (Segment* S : B->bi_sgs) {

				vector<pair<Segment*, int>> temp = {};

				for (auto NB : B->neighbors) { // tuple<int, int, int>
					bi* NBB = tets[get<0>(NB)].get_bi(get<1>(NB));
					for (Segment* NBS : NBB->bi_sgs) {
						temp.push_back(make_pair(NBS, get<2>(NB)));
					}
				}
				
				temp = Remove_Dup(temp);

				for (auto t : temp) {
					// S.AddAdjs(get<0>(t), get<1>(t));
					S->AddAdj(t);
				}
				
			}
			
		}
	}

	// set Rev for each Segment. O(m^3) time algorithm, where m is the number of Segments (including steiner segments)
	for (int i = 0; i < sgs.size(); i++) {
		Segment* Si = sgs[i];
		for (int j = 0; j < Si->getAdjSize(); j++) {
			pair<Segment*, int> pr = Si->getAdj(j);
			
			Si->AddRev(Ln_Search(i, pr)); // linear search to find inverse index
		}
	}
}

void PolyDomain::CountEmptySegs() {
	int num = 0;
	for (int i = 0; i < sgs.size(); i++) {
		Segment* S = sgs[i];
		if (S->sizeX() == 0) {
			num += 1;
			std::cout << i << " ";
		}
	}
	std::cout << "\n";
	std::cout << num << endl;
}

// vertical projection of P on AB
// https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=stkov&logNo=90019263178
Point perpen(Point A, Point B, Point P) {
	MyVec a = Point2Vec(P);
	MyVec d = Points2Vec(A, B);
	MyVec p = Point2Vec(A);

	MyVec ans = p + d * InnerProd(a - p, d) / pow(d.size(), 2);
	return Vec2Point(ans);
}

/*
int main() {

	
	Point P1{ -3.0, 0.0, 0.0 }, P2{ 0.0, -3.0, 0.0 }, P3{ 2.0, 2.0, 0.0 }, P4{ -2.0, 0.3, 1.0 }, P5{ 1.0, 1.0, -0.3 };
	Tri T1{ P1, P2, P3 }, T2{ P1, P2, P4 }, T3{ P2, P3, P4 }, T4{ P3, P1, P4 };
	Tri T5{ P1, P2, P5 }, T6{ P2, P3, P5 }, T7{ P3, P1, P5 };

	// PointInsideTri(T1, Point{ -1.6, -1.6, 0.0 });
	// PointInsideTri(T1, Point{ -10.0, -10.0, 0.0 });

	Tetra Tet1{ T1, T2, T3, T4 }, Tet2{ T1, T5, T6, T7 };
	Point c = TetraCenter(Tet1, Tet2, T1);
	c.print();
	

	// Point center = tetra_center(Tet1, Tet2, T, 0);
}
*/


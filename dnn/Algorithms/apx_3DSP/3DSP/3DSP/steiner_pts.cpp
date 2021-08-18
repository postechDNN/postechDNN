#include <vector>
#include <cmath>
#include <algorithm>
#include <float.h>

#include "dots.h"
#include "propagation.h"

// given a point x, compute d(x), the minimum euclidean distance from x to any point on the boundary of D(x)
double dist(PolyDomain PD, Point P, bool isVertex) {

	if (isVertex) {
		vector<Tri> Tris;

		for (int i : P.get_incid_tets()) {
			for (int j : PD.get_tets()[i].get_fcs()) { // 이래도 되나
				// the face is not incident to P
				if (std::find(P.get_incid_fcs().begin(), P.get_incid_fcs().end(), j) != P.get_incid_fcs().end()) {
					Tris.push_back(PD.get_faces()[j]);
				}
				// the face is incident to P
				else {}
			}
		}

		double val = DBL_MAX;
		for (Tri T : Tris) {
			double temp = FacePointDist(T, P);
			if (val < temp) { val = temp; }
		}

		return val;
	}

	else {
		double val = DBL_MAX;
		for (int i : (PD.get_tets()[P.get_sur_tet()]).get_fcs()) {
			double temp = FacePointDist(PD.get_faces()[i], P);
			if (val < temp) { val = temp; }
		}

		return val;
	}
}

double radius(PolyDomain PD, Point P, bool isVertex) {
	if (isVertex) { return dist(PD, P, true) / 14; }
	else { return dist(PD, P, false) / 24; }
}

double* radius_e(PolyDomain PD, Segment S) {
	// dist_a, dist_b
	double a = DBL_MAX; double b = DBL_MAX;
	for (int i : S.get_incid_tets()) {
		for (int j : PD.get_tets()[i].get_fcs()) {
			vector<int> vec = S.get_incid_fcs();
			if (std::find(vec.begin(), vec.end(), j) != vec.end()) {
				if (FacePointDist(PD.get_faces()[j], Vec2Point(S.geta())) == 0) {
					double temp = FacePointDist(PD.get_faces()[j], Vec2Point(S.getb()));
					if (temp < b) { b = temp; }
				}
				else {
					double temp = FacePointDist(PD.get_faces()[j], Vec2Point(S.geta()));
					if (temp < a) { a = temp; }
				}
			}
		}
	}

	// r_e, dMe, a, b
	double ans[4] = {a * b / (a + b) * 24, a / (a + b), a, b};

	return ans;
}

// vertical projection of P on AB
// https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=stkov&logNo=90019263178
Point perpen(Point A, Point B, Point P) {
	MyVec a = Point2Vec(P);
	MyVec d = Points2Vec(A, B);
	MyVec p = Point2Vec(A);

	MyVec ans = p + d * InnerProd(a - p, d) / pow(d.size(),2);
	return Vec2Point(ans);
}

vector<Segment> MarkPoints(PolyDomain PD, Point A, Point B, Point C, Point D, int Tet_index, int sg_num) {
	Point P = (C + D) / 2;

	Plane PL1(A, B, C), PL2(A, B, D);

	double c = sqrt(EPS / 8) * sin(acos(PlaneAngle(PL1, PL2)) / 2);

	Point H = perpen(A, B, P);
	double dist_PH = PointsDist(P, H);
	double dist_AB = PointsDist(A, B);

	vector<Segment> ans = {};

	vector<double> Pi_s = { c/(1+c) };
	vector<int> ki_s = {};
	
	while (ans.empty() || !ans.back().getX().empty()) {

		// First calculate P_{i+1}
		double Pi = Pi_s.back();
		double Pip1 = Pi + (1 - Pi) * c / (1 + c);
		Pi_s.push_back(Pip1);

		// Then calculate k_{i}
		int ki = int(ceil((dist_AB * Pi) / (dist_PH * (Pip1 - Pi))));
		ki_s.push_back(ki);

		Point Ai = P * Pi + A * (1 - Pi);
		Point Bi = P * Pi + B * (1 - Pi);

		vector<double> Xi = {};

		for (int i = 0; i < ki+2; i++) {
			Point temp_pt = A + (B-A) * (i / (ki+1));

			Segment S(A, B, {});
			S.set_incid_tets({Tet_index});

			if (PointsDist(temp_pt, A) >= radius(PD, A, true) && PointsDist(temp_pt, B) >= radius(PD, B, true)
				&& dist_PH * (1 - Pi) >= radius(PD, perpen(Ai, Bi, temp_pt), false)) {
				Xi.push_back(i / (ki+1));
			}

			Point Pi_pt = P + (H - P) * Pi;

			Point inter_perpen, inter;

			// PointsDist(temp_pt, A) == radius(PD, A, true)이면 Ai로서 이미 steiner point로 추가됨
			if (PointsDist(temp_pt, A) < radius(PD, A, true)) {
				// intersection point
				
				inter_perpen = A + (B-A) * sqrt(pow(radius(PD, A, true), 2) - 
									 pow(PointsDist(Pi_pt, H), 2))/ Points2Vec(A, B).size();
				inter = perpen(Ai, Bi, inter_perpen);
				Xi.push_back(PointsDist(inter, Ai) / PointsDist(Ai, Bi));
			}
			
			if (PointsDist(temp_pt, B) < radius(PD, B, true)) {
				inter_perpen = B + (A-B) * sqrt(pow(radius(PD, B, true), 2) -
									 pow(PointsDist(Pi_pt, H), 2)) / Points2Vec(A, B).size();
				inter = perpen(Ai, Bi, inter_perpen);
				Xi.push_back(PointsDist(inter, Ai) / PointsDist(Ai, Bi));
			}

			double* r_e = radius_e(PD, *(PD.get_edge(sg_num)));
			if (r_e[0] == PointsDist(H, Pi_pt)) {
				inter_perpen = A + (B-A) * r_e[1];
				inter = perpen(Ai, Bi, inter_perpen);
				Xi.push_back(PointsDist(inter, Ai) / PointsDist(Ai, Bi));
			}
			else if (r_e[0] > PointsDist(H, Pi_pt)) {
				inter_perpen = B + (A-B) * (PointsDist(Pi_pt, H) / r_e[2]);
				inter = perpen(Ai, Bi, inter_perpen);
				Xi.push_back(PointsDist(inter, Ai) / PointsDist(Ai, Bi));

				inter_perpen = A + (B-A) * (PointsDist(Pi_pt, H) / r_e[3]);
				inter = perpen(Ai, Bi, inter_perpen);
				Xi.push_back(PointsDist(inter, Ai) / PointsDist(Ai, Bi));
			}
		}

		ans.push_back(Segment(Ai, Bi, Xi));
	}
	return ans;
}

vector<Segment> MarkPoints(PolyDomain PD) {
	vector<double> X;
	
	for (Segment S : PD.get_edges()) {
		MyVec A = S.geta(); MyVec B = S.getb();
		double er_va = EPS * radius(PD, Vec2Point(A), true);
		double er_vb = radius(PD, Vec2Point(B), true);
		double sz = (A - B).size();

		X.push_back(er_va / sz); // A'
		X.push_back(1 - er_vb / sz); // B'

		double value;
		
		value = radius_e(PD, S)[1];
		while (value > er_va / sz) {
			X.push_back(value);
			value -= EPS * radius(PD, Vec2Point(value * A + (1 - value) * B), false);
		}

		value = radius_e(PD, S)[1];
		while (value < 1 - er_vb / sz) {
			X.push_back(value);
			value += EPS * radius(PD, Vec2Point(value * A + (1 - value) * B), false);
		}
	}

	vector<Segment> ans = {};

	for (Tetra Tet : PD.get_tets()) {
		
		vector<Point> pts = {Tet.getp1(), Tet.getp2(), Tet.getp3(), Tet.getp4()};
		vector<vector<int>> perm = {{0,1,2,3}, {0,2,1,3}, {0,3,1,2}, {1,2,0,3}, {1,3,0,2}, {2,3,0,1}};

		vector<Segment> temp;
		int sg_num = 0;
		for (vector<int> seq : perm) {
			temp = MarkPoints(PD, pts[seq[0]], pts[seq[1]], pts[seq[2]], pts[seq[3]], Tet.getindex(), sg_num);
			ans.insert(ans.end(), temp.begin(), temp.end());
			sg_num += 1;
		}
	} 

	return ans;
}


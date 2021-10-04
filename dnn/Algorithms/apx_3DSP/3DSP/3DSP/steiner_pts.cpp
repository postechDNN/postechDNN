#include <vector>
#include <cmath>
#include <algorithm>
#include <float.h>

#include "dots.h"
#include "propagation.h"

// all the 'dist' functions compute d(x) for a given point x, the minimum euclidean distance from x to any point on the boundary of D(x).

// when the input point is a vertex
double dist(PolyDomain PD, int pt_num) { 

	Point P = PD.get_pt(pt_num);

	vector<Tri> Tris;

	vector<int> P_ifcs = P.get_ifcs();

	for (int i : P.get_itets()) {
		for (int j : PD.get_tet(i).get_fcs()) { // 이래도 되나
			// the face is not incident to P
			if (std::find(P_ifcs.begin(), P_ifcs.end(), j) == P_ifcs.end()) {
				Tris.push_back(PD.get_fc(j));
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

// when the input point is not a vertex (it either lies on the interior of a segment or not)
double dist(PolyDomain PD, Point P, int num, bool onSeg) { 

	if (onSeg) {
		Segment S = *(PD.get_sg(num));

		vector<int> S_ifcs = S.get_ifcs();

		double val = DBL_MAX;
		for (int i : S.get_itets()) {
			if (std::find(S_ifcs.begin(), S_ifcs.end(), i) == S_ifcs.end()) {
				double temp = FacePointDist(PD.get_fc(i), P);
				if (val > temp) { val = temp; }
			}
		}

		return val;
	}
	else {
		double val = DBL_MAX;
		for (int i : PD.get_tet(num).get_fcs()) {
			double temp = FacePointDist(PD.get_fc(i), P);
			if (val > temp) { val = temp; }
		}
		return val;
	}
}

double radius(PolyDomain PD, int pt_num) {
	return dist(PD, pt_num) / 14;
}

double radius(PolyDomain PD, Point P, int sg_num, bool onSeg) {
	if (onSeg) { return dist(PD, P, sg_num, true) / 24; }
	else { return dist(PD, P, sg_num, false) / 24; }
}

double* radius_e(PolyDomain PD, Segment S) { // radius of an edge in the polygonal domain

	vector<double> As = {};
	vector<double> Bs = {};

	for (int i : S.get_itets()) { // for each tetrahedron that is incident to the segment
		for (int j : PD.get_tets()[i].get_fcs()) { // among the faces that are incident to the tetrahedron 
			// [c = b-a] for slope

			vector<int> vec = S.get_ifcs();
			if (std::find(vec.begin(), vec.end(), j) == vec.end()) { // if the face is not incident to the segment
				As.push_back(FacePointDist(PD.get_fcs()[j], Vec2Point(S.geta()))); // calculate the distance between the face and the left endpoint of the segment
				Bs.push_back(FacePointDist(PD.get_fcs()[j], Vec2Point(S.getb())));
			}


		}
	}

	vector<double> lines_inter = {0, 1}; // set of intersection points. 0 and 1 correspond to the endpoints of the segment
	for (int i = 0; i < As.size(); i++) {
		for (int j = i + 1; j < As.size(); j++) {
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
			if (temp > val) {temp = val;}
		}
		if (res < temp) { res = temp; res_inter = inter;}
	}

	res /= 24;
	double a_rad = radius(PD, S.geta_ind()), b_rad = radius(PD, S.getb_ind());
	if (res < a_rad) {res = a_rad; res_inter = 0;}
	if (res < b_rad) {res = b_rad; res_inter = 1;}

	double* ans = new double[2];
	ans[0] = res_inter;
	ans[1] = res;

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

// sg_num is the index of the segment AB
vector<Segment> MarkPoints(PolyDomain PD, int a, int b, int c, int d, int Tet_index, int sg_num) {
	Point A = PD.get_pt(a); Point B = PD.get_pt(b); Point C = PD.get_pt(c); Point D = PD.get_pt(d);
	// Segment S = *(PD.get_sg(sg_num)); // represents the segment AB
	
	Point P = (C + D) / 2;

	Plane PL1(A, B, C), PL2(A, B, D);

	double e = sqrt(eps / 8) * sin(acos(PlaneAngle(PL1, PL2)) / 2);

	Point H = perpen(A, B, P);
	double dist_PH = PointsDist(P, H);
	double dist_AB = PointsDist(A, B);

	vector<Segment> ans = {};

	vector<double> Pi_s = { e/(1.0+e) };
	vector<int> ki_s = {};

	int s_num = 0; // for test
	// int i1 = -1, i2p = -1, i2pp = -1;
	// bool i1_set = false;

	double rad_a = radius(PD, a);
	double rad_b = radius(PD, b);
	double rad_Seg = radius_e(PD, *(PD.get_sg(sg_num)))[1];



	// Point M2p = 
	// Point M2pp = 

	while (ans.empty() || !ans.back().getX().empty()) { // if ans.empty() = true, then it should be iterated
		s_num += 1; // for test
		// cout << s_num << endl; // for test

		// First calculate P_{i+1}
		double Pi = Pi_s.back();
		double Pip1 = Pi + (1 - Pi) * e/(1.0+e);
		Pi_s.push_back(Pip1);

		// Then calculate k_{i}
		int ki = 10; // for test
		// int ki = int(ceil((dist_AB * Pi) / (dist_PH * (Pip1 - Pi))));
		ki_s.push_back(ki);

		Point Ai = P * (1 - Pi) + A * Pi;
		Point Bi = P * (1 - Pi) + B * Pi;

		Segment S(Ai, Bi, {});
		S.set_itets({ Tet_index });
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
			for (int i = 0; i < ki+2; i++) {
			
				// Point test1 = (Bi - Ai); // for test
				// Point test2 = (Bi - Ai) * (i / (ki + 1)); // for test

				Point temp_pt = Ai + ((Bi-Ai) * (i / (ki+1.0))); // a steiner point candidate (on the segment)

				// cout << PointsDist(temp_pt, A) << " " << radius(PD, a) << endl; // for test
				// cout << PointsDist(temp_pt, B) << " " << radius(PD, b) << endl; // for test
				// cout << endl; // for test
				
				if (PointsDist(temp_pt, A) >= radius(PD, a) && PointsDist(temp_pt, B) >= radius(PD, b)) {
					Xi.push_back(i / (ki+1.0));
				}
			}		
		}
		// else {	
		// }
		
		S.setX(Xi);
		ans.push_back(S);
	}
	return ans;
}

vector<Segment> MarkPoints(PolyDomain PD) {
	
	// for test
	// int pt_num = 0;

	vector<Segment> ans = {};

	for (Tetra Tet : PD.get_tets()) {

		vector<int> pts = { Tet.getp1().getindex(), Tet.getp2().getindex(), Tet.getp3().getindex(), Tet.getp4().getindex() };
		vector<vector<int>> perm = { {0,1,2,3}, {0,2,1,3}, {0,3,1,2}, {1,2,0,3}, {1,3,0,2}, {2,3,0,1} };

		vector<Segment> temp;
		int sg_num = 0;
		for (vector<int> seq : perm) {
			temp = MarkPoints(PD, pts[seq[0]], pts[seq[1]], pts[seq[2]], pts[seq[3]], Tet.getindex(), Tet.getsg(sg_num));
			ans.insert(ans.end(), temp.begin(), temp.end());
			sg_num += 1;
		}
	}

	// for test
	/*
	for (auto sgsg : ans) {
		pt_num += sgsg.getX().size();
	}
	*/

	for (Segment S : PD.get_sgs()) {
		vector<double> X = {};

		double er_va = eps * radius(PD, S.geta_ind());
		double er_vb = eps * radius(PD, S.getb_ind());

		MyVec A = S.geta(); MyVec B = S.getb();
		double sz = (A - B).size();

		X.push_back(er_va / sz); // A'
		X.push_back(1 - er_vb / sz); // B'

		double value;

		value = radius_e(PD, S)[1];
		while (value > er_va / sz) {
			X.push_back(value);
			value -= eps * radius(PD, Vec2Point(value * A + (1 - value) * B), S.getind(), true);
		}

		value = radius_e(PD, S)[1];
		while (value < 1 - er_vb / sz) {
			X.push_back(value);
			value += eps * radius(PD, Vec2Point(value * A + (1 - value) * B), S.getind(), true);
		}

		PD.get_sg(S.getind())->setX(X);

		// for test
		// pt_num += X.size();
	}

	// for test
	// cout << pt_num;

	return ans;
}


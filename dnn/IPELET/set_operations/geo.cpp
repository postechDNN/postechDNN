

#include <fstream>
#include <iterator>
#include <map>
#include <set>
#include "geo.h"
#include "SOutil.h"


#include "ipepage.h"
#include "ipereference.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define DEPS 0.1
#define DDEPS 0.4 //0.4
#define IPE_INFINITY 10000

using namespace ipe;

// To use STL
struct vectorComp {
	bool operator() (const Vector& lhs, const Vector& rhs) const {
		if (lhs.x == rhs.x) return lhs.y < rhs.y;
		else return lhs.x < rhs.x;
	}
};

// Swap start point and end point(Changing direction)
ipe::CurveSegment inverseCS(const ipe::CurveSegment* input);

void findFirstPointBezier(const ipe::Bezier* const bez, const std::vector<ipe::Vector>* const pts, ipe::Vector* res, const ipe::Vector* const prev);

// Return the value of angle of input points
double computeAngle(ipe::Vector a, ipe::Vector b, ipe::Vector c);

// Extension an input spline with an input point
ipe::CurveSegment& extensionSpline(const ipe::CurveSegment* const spline, ipe::Vector p);

// Determine whether input bezier has a self intersection 
bool bezierSI(const ipe::Bezier *bezier) {
	Segment seg1(bezier->iV[0], bezier->iV[1]);
	Segment seg2(bezier->iV[3], bezier->iV[2]);
	Line l1 = seg1.line();
	Line l2 = seg2.line();
	Vector ip;
	if (!l1.intersects(l2, ip)) return false;
	int dir[3];
	dir[0] = ccw(bezier->iV[1], bezier->iV[2], bezier->iV[0]);
	dir[1] = ccw(bezier->iV[1], bezier->iV[2], bezier->iV[3]);
	dir[2] = ccw(bezier->iV[1], bezier->iV[2], ip);
	if (dir[0] == dir[1] && dir[1] == dir[2]) {
		Bezier left, right;
		bezier->subdivide(left, right);
		std::vector<Vector> ips;
		left.intersect(right, ips);
		for (int i = 0; i < ips.size(); i++) {
			if (ips[i] != left.iV[3] && ips[i] != right.iV[0])
				return true;
		}
		return (bezierSI(&left) | bezierSI(&right));
	}
	return false;
}

// Compute intersection between two open CurveSegments
bool interOpenCS(const ipe::CurveSegment* curvesegment1, const ipe::CurveSegment* curvesegment2, std::vector<ipe::Vector>& res) {
	int t1, t2;
	t1 = curvesegment1->type();
	t2 = curvesegment2->type();
	const CurveSegment* cs1, * cs2;

	if (t1 > t2) {
		cs1 = curvesegment2;
		cs2 = curvesegment1;
		int temp = t1;
		t1 = t2;
		t2 = temp;
	}
	else {
		cs1 = curvesegment1;
		cs2 = curvesegment2;
	}

	std::vector<ipe::Vector> intersect;
	std::set<Vector, vectorComp> endpoints;

	int type = t1 * 10 + t2;
	switch (type) {
	case 0: {// Arc & Arc
		Arc arc1 = cs1->arc();
		Arc arc2 = cs2->arc();
		arc1.intersect(arc2, intersect);
		endpoints.insert(arc1.beginp());
		endpoints.insert(arc1.endp());
		endpoints.insert(arc2.beginp());
		endpoints.insert(arc2.endp());
		break;
	}
	case 1: {// Arc & Segment
		Arc arc1 = cs1->arc();
		Segment seg2(cs2->cp(0), cs2->cp(1));
		arc1.intersect(seg2, intersect);
		endpoints.insert(arc1.beginp());
		endpoints.insert(arc1.endp());
		endpoints.insert(cs2->cp(0));
		endpoints.insert(cs2->cp(1));
		break;
	}
	case 2: {// Arc & Spline
		Arc arc1 = cs1->arc();
		std::vector<Bezier> bez2;
		cs2->beziers(bez2);
		arc1.intersect(bez2[0], intersect);
		endpoints.insert(arc1.beginp());
		endpoints.insert(arc1.endp());
		endpoints.insert(bez2[0].iV[0]);
		endpoints.insert(bez2[0].iV[3]);
		break;
	}
	case 11: {// Segment & Segment
		Vector iv;
		Segment seg1(cs1->cp(0), cs1->cp(1));
		Segment seg2(cs2->cp(0), cs2->cp(1));
		if (seg1.intersects(seg2, iv))  intersect.push_back(iv);
		endpoints.insert(cs1->cp(0));
		endpoints.insert(cs1->cp(1));
		endpoints.insert(cs2->cp(0));
		endpoints.insert(cs2->cp(1));
		break;
	}
	case 12: {// Segment & Spline
		Segment seg1(cs1->cp(0), cs1->cp(1));
		std::vector<Bezier> bez2;
		cs2->beziers(bez2);
		bez2[0].intersect(seg1, intersect);
		endpoints.insert(cs1->cp(0));
		endpoints.insert(cs1->cp(1));
		endpoints.insert(bez2[0].iV[0]);
		endpoints.insert(bez2[0].iV[3]);
		break;
	}
	case 22: {// Spline & Spline
		std::vector<Bezier> bez1;
		cs1->beziers(bez1);
		std::vector<Bezier> bez2;
		cs2->beziers(bez2);
		bez1[0].intersect(bez2[0], intersect);
		endpoints.insert(bez1[0].iV[0]);
		endpoints.insert(bez1[0].iV[3]);
		endpoints.insert(bez2[0].iV[0]);
		endpoints.insert(bez2[0].iV[3]);
		break;
	}
	default:
		break;
	}

	for (int i = 0; i < intersect.size(); i++) {
		/*
		if (endpoints.find(intersect[i]) == endpoints.end())
			res.push_back(intersect[i]);
		*/
		bool flag = true;
		for (auto it = endpoints.begin(); it != endpoints.end(); it++) {
			if (dist(intersect[i], *it) < DEPS) {
				flag = false;
				break;
			}
		}
		if (flag) res.push_back(intersect[i]);
	}
	
	if (res.size() == 0) return false;
	else return true;
}

// Determine whether two open CurveSegments has operlapping
bool overlapCS(const ipe::CurveSegment* cs1, const ipe::CurveSegment* cs2, std::vector<ipe::Vector>& res) {
	int t1, t2;
	t1 = cs1->type();
	t2 = cs2->type();

	Vector ep1[2];
	Vector ep2[2];

	if (t1 == 0) { // Arc & Segment
		Arc arc1 = cs1->arc();
		ep1[0] = arc1.beginp();
		ep1[1] = arc1.endp();
	}
	else if (t1 == 1) {
		ep1[0] = cs1->cp(0);
		ep1[1] = cs1->cp(1);
	}
	else if (t1 == 2) { // Spline
		std::vector<Bezier> bez1;
		cs1->beziers(bez1);
		ep1[0] = bez1[0].iV[0];
		ep1[1] = bez1[0].iV[3];
	}

	if (t2 == 0) { // Arc
		ep2[0] = cs2->cp(0);
		ep2[1] = cs2->cp(1);
		for (int i = 0; i < 2; i++) {
			if ( !eqv(ep1[i],ep2[0]) && !eqv(ep1[i], ep2[1])) {
				if (cs2->arc().distance(ep1[i], 10) < DEPS) {
					//wf << ep1[i].x << ", " << ep1[i].y << "\n";
					//wf << ep2[0].x << ", " << ep2[0].y << "\n";
					//wf << ep2[1].x << ", " << ep2[1].y << "\n";
					res.push_back(ep1[i]);
					return true;
				}
			}
		}
	}

	else if (t2 == 1) { // Segment
		ep2[0] = cs2->cp(0);
		ep2[1] = cs2->cp(1);
		Segment seg2(ep2[0], ep2[1]);
		for (int i = 0; i < 2; i++) {
			if (!eqv(ep1[i], ep2[0]) && !eqv(ep1[i], ep2[1])) {
				if (seg2.distance(ep1[i], 10) < DEPS) {
					//wf << ep1[i].x << ", " << ep1[i].y << "\n";
					//wf << ep2[0].x << ", " << ep2[0].y << "\n";
					//wf << ep2[1].x << ", " << ep2[1].y << "\n";
					res.push_back(ep1[i]);
					return true;
				}
					
			}
		}
	}
	
	else if (t2 == 2) { // Spline
		std::vector<Bezier> bez2;
		cs2->beziers(bez2);
		ep2[0] = bez2[0].iV[0];
		ep2[1] = bez2[0].iV[3];
		for (int i = 0; i < 2; i++) {
			if (!eqv(ep1[i], ep2[0]) && !eqv(ep1[i], ep2[1])) {
				if (bez2[0].distance(ep1[i], 10) < DEPS) {
					//wf << ep1[i].x << ", " << ep1[i].y << "\n";
					//wf << ep2[0].x << ", " << ep2[0].y << "\n";
					//wf << ep2[1].x << ", " << ep2[1].y << "\n";
					res.push_back(ep1[i]);
					return true;
				}
					
			}
		}
	}
	
	return false;
}

bool alignCS(std::vector<ipe::CurveSegment>& res, const std::vector<ipe::CurveSegment>* input) {
	std::vector<bool> visit(input->size() , false);

	std::vector<std::vector<Vector>> vV; // Vector vector
	std::map<Vector, int, vectorComp> vM; // Vector map
	std::map<Vector, int, vectorComp>::iterator it;

	for (int i = 0; i < input->size(); i++) {
		int type = (*input)[i].type();
		//COMMENT endP: vector->pair
		std::vector<Vector> endP;
		if (type == 0 || type == 1) { // arc or segment
			endP.push_back((*input)[i].cp(0));
			endP.push_back((*input)[i].cp(1));
		}
		else if (type == 2) { // spline
			endP.push_back((*input)[i].cp(0));
			endP.push_back((*input)[i].cp((*input)[i].countCP() - 1));
		}
		else { // invalid input
			return false;
		}
		vV.push_back(endP);
		for (int j = 0; j < 2; j++) {
			it = vM.find(endP[j]);
			if (it == vM.end()) vM[endP[j]] = 1;
			else it->second++;
		}
	}

	Vector leftmost; // start, end
	leftmost.x = INFINITY;

	// Determine path is connected & Find leftmost point 
	for (it = vM.begin(); it != vM.end(); it++) {
		if (it->second != 2) return false;
		if (leftmost.x > it->first.x) {
			leftmost = it->first;
		}
		else if (leftmost.x == it->first.x && leftmost.y > it->first.y) {
			leftmost = it->first;
		}
	}

	// Find start segment by CCW
	std::vector<int> candIdx;
	std::vector<Vector> rightP;
	for (int i = 0; i < vV.size(); i++) {
		if (dist(leftmost,vV[i][0]) < DEPS) {
			candIdx.push_back(i);
			rightP.push_back(vV[i][1]);
		}
		else if (dist(leftmost, vV[i][1]) <DEPS) {
			candIdx.push_back(i);
			rightP.push_back(vV[i][0]);
		}
	}

	// Check valid
	if (candIdx.size() != 2 || rightP.size() != 2) return false;
	int sidx;
	ipe::Vector prev;
	int direct = ccw(leftmost,rightP[0],rightP[1]);
	if (direct == 1) {
		sidx = candIdx[1];
		prev = rightP[1];
	}
	else if (direct == -1) {
		sidx = candIdx[0];
		prev = rightP[0];
	}
	else return false;

	// Align ccw and inverse illegal edge
	visit[sidx] = true;
	if (dist(leftmost, vV[sidx][0]) < DEPS) res.push_back((*input)[sidx]);
	else res.push_back(inverseCS(&(*input)[sidx]));
	for (int i = 1; i < vV.size(); i++) {
		for (int j = 0; j < vV.size(); j++) {
			if (visit[j]) continue;
			if (dist(prev, vV[j][0]) < DEPS) {
				visit[j] = true;
				prev = vV[j][1];
				res.push_back((*input)[j]);
			}
			else if (dist(prev, vV[j][1]) < DEPS) {
				visit[j] = true;
				prev = vV[j][0];
				res.push_back(inverseCS(&(*input)[j]));
			}
		}
	}

	if (res.size() != input->size()) return false;

	return true;
}


bool isSimple(ipe::IpeletData *data, const std::vector<ipe::CurveSegment>* input) {
	
	// Find bezier spline which has self intersection
	for (const CurveSegment& cs : *input) {
		if (cs.type() == 2) {
			std::vector<Bezier> bez;
			cs.beziers(bez);
			if (bezierSI(&bez[0]))
				return false;
		}
	}
	
	// Find self intersection
	for (int i = 0; i < input->size() - 1; i++) {
		for (int j = i + 1; j < input->size(); j++) {
			std::vector<Vector> ip;
			if (interOpenCS(&(*input)[i], &(*input)[j],ip)) {
				for (int i = 0; i < ip.size(); i++) {
					Attribute pts(true, String("mark/disk(sx)"));
					Reference* pt = new Reference(data->iAttributes, pts, ip[i]);
					data->iPage->append(ESecondarySelected, data->iLayer, pt);
				}
				return false;
			}
		}
	}

	// Find overlapping CurveSegment
	for (int i = 0; i < input->size() - 1; i++) {
		for (int j = i + 1; j < input->size(); j++) {
			std::vector<Vector> ip;
			if (overlapCS(&(*input)[i], &(*input)[j], ip)) {
				for (int i = 0; i < ip.size(); i++) {
					Attribute pts(true, String("mark/disk(sx)"));
					Reference* pt = new Reference(data->iAttributes, pts, ip[i]);
					data->iPage->append(ESecondarySelected, data->iLayer, pt);
				}
				return false;
			}
		}
	}

	return true;
}

ipe::CurveSegment inverseCS(const ipe::CurveSegment* input) {
	Curve* curve = new Curve();
	int type = input->type();

	if (type == 0) { // Arc
		Matrix m = input->matrix();
		m.a[2] *= -1;
		m.a[3] *= -1;
		curve->appendArc(m, input->cp(1), input->cp(0));
		CurveSegment res = curve->segment(0);
		//delete curve;
		return res;
	}

	if (type == 1) { // Segment
		curve->appendSegment(input->cp(1),input->cp(0));
		CurveSegment res = curve->segment(0);
		//delete curve;
		return res;
	}

	else { // (type == 2) Spline
		std::vector<Vector> v;
		for (int i = input->countCP() - 1; i >= 0; i--) {
			v.push_back(input->cp(i));
		}
		curve->appendSpline(v);
		CurveSegment res = curve->segment(0);
		//delete curve;
		return res;
	}
}

bool cutCS(const std::vector<ipe::CurveSegment>* cs1, const std::vector<ipe::CurveSegment>* cs2,
	std::vector<ipe::CurveSegment>& res, std::vector<int>& idx) {

	// cutting curve segment at intersection points
	for (auto cs : *cs1) {
		ipe::CurveSegment* nowCS = &cs;
		ipe::Vector* prev = NULL;
		while (nowCS != NULL) { // needs deallocate 
			std::vector<ipe::Vector> pts;
			for (auto tcs : *cs2) {
				std::vector<ipe::Vector> temp;
				interOpenCS(nowCS, &tcs, temp);
				pts.insert(pts.end(),temp.begin(),temp.end());
			}
			int type = nowCS->type();
			if (pts.size() == 0) {
				res.push_back(*nowCS);
				break;
			}
			
			if (type == 0) { // Arc
				double minAngle;
				ipe::Arc nowArc = nowCS->arc();
				ipe::Vector cp(nowArc.iM.a[4], nowArc.iM.a[5]);
				ipe::Vector* nearest = &pts[0];
				double min = computeAngle(nowCS->cp(0),cp, pts[0]);
				for (int i = 1; i < pts.size(); i++) {
					double temp = computeAngle(nowCS->cp(0), cp, pts[i]);
					if (min > temp) {
						min = temp;
						nearest = &pts[i];
					}
				}
				ipe::Curve* resCur = new ipe::Curve();
				ipe::Curve* nextCur = new ipe::Curve();
				resCur->appendArc(nowArc.iM,nowCS->cp(0),*nearest);
				ipe::CurveSegment resCS = resCur->segment(0);
				
				//idx.push_back(res.size());
				res.push_back(resCS);
				idx.push_back(res.size());

				nextCur->appendArc(nowArc.iM, *nearest, nowCS->cp(1));
				ipe::CurveSegment* NextCS = new ipe::CurveSegment(nextCur->segment(0));
				nowCS = NextCS;
				prev = NULL;
			}

			else if (type == 1) { // Segment
				ipe::Vector* nearest = &pts[0];
				double min = dist(nowCS->cp(0),pts[0]);
				for (int i = 1; i < pts.size(); i++) {
					double temp = dist(nowCS->cp(0), pts[i]);
					if (min > temp) {
						min = temp;
						nearest = &pts[i];
					}
				}
				ipe::Segment seg1(nowCS->cp(0), *nearest);
				ipe::Segment seg2(*nearest, nowCS->cp(1));
				//idx.push_back(res.size());
				ipe::Curve *curve = new Curve();
				curve->appendSegment(nowCS->cp(0), *nearest);
				curve->appendSegment(*nearest, nowCS->cp(1));
				res.push_back(curve->segment(0));
				idx.push_back(res.size());

				nowCS = new CurveSegment(curve->segment(1));
				prev = NULL;
			}

			else { // (type == 2) Spline
				//Bezier exception handling
				std::vector<ipe::Vector> nowPts;
				if (prev == NULL) {
					nowPts.insert(nowPts.begin(), pts.begin(), pts.end());
				}
				else {
					for (int i = 0; i < pts.size(); i++) {
						if (dist(pts[i],nowCS->cp(0)) <= DDEPS) continue; // DEPS
						nowPts.push_back(pts[i]);
					}
				}
				if (nowPts.size() == 0) {
					res.push_back(*nowCS);
					break;
				}

				ipe::Vector* nearest;
				std::vector<ipe::Bezier> bez;
				nowCS->beziers(bez);
				if (nowPts.size() == 1) {
					nearest = &nowPts[0];
				}
				else {
					nearest = new ipe::Vector();
					findFirstPointBezier(&bez[0], &nowPts, nearest, prev);
				}
				ipe::Vector pos;
				double para,bnd=100;
				ipe::Bezier bez1, bez2;
				bez[0].snap(*nearest, para, pos, bnd); // needs exception handling
				splitBez(&bez[0], bez1, bez2, para);
				ipe::Curve* resCur = new ipe::Curve();
				ipe::Curve* nextCur = new ipe::Curve();
				resCur->appendSpline({ bez1.iV[0],bez1.iV[1],bez1.iV[2],bez1.iV[3] });
				ipe::CurveSegment resCS = resCur->segment(0);
				
				res.push_back(resCS);
				idx.push_back(res.size());

				nextCur->appendSpline({ bez2.iV[0],bez2.iV[1],bez2.iV[2],bez2.iV[3] });
				ipe::CurveSegment* NextCS = new CurveSegment(nextCur->segment(0));
				nowCS = NextCS;
				prev = nearest;
			}
		}
	}
	return true;
	//return !idx.empty();
}

inline int ccw(const ipe::Vector a, const ipe::Vector b, const ipe::Vector c) {
	return ccw(a.x, a.y, b.x, b.y, c.x, c.y);
}

inline int ccw(const double ax, const  double ay, const  double bx,
	const double by, const  double cx, const  double cy) {
	double res = ax * by + bx * cy + cx * ay;
	res = res - ay * bx - by * cx - cy * ax;
	if (res > 0) return 1;
	else if (res < 0) return -1;
	else return 0;
}

inline bool eqv(const ipe::Vector a, const ipe::Vector b) {
	if (std::abs(a.x - b.x) < DEPS && std::abs(a.y - b.y))
		return true;
	else false;
}

inline double dist(const ipe::Vector a, const ipe::Vector b) {
	return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}

void splitBez(const ipe::Bezier* const bez, ipe::Bezier& left, ipe::Bezier& right, const double para) {
	ipe::Vector v[4] = { bez->iV[0], bez->iV[1], bez->iV[2], bez->iV[3] };
	left.iV[0] = v[0];
	left.iV[1].x = (v[0].x += (v[1].x - v[0].x) * para);
	left.iV[1].y = (v[0].y += (v[1].y - v[0].y) * para);
	v[1].x += (v[2].x - v[1].x) * para;
	v[1].y += (v[2].y - v[1].y) * para;
	v[2].x += (v[3].x - v[2].x) * para;
	v[2].y += (v[3].y - v[2].y) * para;
	left.iV[2].x = (v[0].x += (v[1].x - v[0].x) * para);
	left.iV[2].y = (v[0].y += (v[1].y - v[0].y) * para);
	v[1].x += (v[2].x - v[1].x) * para;
	v[1].y += (v[2].y - v[1].y) * para;
	left.iV[3].x = v[0].x + (v[1].x - v[0].x) * para;
	left.iV[3].y = v[0].y + (v[1].y - v[0].y) * para;
	right.iV[0] = left.iV[3];
	right.iV[1] = v[1];
	right.iV[2] = v[2];
	right.iV[3] = v[3];
}


void findFirstPointBezier(const ipe::Bezier* const bez, const std::vector<ipe::Vector>* const pts, ipe::Vector* res, const ipe:: Vector* const prev) {
	ipe::Bezier copyBez(*bez); // constructer check!
	ipe::Bezier* nowBez = &copyBez;
	std::vector<ipe::Vector> nowPts(pts->begin(), pts->end());


	std::vector<ipe::Vector> lpts, rpts;
	while (nowBez != NULL) {
		ipe::Bezier lbez, rbez;
		nowBez->subdivide(lbez, rbez);
		
		for (auto i = 0; i < nowPts.size(); i++) {
			if (lbez.iV[3] == nowPts[i]) {
				lpts.push_back(nowPts[i]);
			}
			else {
				double ld = lbez.distance(nowPts[i], 10);
				double rd = rbez.distance(nowPts[i], 10);
				if (ld <= 2.0 && rd <= 2.0) {
					std::vector<ipe::Vector> lapp,rapp;
					lbez.approximate(1.0, lapp);
					rbez.approximate(1.0, rapp);
				}
				else if (ld <= 2.0) {
					lpts.push_back(nowPts[i]);
				}
				else if (rd <= 2.0) {
					rpts.push_back(nowPts[i]);
				}
			}
		}
		

		if (lpts.size() == 0) {
			if (rpts.size() == 1) {
				res = &rpts[0];
				break;
			}
			else {
				nowBez = &rbez;
				nowPts.clear();
				nowPts.insert(nowPts.begin(), rpts.begin(), rpts.end());
				lpts.clear();
				rpts.clear();
			}
		}
		else if (lpts.size() == 1) {
			res = &lpts[0];
			break;
		}
		else {
			nowBez = &lbez;
			nowPts.clear();
			nowPts.insert(nowPts.begin(),lpts.begin(), lpts.end());
			lpts.clear();
			rpts.clear();
		}
	}
}

double computeAngle(ipe::Vector a, ipe::Vector b, ipe::Vector c) {
	double res;
	a -= b;
	c -= b;
	res = std::atan2(a.y * c.x - a.x * c.y, a.x * c.x + a.y * c.y);
	res = res * 180 / M_PI;
	if (res < 0) res += 360;
	return res;
}

int verticalRay(const ipe::Vector p, const std::vector<ipe::CurveSegment>* const cs) {
	int res = 0;
	ipe::Curve vrayC;
	vrayC.appendSegment(p, ipe::Vector(p.x, IPE_INFINITY));
	ipe::CurveSegment vrayCS = vrayC.segment(0);
	for (const ipe::CurveSegment& nowCS : *cs) {
		std::vector<ipe::Vector> temp;
		if (interOpenCS(&vrayCS, &nowCS, temp)) res += temp.size();
		else if (std::abs(p.x - nowCS.cp(0).x) < DEPS && p.y < nowCS.cp(0).y) res++;
		//If nowCS is arc or spline??
	}
	
	return res;
}

void divideRaw(const std::vector<ipe::CurveSegment>* const raw, std::vector<std::vector<ipe::CurveSegment>>& result) {
	std::vector<bool> visit(raw->size(), false);

	std::set<int> remain;
	for (int i = 0; i < raw->size(); i++) {
		remain.insert(i);
	}

	while (!remain.empty()) {
		std::vector<ipe::CurveSegment> temp;
		const ipe::CurveSegment* prevCS = NULL;
		const ipe::CurveSegment* nowCS = NULL;
		auto it = remain.begin();
		prevCS = &(*raw)[*it];
		remain.erase(it);

		if (prevCS == NULL) break;
		//temp.push_back(*nowCS);
		ipe::Vector nowP = ipe::Vector(prevCS->cp(0));
		ipe::Vector endP = ipe::Vector(prevCS->cp(prevCS->countCP() - 1));

		bool find = true;
		while (find) {
			find = false;
			for (auto it = remain.begin(); it != remain.end(); it++) {
				// determine type
				nowCS = &(*raw)[*it];
				int type = nowCS->type();
				ipe::CurveSegment* sp = NULL;
				if (type == 0) {
					if (dist(nowP, nowCS->cp(0)) < DDEPS) {
						if (prevCS->type() == 2) temp.push_back(extensionSpline(prevCS, nowCS->cp(0)));
						else temp.push_back(*prevCS);
						nowP = ipe::Vector(nowCS->cp(1));
					}
					else if (dist(nowP, nowCS->cp(1)) < DDEPS) {
						if (prevCS->type() == 2) temp.push_back(extensionSpline(prevCS, nowCS->cp(1)));
						else temp.push_back(*prevCS);
						nowP = ipe::Vector(nowCS->cp(0));
					}
					else continue;
				}
				else if (type == 1) {
					if (dist(nowP, nowCS->cp(0)) < DDEPS) {
						if (prevCS->type() == 2) temp.push_back(extensionSpline(prevCS, nowCS->cp(0)));
						else temp.push_back(*prevCS);
						nowP = ipe::Vector(nowCS->cp(1));
					}
					else if (dist(nowP, nowCS->cp(1)) < DDEPS) {
						if (prevCS->type() == 2) temp.push_back(extensionSpline(prevCS, nowCS->cp(1)));
						else temp.push_back(*prevCS);
						nowP = ipe::Vector(nowCS->cp(0));
					}
					else continue;
				}
				else if(type == 2) {
					ipe::Vector nowEndP(nowCS->cp(nowCS->countCP() - 1));
					if (prevCS->type() == 2) {
						if (dist(nowP, nowCS->cp(0)) < DDEPS) {
							ipe::Vector med = (nowP,nowCS->cp(0));
							temp.push_back(extensionSpline(prevCS, med));
							sp = &extensionSpline(nowCS, med);
							nowP = ipe::Vector(nowEndP);
						}
						else if (dist(nowP, nowEndP) < DDEPS) {
							ipe::Vector med = (nowP, nowEndP);
							temp.push_back(extensionSpline(prevCS, med));
							sp = &extensionSpline(nowCS, med);
							nowP = ipe::Vector(nowCS->cp(0));
						}
						else continue;
					}
					else {
						if (dist(nowP, nowCS->cp(0)) < DDEPS) {
							temp.push_back(*prevCS);
							sp = &extensionSpline(nowCS, nowP);
							nowP = ipe::Vector(nowEndP);
						}
						else if (dist(nowP, nowEndP) < DDEPS) {
							temp.push_back(*prevCS);
							sp = &extensionSpline(nowCS, nowP);
							nowP = ipe::Vector(nowCS->cp(0));
						}
						else continue;
					}
				}
				
				if (dist(endP,nowP) < DDEPS) {
					if (sp == NULL) temp.push_back(*nowCS);
					else {				
						ipe::CurveSegment* sp2 = &extensionSpline(sp, endP);
						temp.push_back(*sp2);
					}
				}
				else {
					if (sp == NULL) prevCS = nowCS;
					else prevCS = sp;
				}
				find = true;
				remain.erase(it); 
				break;
			}
		}
		result.push_back(temp);
	}
}

ipe::CurveSegment& extensionSpline(const ipe::CurveSegment* const spline, ipe::Vector p) {
	ipe::Curve* newC = new Curve();
	if (dist(spline->cp(0), p) < DDEPS) newC->appendSpline({ p, spline->cp(1), spline->cp(2), spline->cp(3)});
	else newC->appendSpline({ spline->cp(0), spline->cp(1), spline->cp(2), p });
	ipe::CurveSegment* newCS = new CurveSegment(newC->segment(0));
	return *newCS;
}

ipe::Vector median(const ipe::Vector a, const ipe::Vector b) {
	ipe::Vector res((a.x + b.x) / 2, (a.y + b.y) / 2);
	return res;
}
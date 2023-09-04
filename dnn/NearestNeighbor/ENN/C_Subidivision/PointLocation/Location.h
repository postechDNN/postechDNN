#pragma once
#include "../DCEL/DCEL.h"
#include "../DCEL/DCEL_operation.h"
#include "../PBST/PBST.h"
// #include <functional>
#include <vector>

using namespace std;

const double EPS = tolerance;

class CEdge : public HEdge {
public:
	static double x;

	friend bool operator<(CEdge& const c1, CEdge& const c2) {
		Edge e1 = c1.getEdge();
		Edge e2 = c2.getEdge();
		double t1 = (x - e1.gets().getx()) / (e1.gett().getx() - e1.gets().getx());
		double t2 = (x - e2.gets().getx()) / (e2.gett().getx() - e2.gets().getx());
		return (1. - t1) * e1.gets().gety() + t1 * e1.gett().gety() <
			(1. - t2) * e2.gets().gety() + t2 * e2.gett().gety();
		return true;
	}

	friend bool operator==(CEdge& const c1, CEdge& const c2) {
		Edge e1 = c1.getEdge();
		Edge e2 = c2.getEdge();
		double t1 = (x - e1.gets().getx()) / (e1.gett().getx() - e1.gets().getx());
		double t2 = (x - e2.gets().getx()) / (e2.gett().getx() - e2.gets().getx());
		return ((1. - t1) * e1.gets().gety() + t1 * e1.gett().gety() -
			(1. - t2) * e2.gets().gety() + t2 * e2.gett().gety() < EPS);
		return true;
	}

	CEdge() : HEdge() {};
	CEdge(HEdge& he) : HEdge(he) {}
	CEdge(Point* p1, Point* p2) : HEdge(new Vertex(*p1), new Vertex(*p2)) {}
};

class Location
{
public:
	P_PBST<CEdge> vt;
	vector<double> xcoords;
	Location(DCEL* dcel);
	Face* locate(Point* p);
};

Face* locate(P_PBST<CEdge>* t, Point* p);
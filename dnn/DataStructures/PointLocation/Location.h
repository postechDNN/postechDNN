#pragma once
#include"../DCEL/DCEL.h"
#include"../PersistentRBTree/persistent.h"
#include<vector>
#include<functional>

using namespace std;

const double EPS = 1e-8;

class CEdge : public HEdge {
public:
	static double x;
	friend bool operator<(CEdge& const e1, CEdge& const e2) {
		double t1 = (x - e1.gets().getx()) / (e1.gett().getx() - e1.gets().getx());
		double t2 = (x - e2.gets().getx()) / (e2.gett().getx() - e2.gets().getx());
		return (1. - t1) * e1.gets().gety() + t1 * e1.gett().gety() <
			(1. - t2) * e2.gets().gety() + t2 * e2.gett().gety();
	}
	friend bool operator>(CEdge& const e1, CEdge& const e2) {
		double t1 = (x - e1.gets().getx()) / (e1.gett().getx() - e1.gets().getx());
		double t2 = (x - e2.gets().getx()) / (e2.gett().getx() - e2.gets().getx());
		return (1. - t1) * e1.gets().gety() + t1 * e1.gett().gety() >
			(1. - t2) * e2.gets().gety() + t2 * e2.gett().gety();
	}
	friend bool operator==(CEdge& const e1, CEdge& const e2) {
		double t1 = (x - e1.gets().getx()) / (e1.gett().getx() - e1.gets().getx());
		double t2 = (x - e2.gets().getx()) / (e2.gett().getx() - e2.gets().getx());
		return ((1. - t1) * e1.gets().gety() + t1 * e1.gett().gety() -
			(1. - t2) * e2.gets().gety() + t2 * e2.gett().gety() < EPS);
		
	}
	CEdge(HEdge& he) : HEdge(he) {}
	CEdge(Point* p1, Point* p2) : HEdge(new Vertex(*p1), new Vertex(*p2)) {}
};


VersionTree<CEdge>* buildPM(DCEL* dcel);
Face* locate(VersionTree<CEdge>* t, Point* p);
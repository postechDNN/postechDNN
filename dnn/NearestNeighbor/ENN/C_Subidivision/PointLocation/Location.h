#pragma once
#include "../DCEL/DCEL.h"
#include "../DCEL/DCEL_operation.h"
#include "../PBST/PBST.h"
// #include <functional>
#include <vector>

using namespace std;

const double EPS = tolerance;

class CEdge : public HEdge {
private:
	static double constant;
	static bool is_vertical;

	HEdge* ref;

	double getCoord(Vertex* p, bool parallel);
public:
	CEdge();
	CEdge(HEdge& he);
	CEdge(Point* p1, Point* p2);

	bool operator<(CEdge& other);
	bool operator==(CEdge& other);
	HEdge* getRef();

	static void setConstantLine(double constant, bool is_vertical);
	static double getConstant();
};

class Location
{
private:
	int getVersion(Point* p, bool is_vertical);
public:
	static const int N = 0, S = 1, W = 2, E = 3;
	P_PBST<CEdge> v_slab, h_slab;
	vector<double> xcoords, ycoords;
	Face* outmost;
	Location(DCEL* dcel);
	Face* locate(Point* p);

	HEdge* ortho_ray(Point* p, int dir);
};
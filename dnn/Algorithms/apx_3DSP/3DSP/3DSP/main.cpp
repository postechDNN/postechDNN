#include <string>
#include <iostream>
#include "dots.h"
#include "propagation.h"
#include "steiner_pts.h"
#include "parser.h"

int main() {
	/*
	string FileName;
	cout << "Enter tetgen file name: ";
	cin >> FileName;
	PolyDomain D = BuildPolyDomain(FileName);
	*/
	PolyDomain D = BuildPolyDomain("cube.1");
	D.init_neigh_sgs();
	D.MarkPoints();
	D.ConnectSgs();
	D.SetSgs();
	/*
	double x, y, z;
	cout << "point: ";
	cin >> x >> y >> z;
	Point p0(x, y, z);*/
	Point p0(0.1, 0.1, 0.1);
	D.BuildSPM(p0);
	return 0;
}
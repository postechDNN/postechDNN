#include <string>
#include <iostream>
#include "dots.h"
#include "propagation.h"
#include "steiner_pts.h"
#include "parser.h"

int main() {
	string FileName;
	cout << "Enter tetgen file name: ";
	cin >> FileName;
	PolyDomain D = BuildPolyDomain(FileName);
	D.init_neigh_sgs();
	D.MarkPoints();
	D.ConnectSgs();
	return 0;
}
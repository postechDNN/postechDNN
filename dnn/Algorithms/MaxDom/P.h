#pragma once

#include <iostream>
#include <vector>
#include "Q.h"

using namespace std;

class P {
public:
	int x;
	int tau;
	int y;
	int w;
	void binarySearch(vector<Q*> Qpis, int l, int r);
};

#pragma once
#include"Rectangle.h"
#include"Cgraph.h"
#include<vector>
using namespace std;
class RectangularDomain {
	vector < Rectangle > rectset; // input rectangles and points
	Cgraph xpos, ypos, xneg, yneg;
	Rectangle bbox;
	void makeshadow();
};
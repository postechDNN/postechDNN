#include "LINE.h"
#include "ShortestPathTree.h"

/*
LINE::LINE(TYPE _type, Point p1, Point p2, int _v)
{
	endP[0] = p1;
	endP[1] = p2;
	type = _type;
	v = _v;
	slope = slopeComputation(p1, p2);
}*/

float slopeComputation(Point p1, Point p2) {
	float x1 = p1.get_x();
	float x2 = p2.get_x();
	float y1 = p1.get_y();
	float y2 = p2.get_y();

	if (y1 == y2)
		return 0;
	if (x1 == x2)
		return std::numeric_limits<float>::infinity();

	return (y1 - y2) / (x1 - x2);
}

Point computeEndpoint(int lineFrom, int lineTo)
{
	//find triangle that the line penetrates through
	//and the edge it penetrates
	
	vector<int> adjacentTraingles = point_state.find_all_triangles()
	//repeat:
	//find the other triangle that shares that edge
	//find the edge that the line penetrates through

	Point p1(-1, -1);

	return p1;
}
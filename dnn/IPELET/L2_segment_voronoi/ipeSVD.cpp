#include "MACRO.h"
#include "ipeSVD.h"
#include "Segment.h"
#include "SimplePolygon.h"

using namespace std;


bool L2SVDIpelet::run(int, ipe::IpeletData* data, ipe::IpeletHelper* helper) {
	
	//IPE Input Read
	vector<ipe::CurveSegment> ipeInput;
	Get_segments(data, helper, true, ipeInput);
	
	//Convert ipe format to Segment class
	vector<_Segment> inputSegments;
	for (int i = 0; i < ipeInput.size(); i++) {
		_Segment* temp = new _Segment(Point(ipeInput[i].cp(0).x, ipeInput[i].cp(0).y), Point(ipeInput[i].cp(1).x, ipeInput[i].cp(1).y));
		inputSegments.push_back(*temp);
	}

	//Check invalid input
	int check = checkInput(&inputSegments);
	if (check != VALID) return false;

	//Compute boundary
	double boundary[4] = { INF,-INF,-INF,INF };// left, top, right, bottom
	ComputeBoundary(boundary, inputSegments);

	//Brute Force Algorithm
	vector<vector<SimplePolygon*>*> BFCells;
	BFAlgorithm(&inputSegments, boundary, &BFCells);

	//Cells Intersection
	vector<SimplePolygon> result;
	CellsIntersection(&BFCells, &result);

	//IPE Drawing
	IPEIO ipeAttr;
	ipeAttr.set_color(0, 0, 255); // blue
	ipe::Vector left;
	ipe::Vector right;
	for (unsigned int i = 0; i < result.size(); i++) {
		vector<Edge> edges = result[i].getEdges();
		for (unsigned int j = 0; j < edges.size(); j++) {
			left.x = edges[j].getOrigin().getX(); left.y = edges[j].getOrigin().getY();
			right.x = edges[j].getOrigin().getX(); left.y = edges[j].getOrigin().getY();
			ipeAttr.Draw_segment(data, helper, left, right);
		}
	}

	inputSegments.clear();
	for (int i = 0; i < BFCells.size(); i++) (*BFCells[i]).clear();
	BFCells.clear();
	result.clear();

	return true;
}

IPELET_DECLARE ipe::Ipelet* newIpelet()
{
	return new L2SVDIpelet;
}
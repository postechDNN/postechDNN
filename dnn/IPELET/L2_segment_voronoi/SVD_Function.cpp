#include "SVD_Function.h"
#include "PolygonClipping.h"
#include "Edge.h"
#include "MACRO.h"


#include <cmath>

using namespace std;

// Compute two L2 segment voronoi diagram cells
void computeCell(ISegment s1, ISegment s2, vector<SimplePolygon*>* c1, vector<SimplePolygon*>* c2, double boundary[]);

// Return result of two simple polygon intersection
SimplePolygon* polygonsIntersection(vector<SimplePolygon*>* polygons);

// Make two simple polygons
void makeSimplePolygon(double boundary[], std::vector<Edge> *bisector, SimplePolygon *result1, SimplePolygon *result2);

// Determine the segment intersects with boundary
int isIntersectionBoundary(double boundary[], ISegment seg);

// Determine the point lying inside of boundary
bool isPointInBoundary(double boundary[], Point p);

// Determine the point lying on boundary
bool isPointOnBoundary(Point b1, Point b2, Point p);

// Compute bisector of two segments
vector<Edge> computeBisector(ISegment s1, ISegment s2);

int checkInput(std::vector<ISegment>* input) {
	// If there are less than two segment
	if (input->size() < 2) return INSUFFICIENT;

	// If there are intersection
	for (unsigned int i = 0; i < (*input).size()-1; i++) {
		for (unsigned int j = i + 1; i < (*input).size(); j++) {
			Point temp = (*input)[i].getIntersect((*input)[j]);
			if ((*input)[i].isOnSeg(temp) && (*input)[j].isOnSeg(temp)) return INTERSECTION;
		}
	}

	// Modify vertical segment and horizon segment
	for (unsigned int i = 0; i < (*input).size(); i++) {
		if ((*input)[i].getLeft().getX() == (*input)[i].getRight().getX()) (*input)[i].getRight().setX((*input)[i].getRight().getX() + EPS);
		if ((*input)[i].getLeft().getY() == (*input)[i].getRight().getY()) (*input)[i].getRight().setY((*input)[i].getRight().getY() + EPS);
	}

	return VALID;
}

void ComputeBoundary(double *boundary, std::vector<ISegment> inputSegments) {
	// boundary idx : left, top, right, bottom
	for (unsigned int i = 0; i < inputSegments.size(); i++) {
		if (boundary[0] > inputSegments[i].getLeft().getX()) boundary[0] = inputSegments[i].getLeft().getX();
		if (boundary[1] < inputSegments[i].getLeft().getY()) boundary[1] = inputSegments[i].getLeft().getY();
		if (boundary[1] < inputSegments[i].getRight().getY()) boundary[1] = inputSegments[i].getRight().getY();
		if (boundary[2] < inputSegments[i].getRight().getX()) boundary[2] = inputSegments[i].getRight().getX();
		if (boundary[3] > inputSegments[i].getRight().getY()) boundary[3] = inputSegments[i].getRight().getY();
		if (boundary[3] > inputSegments[i].getLeft().getY()) boundary[3] = inputSegments[i].getLeft().getY();
	}
	boundary[0] -= BOUNDARY_PADDING;
	boundary[1] += BOUNDARY_PADDING;
	boundary[2] += BOUNDARY_PADDING;
	boundary[3] -= BOUNDARY_PADDING;
}

void BFAlgorithm(std::vector<ISegment> *inputSegments, double boundary[], std::vector<std::vector<SimplePolygon*>*>* Cells) {
	
	for (unsigned int i = 0; i < (*inputSegments).size(); i++) {
		vector<SimplePolygon*>* newVector = new vector<SimplePolygon*>;
		(*Cells).push_back(newVector);
	}

	// Compute all pairs of segments 
	for (unsigned int i = 0; i < (*inputSegments).size()-1; i++) {
		for (unsigned int j = i + 1; j < (*inputSegments).size(); j++) {
			computeCell((*inputSegments)[i], (*inputSegments)[j],(*Cells)[i], (*Cells)[j], boundary);
		}
	}
}

void CellsIntersection(std::vector<std::vector<SimplePolygon*>*>* Cells, std::vector<SimplePolygon>* resultCells) {
	for (unsigned int i = 0; i < (*Cells).size(); i++) {
		SimplePolygon temp = *polygonsIntersection((*Cells)[i]);
		(*resultCells).push_back(temp);
	}
}

void computeCell(ISegment s1, ISegment s2, vector<SimplePolygon*>* c1, vector<SimplePolygon*>* c2, double boundary[]) {
	SimplePolygon* result1 = NULL;
	SimplePolygon* result2 = NULL;

	// Compute bisector of two segments
	vector<Edge> bisector = computeBisector(s1, s2);

	// Determine direction of bisector
	bool flag;
	ISegment medianBisector = *s1.getBisector(s2);
	if (medianBisector.ccw(s1.getMedian()) == CCW) flag = true;
	else flag = false;

	// Make closed polygon
	if (flag) makeSimplePolygon(boundary, &bisector, result1, result2);
	else makeSimplePolygon(boundary, &bisector, result2, result1);

	// Store result
	(*c1).push_back(result1);
	(*c2).push_back(result2);
	bisector.clear();
}

SimplePolygon* polygonsIntersection(vector<SimplePolygon*>* polygons) {
	vector<SimplePolygon*> result;
	PolygonClipping pc;
	result = pc.clipping(*(*polygons)[0], *(*polygons)[1]);
	for (unsigned int i = 2; i < (*polygons).size(); i++) {
		result = pc.clipping(*result.front(), *(*polygons)[i]);
	}
	vector<Point> resultPoints = result.front()->getVertices();
	SimplePolygon* ret = new SimplePolygon(resultPoints,false);
	return ret;
}

void makeSimplePolygon(double boundary[], std::vector<Edge>* bisector, SimplePolygon* result1, SimplePolygon* result2) {
	vector<Edge> edges1;
	vector<Edge> edges2;

	for (unsigned int i = 0; i < (*bisector).size(); i++) {
		
	}
	


	// Make boundary point data structure
	vector<Point> boundaryPoints;
	boundaryPoints.push_back(Point(boundary[0], boundary[1]));
	boundaryPoints.push_back(Point(boundary[2], boundary[1]));
	boundaryPoints.push_back(Point(boundary[2], boundary[3]));
	boundaryPoints.push_back(Point(boundary[0], boundary[3]));

	SimplePolygon* unboundedPolygon1 = new SimplePolygon(edges1);
	SimplePolygon* unboundedPolygon2 = new SimplePolygon(edges2);
	SimplePolygon* BoundaryPolygon = new SimplePolygon(boundaryPoints, false);

	PolygonClipping pc1;
	PolygonClipping pc2;

	result1 = pc1.clipping(*unboundedPolygon1, *BoundaryPolygon).front();
	result2 = pc2.clipping(*unboundedPolygon2, *BoundaryPolygon).front();
	edges1.clear();
	edges2.clear();
	boundaryPoints.clear();
}

int isIntersectionBoundary(double boundary[], ISegment seg) {
	// One of endpoint of edge must be in boundary
	int flag = 0;
	if (isPointInBoundary(boundary, seg.getLeft())) flag += 1;
	if (isPointInBoundary(boundary, seg.getRight())) flag += 2;

	double tempX, tempY;
	// Intersection cases
	if (flag == 1 || flag == 2) {
		if (flag == 1) {
			tempX = boundary[2];
		}
		else if (flag == 2) {
			tempX = boundary[0];
		}
		tempY = seg.getSlope() * tempX + seg.getIntercept();

		if (tempY > boundary[1]) return 1;
		else if (tempY > boundary[3]) {
			if (flag == 2) return 0;
			if (flag == 1) return 2;
		}
		else return 3;
	}
	
	// No intersection cases (flag == 0 || flag == 3)
	else return -1;
}

bool isPointInBoundary(double boundary[], Point p) {
	if (p.getX() < boundary[0]) return false;
	if (p.getY() > boundary[1]) return false;
	if (p.getX() > boundary[2]) return false;
	if (p.getY() < boundary[3]) return false;

	return true;
}

bool isPointOnBoundary(Point b1, Point b2, Point p) {
	if (abs(b1.getX() - b2.getX()) < DOUBLE_EPS) {
		Point* left, *right;
		if (b1.getX() < b2.getX()) {
			left = &b1; right = &b2;
		}
		else {
			left = &b2; right = &b1;
		}
		if (abs(left->getY() - p.getY()) < DOUBLE_EPS) {
			if (left->getX() < p.getX() && p.getX() < right->getX()) return true;
		}
	}
	else if (abs(b1.getY() - b2.getY()) < DOUBLE_EPS) {
		Point *top, *bottom;
		if (b1.getY() < b2.getY()) {
			bottom = &b1; top = &b2;
		}
		else {
			bottom = &b2; top = &b1;
		}
		if (abs(bottom->getX() - p.getX()) < DOUBLE_EPS) {
			if (bottom->getY() < p.getY() && p.getY() < top->getY()) return true;
		}
	}
	return false;
}

vector<Edge> computeBisector(ISegment seg1, ISegment seg2) {
	vector<Edge> result;

	vector<ISegment> segTempVector;
	// Compute intersection point of two segments
	ISegment bisector = *seg1.getBisector(seg2);

	ISegment segPerLine[4];
	segPerLine[0] = *seg1.getPerLine(seg1.getLeft());
	segPerLine[0] = *seg1.getPerLine(seg1.getRight());
	segPerLine[0] = *seg2.getPerLine(seg2.getLeft());
	segPerLine[0] = *seg2.getPerLine(seg2.getRight());
	// intersection point of Perpendicular lines and bisector 
	Point interPerAndBi[4];
	for (int i = 0; i < 4; i++) interPerAndBi[i] = *segPerLine[i].getIntersect(bisector);

	ISegment medianBisec;
	ISegment leftBisec, leftBisecVerLine, leftPara;
	ISegment rightBisec, rightBisecVerLine, rightPara;
	Point median;
	Point leftFocus;
	Point rightFocus;
	ISegment leftDirectrix;
	ISegment rightDirectrix;
	double tempX;
	if (interPerAndBi[0].getX() > interPerAndBi[3].getX()) { // case 6
		medianBisec = ISegment(seg2.getRight(), seg1.getLeft());
		median = seg2.getRight().getMedian(seg1.getLeft());
		bisector = *medianBisec.getPerLine(median);
		bisector.setStart(bisector.getIntersect(segPerLine[3]));
		bisector.setEnd(bisector.getIntersect(segPerLine[0]));

		leftBisec = ISegment(seg1.getLeft(), seg2.getLeft());
		leftBisecVerLine = *leftBisec.getPerLine(segPerLine[2],true);
		leftPara.setStart(leftBisecVerLine.getRight());
		leftPara.setEnd(bisector.getLeft());
		if (leftBisecVerLine.getRight().getX() == MAXIMUM) leftPara.setStart(leftBisecVerLine.getLeft());

		rightBisec = ISegment(seg1.getRight(), seg2.getRight());
		rightBisecVerLine = *rightBisec.getPerLine(segPerLine[1],false);
		rightPara.setStart(bisector.getRight());
		rightPara.setEnd(rightBisecVerLine.getLeft());
		if (rightBisecVerLine.getLeft().getX() == MAXIMUM) rightPara.setEnd(rightBisecVerLine.getRight());

		leftFocus = seg1.getLeft();
		rightFocus = seg2.getRight();
		
		leftDirectrix.setStart(seg2.getIntersectPerLine(leftFocus));
		if (leftFocus.getY() > leftDirectrix.getStart().getY()) tempX = leftDirectrix.getStart().getX() + 1;
		else tempX = leftDirectrix.getStart().getX() - 1;
		leftDirectrix.setEnd(Point(tempX, seg2.getSlope()*tempX + seg2.getIntercept()));

		rightDirectrix.setStart(seg1.getIntersectPerLine(rightFocus));
		if (rightFocus.getY() > rightDirectrix.getStart().getY()) tempX = rightDirectrix.getStart().getX() + 1;
		else tempX = rightDirectrix.getStart().getX() - 1;
		rightDirectrix.setEnd(Point(tempX, seg1.getSlope()*tempX + seg1.getIntercept()));
		
	}
	else if (interPerAndBi[1].getX() < interPerAndBi[2].getX()) { // case 1
		medianBisec = ISegment(seg1.getRight(), seg2.getLeft());
		median = seg1.getRight().getMedian(seg2.getLeft());
		bisector = *medianBisec.getPerLine(median);
		bisector.setStart(bisector.getIntersect(segPerLine[1]));
		bisector.setEnd(bisector.getIntersect(segPerLine[2]));

		leftBisec = ISegment(seg1.getLeft(), seg2.getLeft());
		leftBisecVerLine = *leftBisec.getPerLine(segPerLine[0],true);
		leftPara.setStart(leftBisecVerLine.getRight());
		leftPara.setEnd(bisector.getLeft());
		if (leftBisecVerLine.getRight().getX() == MAXIMUM) leftPara.setStart(leftBisecVerLine.getLeft());

		rightBisec = ISegment(seg1.getRight(), seg2.getRight());
		rightBisecVerLine = *rightBisec.getPerLine(segPerLine[3],false);
		rightPara.setStart(bisector.getRight());
		rightPara.setEnd(rightBisecVerLine.getLeft());
		if (rightBisecVerLine.getLeft().getX() == MAXIMUM) rightPara.setEnd(rightBisecVerLine.getRight());


		leftFocus = seg2.getLeft();
		rightFocus = seg1.getRight();

		leftDirectrix.setStart(seg1.getIntersectPerLine(leftFocus));		
		if (leftFocus.getY() > leftDirectrix.getStart().getY()) tempX = leftDirectrix.getStart().getX() + 1;
		else tempX = leftDirectrix.getStart().getX() - 1;
		leftDirectrix.setEnd(Point(tempX, seg1.getSlope()*tempX + seg1.getIntercept()));

		rightDirectrix.setStart(seg2.getIntersectPerLine(rightFocus));
		if (rightFocus.getY() > rightDirectrix.getStart().getY()) tempX = rightDirectrix.getStart().getX() + 1;
		else tempX = rightDirectrix.getStart().getX() - 1;
		rightDirectrix.setEnd(Point(tempX, seg2.getSlope()*tempX + seg2.getIntercept()));
	}
	else { // case 2,3,4,5
		leftBisec = ISegment(seg1.getLeft(), seg2.getLeft()); // case 2
		if (interPerAndBi[0].getX() > interPerAndBi[2].getX()) {
			leftBisecVerLine = *leftBisec.getPerLine(segPerLine[2],true);
			leftPara.setStart(leftBisecVerLine.getRight());
			leftPara.setEnd(interPerAndBi[0]);
			bisector.setStart(interPerAndBi[0]);

			leftFocus = seg1.getLeft();
			leftDirectrix.setStart(seg2.getIntersectPerLine(leftFocus));
			if (leftFocus.getY() > leftDirectrix.getStart().getY()) tempX = leftDirectrix.getStart().getX() + 1;
			else tempX = leftDirectrix.getStart().getX() - 1;
			leftDirectrix.setEnd(Point(tempX, seg2.getSlope()*tempX + seg2.getIntercept()));
		}
		else {
			leftBisecVerLine = *leftBisec.getPerLine(segPerLine[0],false);
			leftPara.setStart(leftBisecVerLine.getRight());
			leftPara.setEnd(interPerAndBi[2]);
			bisector.setStart(interPerAndBi[2]);

			leftFocus = seg2.getLeft();
			leftDirectrix.setStart(seg1.getIntersectPerLine(leftFocus));
			if (leftFocus.getY() > leftDirectrix.getStart().getY()) tempX = leftDirectrix.getStart().getX() + 1;
			else tempX = leftDirectrix.getStart().getX() - 1;
			leftDirectrix.setEnd(Point(tempX, seg1.getSlope()*tempX + seg1.getIntercept()));
		}
		if (leftBisecVerLine.getRight().getX() == MAXIMUM) leftPara.setStart(leftBisecVerLine.getLeft());

		rightBisec = ISegment(seg1.getRight(), seg2.getRight());
		if (interPerAndBi[1].getX() > interPerAndBi[3].getX()) {
			rightBisecVerLine = *rightBisec.getPerLine(segPerLine[1],true);
			rightPara.setStart(interPerAndBi[3]);
			rightPara.setEnd(rightBisecVerLine.getLeft());
			bisector.setEnd(interPerAndBi[3]);

			rightFocus = seg2.getRight();
			rightDirectrix.setStart(seg1.getIntersectPerLine(rightFocus));
			if (rightFocus.getY() > rightDirectrix.getStart().getY()) tempX = rightDirectrix.getStart().getX() + 1;
			else tempX = rightDirectrix.getStart().getX() - 1;
			rightDirectrix.setEnd(Point(tempX, seg1.getSlope()*tempX + seg1.getIntercept()));
		}
		else { // case4
			rightBisecVerLine = *rightBisec.getPerLine(segPerLine[3],false);
			rightPara.setStart(interPerAndBi[1]);
			rightPara.setEnd(rightBisecVerLine.getLeft());
			bisector.setEnd(interPerAndBi[1]);

			rightFocus = seg1.getRight();
			rightDirectrix.setStart(seg2.getIntersectPerLine(rightFocus));
			if (rightFocus.getY() > rightDirectrix.getStart().getY()) tempX = rightDirectrix.getStart().getX() + 1;
			else tempX = rightDirectrix.getStart().getX() - 1;
			rightDirectrix.setEnd(Point(tempX, seg2.getSlope()*tempX + seg2.getIntercept()));
		}
		if (rightBisecVerLine.getLeft().getX() == MAXIMUM) rightPara.setEnd(rightBisecVerLine.getRight());
	}

	Edge(leftPara.getStart(), leftPara.getEnd(),leftFocus, leftDirectrix.getStart(), leftDirectrix.getEnd());
	Edge(rightPara.getStart(), rightPara.getEnd(), rightFocus, rightDirectrix.getStart(), rightDirectrix.getEnd());
	result.push_back(Edge(leftBisecVerLine.getStart(), leftBisecVerLine.getEnd()));
	result.push_back(Edge(bisector.getStart(), bisector.getEnd()));
	result.push_back(Edge(rightBisecVerLine.getStart(), rightBisecVerLine.getEnd()));
	
	return result;
}

vector<ISegment> makeParabola(ISegment lineParabola, Point focus, ISegment directrix, bool direction) {
	vector<ISegment> result;
	Point* start;
	Point* end;
	if (lineParabola.getStart().computeDist(focus) < lineParabola.getEnd().computeDist(focus)) {
		start = new Point(lineParabola.getStart());
		end = new Point(lineParabola.getEnd());
	}
	else {
		end = new Point(lineParabola.getStart());
		start = new Point(lineParabola.getEnd());
	}

	double translation[2];
	double rotation;
	bool reverse = false;
	ISegment temp = *directrix.getPerLine(focus);
	translation[0] = temp.getStart().getX();
	translation[1] = temp.getStart().getY();
	
	Point t_focus;
	t_focus.translation(translation);
	if (t_focus.getX() < 0) reverse = true;
	if (reverse) rotation = (0.5*PI) + atan(t_focus.getY() / t_focus.getX());
	else rotation = (1.5*PI) + atan(t_focus.getY() / t_focus.getX());
	t_focus.rotation(rotation);
	Point t_endX = directrix.getPerLine(end)->getStart();
	t_endX.translation(translation);
	t_endX.rotation(rotation);
	

	vector<Point> points;
	vector<int> xArray;
	if (t_endX.getX() < 0) {
		int x = -PARABOLA_LENGTH;
		while (x > t_endX.getX()) {
			xArray.push_back(x);
			x -= PARABOLA_LENGTH;
		}
	}
	else {
		int x = PARABOLA_LENGTH;
		while (x < t_endX.getX()) {
			xArray.push_back(x);
			x += PARABOLA_LENGTH;
		}
	}

	for (int i = 0; i < xArray.size(); i++) {
		Point p = Point(xArray[i],(pow(xArray[i],2)/(2*t_focus.getY()))+(t_focus.getY()/2));
		points.push_back(p);
	}

	translation[0] = -(translation[0]);
	translation[1] = -(translation[1]);
	rotation = -(rotation);

	vector<Point> t_points;
	if (direction) {
		t_points.push_back(end);
		for (unsigned int i = 0; i < points.size(); i++) {
			Point tempP = points[i];
			tempP.translation(translation);
			tempP.rotation(rotation);
		}
		t_points.push_back(start);
	}
	else {
		t_points.push_back(start);
		for (int i = points.size()-1; i >= 0; i--) {
			Point tempP = points[i];
			tempP.translation(translation);
			tempP.rotation(rotation);
		}
		t_points.push_back(end);
	}

	for (int i = 0; i < t_points.size()-1; i++) {
		temp = ISegment(t_points[i], t_points[i + 1]);
		result.push_back(temp);
	}

	return result;
}
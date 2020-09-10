#include "Segment.h"
#include <cmath>
#include "MACRO.h"

ISegment::ISegment(){}

ISegment::ISegment(Point s, Point e) {
	start = s;
	end = e;
	computeSlope();
	computeIntercept();
}

ISegment::ISegment(ISegment* s) {
	start = s->getStart();
	end = s->getEnd();
	computeSlope();
	computeIntercept();
}

ISegment::~ISegment() {}

void ISegment::setStart(Point s) {
	start = s;
}
void ISegment::setEnd(Point e) {
	end = e;
}

void ISegment::setSlope(double s) {
	slope = s;
}
void ISegment::setIntercept(double i) {
	intercept = i;
}

void ISegment::computeSlope() {
	if (abs(start.getX() - end.getX()) < DOUBLE_EPS) {
		end.setX(end.getX() + DOUBLE_EPS);
	}
	slope = (start.getY() - end.getY()) / (start.getX() - end.getX());
}

void ISegment::computeIntercept() {
	intercept = start.getY() - slope * start.getX();
}

Point ISegment::getStart() {
	return start;
}

Point ISegment::getEnd() {
	return end;
}

Point ISegment::getLeft() {
	if (start.getX() < end.getX()) return start;
	else return end;
}

Point ISegment::getRight() {
	if (start.getX() > end.getX()) return start;
	else return end;
}

Point ISegment::getTop() {
	if (start.getY() > end.getY()) return start;
	else return end;
}

Point ISegment::getBottom() {
	if (start.getY() < end.getY()) return start;
	else return end;
}

double ISegment::getSlope() {
	return slope;
}
double ISegment::getIntercept() {
	return intercept;
}

double ISegment::computeX(double y) {
	return (y - intercept) / slope;
}

double ISegment::computeY(double x) {
	return slope * x + intercept;
}

ISegment* ISegment::getBisector(ISegment seg) {
	Point a = getIntersect(seg);
	Point b = getBisecPoint(seg, a);
	ISegment* result = new ISegment(a,b);

	return result;
}

ISegment* ISegment::getPerLine(Point p) {
	ISegment* result = new ISegment;
	result->setSlope(-1.0 / slope);
	result->setStart(p);
	result->getEnd().setX(p.getX() + 1);
	result->getEnd().setY(p.getY() + result->getSlope());
	result->computeIntercept();

	return result;
}

ISegment* ISegment::getPerLine(ISegment seg, bool direction) {
	ISegment* result = new ISegment;
	Point median = getLeft().getMedian(getRight());
	result->setStart(median);
	result->setSlope(-1.0 / slope);
	result->setEnd(Point(median.getX() + 1, median.getY() + result->getSlope()));
	result->computeIntercept();
	double tempX;
	double tempY;
	double maxY;
	Point startPoint = result->getIntersect(seg);
	Point infPoint;
	if (startPoint.getX() < median.getX()) {
		if (abs(result->slope - 1) < 0.1) {
			if (result->slope > 0) maxY = (-MAXIMUM);
			else maxY = (MAXIMUM);
			tempX = result->computeX(maxY);
			tempY = result->computeY(-MAXIMUM);
			if (tempX < -MAXIMUM) infPoint.setPos(-MAXIMUM, tempY);
			else infPoint.setPos(tempX, maxY);

		}
		else if (abs(result->slope) < 1) infPoint.setPos(-MAXIMUM, computeY(-MAXIMUM));
		else {
			if (result->slope > 0) infPoint.setPos(computeX(-MAXIMUM), computeY(-MAXIMUM));
			else infPoint.setPos(computeX(MAXIMUM), computeY(MAXIMUM));
		}
	}
	else {
		if (abs(result->slope - 1) < 0.1) {
			if (result->slope > 0) maxY = (-MAXIMUM);
			else maxY = (MAXIMUM);
			tempX = result->computeX(maxY);
			tempY = result->computeY(-MAXIMUM);
			if (tempX < -MAXIMUM) infPoint.setPos(-MAXIMUM, tempY);
			else infPoint.setPos(tempX, maxY);

		}
		else if (abs(result->slope) < 1) infPoint.setPos(-MAXIMUM, computeY(-MAXIMUM));
		else {
			if (result->slope > 0) infPoint.setPos(computeX(-MAXIMUM), computeY(-MAXIMUM));
			else infPoint.setPos(computeX(MAXIMUM), computeY(MAXIMUM));
		}
	}
	if (direction) {
		result->setStart(infPoint);
		result->setEnd(startPoint);
	}
	else {
		result->setStart(startPoint);
		result->setEnd(infPoint);
	}

	return result;
}

Point* ISegment::getIntersectPerLine(Point p) {
	ISegment PerLine;
	PerLine.setSlope(-1.0 / slope);
	PerLine.setStart(p);
	PerLine.getEnd().setX(p.getX() + 1);
	PerLine.getEnd().setY(p.getY() + PerLine.getSlope());
	PerLine.computeIntercept();

	double x, y;
	x = (PerLine.getIntercept() - intercept) / (2 * slope);
	y = slope * x + intercept;
	
	Point *result = new Point(x, y);
	return result;
}

Point* ISegment::getIntersect(ISegment seg) {  // Compute intersection point with another ISegment
	double x = -1 * (intercept - seg.getIntercept()) / (slope - seg.getSlope());
	double y = slope * x + intercept;
	Point* result = new Point(x, y);
	return result;
}

Point* ISegment::getBisecPoint(ISegment seg, Point p) {
	double a = getLeft().computeDist(p);
	double b = seg.getLeft().computeDist(p);
	Point* result = new Point((getLeft().getX() * b / (a + b)) + (seg.getLeft().getX() * a / (a + b)),
		(getLeft().getY() * b / (a + b)) + (seg.getLeft().getY() * a / (a + b)));

	return result;
}

Point* ISegment::getMedian() {
	Point* result = new Point(getLeft().getMedian(getRight()));
	return result;
}

int ISegment::ccw(Point p) {
	return getLeft().ccw(getRight(), p);
}

bool ISegment::isOnSeg(Point p) {
	if (p.getX() < getLeft().getX()) return false;
	else if (p.getX() > getRight().getX()) return false;
	return true;
}

ISegment ISegment::operator =(ISegment s) {
	start = s.getStart();
	end = s.getEnd();
	slope = s.getSlope();
	return this;
}
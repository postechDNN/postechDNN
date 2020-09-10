#pragma once

#include "Point.h"

class ISegment {
private:
	Point start;
	Point end;
	double slope;
	double intercept;

public:
	ISegment();
	ISegment(Point s, Point e);
	ISegment(ISegment*);
	~ISegment();

	void setStart(Point s);
	void setEnd(Point e);
	void setSlope(double s);
	void setIntercept(double i);

	void computeSlope();
	void computeIntercept();

	Point getStart();
	Point getEnd();
	Point getLeft();
	Point getRight();
	Point getTop();
	Point getBottom();
	double getSlope();
	double getIntercept();

	double computeX(double y);
	double computeY(double x);
	

	ISegment* getBisector(ISegment seg);
	ISegment* getPerLine(Point p); // Compute perpendicular line
	ISegment* getPerLine(ISegment seg, bool direction);
	Point* getIntersectPerLine(Point p);
	Point* getIntersect(ISegment seg); // Compute intersection point with another segment
	Point* getBisecPoint(ISegment seg, Point p);
	Point* getMedian();
	
	

	int ccw(Point p);

	bool isOnSeg(Point p);

	ISegment operator =(ISegment e);
};
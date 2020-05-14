#ifndef __POLYGONCLIPPING_H__
#define __POLYGONCLIPPING_H__

#include "Point.h"
#include "Polygon.h"
#include <list>
#include <iostream>
#include <vector>

enum class __Type {NONE = 0,CROSSING, BOUNCING,DELAYED};
enum class __Delayed {NONE = 0,LEFT,RIGHT,ON};

struct __SubLabel {
	__Type type;
	__Delayed d1, d2;

	__SubLabel() :type(__Type::NONE), d1(__Delayed::NONE), d2(__Delayed::NONE) {};
};

struct __Node {
	Point p;
	bool intersect;
	int label;	//0 : None, -1 : Exit, 1 : Entry
	bool visit;
	__SubLabel sub_label;
	std::list<__Node>::iterator link;
	__Node(Point p, bool intersect) :p(p), intersect(intersect) { label = 0;sub_label = __SubLabel(); visit = false; }
};

class PolygonClipping{
private:
	inline double A_func(Point P, Point Q, Point R);
	inline void circularNext(std::list<__Node>& l, std::list<__Node>::iterator &it);
	inline void circularPrev(std::list<__Node>& l, std::list<__Node>::iterator &it);
	void intersection_phase(std::list<__Node>& P, std::list<__Node>& Q);
	void intersection_case(std::list<__Node>& P, std::list<__Node>::iterator e_p, std::list<__Node>& Q, std::list<__Node>::iterator e_q);
	void overlap_case(std::list<__Node>& P, std::list<__Node>::iterator e_p, std::list<__Node>& Q, std::list<__Node>::iterator e_q);
	int labeling_phase(SimplePolygon& poly_Q,std::list<__Node>& P, std::list<__Node>& Q);
	int turn_direction(Point Q, Point P1, Point P2, Point P3);
	__SubLabel sublabelling(Point P1, Point P2, Point I, Point Q1, Point Q2);
	std::vector<SimplePolygon*> tracing_phase(std::list<__Node>& P, std::list<__Node>& Q);
public:
	PolygonClipping();
	~PolygonClipping();
	std::vector<SimplePolygon*> clipping(SimplePolygon& P, SimplePolygon& Q);
};

#endif
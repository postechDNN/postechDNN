#pragma once

#include <vector>
#include "ipegeo.h"
#include "ipeshape.h"
#include "ipelet.h"


// Determine whether input bezier has a self intersection 
bool bezierSI(const ipe::Bezier *bezier);

// Compute intersection between two open CurveSegments
bool interOpenCS(const ipe::CurveSegment *cs1, const ipe::CurveSegment* cs2, std::vector<ipe::Vector> &res);

// Determine whether two open CurveSegments has operlappping
bool overlapCS(const ipe::CurveSegment* cs1, const ipe::CurveSegment* cs2, std::vector<ipe::Vector>& res);

// Align set CCW
bool alignCS(std::vector<ipe::CurveSegment> &res, const std::vector<ipe::CurveSegment> *input);

// Determine whether input set is simple
bool isSimple(ipe::IpeletData *data,  const std::vector<ipe::CurveSegment> *input);

// Divide CurveSegment and Return intersection idx
bool cutCS(const std::vector<ipe::CurveSegment> *cs1, const std::vector<ipe::CurveSegment> *cs2,
	std::vector<ipe::CurveSegment>& res, std::vector<int>& idx);

// Return ccw 1:ccw, 0:colinear -1:cw
inline int ccw(const ipe::Vector a, const  ipe::Vector b, const  ipe::Vector c);

// Return ccw 1:ccw, 0:colinear -1:cw
inline int ccw(const double ax, const  double ay, const  double bx,
	const double by, const  double cx, const  double cy);

inline bool eqv(const ipe::Vector a, const  ipe::Vector b);

inline double dist(const ipe::Vector a, const ipe::Vector b);

void splitBez(const ipe::Bezier* const bez, ipe::Bezier& left, ipe::Bezier& right, const double para);

// Return the number of intersection points
int verticalRay(const ipe::Vector p, const std::vector<ipe::CurveSegment>* const cs);

void divideRaw(const std::vector<ipe::CurveSegment>* const raw, std::vector<std::vector<ipe::CurveSegment>> &result);


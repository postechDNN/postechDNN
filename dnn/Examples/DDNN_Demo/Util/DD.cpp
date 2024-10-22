#include "dd.h"


ConvexDistPoint::ConvexDistPoint(double x, double y) {
	pos[0] = x;
	pos[1] = y;
}

bool ConvexDistPoint::operator<(const ConvexDistPoint& p2) {
	if (pos[0] == p2.pos[0] && pos[1] == p2.pos[1]) return false;
	if (pos[0] >= 0 && p2.pos[0] < 0) return true;
	else if (pos[0] < 0 && p2.pos[0] >= 0) return false;
	else if (pos[0] >= 0 && p2.pos[0] >= 0) {
		if (pos[1] * p2.pos[0] < p2.pos[1] * pos[0]) return true;
		return false;
	}
	else {
		if (pos[1] * p2.pos[0] < p2.pos[1] * pos[0]) return true;
		return false;
	}
}

ConvexDistPoint ConvexDistPoint::operator-(const ConvexDistPoint& p2) {
	ConvexDistPoint result(pos[0] - p2.pos[0], pos[1] - p2.pos[1]);

	return result;
}

void ConvexDistance::ResetDist(std::vector<ConvexDistPoint>& dist) {
	if (distPolygon.size() != 0) distPolygon.clear();
	distPolygon = dist;
	std::sort(distPolygon.begin(), distPolygon.end());
}

double ConvexDistance::GetDist(ConvexDistPoint& p1, ConvexDistPoint& p2) {
	ConvexDistPoint Qp = p1 - p2;
	if (Qp.pos[0] == 0 && Qp.pos[1] == 0) return 0;

	std::vector<ConvexDistPoint>::iterator distIt = std::lower_bound(distPolygon.begin(), distPolygon.end(), Qp);
	std::vector<ConvexDistPoint>::iterator distBt;

	if (distIt == distPolygon.end()) {
		distIt = distPolygon.begin();
	}

	if (distIt == distPolygon.begin()) {
		distBt = distPolygon.end() - 1;
	}
	else {
		distBt = distIt - 1;
	}
	double upflo = Qp.pos[0] * (distIt->pos[1] - distBt->pos[1]) - Qp.pos[1] * (distIt->pos[0] - distBt->pos[0]);
	double doflo = distBt->pos[1] * distIt->pos[0] - distBt->pos[0] * distIt->pos[1];
	return -upflo / doflo;
}


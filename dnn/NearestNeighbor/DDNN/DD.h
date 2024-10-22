#pragma once
#include <vector>
#include <algorithm>


class ConvexDistPoint {
public:
	double pos[2];
	ConvexDistPoint(double x, double y);
	bool operator<(const ConvexDistPoint& p2);
	ConvexDistPoint operator-(const ConvexDistPoint& p2);
};


class ConvexDistance {
public:
	std::vector<ConvexDistPoint> distPolygon;


	ConvexDistance(std::vector<ConvexDistPoint>& dist) {
		distPolygon = dist;
		std::sort(distPolygon.begin(), distPolygon.end());
	}

	~ConvexDistance() {
		if (!distPolygon.empty()) distPolygon.clear();
	}

	void ResetDist(std::vector<ConvexDistPoint>& dist);
	double GetDist(ConvexDistPoint& p1, ConvexDistPoint& p2);
};

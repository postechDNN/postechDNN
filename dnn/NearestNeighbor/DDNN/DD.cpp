#pragma once
#include <vector>
#include <algorithm>

class ConvexDistPoint {
	public :
		double pos[2];
		bool operator<(ConvexDistPoint &p2) {
			if (pos[0] == p2.pos[0] && pos[1] == p2.pos[1]) return false;
			if (pos[0] >= 0 && p2.pos[0] < 0) return true;
			else if (pos[0] < 0 && p2.pos[0] >= 0) return false;
			else if (pos[0] >= 0 && p2.pos[0] >= 0) {
				if (pos[1] * p2.pos[0] < p2.pos[1] * pos[0]) return true;
				return false;
			}
			else {
				if (pos[1] * p2.pos[0] > p2.pos[1] * pos[0]) return true;
				return false;
			}
		}
		
		ConvexDistPoint operator-(ConvexDistPoint &p2) {
			ConvexDistPoint result;
			result.pos[0] = pos[0] - p2.pos[0];
			result.pos[1] = pos[1] - p2.pos[1];
			return result;
		}

};

class ConvexDistance {
	std::vector<ConvexDistPoint> distPolygon;

	public :
		ConvexDistance(std::vector<ConvexDistPoint>& dist) {
			distPolygon = dist;
			std::sort(distPolygon.begin(), distPolygon.end());
		}
		void ResetDist(std::vector<ConvexDistPoint>& dist) {
			if (distPolygon.size() != 0) distPolygon.clear();
			distPolygon = dist;
			std::sort(distPolygon.begin(), distPolygon.end());
		}
		double GetDist(ConvexDistPoint& p1, ConvexDistPoint& p2) {
			ConvexDistPoint Qp = p1 - p2;
			std::vector<ConvexDistPoint>::iterator distIt = std::lower_bound(distPolygon.begin(), distPolygon.end(), Qp);
			std::vector<ConvexDistPoint>::iterator distbt;
			if (distIt == distPolygon.begin()) {
				distbt = std::prev(distPolygon.end());
			}
			else {
				distbt = std::prev(distIt);
			}
			return 0;
		}
};

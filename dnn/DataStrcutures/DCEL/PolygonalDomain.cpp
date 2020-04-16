#include "PolygonalDomain.h"
#include <vector>

PolygonalDomain::PolygonalDomain() {
}

PolygonalDomain::PolygonalDomain(std::vector<SimplePolygon*>* spv) {
}
PolygonalDomain::~PolygonalDomain() {

}
bool PolygonalDomain::validPoint(Point p) {
	return true;
}
std::vector<SimplePolygon*>* PolygonalDomain::getObstacles() {
	return this->obstacles
}
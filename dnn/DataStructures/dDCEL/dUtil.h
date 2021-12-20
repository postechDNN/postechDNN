#ifndef DDCEL_UTIL
#define DDCEL_UTIL

#include <vector>
#include <cassert>
#include <iterator>
class dPoint;
class dEdge;

constexpr double ERR = 1e-6;

template <typename BinaryOperation>
dPoint* pts_fcn(dPoint* p1, dPoint* p2, BinaryOperation binary_op) {
	assert(p1->getd() == p2->getd());
	std::vector<double> *tmp = new std::vector<double>;
	tmp->reserve(p1->getd());
	transform(p1->cbegin(), p1->cend(), p2->cbegin(),
		back_inserter(*tmp), binary_op);
	return new dPoint(tmp);
}

dPoint* pts_sum(dPoint*, dPoint*);
dPoint* pts_dif(dPoint*, dPoint*);
bool same_vec(dPoint*, dPoint*);

bool dCoplanar(dEdge*, dEdge*);
bool dCoplanar(std::vector<dEdge*>);
bool dCoplanar(dPoint*, dPoint*, dPoint*, dPoint*);
bool dCoplanar(std::vector<dPoint*>);

dPoint normal3d(dPoint*, dPoint*, dPoint*);

//require {std::min, std::max, <} operations
template <typename T>
bool is_between(T a, T b, T x) {
	if (x < std::min(a, b) || std::max(a, b) < x) return false;
	return true;
}

#endif
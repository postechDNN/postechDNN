#include "KDtree.h"
#include <vector>

using namespace std;

double distanceBetweenCenters(KDtreeNode* c1, KDtreeNode* c2) {
	if (c1->p == nullptr || c2->p == nullptr) return 0.0;
	// else distance(c1->p->
}

template <typename T>
vector<T*> vectorSlice(vector<T*> vec, int a, int b) {
	return vector<T*>(vec.begin() + a, vec.begin() + b);
}

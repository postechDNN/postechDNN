#include "P.h"

void P::binarySearch(vector<Q*> Qpis, int l, int r) {
	if (l >= r) {
		if (Qpis[l]->x > this->x) this->tau = l;
		else this->tau = l + 1;
		return;
	}

	int m = (l + r) / 2;

	if (Qpis[m]->x == this->x) {
		this->tau = m;
		return;
	}
	else if (Qpis[m]->x > this->x) {
		binarySearch(Qpis, l, m - 1);
	}
	else if (Qpis[m]->x < this->x) {
		binarySearch(Qpis, m + 1, r);
	}
}
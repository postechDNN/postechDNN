#include <cmath>
#include <algorithm>
#include <iostream>
#include "propagation.h"

pair<double, double> Segment::Vinterval(int i, int i1, int lindex)
{
	Segment l1 = *Adjs[lindex].first;
	if (abs(dist[i] - dist[i1]) < EPS)
	{
		double t = InnerProd(b - a, a + ((b - a) * (X[i] + X[i1])) / 2 - l1.a) / InnerProd(b - a, l1.b - l1.a);
	}
	return pair<double, double>();
}

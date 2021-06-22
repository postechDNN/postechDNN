#include <cmath>
#include <algorithm>
#include <iostream>
#include "propagation.h"

//dist[i]>=dist[i1] 이고 Segment l위의 site가 X[i] 와 X[i1]에 있을 때, X[i]가 차지하는 l1위의 구간을 구함
pair<double, double> Segment::Vinterval(int i, int i1, int lindex)
{
	Segment l1 = *Adjs[lindex].first;
	double c = dist[i] - dist[i1];
	if (abs(c) < EPS)
	{
		double t = InnerProd(b - a, a + ((b - a) * (X[i] + X[i1])) / 2 - l1.a) / InnerProd(b - a, l1.b - l1.a);
		if (InnerProd(b - a, l1.b - l1.a) > 0) 
			return pair<double, double>(t,1);
		else
			return pair<double, double>(0,t);
	}
	double Bbar = (X[i] - X[i1]) * InnerProd(b - a, (l1.a - a) * 2 - (b - a) * (X[i] + X[i1])) - c * c;
	double A = 4 * InnerProd(l1.b - l1.a, l1.b - l1.a) * (c * c - (X[i] - X[i1]) * (X[i] - X[i1]) * InnerProd(b - a, b - a));
	double B = 8 * c * c * InnerProd(l1.b - l1.a, l1.a - a - (b - a) * X[i]) - 4 * (X[i] - X[i1]) * InnerProd(b - a, l1.b - l1.a) * Bbar;
	double C = 4 * c * c * InnerProd(l1.a - a - (b - a) * X[i], l1.a - a - (b - a) * X[i]) - Bbar * Bbar;

	double D = B * B - 4 * A * C;
	if (D < EPS)
		return pair<double, double>(1, 0);
	if (abs(A) < EPS)
	{
		double t = -C / B;
		if (!IsVvertex(i,i1,l1,t))
			return pair<double, double>(0,1);
		if (IsVvertex(i, i1, l1, t + 1))
			return pair<double, double>(t, 1);
		return pair<double, double>(0, t);
	}
	double t1 = (-B - sqrt(D)) / (2 * A);
	double t2 = (-B + sqrt(D)) / (2 * A);
	bool val1 = IsVvertex(i, i1, l1, t1);
	bool val2 = IsVvertex(i, i1, l1, t2);
	if (val1 && val2)
		return pair<double, double>(max(0., t1), min(1., t2));
	else if (!val1 && val2)
		return pair<double, double>(t2, 1);
	else if (val1 && !val2)
		return pair<double, double>(0, t1);
	else
		return pair<double, double>(1, 0);
}

bool Segment::IsVvertex(int i, int i1, Segment& l1, double t)
{
	MyVec tv = l1.a * (1 - t) + l1.b * t;
	MyVec v = l1.a * (1 - t) + l1.b * t;
	MyVec v1 = l1.a * (1 - t) + l1.b * t;
	return abs(VecSize(tv - v) + dist[i] - VecSize(tv - v1) - dist[i1]) < EPS;
}

bool Segment::IsContain(int i, int i1, Segment& l1, double t)
{
	MyVec tv = l1.a * (1 - t) + l1.b * t;
	MyVec v = l1.a * (1 - t) + l1.b * t;
	MyVec v1 = l1.a * (1 - t) + l1.b * t;
	return VecSize(tv - v) + dist[i] < VecSize(tv - v1) + dist[i1];
}

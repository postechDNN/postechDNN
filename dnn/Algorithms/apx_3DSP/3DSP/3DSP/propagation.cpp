#include <cmath>
#include <algorithm>
#include <iostream>
#include "propagation.h"

void Segment::SetAdjDiagram()
{
	for (size_t i = 0; i < X.size(); i++)
	{
		for (size_t j = 0; j < Adjs.size(); j++)
		{
			Segment l1 = *Adjs[j].first;
			if (Adjs[j].second == -1)
			{
				AdjDiagram[i][j] = pair<double, double>(0, 1);
				continue;
			}
			Tri f = Tris[Adjs[j].second];
			vector<MyVec> v;
			v.push_back(f.p1);
			v.push_back(f.p2);
			v.push_back(f.p3);
			auto res = pair<double, double>(0., 1.);
			for (size_t k = 0; k < 3; k++)
			{
				auto temp = Interval(i, v[k], v[(k + 1) % 3], v[(k + 2) % 3], f, l1);
				res = pair<double, double>(max(res.first, temp.first), min(res.second, temp.second));
			}
			AdjDiagram[i][j] = res;
		}
	}
}

pair<double, double> Segment::Interval(int i,MyVec& v1, MyVec& v2, MyVec& v3, Tri& f, Segment& l1)
{
	MyVec v = (b - a) * X[i] + a;
	MyVec n = OuterProd(v1 - v, v2 - v);
	if (abs(InnerProd(n, l1.b - l1.a)) < EPS)
	{
		if (InnerProd(n, v3 - v) * InnerProd(n, l1.a - v) > 0)
			return pair<double, double>(0., 1.);
		else
			return pair<double, double>(1., 0.);
	}
	double t = InnerProd(v - l1.a, n) / InnerProd(l1.b - l1.a, n);
	if (InnerProd(l1.b - l1.a, n) * InnerProd(v3 - v, n) > 0)
		return pair<double, double>(t, 1.);
	else
		return pair<double, double>(0., t);
}

void Segment::SetNear()
{
	for (size_t i = 0; i < X.size(); i++)
	{
		for (size_t j = 0; j < Adjs.size(); j++)
		{
			Segment l1 = *Adjs[j].first;
			MyVec v = (b - a) * X[i] + a;
			double t = InnerProd(l1.a - l1.b, l1.a - v) / InnerProd(l1.b - l1.a, l1.b - l1.a);
			Near[i][j] = min(max(t, 0.), 1.);
		}
	}
}

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
		return pair<double, double>(1., 0.);
	if (abs(A) < EPS)
	{
		double t = -C / B;
		if (!IsVvertex(i, i1, l1, t))
			return pair<double, double>(0., 1.);
		if (IsVvertex(i, i1, l1, t + 1))
			return pair<double, double>(t, 1.);
		return pair<double, double>(0., t);
	}
	double t1 = (-B - sqrt(D)) / (2 * A);
	double t2 = (-B + sqrt(D)) / (2 * A);
	bool val1 = IsVvertex(i, i1, l1, t1);
	bool val2 = IsVvertex(i, i1, l1, t2);
	if (val1 && val2)
		return pair<double, double>(max(0., t1), min(1., t2));
	else if (!val1 && val2)
		return pair<double, double>(t2, 1.);
	else if (val1 && !val2)
		return pair<double, double>(0., t1);
	else
		return pair<double, double>(1., 0.);
}


bool Segment::IsActive(int i)
{
	return static_cast<bool>(S.count(i));
}

void Segment::UpdateV(int i, int lindex)
{
	Segment l1 = *Adjs[lindex].first;
	set<pair<double, int>>& addV = AddVoronoi[lindex];
	// k번째 원소 O(log n)에 계산가능한 tree 구현해서 수정해야 함
	vector<pair<double, int>> vList;
	vList.assign(addV.begin(), addV.end());

	int start = 0;
	int end = addV.size() - 1;
	int i2;
	double x1, x2;
	while (true)
	{
		if (end - start < 2)
		{
			pair<double, double> intv(0, 1);
			for (int j = max(0,start-1); j <= min(end, static_cast<int>(addV.size() - 1)); j++)
			{
				pair<double, double> temp = Vinterval(i, vList[j].second, lindex);
				intv.first = max(intv.first, temp.first);
				intv.second = min(intv.second, temp.second);
			}
			if (intv.first > intv.second)
				return;
			x1 = intv.first;
			break;
		}
		int mid = (start + end) / 2;
		int mi = vList[mid].second;
		auto D = [=](int n)->double {
			return dist[n] + VecSize(l1.a + (l1.b - l1.a) * l1.X[mid] - a - (b - a) * X[n]);
		};
		if (D(mi) > D(i))
		{
			end = mid;
			continue;
		}
		pair<double, double> intv(0, 1);
		for (int j = mid-1; j < mid+1; j++)
		{
			pair<double, double> temp = Vinterval(i, vList[j].second, lindex);
			intv.first = max(intv.first, temp.first);
			intv.second = min(intv.second, temp.second);
		}
		if (intv.first > intv.second)
			return ;
		if (vList[mid].first > intv.first)
			end = mid;
		else
			start = mid;
	}
	while (true)
	{
		if (end - start < 2)
		{
			pair<double, double> intv(0, 1);
			for (int j = max(0, start - 1); j <= min(end, static_cast<int>(addV.size()-1)); j++)
			{
				pair<double, double> temp = Vinterval(i, vList[j].second, lindex);
				intv.first = max(intv.first, temp.first);
				intv.second = min(intv.second, temp.second);
			}
			if (intv.first > intv.second)
				return;
			x2 = intv.second;
			i2 = vList[start].second;
			break;
		}
		int mid = (start + end) / 2;
		int mi = vList[mid].second;
		auto D = [=](int n)->double {
			return dist[n] + VecSize(l1.a + (l1.b - l1.a) * l1.X[mid] - a - (b - a) * X[n]);
		};
		if (D(mi) > D(i))
		{
			start = mid;
			continue;
		}
		pair<double, double> intv(0., 1.);
		for (int j = mid - 1; j < mid + 1; j++)
		{
			pair<double, double> temp = Vinterval(i, vList[j].second, lindex);
			intv.first = max(intv.first, temp.first);
			intv.second = min(intv.second, temp.second);
		}
		if (intv.first > intv.second)
			return;
		if (vList[mid].first > intv.second)
			end = mid;
		else
			start = mid;
	}
	
	auto it = addV.lower_bound(pair<double, int>(x1, 0));
	while (it != addV.lower_bound(pair<double, int>(x2, sizeX())))
		it = addV.erase(it);
	addV.emplace(x1, i);
	addV.emplace(x2, i2);
	return ;
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

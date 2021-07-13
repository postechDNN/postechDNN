#include <cmath>
#include <algorithm>
#include <iostream>
#include <iterator>
#include "propagation.h"

#include <random>

void Segment::SetAdjDiagram()
{
	for (size_t i = 0; i < X.size(); i++)
	{
		for (size_t j = 0; j < Adjs.size(); j++)
		{
			Segment& l1 = *Adjs[j].first;
			if (Adjs[j].second == -1)
			{
				AdjDiagram[i][j] = { 0, 1 };
				continue;
			}
			Tri f = Tris[Adjs[j].second];
			vector<MyVec> v;
			v.push_back(f.p1);
			v.push_back(f.p2);
			v.push_back(f.p3);
			pair<double, double> res(0., 1.);
			for (size_t k = 0; k < 3; k++)
			{
				auto temp = Interval(i, v[k], v[(k + 1) % 3], v[(k + 2) % 3], f, l1);
				res = { max(res.first, temp.first), min(res.second, temp.second) };
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
			return { 0., 1. };
		else
			return { 1., 0. };
	}
	double t = InnerProd(v - l1.a, n) / InnerProd(l1.b - l1.a, n);
	if (InnerProd(l1.b - l1.a, n) * InnerProd(v3 - v, n) > 0)
		return { t, 1. };
	else
		return { 0., t };
}

void Segment::SetNear()
{
	for (size_t i = 0; i < Adjs.size(); i++)
	{
		for (size_t j = 0; j < X.size(); j++)
		{
			Segment& l1 = *Adjs[i].first;
			MyVec v = (b - a) * X[j] + a;
			double t = InnerProd(l1.a - l1.b, l1.a - v) / InnerProd(l1.b - l1.a, l1.b - l1.a);
			Near[i][j] = min(max(t, 0.), 1.);
		}
	}
}

//dist[i]>=dist[i1] 이고 Segment l위의 site가 X[i] 와 X[i1]에 있을 때, X[i]가 차지하는 l1위의 구간을 구함
pair<double, double> Segment::Vinterval(int i, int i1, int lindex)
{
	Segment& l1 = *Adjs[lindex].first;
	double c = dist[i] - dist[i1];
	if (abs(c) < EPS)
	{
		double t = InnerProd(b - a, a + ((b - a) * (X[i] + X[i1])) / 2 - l1.a) / InnerProd(b - a, l1.b - l1.a);
		if (InnerProd(b - a, l1.b - l1.a) > 0)
			return { t,1. };
		else
			return { 0.,t };
	}
	double Bbar = (X[i] - X[i1]) * InnerProd(b - a, (l1.a - a) * 2 - (b - a) * (X[i] + X[i1])) - c * c;
	double A = 4 * InnerProd(l1.b - l1.a, l1.b - l1.a) * (c * c - (X[i] - X[i1]) * (X[i] - X[i1]) * InnerProd(b - a, b - a));
	double B = 8 * c * c * InnerProd(l1.b - l1.a, l1.a - a - (b - a) * X[i]) - 4 * (X[i] - X[i1]) * InnerProd(b - a, l1.b - l1.a) * Bbar;
	double C = 4 * c * c * InnerProd(l1.a - a - (b - a) * X[i], l1.a - a - (b - a) * X[i]) - Bbar * Bbar;

	double D = B * B - 4 * A * C;
	if (D < EPS)
		return { 1., 0. };
	if (abs(A) < EPS)
	{
		double t = -C / B;
		if (!IsVvertex(i, i1, l1, t))
			return { 0., 1. };
		if (IsVvertex(i, i1, l1, t + 1))
			return { t, 1. };
		return { 0., t };
	}
	double t1 = (-B - sqrt(D)) / (2 * A);
	double t2 = (-B + sqrt(D)) / (2 * A);
	bool val1 = IsVvertex(i, i1, l1, t1);
	bool val2 = IsVvertex(i, i1, l1, t2);
	if (val1 && val2)
		return { max(0., t1), min(1., t2) };
	else if (!val1 && val2)
		return { t2, 1. };
	else if (val1 && !val2)
		return { 0., t1 };
	else
		return { 1., 0. };
}


bool Segment::IsActive(int i)
{
	return static_cast<bool>(S.count(X[i]));
}

void Segment::Update(int i, double val, priority_queue<Repr, vector<Repr>>& Reprs)
{
	dist[i] = val;
	for (size_t lindex = 0; lindex < Adjs.size(); lindex++)
		UpdateSeg(i, lindex, Reprs);
	S[X[i]] = i;
	Sbar.erase(X[i]);
}

void Segment::UpdateSeg(int i, int lindex, priority_queue<Repr, vector<Repr>>& Reprs)
{
	Segment& l1 = *Adjs[lindex].first;
	AVLTree<pair<double, int>>& addV = AddVoronoi[lindex];
	if (addV.isEmpty())
	{
		addV.insert({ 0.,-1 });
		addV.insert({ 1.,i });
		return;
	}

	int start = 0;
	int end = addV.size() - 1;
	int i1;
	double x1, x2;
	while (true)
	{
		if (end - start < 2)
		{
			pair<double, double> intv(0., 1.);
			for (int j = max(0, start); j <= min(end + 1, static_cast<int>(addV.size() - 1)); j++)
			{
				auto temp = Vinterval(i, addV.getkthNode(j)->value.second, lindex);
				intv.first = max(intv.first, temp.first);
				intv.second = min(intv.second, temp.second);
			}
			if (intv.first > intv.second)
				return;
			x1 = intv.first;
			break;
		}
		int mid = (start + end) / 2;
		int mi = addV.getkthNode(mid)->value.second;
		auto D = [=](int n)->double {
			return dist[n] + VecSize(l1.a + (l1.b - l1.a) * l1.X[mid] - a - (b - a) * X[n]);
		};
		if (D(mi) > D(i))
		{
			end = mid;
			continue;
		}
		pair<double, double> intv(0., 1.);
		for (int j = mid; j < mid + 2; j++)
		{
			auto temp = Vinterval(i, addV.getkthNode(j)->value.second, lindex);
			intv.first = max(intv.first, temp.first);
			intv.second = min(intv.second, temp.second);
		}
		if (intv.first > intv.second)
			return;
		if (addV.getkthNode(mid)->value.first > intv.first)
			end = mid;
		else
			start = mid;
	}
	while (true)
	{
		if (end - start < 2)
		{
			pair<double, double> intv(0, 1);
			for (int j = max(0, start); j <= min(end + 1, static_cast<int>(addV.size() - 1)); j++)
			{
				auto temp = Vinterval(i, addV.getkthNode(j)->value.second, lindex);
				intv.first = max(intv.first, temp.first);
				intv.second = min(intv.second, temp.second);
			}
			if (intv.first > intv.second)
				return;
			x2 = intv.second;
			break;
		}
		int mid = (start + end) / 2;
		int mi = addV.getkthNode(mid)->value.second;
		auto D = [=](int n)->double {
			return dist[n] + VecSize(l1.a + (l1.b - l1.a) * l1.X[mid] - a - (b - a) * X[n]);
		};
		if (D(mi) > D(i))
		{
			start = mid;
			continue;
		}
		pair<double, double> intv(0., 1.);
		for (int j = mid; j < mid + 2; j++)
		{
			auto temp = Vinterval(i, addV.getkthNode(j)->value.second, lindex);
			intv.first = max(intv.first, temp.first);
			intv.second = min(intv.second, temp.second);
		}
		if (intv.first > intv.second)
			return;
		if (addV.getkthNode(mid)->value.first > intv.second)
			end = mid;
		else
			start = mid;
	}
	if (x1 >= x2)
		return;
	SetReprInv(i, lindex, Reprs);
	auto it = addV.getRightNode({ x1, -1 });
	i1 = it->value.second;
	if (it != addV.getkthNode(0)) //it != addV.begin()
	{
		auto prev = FindRepr(i1, lindex, { addV.getLeftNode(it->value)->value.first,it->value.first }); //{ std::prev(it)->first,it->first }
		if (prev.first != -1 && l1.X[prev.first] > x1)
			SetRepr(i1, lindex, { addV.getLeftNode(it->value)->value.first,x1 }, Reprs);
	}
	auto itEnd = addV.getRightNode({ x2, sizeX() });
	if (itEnd != nullptr)
	{
		auto prev = FindRepr(itEnd->value.second, lindex, { addV.getLeftNode(itEnd->value)->value.first,itEnd->value.first });
		if (prev.first != -1 && l1.X[prev.first] < x2)
			SetRepr(itEnd->value.second, lindex, { x2,itEnd->value.first }, Reprs);
	}
	while (it != addV.getRightNode({ x2, sizeX() }))
	{
		addV.pop(it->value);
		it = addV.getRightNode(it->value);
	}
	addV.insert({ x1, i1 });
	addV.insert({ x2, i });
	SetRepr(i, lindex, { x1,x2 }, Reprs);
	return;
}

pair<int, double> Segment::FindRepr(int i, int lindex, pair<double, double> intv)
{
	Segment& l1 = *Adjs[lindex].first;
	intv = { max(intv.first,AdjDiagram[lindex][i].first), min(intv.second,AdjDiagram[lindex][i].second) };
	auto it1 = l1.Sbar.lower_bound(intv.first);
	auto it2 = l1.Sbar.upper_bound(intv.second);
	if (intv.first > intv.second || it1->first > intv.second || it2 == l1.Sbar.begin())
		return { -1,0. };
	it2--;
	int index;
	auto D = [=](int n)->double {
		return dist[i] + VecSize(l1.a + (l1.b - l1.a) * l1.X[n] - a - (b - a) * X[i]);
	};
	if (Near[lindex][i] <= it1->first)
		index = it1->second;
	else if (Near[lindex][i] >= it2->first)
		index = it2->second;
	else
	{
		it2 = l1.Sbar.lower_bound(Near[lindex][i]);
		if (it2->first == Near[lindex][i])
			index = it2->second;
		else
		{
			it1 = it2;
			it1--;
			if (D(it1->second) > D(it2->second))
				index = it2->second;
			else
				index = it1->second;
		}
	}
	return { index,D(index) };
}

void Segment::SetRepr(int i, int lindex, pair<double, double> intv, priority_queue<Repr, vector<Repr>>& Reprs)
{
	auto res = FindRepr(i, lindex, intv);
	if (res.first == -1)
		return;
	Repr p;
	p.src.index = i;
	p.src.l = this;
	p.dst.l = Adjs[lindex].first;
	p.dst.index = res.first;
	p.val = res.second;
	Reprs.push(p);
}

void Segment::SetReprInv(int i, int lindex, priority_queue<Repr, vector<Repr>>& Reprs)
{
	Segment& l1 = *Adjs[lindex].first;
	auto& addV = l1.AddVoronoi[Revs[lindex]];
	auto it = addV.getRightNode({ X[i],-1 });
	if (i == 0)
		it = addV.getkthNode(1); //std::next(addV.begin());
	if (it == nullptr) //it == addV.end()
		it = addV.getkthNode(addV.size()-1);
	pair<double, double> intv(addV.getLeftNode(it->value)->value.first, it->value.first); //intv(std::prev(it)->first, it->first);
	int index = it->value.second;
	if (i == l1.FindRepr(index, Revs[lindex], intv).first)
	{
		Sbar.erase(X[i]);
		l1.SetRepr(index, Revs[lindex], intv, Reprs);
		Sbar[X[i]] = i;
	}
	return;
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


int main()
{
	std::random_device rd;
	std::mt19937 g(rd());

	AVLTree<int> tree;
	int size = 1000000;
	int intv = 10000;
	vector<int> vec;
	vector<int> evec;
	for (size_t i = 0; i < size; i++)
		vec.push_back(i);
	for (size_t i = 0; i < size / 2; i++)
		evec.push_back(i * 2 + 1);
	shuffle(vec.begin(), vec.end(),g);
	shuffle(evec.begin(), evec.end(), g);
	for (int i:vec)
		tree.insert(i);
	for (size_t i = 0; i < size/intv; i++)
		cout << tree.getkthNode(i*intv)->value << endl;
	int count = 0;
	for (int i : evec)
	{
		if (tree.pop(i) == nullptr)
			count++;
	}
	for (size_t i = 0; i < size / (2*intv); i++)
		cout << tree.getkthNode(i * intv)->value << endl;
	return 0;
}

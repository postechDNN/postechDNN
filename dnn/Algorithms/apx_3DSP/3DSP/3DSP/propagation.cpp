#include <cmath>
#include <algorithm>
#include <iostream>
#include <iterator>
#include "propagation.h"

#include <random>


void Segment::SetAdjDiagram(PolyDomain& D)
{
	AdjDiagram.resize(Adjs.size());
	for (size_t i = 0; i < Adjs.size(); i++)
	{
		AdjDiagram[i].resize(X.size());
		for (size_t j = 0; j < X.size(); j++)
		{
			Segment* l1 = Adjs[i].first;
			if (Adjs[i].second == -1)
			{
				AdjDiagram[i][j] = { 0, 1 };
				continue;
			}
			Tri f =  D.get_fc(Adjs[i].second);
			vector<MyVec> v;
			for (size_t k = 1; k < 4; k++)
			{
				v.push_back(D.get_pt(f.getPoint(k)));
			}
			pair<double, double> res(0., 1.);
			for (size_t k = 0; k < 3; k++)
			{
				auto temp = Interval(j, v[k], v[(k + 1) % 3], v[(k + 2) % 3], f, l1);
				res = { max(res.first, temp.first), min(res.second, temp.second) };
			}
			AdjDiagram[i][j] = res;
		}
	}
}

//Return the interval of segment l1 contained in the halfspace(side of v3) defined by X[i], v1 and v2
pair<double, double> Segment::Interval(int i,MyVec& v1, MyVec& v2, MyVec& v3, Tri& f, Segment* l1)
{
	MyVec v = getPoint(i);
	MyVec n1 = OuterProd(v1 - v3, v2 - v3);
	if (InnerProd(n1, v3 - v) * InnerProd(n1, v3- l1->a) > 0 && InnerProd(n1, v3 - v) * InnerProd(n1, v3 - l1->b) > 0)
		return { 0., 1. };

	MyVec n = OuterProd(v1 - v, v2 - v);
	if (abs(InnerProd(n, l1->b - l1->a)) < EPS)
	{
		if (InnerProd(n, v3 - v) * InnerProd(n, l1->a - v) > 0)
			return { 0., 1. };
		else
			return { 1., 0. };
	}
	double t = InnerProd(v - l1->a, n) / InnerProd(l1->b - l1->a, n);
	if (InnerProd(l1->b - l1->a, n) * InnerProd(v3 - v, n) > 0)
		return { max(0.,t), 1. };
	else
		return { 0., min(1.,t) };
}

//Set the neareast positions
void Segment::SetNear()
{
	Near.resize(Adjs.size());
	for (size_t i = 0; i < Adjs.size(); i++)
	{
		Near[i].resize(X.size());
		for (size_t j = 0; j < X.size(); j++)
		{
			Segment* l1 = Adjs[i].first;
			MyVec v = (b - a) * X[j] + a;
			double t = InnerProd(l1->a - l1->b, l1->a - v) / InnerProd(l1->b - l1->a, l1->b - l1->a);
			Near[i][j] = min(max(t, 0.), 1.);
		}
	}
}


//dist[i]>=dist[i1] �̰� Segment l���� site�� X[i] �� X[i1]�� ���� ��, X[i]�� �����ϴ� l1���� ������ ����
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
	double B = 8 * c * c * InnerProd(l1.b - l1.a, l1.a - this->getPoint(i)) - 4 * (X[i] - X[i1]) * InnerProd(b - a, l1.b - l1.a) * Bbar;
	double C = 4 * c * c * InnerProd(l1.a - this->getPoint(i), l1.a - this->getPoint(i)) - Bbar * Bbar;

	double D = B * B - 4 * A * C;
	if (abs(A) < EPS)
	{
		if (abs(B) < EPS)
			return { 1., 0. };
		double t = -C / B;
		if (!IsVvertex(i, i1, l1, t))
			return { 0., 1. };
		if (IsVvertex(i, i1, l1, t + 1))
			return { t, 1. };
		return { 0., t };
	}
	if (D / abs(A) < EPS)
		return { 1., 0. };
	double t1 = (-B - sqrt(D)) / (2 * A);
	double t2 = (-B + sqrt(D)) / (2 * A);
	if (t1 > t2)
		swap(t1, t2);
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

//Update informations(Additive Voronoi diagram, Set of representatives) related to i-th point
void Segment::Update(int i, double val, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs)
{
	dist[i] = val;
	S[X[i]] = i;
	Sbar.erase(X[i]);
	for (size_t lindex = 0; lindex < Adjs.size(); lindex++) {
		UpdateSeg(i, lindex, Reprs);
	}
}

//Debug
void Segment::AddAdjs(Segment* s, int i)
{
	Revs.push_back(s->Adjs.size());
	Adjs.emplace_back(s, i);
	AddVoronoi.push_back(AVLTree<pair<double, int>>());
	if (this != s)
	{
		s->Revs.push_back(Adjs.size() - 1);
		s->Adjs.emplace_back(this, i);
		s->AddVoronoi.push_back(AVLTree<pair<double, int>>());
	}	
}

//Update informations of lindex-th segment
void Segment::UpdateSeg(int i, int lindex, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs)
{
	Segment* l1 = Adjs[lindex].first;
	AVLTree<pair<double, int>> &addV = AddVoronoi[lindex];
	if (addV.isEmpty())
	{
		addV.insert({ 0.,-1 });
		addV.insert({ 1.,i });
		SetReprInv(i, lindex, Reprs);
		SetRepr(i, lindex, { 0.,1. }, Reprs);
		return;
	}
	int start = 0;
	int end = addV.size() - 1;
	int i1;
	double x1, x2;
	bool stop = false;
	while (true)
	{
		if (end - start < 2)
		{
			pair<double, double> intv(0., 1.);
			for (int j = max(1, start); j <= min(end + 1, static_cast<int>(addV.size() - 1)); j++)
			{
				auto temp = Vinterval(i, addV.getkthNode(j)->value.second, lindex); 
				intv.first = max(intv.first, temp.first);
				intv.second = min(intv.second, temp.second);
			}
			if (intv.first > intv.second)
			{
				stop = true;
				break;
			}
			x1 = intv.first;
			break;
		}
		int mid = (start + end) / 2;
		int mi = addV.getkthNode(mid)->value.second;
		double miv = addV.getkthNode(mid)->value.first;

		auto D = [=](int n)->double {
			//return dist[n] + VecSize(l1->a + (l1->b - l1->a) * l1->X[mid] - a - (b - a) * X[n]);
			return dist[n] + VecSize(l1->a + (l1->b - l1->a) * miv - a - (b - a) * X[n]);
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
		{
			stop = true;
			break;
		}
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
			for (int j = max(1, start); j <= min(end + 1, static_cast<int>(addV.size() - 1)); j++)
			{
				auto temp = Vinterval(i, addV.getkthNode(j)->value.second, lindex);
				intv.first = max(intv.first, temp.first);
				intv.second = min(intv.second, temp.second);
			}
			if (intv.first > intv.second)
			{
				stop = true;
				break;
			}
			x2 = intv.second;
			break;
		}
		int mid = (start + end) / 2;
		int mi = addV.getkthNode(mid)->value.second;
		double miv = addV.getkthNode(mid)->value.first;
		auto D = [=](int n)->double {
			//return dist[n] + VecSize(l1->a + (l1->b - l1->a) * l1->X[mid] - a - (b - a) * X[n]);
			return dist[n] + VecSize(l1->a + (l1->b - l1->a) * miv - this->getPoint(n));
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
		{
			stop = true;
			break;
		}
		if (addV.getkthNode(mid)->value.first > intv.second)
			end = mid;
		else
			start = mid;
	}
	if (!stop && x1 >= x2)
		stop = true;
	SetReprInv(i, lindex, Reprs); 
	if (stop)
		return;
	auto it = addV.getRightNode({ x1, -1 });
	i1 = it->value.second;
	if (it != addV.getkthNode(0)) //it != addV.begin()
	{
		auto prev = FindRepr(i1, lindex, { addV.getLeftNode(it->value)->value.first,it->value.first }); //{ std::prev(it)->first,it->first }
		if (prev.first != -1 && l1->X[prev.first] > x1)
			SetRepr(i1, lindex, { addV.getLeftNode(it->value)->value.first,x1 }, Reprs);
	}
	auto itEnd = addV.getRightNode({ x2, sizeX() });
	if (itEnd != nullptr)
	{
		auto prev = FindRepr(itEnd->value.second, lindex, { addV.getLeftNode(itEnd->value)->value.first,itEnd->value.first });
		if (prev.first != -1 && l1->X[prev.first] < x2)
			SetRepr(itEnd->value.second, lindex, { x2,itEnd->value.first }, Reprs);
	}
	while (it != addV.getRightNode({ x2, sizeX() }))
	{
		addV.pop(it->value);
		it = addV.getRightNode(it->value);
	}
	if (x1 != 0.)
		addV.insert({ x1, i1 });
	addV.insert({ x2, i });
	SetRepr(i, lindex, { x1,x2 }, Reprs);
	return;
}

//Return the representative of i-th point for lindex-th segment
pair<int, double> Segment::FindRepr(int i, int lindex, pair<double, double> intv)
{
	Segment* l1 = Adjs[lindex].first;
	if (l1->Sbar.empty())
		return { -1,0. };
	intv = { max(intv.first,AdjDiagram[lindex][i].first), min(intv.second,AdjDiagram[lindex][i].second) };
	auto it1 = l1->Sbar.lower_bound(intv.first);
	auto it2 = l1->Sbar.upper_bound(intv.second);
	if (it1 == l1->Sbar.end() || it2 != l1->Sbar.end() && (intv.first > intv.second || it1->first > intv.second || it2 == l1->Sbar.begin()))
		return { -1,0. };
	it2--;
	int index;
	auto D = [=](int n)->double {
		return dist[i] + VecSize(l1->getPoint(n) - this->getPoint(i));
	};
	if (Near[lindex][i] <= it1->first)
		index = it1->second;
	else if (Near[lindex][i] >= it2->first)
		index = it2->second;
	else
	{
		it2 = l1->Sbar.lower_bound(Near[lindex][i]);
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

//Find the representative and push to the priority queue
void Segment::SetRepr(int i, int lindex, pair<double, double> intv, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs)
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

void Segment::SetReprInv(int i, int lindex, priority_queue<Repr, vector<Repr>, greater<Repr>>& Reprs)
{
	Segment* l1 = Adjs[lindex].first;
	auto& addV = l1->AddVoronoi[Revs[lindex]];
	if (addV.isEmpty())
		return;
	auto it = addV.getRightNode({ X[i],-1 });
	if (i == 0)
		it = addV.getkthNode(1); //std::next(addV.begin());
	if (it == nullptr) //it == addV.end()
		it = addV.getkthNode(addV.size()-1);
	pair<double, double> intv(addV.getLeftNode(it->value)->value.first, it->value.first); //intv(std::prev(it)->first, it->first);
	int index = it->value.second;
	Sbar[X[i]] = i;
	if (i == l1->FindRepr(index, Revs[lindex], intv).first)
	{
		Sbar.erase(X[i]);
		l1->SetRepr(index, Revs[lindex], intv, Reprs);
	}
	else
	{
		Sbar.erase(X[i]);
	}
	return;
}

bool Segment::IsVvertex(int i, int i1, Segment& l1, double t)
{
	MyVec tv = l1.a * (1. - t) + l1.b * t;
	MyVec v = this->getPoint(i);
	MyVec v1 = this->getPoint(i1);
	return abs(VecSize(tv - v) + dist[i] - VecSize(tv - v1) - dist[i1]) < EPS;
}

bool Segment::IsContain(int i, int i1, Segment& l1, double t)
{
	MyVec tv = l1.a * (1. - t) + l1.b * t;
	MyVec v = this->getPoint(i);
	MyVec v1 = this->getPoint(i1);
	return VecSize(tv - v) + dist[i] < VecSize(tv - v1) + dist[i1];
}

int Segment::Ln_Search(int i, int f) {
	vector<pair<Segment*, int>> V = this->Adjs;
	for (int j = 0; j < V.size(); j++) {
		if (get<0>(V[j])->getind() == i && get<1>(V[j]) == f) { return j; }
	}
}

/*
int main1()
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
 
int main()
{
	priority_queue<Repr, vector<Repr>, greater<Repr>> Reprs;
	vector<shared_ptr<Segment>> S;
	vector<MyVec> A, B;
	vector<double> X;
	int n = 10;
	A.emplace_back(0., 0., 0.);
	B.emplace_back(0., 0., 3.);
	A.emplace_back(0., 1., 0.);
	B.emplace_back(0., 1., 3.);
	A.emplace_back(0., 2., 0.);
	B.emplace_back(0., 2., 3.);
	A.emplace_back(0., 3., 0.);
	B.emplace_back(0., 3., 3.);
	for (size_t i = 0; i <= n; i++)
		X.push_back(static_cast<double>(i)/static_cast<double>(n));
	for (size_t i = 0; i < A.size(); i++)
	{
		S.push_back(make_shared<Segment>(A[i], B[i], X));
	}
	for (size_t i = 0; i < A.size(); i++)
	{
		S[i]->AddAdjs(S[i].get(), -1);
		if (i != A.size()-1)
			S[i]->AddAdjs(S[i + 1].get(), -1);
	}
	for (size_t i = 0; i < A.size(); i++)
	{
		S[i]->SetAdjDiagram();
		S[i]->SetNear();

	}
	Repr start;
	start.dst.l = S[0].get();
	start.dst.index = 5;
	start.val = 0.;
	Reprs.push(start);
	while (!Reprs.empty())
	{
		Repr p = Reprs.top();
		Segment* l = p.dst.l;
		Reprs.pop();
		if (l->IsActive(p.dst.index))
			continue;
		l->Update(p.dst.index, p.val, Reprs);
	}
	return 0;
}
*/
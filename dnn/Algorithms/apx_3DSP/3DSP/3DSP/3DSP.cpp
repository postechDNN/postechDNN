#include "dots.h"
#include "propagation.h"
#include <limits>

void PolyDomain::BuildSPM(Point p0) 
{
	//p0의 tetrahedron 찾기
	query = p0;
	int indTet = -1;
	for (int i = 0; i < tets.size(); i++) 
	{
		if (inTet(i, p0)) 
		{
			indTet = i;
			break;
		}
	}
	//p0에 이웃한 segment들 추가(최적화 필요)
	vector<bi*> bis;
	vector<int> itets(tets[indTet].get_itets());
	itets.push_back(indTet);
	for (size_t i = 0; i < itets.size(); i++) 
	{
		Tetra t = tets[itets[i]];
		const vector<bi*>& temp = t.get_bis();
		bis.insert(bis.end(), temp.begin(), temp.end());
	}

	//Representative들의 priority queue
	priority_queue<Repr, vector<Repr>, greater<Repr>> Reprs;
	//v0에 이웃한 점들 queue에 push
	for (bi* b : bis) 
	{
		int tInd = -1;
		for (size_t i = 0; i < 4; i++) 
		{
			if (b->f1 == tets[indTet].getTri(i)) 
			{
				tInd = b->f1;
				break;
			}
			else if (b->f2 == tets[indTet].getTri(i)) 
			{
				tInd = b->f2;
				break;
			}
		}
		if (tInd == -1)
			continue;
		Tri f = this->get_fc(tInd);
		for (Segment* s : b->bi_sgs) 
		{
			for (size_t i = 0; i < s->sizeX(); i++) 
			{
				if (b->i == indTet || penetTri(i, s, p0, f))
				{
					Repr r;
					r.val = VecSize(s->getPoint(i) - MyVec(p0));
					r.dst.index = i;
					r.dst.l = s;
					Reprs.push(r);
				}
			}
		}
	}

	while (!Reprs.empty())
	{
		Repr p = Reprs.top();
		Segment* l = p.dst.l;
		Reprs.pop();
		if (l->IsActive(p.dst.index))
			continue;
		l->Update(p.dst.index, p.val, Reprs);
		l->setPrev(p.dst.index, { p.src.l,p.src.index }); 
	}
}

double PolyDomain::shortest(Point q) {
	//q의 tetrahedron 찾기
	int indTet = -1;
	for (int i = 0; i < tets.size(); i++) {
		if (inTet(i, q)) {
			indTet = i;
			break;
		}
	}
	//q에 이웃한 segment들 추가(최적화 필요)
	vector<bi*> bis;
	vector<int> itets(tets[indTet].get_itets());
	itets.push_back(indTet);
	for (size_t i = 0; i < itets.size(); i++) {
		Tetra t = tets[itets[i]];
		const vector<bi*>& temp = t.get_bis();
		bis.insert(bis.end(), temp.begin(), temp.end());
	}
	//q에 이웃한 점들과의 거리를 이용해 최단거리 구함
	double res = std::numeric_limits<double>::max();
	for (bi* b : bis) {
		int tInd = -1;
		for (size_t i = 0; i < 4; i++)
		{
			if (b->f1 == tets[indTet].getTri(i))
			{
				tInd = b->f1;
				break;
			}
			else if (b->f2 == tets[indTet].getTri(i))
			{
				tInd = b->f2;
				break;
			}
		}
		if (tInd == -1)
			continue;
		Tri f = this->get_fc(tInd);
		for (Segment* s : b->bi_sgs) 
		{
			for (size_t i = 0; i < s->sizeX(); i++) 
			{
				double temp = s->getDist(i) + PointsDist(q, Vec2Point(s->getPoint(i)));
				if (temp < res && temp > PointsDist(q, query) && (b->i == indTet || penetTri(i, s, q, f))) {
					res = temp;
				}
			}
		}
	}
	return res;
}

Point PolyDomain::Nearest(Point q, vector<Point>& P)
{
	BuildSPM(q);
	double val = std::numeric_limits<double>::max();
	Point res;
	for (Point p : P)
	{
		double temp = shortest(p);
		if (temp < val)
		{
			val = temp;
			res = p;
		}
			
	}
	return res;
}


bool PolyDomain::penetTri(int i, Segment* s, Point p0, Tri& f)
{
	MyVec p1(p0);
	MyVec p2 = s->getPoint(i);
	vector<MyVec> v;
	for (size_t i = 1; i < 4; i++)
		v.push_back(get_pt(f.getPoint(i)));
	MyVec n = OuterProd(v[1] - v[0], v[2] - v[0]);
	if (InnerProd(p1 - v[0], n) * InnerProd(p2 - v[0], n) > EPS)
		return false;
	for (size_t k = 0; k < 3; k++)
	{
		n = OuterProd(v[k] - p1, v[(k + 1) % 3] - p1);
		if (InnerProd(p2 - v[k], n) * InnerProd(v[(k + 2) % 3] - v[k], n) < -EPS)
			return false;
	}
	return true;
}


double PolyDomain::Nearest_Dist(Point q, vector<Point>& P)
{
	BuildSPM(q);
	double val = std::numeric_limits<double>::max();
	for (Point p : P)
	{
		double temp = shortest(p);
		if (temp < val)
		{
			val = temp;
		}

	}
	return val;
}
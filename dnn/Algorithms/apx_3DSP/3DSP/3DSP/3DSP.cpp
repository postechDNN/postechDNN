#include "dots.h"
#include "propagation.h"
#include <limits>

void PolyDomain::BuildSPM(Point p0) {
	//p0�� tetrahedron ã��
	query = p0;
	int indTet = -1;
	for (int i = 0; i < tets.size(); i++) {
		if (inTet(i, p0)) {
			indTet = i;
			break;
		}
	}
	//p0�� �̿��� segment�� �߰�(����ȭ �ʿ�)
	vector<bi*> bis;
	vector<int> itets(tets[indTet].get_itets());
	itets.push_back(indTet);
	for (size_t i = 0; i < itets.size(); i++) {
		Tetra t = tets[itets[i]];
		const vector<bi*>& temp = t.get_bis();
		bis.insert(bis.end(), temp.begin(), temp.end());
	}

	//Representative���� priority queue
	priority_queue<Repr, vector<Repr>, greater<Repr>> Reprs;
	//v0�� �̿��� ���� queue�� push
	for (bi* b : bis) {
		for (Segment* s : b->bi_sgs) {
			for (size_t i = 0; i < s->sizeX(); i++) {
				Repr r;
				r.val = VecSize(s->getPoint(i)-MyVec(p0));
				r.dst.index = i;
				r.dst.l = s;
				Reprs.push(r);
			}
		}
	}

	int countdown = 2; // Debug
	while (!Reprs.empty())
	{
		if (Reprs.size() == 11026) {
			countdown -= 1;
			if (countdown == 0) {
				std::cout << Reprs.size() << " ";
			}
		}
		Repr p = Reprs.top();
		Segment* l = p.dst.l;
		Reprs.pop();
		if (l->IsActive(p.dst.index))
			continue;
		//pop�� vertex�� tetrahedron�� vertex�̸� vertex�� �����ϴ� ��� segment�� ���� ���� �ʿ�
		if (p.dst.l->IsTetra() && (p.dst.index == 0 || p.dst.index == l->sizeX()-1))
		{
			int ipts = p.dst.index == 0 ? p.dst.l->geta_ind() : p.dst.l->getb_ind();
			for (int i : pts[ipts].get_iegs()) {
				Segment* l1 = sgs[i];
				if (l1->geta_ind() == ipts)
					l1->Update(0 ,p.val, Reprs);
				else
					l1->Update(l1->sizeX()-1, p.val, Reprs);
			}
		}
		else
			l->Update(p.dst.index, p.val, Reprs);
	}
}

double PolyDomain::shortest(Point q) {
	//q�� tetrahedron ã��
	int indTet = -1;
	for (int i = 0; i < tets.size(); i++) {
		if (inTet(i, q)) {
			indTet = i;
			break;
		}
	}
	//q�� �̿��� segment�� �߰�(����ȭ �ʿ�)
	vector<bi*> bis;
	vector<int> itets(tets[indTet].get_itets());
	itets.push_back(indTet);
	for (size_t i = 0; i < itets.size(); i++) {
		Tetra t = tets[itets[i]];
		const vector<bi*>& temp = t.get_bis();
		bis.insert(bis.end(), temp.begin(), temp.end());
	}
	//q�� �̿��� ������� �Ÿ��� �̿��� �ִܰŸ� ����
	double res = std::numeric_limits<double>::max();
	for (bi* b : bis) {
		for (Segment* s : b->bi_sgs) {
			for (size_t i = 0; i < s->sizeX(); i++) {
				double temp = s->getDist(i) + PointsDist(q,Vec2Point(s->getPoint(i)));
				if (temp < res && temp > PointsDist(q, query))
					res = temp;
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


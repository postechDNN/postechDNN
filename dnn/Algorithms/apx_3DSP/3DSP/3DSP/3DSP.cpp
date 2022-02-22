#include "dots.h"
#include "propagation.h"

void PolyDomain::BuildSPM(Point p0) {
	//p�� tetrahedron ã��
	int indTet = -1;
	for (int i = 0; i < tets.size(); i++) {
		if (inTet(i, p0)) {
			indTet = i;
			break;
		}
	}
	//v0�� �̿��� segment�� �߰�(����ȭ �ʿ�)
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

	while (!Reprs.empty())
	{
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

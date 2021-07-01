#include "dots.h"
#include "propagation.h"

double ASP(Point v0) {
	//Representative들의 priority queue
	priority_queue<Repr,vector<Repr>> Reprs;

	while (!Reprs.empty())
	{
		Repr p = Reprs.top();
		Segment* l = p.dst.l;
		Reprs.pop();
		if (l->IsActive(p.dst.index))
			continue;
		//pop된 vertex가 tetrahedron의 vertex이면 vertex를 포함하는 모든 segment에 대해 연산 필요
		if (p.dst.l->IsTetra() && (p.dst.index == 0 || p.dst.index == l->sizeX()-1))
		{
			
		}
		l->Update(p.dst.index, p.val, Reprs);
	}
	return 0.;
}
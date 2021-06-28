#include <queue>
#include "dots.h"
#include "propagation.h"

double ASP(Point v0) {
	//Representative들의 priority queue
	priority_queue<Repr,vector<Repr>> Reprs;

	while (!Reprs.empty())
	{
		Repr p = Reprs.top();
		Reprs.pop();
		if (p.dst.l->IsActive(p.dst.index))
			continue;
		//pop된 vertex가 tetrahedron의 vertex이면 vertex를 포함하는 모든 segment에 대해 연산 필요
		if (p.dst.tetra && (p.dst.index == 0 || p.dst.index == p.dst.l->sizeX()))
		{

		}
		else
		{

		}
	}
	return 0;
}
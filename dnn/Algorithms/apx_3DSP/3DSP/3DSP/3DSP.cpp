#include <queue>
#include "dots.h"
#include "propagation.h"

double ASP(Point v0) {
	//Representative���� priority queue
	priority_queue<Repr,vector<Repr>> Reprs;

	while (!Reprs.empty())
	{
		Repr p = Reprs.top();
		Reprs.pop();
		if (p.dst.l->IsActive(p.dst.index))
			continue;
		//pop�� vertex�� tetrahedron�� vertex�̸� vertex�� �����ϴ� ��� segment�� ���� ���� �ʿ�
		if (p.dst.tetra && (p.dst.index == 0 || p.dst.index == p.dst.l->sizeX()))
		{

		}
		else
		{

		}
	}
	return 0;
}
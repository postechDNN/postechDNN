#include"Location.h"



double CEdge::x = 0;

VersionTree<CEdge>* buildPM(DCEL* dcel) {
	Vertex *l = dcel->getLmost();
	CEdge::x = l->getx();
	VersionTree<CEdge>* edgeTree = new VersionTree<CEdge>();
	return edgeTree;
}

Face* locate(VersionTree<CEdge>* t, Point* p) {
	CEdge::x = p->getx();
	int ver = 0; //Find the version of p in tree
	CEdge pe(p,p);
	Pnode<CEdge>* e = t->searchLEQ(ver, pe); //Find the edge below p
	if (e == nullptr) {
		e = t->searchGEQ(ver, pe);//Find the edge above p
		return e->key.getTwin()->getIncidentFace();
	}
	return e->key.getIncidentFace();
}

int main() {
	return 0;
}
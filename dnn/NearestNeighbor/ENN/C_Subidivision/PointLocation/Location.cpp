#include"Location.h"
#include <algorithm>

double CEdge::x = 0;
typedef pair<HEdge*, bool> Event;

double key(Event e) {
	double s = e.first->getOrigin()->getx();
	double t = e.first->getTwin()->getOrigin()->getx();
	return e.second ? min(s,t) : max(s,t);
}

bool cmp(Event e1, Event e2) {
	double ke1 = key(e1);
	double ke2 = key(e2);
	if (ke1 == ke2) {
		return e1.second;
	}
	return ke1 < ke2;
}

Location::Location(DCEL* dcel)
{
	vector<Vertex*> vs = dcel->getVertices();
	Vertex* l = vs[0];
	double temp = l->getx();
	for (auto l1 : vs) {
		if (temp > l1->getx()) {
			temp = l1->getx();
			l = l1;
		}
	}
	CEdge::x = l->getx();
	vector<HEdge*> hes = dcel->getHedges();
	//Make events
	vector<Event> events;
	for (auto he : hes) {
		if (he->getOrigin()->getx() != he->getTwin()->getOrigin()->getx()) {
			events.push_back({ he, true });
			events.push_back({ he, false });
		}
	}

	std::sort(events.begin(), events.end(), cmp);

	//Build Persistent RBTree
	for (auto e : events) {
		if (e.second) { //insert
			vt.insert(CEdge(*e.first));
		}
		else { //delete
			vt.remove(CEdge(*e.first));
		}
		xcoords.push_back(key(e));
	}
}


Face* Location::locate(Point* p) {
	CEdge::x = p->getx();
	
	int start = 0;
	int end = vt.lastVersion();
	int ver = (start + end) / 2;
	while (start < end) {
		ver = (start + end) / 2;
		// TODO: Initialize xcoords
		if (CEdge::x < xcoords[ver]) {
			end = ver;
		}
		else {
			start = ver;
		}
	}

	/*
	if (e == nullptr) {
		e = vt->searchGEQ(ver, pe);//Find the edge above p
		return e->data.getTwin()->getIncidentFace();
	}
	return e->data.getIncidentFace();
	*/

	CEdge pe(p, p);
	CEdge e;
	if (vt.searchLEQ(ver, pe, e)) {
		return e.getIncidentFace();
	}
	else {
		return NULL;
	}

}
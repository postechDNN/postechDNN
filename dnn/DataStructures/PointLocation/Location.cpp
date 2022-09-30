#include"Location.h"



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
	vt = new VersionTree<CEdge>();
	vector<HEdge*> hes = dcel->getHedges();
	//Make events
	vector<Event> fevents;
	for (auto he : hes) {
		if (he->getOrigin()->getx() != he->getTwin()->getOrigin()->getx()) {
			fevents.push_back({ he, true });
			fevents.push_back({ he, false });
		}
	}
	vector<Event> events;
	for (size_t i = 0; i < fevents.size(); i++) {
		if (i % 2 == 0)
			events.push_back(fevents[i]);
	}
	//Build Persistent RBTree
	int ver = -1;
	for (auto e : events) {
		if (e.second) { //insert
			vt->insertNode(ver, CEdge(*e.first));
		}
		else { //delete
			vt->deleteNode(ver, CEdge(*e.first));
		}
		ver++;
	}
}


Face* Location::locate(Point* p) {
	CEdge::x = p->getx();
	
	int ver = xcoords.size()/2; //Find the version of p in tree using binary search
	int start = 0;
	int end = xcoords.size();
	while (start < end) {
		ver = (start + end) / 2;
		if (CEdge::x < xcoords[ver]) {
			end = ver;
		}
		else {
			start = ver;
		}
	}
	CEdge pe(p,p);
	Node<CEdge>* e = vt->searchLEQ(ver, pe); //Find the edge below p
	if (e == nullptr) {
		e = vt->searchGEQ(ver, pe);//Find the edge above p
		return e->data.getTwin()->getIncidentFace();
	}
	return e->data.getIncidentFace();
}
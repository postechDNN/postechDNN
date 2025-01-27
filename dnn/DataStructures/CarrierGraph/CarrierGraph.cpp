#include"CarrierGraph.h"

CarrierGraph::CarrierGraph() {
	vcnt = ecnt = 0;
}

CarrierGraph::CarrierGraph(vector<Rect*> rset, vector<Point*> pset, Rect* bb, int dir) {
	for (int i = 0; i < rset.size(); i++) {
		rectset.push_back(rset[i]);
		vlist.push_back(rset[i]->getld());
		vlist.push_back(rset[i]->getlu());
		vlist.push_back(rset[i]->getrd());
		vlist.push_back(rset[i]->getru());
	}
	for (int i = 0; i < pset.size(); i++) {
		pointset.push_back(pset[i]);
		vlist.push_back(pset[i]);
	}
	bbox = bb;
	vector<Point*> slist;
	vector<Point*> tlist;
	for (int i = 0; i < rset.size(); i++) {
		elist.push_back(new Edge(*rset[i]->getld(), *rset[i]->getlu()));
		slist.push_back(rset[i]->getld());
		tlist.push_back(rset[i]->getlu());
		elist.push_back(new Edge(*rset[i]->getrd(), *rset[i]->getru()));
		slist.push_back(rset[i]->getrd());
		tlist.push_back(rset[i]->getru());
		elist.push_back(new Edge(*rset[i]->getld(), *rset[i]->getrd()));
		slist.push_back(rset[i]->getld());
		tlist.push_back(rset[i]->getrd());
		elist.push_back(new Edge(*rset[i]->getlu(), *rset[i]->getru()));
		slist.push_back(rset[i]->getlu());
		tlist.push_back(rset[i]->getru());
	}

	switch (dir) {
	case 0: CarrierGraphxpos(); break;
	case 1: CarrierGraphxneg(); break;
	case 2: CarrierGraphypos(); break;
	case 3: CarrierGraphyneg(); break;
	}
	vcnt = vlist.size();
	ecnt = elist.size();

	for (int i = 0; i < vcnt; i++)
		vconnect.insert(std::pair<Point*, int>(vlist[i], i));

	adj = vector<vector<pair<int, double>>>(vcnt);
	for (int i = 0; i < ecnt; i++) {
		int u = vconnect[slist[i]];
		int v = vconnect[tlist[i]];
		double w = elist[i]->length();
		adj[u].push_back(make_pair(v, w));
		adj[v].push_back(make_pair(u, w));
	}

	int rectonly = rectset.size() * 4;
	matrix = vector<vector<double>>(rectonly);
	for (int i = 0; i < rectonly; i++)
		dijkstra_pointonly(i);
}

CarrierGraph::~CarrierGraph() {
	for (int i = 0; i < ecnt; i++)
		delete elist[i];
	for (int i = rectset.size() * 4 + pointset.size(); i < vcnt; i++)
		delete vlist[i];
}

void CarrierGraph::dijkstra_pointonly(int s)
{
	priority_queue< pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
	vector<pair<double,bool>> dist(vcnt, pair<double,bool>(INF,false));
	int rectonly = rectset.size() * 4;
	int pointonly = pointset.size();
	int updatecount = 0;
	pq.push(make_pair(0.0, s));
	while (!pq.empty())
	{
		if (updatecount >= pointonly)
			break;
		int index = pq.top().second;
		double cost = pq.top().first;
		pq.pop();
		if (dist[index].second == true)
			continue;
		dist[index].second = true;
		dist[index].first = cost;
		if (rectonly <= index && index < rectonly + pointonly)
			updatecount++;

		for (int i = 0; i < adj[index].size(); i++)
		{
			int x = adj[index][i].first;
			double c = adj[index][i].second;
			pq.push(make_pair(cost + c, x));
		}
	}
	for (int i = 0; i < pointset.size(); i++)
		matrix[s].push_back(dist[rectonly+i].first);
}

void CarrierGraph::CarrierGraphxpos() {
	for (int i = 0; i < rectset.size(); i++) {
		Point* ld = rectset[i]->getld();
		Point* lu = rectset[i]->getlu();
		Point* rd = rectset[i]->getrd();
		Point* ru = rectset[i]->getru();
		double x1 = rectset[i]->getl();
		double x2 = rectset[i]->getr();
		double y1 = rectset[i]->getd();
		double y2 = rectset[i]->getu();
		rightraydivide(rd);
		rightraydivide(ru);
		upraydivide(lu);
		downraydivide(ld);
	}
	for (int i = 0; i < pointset.size(); i++) {
		rightraydivide(pointset[i]);
		upraydivide(pointset[i]);
		downraydivide(pointset[i]);
	}
}

void CarrierGraph::CarrierGraphxneg() {
	for (int i = 0; i < rectset.size(); i++) {
		Point* ld = rectset[i]->getld();
		Point* lu = rectset[i]->getlu();
		Point* rd = rectset[i]->getrd();
		Point* ru = rectset[i]->getru();
		double x1 = rectset[i]->getl();
		double x2 = rectset[i]->getr();
		double y1 = rectset[i]->getd();
		double y2 = rectset[i]->getu();
		leftraydivide(ld);
		leftraydivide(lu);
		upraydivide(ru);
		downraydivide(rd);
	}
	for (int i = 0; i < pointset.size(); i++) {
		leftraydivide(pointset[i]);
		upraydivide(pointset[i]);
		downraydivide(pointset[i]);
	}
}

void CarrierGraph::CarrierGraphypos() {
	for (int i = 0; i < rectset.size(); i++) {
		Point* ld = rectset[i]->getld();
		Point* lu = rectset[i]->getlu();
		Point* rd = rectset[i]->getrd();
		Point* ru = rectset[i]->getru();
		double x1 = rectset[i]->getl();
		double x2 = rectset[i]->getr();
		double y1 = rectset[i]->getd();
		double y2 = rectset[i]->getu();
		upraydivide(lu);
		upraydivide(ru);
		leftraydivide(ld);
		rightraydivide(rd);
	}
	for (int i = 0; i < pointset.size(); i++) {
		rightraydivide(pointset[i]);
		leftraydivide(pointset[i]);
		upraydivide(pointset[i]);
	}
}

void CarrierGraph::CarrierGraphyneg() {
	for (int i = 0; i < rectset.size(); i++) {
		Point* ld = rectset[i]->getld();
		Point* lu = rectset[i]->getlu();
		Point* rd = rectset[i]->getrd();
		Point* ru = rectset[i]->getru();
		double x1 = rectset[i]->getl();
		double x2 = rectset[i]->getr();
		double y1 = rectset[i]->getd();
		double y2 = rectset[i]->getu();
		downraydivide(ld);
		downraydivide(rd);
		leftraydivide(lu);
		rightraydivide(ru);
	}
	for (int i = 0; i < pointset.size(); i++) {
		rightraydivide(pointset[i]);
		leftraydivide(pointset[i]);
		downraydivide(pointset[i]);
	}
}

void CarrierGraph::leftraydivide(Point *p) {
	double most = bbox->getl();
	double index = BOUNDINGBOX;
	double x = p->getx();
	double y = p->gety();
	for (int i = 0; i < elist.size(); i++) {
		double x1 = elist[i]->gets().getx();
		double y1 = elist[i]->gets().gety();
		double x2 = elist[i]->gett().getx();
		double y2 = elist[i]->gett().gety();
		if (ishit(y1, y, y2)) {
			if (isclosest(most, x2, x)) {
				most = x2;
				index = i;
			}
		}
	}
	Point* newv = new Point(most, y);
	vlist.push_back(newv);
	elist.push_back(new Edge(*newv, *p));
	if (index != BOUNDINGBOX) {
		//Point* temp = &(elist[index]->gett());
		elist[index]->sett(*newv);
		elist.push_back(new Edge(*newv, elist[index]->gett()));
	}
}

void CarrierGraph::rightraydivide(Point *p) {
	double most = bbox->getr();
	double index = BOUNDINGBOX;
	double x = p->getx();
	double y = p->gety();
	for (int i = 0; i < elist.size(); i++) {
		double x1 = elist[i]->gets().getx();
		double y1 = elist[i]->gets().gety();
		double x2 = elist[i]->gett().getx();
		double y2 = elist[i]->gett().gety();
		if (ishit(y1, y, y2)) {
			if (isclosest(x, x1, most)) {
				most = x1;
				index = i;
			}
		}
	}
	Point* newv = new Point(most, y);
	vlist.push_back(newv);
	elist.push_back(new Edge(*p, *newv));
	if (index != BOUNDINGBOX) {
		//Point* temp = &elist[index]->gett();
		elist[index]->sett(*newv);
		elist.push_back(new Edge(*newv, elist[index]->gett()));
	}
}

void CarrierGraph::upraydivide(Point *p) {
	double most = bbox->getu();
	double index = BOUNDINGBOX;
	double x = p->getx();
	double y = p->gety();
	for (int i = 0; i < elist.size(); i++) {
		double x1 = elist[i]->gets().getx();
		double y1 = elist[i]->gets().gety();
		double x2 = elist[i]->gett().getx();
		double y2 = elist[i]->gett().gety();
		if (ishit(x1, x, x2)) {
			if (isclosest(y, y1, most)) {
				most = y1;
				index = i;
			}
		}
	}
	Point* newv = new Point(x, most);
	vlist.push_back(newv);
	elist.push_back(new Edge(*p, *newv));
	if (index != BOUNDINGBOX) {
		//Point* temp = &elist[index]->gett();
		elist[index]->sett(*newv);
		elist.push_back(new Edge(*newv, elist[index]->gett()));
	}
}

void CarrierGraph::downraydivide(Point *p) {
	double most = bbox->getd();
	double index = BOUNDINGBOX;
	double x = p->getx();
	double y = p->gety();
	for (int i = 0; i < elist.size(); i++) {
		double x1 = elist[i]->gets().getx();
		double y1 = elist[i]->gets().gety();
		double x2 = elist[i]->gett().getx();
		double y2 = elist[i]->gett().gety();
		if (ishit(x1, x, x2)) {
			if (isclosest(most, y2, y)) {
				most = y2;
				index = i;
			}
		}
	}
	Point* newv = new Point(x, most);
	vlist.push_back(newv);
	elist.push_back(new Edge(*newv, *p));
	if (index != BOUNDINGBOX) {
		//Point* temp = &elist[index]->gett();
		elist[index]->sett(*newv);
		elist.push_back(new Edge(*newv, elist[index]->gett()));
	}
}

bool CarrierGraph::ishit(double a, double b, double c) {
	return a <= b && b <= c;
}

bool CarrierGraph::isclosest(double a, double b, double c) {
	return a < b && b < c;
}

vector<vector<double>> CarrierGraph::getmatrix() {
	return this->matrix;
}
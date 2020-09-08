#include"RectangularDomain.h"

RectangularDomain::RectangularDomain() {
	//sample data
	/*
	obstacles.push_back(new Rect(1.6, 5.7, 5.9,12.4));
	obstacles.push_back(new Rect(3.4, 7.8, 13.5,17.0));
	obstacles.push_back(new Rect(8.5, 13.4, 4.7, 9.7));
	obstacles.push_back(new Rect(11.2, 15.8, 12.3, 14.3));
	obstacles.push_back(new Rect(16.6, 23.1, 14.1, 15.5));
	obstacles.push_back(new Rect(17.9, 30.0, 7.8, 12.8));
	obstacles.push_back(new Rect(14.9, 20.0, 1.5, 5.1));
	data.push_back(new Point(15.95, 14.8));
	data.push_back(new Point(15.98, 13.6));
	data.push_back(new Point(16.14, 9.9));
	data.push_back(new Point(16.22, 5.7));*/
	domainconstruct(INIT);
}

RectangularDomain::RectangularDomain(vector<Rect*> rinput, vector<Point*> pinput) {
	for (int i = 0; i < rinput.size(); i++)
		obstacles.push_back(rinput[i]);
	for (int i = 0; i < pinput.size(); i++)
		data.push_back(pinput[i]);
	domainconstruct(INIT);
}

void RectangularDomain::domainconstruct(int state) {
	obscnt = obstacles.size();
	for (int i = 0; i < obscnt; i++)
		obstacles[i]->setid(i);
	datacnt = data.size();
	if(state == INIT){
		bbox = new Rect(-INF, INF, -INF, INF);
		bbox->setid(BOUNDINGBOX);
	}
	if (state == DYNAMIC) {
		dfromld.clear();
		lfromld.clear();
		ufromlu.clear();
		lfromlu.clear();
		dfromrd.clear();
		rfromrd.clear();
		ufromru.clear();
		rfromru.clear();
		lfromp.clear();
		rfromp.clear();
		ufromp.clear();
		dfromp.clear();
		delete xpos;
		delete xneg;
		delete yneg;
		delete ypos;
	}
	setray();
	setlwake();
	setrwake();
	setuwake();
	setdwake();
}

RectangularDomain::~RectangularDomain() {
	delete bbox;
	for (int i = 0; i < obscnt; i++)
		delete obstacles[i];
	for (int i = 0; i < datacnt; i++)
		delete data[i];
	for (int i = 0; i < querylog.size(); i++)
		delete querylog[i];
	delete xpos;
	delete xneg;
	delete ypos;
	delete yneg;
	for (int i = 0; i < obscnt; i++) {
		for (int j = 0; j < datacnt; j++) {
			delete lwake[i][j];
			delete rwake[i][j];
			delete uwake[i][j];
			delete dwake[i][j];
		}
	}
}

void RectangularDomain::setray() {
	for (int i = 0; i < obscnt; i++) {
		double x1 = obstacles[i]->getl();
		double x2 = obstacles[i]->getr();
		double y1 = obstacles[i]->getd();
		double y2 = obstacles[i]->getu();
		dfromld.push_back(getdownray(x1, y1));
		lfromld.push_back(getleftray(x1, y1));
		ufromlu.push_back(getupray(x1, y2));
		lfromlu.push_back(getleftray(x1, y2));
		dfromrd.push_back(getdownray(x2, y1));
		rfromrd.push_back(getrightray(x2, y1));
		ufromru.push_back(getupray(x2, y2));
		rfromru.push_back(getrightray(x2, y2));
	}
	for (int i = 0; i < datacnt; i++) {
		double x = data[i]->getx();
		double y = data[i]->gety();
		lfromp.push_back(getleftray(x, y));
		rfromp.push_back(getrightray(x, y));
		ufromp.push_back(getupray(x, y));
		dfromp.push_back(getdownray(x, y));
	}
}

Rect* RectangularDomain::getleftray(double x, double y) {
	double most = bbox->getl();
	double index = BOUNDINGBOX;
	for (int i = 0; i < obscnt; i++) {
		double x1 = obstacles[i]->getl();
		double x2 = obstacles[i]->getr();
		double y1 = obstacles[i]->getd();
		double y2 = obstacles[i]->getu();
		if (ishit(y1, y, y2)) {
			if (isclosest(most, x2, x)) {
				most = x2;
				index = i;
			}
		}
	}
	if (index == BOUNDINGBOX)
		return bbox;
	return obstacles[index];
}

Rect* RectangularDomain::getrightray(double x, double y) {
	double most = bbox->getr();
	double index = BOUNDINGBOX;
	for (int i = 0; i < obscnt; i++) {
		double x1 = obstacles[i]->getl();
		double x2 = obstacles[i]->getr();
		double y1 = obstacles[i]->getd();
		double y2 = obstacles[i]->getu();
		if (ishit(y1, y, y2)) {
			if (isclosest(x, x1, most)) {
				most = x1;
				index = i;
			}
		}
	}
	if (index == BOUNDINGBOX)
		return bbox;
	return obstacles[index];
}

Rect* RectangularDomain::getdownray(double x, double y) {
	double most = bbox->getd();
	double index = BOUNDINGBOX;
	for (int i = 0; i < obscnt; i++) {
		double x1 = obstacles[i]->getl();
		double x2 = obstacles[i]->getr();
		double y1 = obstacles[i]->getd();
		double y2 = obstacles[i]->getu();
		if (ishit(x1, x, x2)) {
			if (isclosest(most, y2, y)) {
				most = y2;
				index = i;
			}
		}
	}
	if (index == BOUNDINGBOX)
		return bbox;
	return obstacles[index];
}

Rect* RectangularDomain::getupray(double x, double y) {
	double most = bbox->getu();
	double index = BOUNDINGBOX;
	for (int i = 0; i < obscnt; i++) {
		double x1 = obstacles[i]->getl();
		double x2 = obstacles[i]->getr();
		double y1 = obstacles[i]->getd();
		double y2 = obstacles[i]->getu();
		if (ishit(x1, x, x2)) {
			if (isclosest(y, y1, most)) {
				most = y1;
				index = i;
			}
		}
	}
	if (index == BOUNDINGBOX)
		return bbox;
	return obstacles[index];
}

bool RectangularDomain::ishit(double a, double b, double c) {
	return a <= b && b <= c;
}

bool RectangularDomain::isclosest(double a, double b, double c) {
	return a < b && b < c;
}

void RectangularDomain::setlwake() {
	xpos = new CarrierGraph(obstacles, data, bbox, 0);
	vector<vector<double>> rpmat = xpos->getmatrix();
	lwake = vector<vector<Wake*>>(obscnt, vector<Wake*>(datacnt));
	for (int i = 0; i < obscnt; i++) {
		for (int j = 0; j < datacnt; j++) {
			lwake[i][j] = new Wake{false, INF, INF, INF, INF};
			double x = data[j]->getx();
			double y = data[j]->gety();
			if (x < obstacles[i]->getl())
				continue;
			Rect* ltop = obstacles[i];
			double ltopx = ltop->getl();
			double rtopx = rfromru[ltop->getid()]->getl();
			if (ltopx > rtopx)
				rtopx = rfromru[ltop->getid()]->getr();
			while (!ishit(ltopx, x, rtopx)) {
				ltop = rfromru[ltop->getid()];
				ltopx = ltop->getl();
				rtopx = rfromru[ltop->getid()]->getl();
				if (ltopx > rtopx)
					rtopx = rfromru[ltop->getid()]->getr();
			}
			Rect* lbot = obstacles[i];
			double lbotx = lbot->getl();
			double rbotx = rfromrd[lbot->getid()]->getl();
			if (lbotx > rbotx)
				rbotx = rfromrd[lbot->getid()]->getr();
			while (!ishit(lbotx, x, rbotx)) {
				lbot = rfromrd[lbot->getid()];
				lbotx = lbot->getl();
				rbotx = rfromrd[lbot->getid()]->getl();
				if (lbotx > rbotx)
					rbotx = rfromrd[lbot->getid()]->getr();
			}

			if (ishit(lbot->getd(), y, ltop->getu())) {
				lwake[i][j]->reachable = true;
				lwake[i][j]->ld = rpmat[i * 4][j];
				lwake[i][j]->lu = rpmat[i * 4 + 1][j];
			}
		}
	}
}

void RectangularDomain::setrwake() {
	xneg = new CarrierGraph(obstacles, data, bbox, 1);
	vector<vector<double>> rpmat = xneg->getmatrix();
	rwake = vector<vector<Wake*>>(obscnt, vector<Wake*>(datacnt));
	for (int i = 0; i < obscnt; i++) {
		for (int j = 0; j < datacnt; j++) {
			rwake[i][j] = new Wake{ false, INF, INF, INF, INF };
			double x = data[j]->getx();
			double y = data[j]->gety();
			if (x > obstacles[i]->getr())
				continue;
			Rect* rtop = obstacles[i];
			double rtopx = rtop->getr();
			double ltopx = lfromlu[rtop->getid()]->getr();
			if (ltopx > rtopx)
				ltopx = lfromlu[rtop->getid()]->getl();
			while (!ishit(ltopx, x, rtopx)) {
				rtop = lfromlu[rtop->getid()];
				rtopx = rtop->getr();
				ltopx = lfromlu[rtop->getid()]->getr();
				if (ltopx > rtopx)
					ltopx = lfromlu[rtop->getid()]->getl();
			}
			Rect* rbot = obstacles[i];
			double rbotx = rbot->getr();
			double lbotx = lfromld[rbot->getid()]->getr();
			if (lbotx > rbotx)
				lbotx = lfromld[rbot->getid()]->getl();
			while (!ishit(lbotx, x, rbotx)) {
				rbot = lfromld[rbot->getid()];
				rbotx = rbot->getr();
				lbotx = lfromld[rbot->getid()]->getr();
				if (lbotx > rbotx)
					lbotx = lfromld[rbot->getid()]->getl();
			}

			if (ishit(rbot->getd(), y, rtop->getu())) {
				rwake[i][j]->reachable = true;
				rwake[i][j]->rd = rpmat[i * 4 + 2][j];
				rwake[i][j]->ru = rpmat[i * 4 + 3][j];
			}
		}
	}
}

void RectangularDomain::setdwake() {
	ypos = new CarrierGraph(obstacles, data, bbox, 2);
	vector<vector<double>> rpmat = ypos->getmatrix();
	dwake = vector<vector<Wake*>>(obscnt, vector<Wake*>(datacnt));
	for (int i = 0; i < obscnt; i++) {
		for (int j = 0; j < datacnt; j++) {
			dwake[i][j] = new Wake{ false, INF, INF, INF, INF };
			double x = data[j]->getx();
			double y = data[j]->gety();
			if (y < obstacles[i]->getd())
				continue;
			Rect* dleft = obstacles[i];
			double dlefty = dleft->getd();
			double ulefty = ufromlu[dleft->getid()]->getd();
			if (dlefty > ulefty)
				ulefty = ufromlu[dleft->getid()]->getu();
			while (!ishit(dlefty, y, ulefty)) {
				dleft = ufromlu[dleft->getid()];
				dlefty = dleft->getd();
				ulefty = ufromlu[dleft->getid()]->getd();
				if (dlefty > ulefty)
					ulefty = ufromlu[dleft->getid()]->getu();
			}
			Rect* dright = obstacles[i];
			double drighty = dright->getd();
			double urighty = ufromru[dright->getid()]->getd();
			if (drighty > urighty)
				urighty = ufromru[dright->getid()]->getu();
			while (!ishit(drighty, y, urighty)) {
				dright = ufromru[dright->getid()];
				drighty = dright->getd();
				urighty = ufromru[dright->getid()]->getd();
				if (drighty > urighty)
					urighty = ufromru[dright->getid()]->getu();
			}

			if (ishit(dleft->getl(), x, dright->getr())) {
				dwake[i][j]->reachable = true;
				dwake[i][j]->ld = rpmat[i * 4][j];
				dwake[i][j]->rd = rpmat[i * 4 + 2][j];
			}
		}
	}
}

void RectangularDomain::setuwake() {
	yneg = new CarrierGraph(obstacles, data, bbox, 3);
	vector<vector<double>> rpmat = yneg->getmatrix();
	uwake = vector<vector<Wake*>>(obscnt, vector<Wake*>(datacnt));
	for (int i = 0; i < obscnt; i++) {
		for (int j = 0; j < datacnt; j++) {
			uwake[i][j] = new Wake{ false, INF, INF, INF, INF };
			double x = data[j]->getx();
			double y = data[j]->gety();
			if (y > obstacles[i]->getu())
				continue;
			Rect* uleft = obstacles[i];
			double ulefty = uleft->getu();
			double dlefty = dfromld[uleft->getid()]->getu();
			if (dlefty > ulefty)
				dlefty = dfromld[uleft->getid()]->getd();
			while (!ishit(dlefty, y, ulefty)) {
				uleft = dfromld[uleft->getid()];
				ulefty = uleft->getu();
				dlefty = dfromld[uleft->getid()]->getu();
				if (dlefty > ulefty)
					dlefty = dfromld[uleft->getid()]->getd();
			}
			Rect* uright = obstacles[i];
			double urighty = uright->getu();
			double drighty = dfromrd[uright->getid()]->getu();
			if (drighty > urighty)
				drighty = dfromrd[uright->getid()]->getd();
			while (!ishit(drighty, y, urighty)) {
				uright = dfromrd[uright->getid()];
				urighty = uright->getu();
				drighty = dfromrd[uright->getid()]->getu();
				if (drighty > urighty)
					drighty = dfromrd[uright->getid()]->getd();
			}

			if (ishit(uleft->getl(), x, uright->getr())) {
				uwake[i][j]->reachable = true;
				uwake[i][j]->lu = rpmat[i * 4 + 1][j];
				uwake[i][j]->ru = rpmat[i * 4 + 3][j];
			}
		}
	}
}

Point* RectangularDomain::NNS(Point* query) {
	return kNNS(query, 1)[0];
}

Point* RectangularDomain::FNS(Point* query) {
	return kNNS(query, -1)[0];
}

vector<Point*> RectangularDomain::kNNS(Point* query, int k)
{
	bool notinterrect = true, notinterpoint = true;
	for (int j = 0; j < obscnt; j++) {
		if (intersect(obstacles[j], query)) {
			notinterrect = false;
			break;
		}
	}
	for (int j = 0; j < datacnt; j++) {
		if (*query == *data[j]) {
			notinterpoint = false;
			break;
		}
	}
	if (!(notinterrect && notinterpoint)) {
		delete query;
		return vector<Point*>(0);
	}

	querylog.push_back(query);
	dist = vector<double>(datacnt, INF);
	lwakeNNS(query);
	rwakeNNS(query);
	dwakeNNS(query);
	uwakeNNS(query);
	for (int i = 0; i < datacnt; i++) {
		if (dist[i] == INF)
			dist[i] = abs(data[i]->getx() - query->getx()) + abs(data[i]->gety() - query->gety());
	}

	vector<Point*> result;
	if (k == -1) {
		int resindex = 0;
		for (int i = 1; i < datacnt; i++) {
			if (dist[i] > dist[resindex])
				resindex = i;
		}
		result.push_back(data[resindex]);
		return result;
	}
	else {
		vector<bool> tempcheck(datacnt, false);
		for (; k > 0; k--) {
			int resindex = -1;
			double mindist = INF;
			for (int i = 0; i < datacnt; i++) {
				if (dist[i] < mindist && tempcheck[i] == false) {
					mindist = dist[i];
					resindex = i;
				}
			}
			if (resindex != -1) {
				tempcheck[resindex] = true;
				result.push_back(data[resindex]);
			}
		}
		return result;
	}
}

void RectangularDomain::lwakeNNS(Point* q) {
	double qx = q->getx();
	double qy = q->gety();
	int index = getrightray(qx, qy)->getid();
	if (index == -1)
		return;
	vector<Wake*> wake = lwake[index];
	for (int i = 0; i < datacnt; i++) {
		if (wake[i]->reachable == true) {
			double var1 = abs(qx - obstacles[index]->getl()) + abs(qy - obstacles[index]->getu());
			double var2 = abs(qx - obstacles[index]->getl()) + abs(qy - obstacles[index]->getd());
			dist[i] = threemin(var1 + wake[i]->lu, var2 + wake[i]->ld, dist[i]);
		}
	}
}

void RectangularDomain::rwakeNNS(Point* q) {
	double qx = q->getx();
	double qy = q->gety();
	int index = getleftray(qx, qy)->getid();
	if (index == -1)
		return;
	vector<Wake*> wake = rwake[index];
	for (int i = 0; i < datacnt; i++) {
		if (wake[i]->reachable == true) {
			double var1 = abs(qx - obstacles[index]->getr()) + abs(qy - obstacles[index]->getu());;
			double var2 = abs(qx - obstacles[index]->getr()) + abs(qy - obstacles[index]->getd());;
			dist[i] = threemin(var1 + wake[i]->ru, var2 + wake[i]->rd, dist[i]);
		}
	}
}

void RectangularDomain::dwakeNNS(Point* q) {
	double qx = q->getx();
	double qy = q->gety();
	int index = getupray(qx, qy)->getid();
	if (index == -1)
		return;
	vector<Wake*> wake = dwake[index];
	for (int i = 0; i < datacnt; i++) {
		if (wake[i]->reachable == true) {
			double var1 = abs(qx - obstacles[index]->getl()) + abs(qy - obstacles[index]->getd());;
			double var2 = abs(qx - obstacles[index]->getr()) + abs(qy - obstacles[index]->getd());;
			dist[i] = threemin(var1 + wake[i]->ld, var2 + wake[i]->rd, dist[i]);
		}
	}
}

void RectangularDomain::uwakeNNS(Point* q) {
	double qx = q->getx();
	double qy = q->gety();
	int index = getdownray(qx, qy)->getid();
	if (index == -1)
		return;
	vector<Wake*> wake = uwake[index];
	for (int i = 0; i < datacnt; i++) {
		if (wake[i]->reachable == true) {
			double var1 = abs(qx - obstacles[index]->getl()) + abs(qy - obstacles[index]->getu());;
			double var2 = abs(qx - obstacles[index]->getr()) + abs(qy - obstacles[index]->getu());;
			dist[i] = threemin(var1 + wake[i]->lu, var2 + wake[i]->ru, dist[i]);
		}
	}
}

double RectangularDomain::threemin(double a,double b,double c) {
	if (a > b) {
		if (b > c)
			return c;
		return b;
	}
	else {
		if (a > c)
			return c;
		return a;
	}
}

bool RectangularDomain::intersect(Rect* a, Rect* b) {
	return intersect(a,b->getlu()) || intersect(a,b->getld()) || intersect(a,b->getrd()) || intersect(a,b->getru());
}

bool RectangularDomain::intersect(Rect* a, Point* p) {
	return a->getl() <= p->getx() && p->getx() <= a->getr() && a->getd() <= p->gety() && p->gety() <= a->getu();
}

int RectangularDomain::insertion(vector<Rect*> rins, vector<Point*> pins) {
	int successcnt = 0, failcnt = 0;
	for (int i = 0; i < rins.size(); i++) {
		bool notinterrect = true, notinterpoint = true;
		for (int j = 0; j < obstacles.size(); j++) {
			if (intersect(rins[i],obstacles[j])) {
				notinterrect = false;
				break;
			}
		}
		for (int j = 0; j < data.size(); j++) {
			if (intersect(rins[i], data[j])) {
				notinterpoint = false;
				break;
			}
		}
		if (notinterrect && notinterpoint) {
			successcnt++;
			obstacles.push_back(rins[i]);
		}
		else {
			failcnt++;
			delete rins[i];
		}
	}

	for (int i = 0; i < pins.size(); i++) {
		bool notinterrect = true, notinterpoint = true;
		for (int j = 0; j < obstacles.size(); j++) {
			if (intersect(obstacles[j], pins[i])) {
				notinterrect = false;
				break;
			}
		}
		for (int j = 0; j < data.size(); j++) {
			if (*pins[i] == *data[j]) {
				notinterpoint = false;
				break;
			}
		}
		if (notinterrect && notinterpoint) {
			successcnt++;
			data.push_back(pins[i]);
		}
		else {
			failcnt++;
			delete pins[i];
		}
	}

	if (successcnt > 0) {
		domainconstruct(DYNAMIC);
		return SUCCESS;
	}
	return FAIL;
}

int RectangularDomain::insertion(vector<Rect*> rins) {
	return insertion(rins, vector<Point*>());
}
int RectangularDomain::insertion(vector<Point*> pins) {
	return insertion(vector<Rect*>(), pins);
}

int RectangularDomain::deletion(vector<Rect*> rdel, vector<Point*> pdel) {
	int successcnt = 0, failcnt = 0;
	for (int i = 0; i < rdel.size(); i++) {
		for (int j = 0; j < obstacles.size(); j++) {
			if (*rdel[i] == *obstacles[j]) {
				delete obstacles[j];
				obstacles.erase(obstacles.begin() + j);
				successcnt++;
				failcnt--;
				break;
			}
		}
		delete rdel[i];
		failcnt++;
	}

	for (int i = 0; i < pdel.size(); i++) {
		for (int j = 0; j < data.size(); j++) {
			if (*pdel[i] == *data[j]) {
				delete data[j];
				data.erase(data.begin() + j);
				successcnt++;
				failcnt--;
				break;
			}
		}
		delete pdel[i];
		failcnt++;
	}
	if (successcnt > 0) {
		domainconstruct(DYNAMIC);
		return SUCCESS;
	}
	return FAIL;
}

int RectangularDomain::deletion(vector<Rect*> rins) {
	return deletion(rins, vector<Point*>());
}
int RectangularDomain::deletion(vector<Point*> pins) {
	return deletion(vector<Rect*>(), pins);
}

int RectangularDomain::getdatacnt() {
	return datacnt;
}

int RectangularDomain::getobscnt() {
	return obscnt;
}

vector<Rect*> RectangularDomain::getobstacles() {
	return obstacles;
}

vector<Point*> RectangularDomain::getdata() {
	return data;
}

Rect* RectangularDomain::getobstacle(int i) {
	if(0 <= i && i < obscnt)
		return obstacles[i];
	return bbox;
}

Point* RectangularDomain::getdatum(int i) {
	if (0 <= i && i < datacnt)
		return data[i];
	return NULL;
}

Rect* RectangularDomain::getbbox() {
	return bbox;
}

#include"Cgraph.h"

Cgraph::Cgraph()
{}

int Cgraph::edgecnt()
{
	return this->elist.size();
}

int Cgraph::vertexcnt()
{
	return this->vcnt;
}

Edge* Cgraph::getedge(int i)
{
	return this->elist[i];
}

Point* Cgraph::getvertex(int i)
{
	return this->vlist[i];
}

vector<vector<double>> Cgraph::getmatrix()
{
	return this->matrix;
}

Cgraph::~Cgraph()
{/*
	for (int i = 0; i < elist.size(); i++)
		delete elist[i];
	for (int i = 0; i < vlist.size(); i++)
		delete vlist[i];*/
}

Cgraph::Cgraph(vector<Rect> rset, Rect bbox, int dir)
{
	vcnt = 0;
	switch(dir)
	{
	case 0: Cgraphxpos(rset,bbox); break;
	case 1: Cgraphxneg(rset,bbox); break;
	case 2: Cgraphypos(rset,bbox); break;
	case 3: Cgraphyneg(rset,bbox); break;
	}

	double INF = 100000000.0;
	int n = vcnt;
	int m = elist.size();
	/* Adj Matrix
	adj = vector<vector<double>>(n, vector<double>(n, INF));
	for (int i = 0; i < m; i++) {
		int u = elist[i]->gets()->getid();
		int v = elist[i]->gett()->getid();
		double c = elist[i]->length();
		adj[u][v] = c;
		adj[v][u] = c;
	}
	*/
	adj = vector<vector<pair<int, double>>>(n);
	for (int i = 0; i < m; i++)
	{
		int u = elist[i]->gets()->getid();
		int v = elist[i]->gett()->getid();
		double c = elist[i]->length();
		adj[u].push_back(make_pair(v, c));
		adj[v].push_back(make_pair(u, c));
	}
	/* Cache Issue
	matrix = vector<vector<double>>(n, vector<double>(n, 0.0));
	for (int i = 0; i < m; i++) {
		int u = elist[i]->gets()->getid();
		int v = elist[i]->gett()->getid();
		double c = elist[i]->length();
		matrix[u][v] = c;
		matrix[v][u] = c;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j) matrix[i][j] = 0.0;
			else if (matrix[i][j] == 0) matrix[i][j] = INF;
		}
	}
	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
			if (matrix[i][j] > matrix[i][k] + matrix[k][j]);
					matrix[i][j] = matrix[i][k] + matrix[k][j];
			}
		}
	}
	*/

	//Dijkstra
	matrix = vector<vector<double>>(n);
	for (int i = 0; i < rset.size(); i++)
	{
		if(rset[i].isPoint() == true)
			matrix[rset[i].getp()->getid()] = Dijkstra(rset[i].getp()->getid());
		else
		{
			matrix[rset[i].getlt()->getid()] = Dijkstra(rset[i].getlt()->getid());
			matrix[rset[i].getlb()->getid()] = Dijkstra(rset[i].getlb()->getid());
			matrix[rset[i].getrt()->getid()] = Dijkstra(rset[i].getrt()->getid());
			matrix[rset[i].getrb()->getid()] = Dijkstra(rset[i].getrb()->getid());
		}
	}
}

vector<double> Cgraph::Dijkstra(int s) 
{
	priority_queue< pair<double,int>, vector<pair<double,int>>, greater<pair<double, int>>> pq;
	double INF = 100000000.0;
	int n = vcnt;
	int m = elist.size();
	vector<double> dist(n, INF);
	vector<bool> check(n, bool(false));
	pq.push(make_pair(0.0,s));
	while(!pq.empty())
	{
		int index = pq.top().second;
		int cost = pq.top().first;
		pq.pop();
		if (check[index] == true)
			continue;
		check[index] = true;
		dist[index] = cost;

		m = adj[index].size();
		for (int i = 0; i < m; i++)
		{
			int x = adj[index][i].first;
			double c = adj[index][i].second;
			pq.push(make_pair(cost+c,x));
		}
	}
	return dist;
}

void Cgraph::Cgraphxpos(vector<Rect> rset, Rect bbox)
{
	int n = rset.size();
	// make basic vertex, edge set from input
	for (int i = 0; i < n; i++) {
		if (rset[i].isPoint() == true) {
			vlist.push_back(rset[i].getp());
			rset[i].getp()->setid(vcnt++);
		}
		else {
			vlist.push_back(rset[i].getlt());
			vlist.push_back(rset[i].getlb());
			vlist.push_back(rset[i].getrt());
			vlist.push_back(rset[i].getrb());
			rset[i].getlt()->setid(vcnt++);
			rset[i].getlb()->setid(vcnt++);
			rset[i].getrt()->setid(vcnt++);
			rset[i].getrb()->setid(vcnt++);
			elist.push_back(new Edge(rset[i].getlt(), rset[i].getrt()));
			elist.push_back(new Edge(rset[i].getlb(), rset[i].getlt()));
			elist.push_back(new Edge(rset[i].getlb(), rset[i].getrb()));
			elist.push_back(new Edge(rset[i].getrb(), rset[i].getrt()));
		}
	}

	double most; // temp value: x-coordinate first hit by ray
	int index; // temp index: Rect first hit by ray
	// Handling and making horizontal edge
	for (int i = 0; i < n; i++)
	{
		//point -> horizontal ray
		if (rset[i].isPoint() == true) {
			most = bbox.getr();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getp()->gety() && rset[i].getp()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getp()->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getp()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getp(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}

		//Rect -> horizontal ray
		else {
			//above horizontal ray
			most = bbox.getr();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getrt()->gety() && rset[i].getrt()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getrt()->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getrt()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getrt(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//below horizontal ray
			most = bbox.getr();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getrb()->gety() && rset[i].getrb()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getrb()->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getrb()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getrb(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}
	}

	//handling and making vertical edge + separate horizontal edge
	for (int i = 0; i < n; i++)
	{
		//point -> vertical ray
		if (rset[i].isPoint() == true) {
			//upward
			most = bbox.gett();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getp()->getx() && rset[i].getp()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getp()->gety() < elist[j]->gets()->gety() && elist[j]->gets()->gety() < most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getp()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getp(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//downward
			most = bbox.getb();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getp()->getx() && rset[i].getp()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getp()->gety() > elist[j]->gets()->gety() && elist[j]->gets()->gety() > most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getp()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getp()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}

		//Rect -> vertical ray
		else {
			//upward
			most = bbox.gett();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getlt()->getx() && rset[i].getlt()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getlt()->gety() < elist[j]->gets()->gety() && elist[j]->gets()->gety() < most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getlt()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getlt(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//downward
			most = bbox.getb();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getlb()->getx() && rset[i].getlb()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getlb()->gety() > elist[j]->gets()->gety() && elist[j]->gets()->gety() > most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getlb()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getlb()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}
	}
}

void Cgraph::Cgraphxneg(vector<Rect> rset, Rect bbox)
{
	int n = rset.size();
	// make basic vertex, edge set from input
	for (int i = 0; i < n; i++) {
		if (rset[i].isPoint() == true) {
			vlist.push_back(rset[i].getp());
			rset[i].getp()->setid(vcnt++);
		}
		else {
			vlist.push_back(rset[i].getlt());
			vlist.push_back(rset[i].getlb());
			vlist.push_back(rset[i].getrt());
			vlist.push_back(rset[i].getrb());
			rset[i].getlt()->setid(vcnt++);
			rset[i].getlb()->setid(vcnt++);
			rset[i].getrt()->setid(vcnt++);
			rset[i].getrb()->setid(vcnt++);
			elist.push_back(new Edge(rset[i].getlt(), rset[i].getrt()));
			elist.push_back(new Edge(rset[i].getlb(), rset[i].getlt()));
			elist.push_back(new Edge(rset[i].getlb(), rset[i].getrb()));
			elist.push_back(new Edge(rset[i].getrb(), rset[i].getrt()));
		}
	}
	double most; // temp value: x-coordinate first hit by ray
	int index; // temp index: Rect first hit by ray
	// Handling and making horizontal edge
	for (int i = 0; i < n; i++)
	{
		//point -> horizontal ray
		if (rset[i].isPoint() == true) {
			most = bbox.getl();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getp()->gety() && rset[i].getp()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getp()->getx() > elist[j]->gets()->getx() && elist[j]->gets()->getx() > most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getp()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt],rset[i].getp()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}

		//Rect -> horizontal ray
		else {
			//above horizontal ray
			most = bbox.getl();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getlt()->gety() && rset[i].getlt()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getlt()->getx() > elist[j]->gets()->getx() && elist[j]->gets()->getx() > most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getlt()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getlt()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//below horizontal ray
			most = bbox.getl();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getlb()->gety() && rset[i].getlb()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getlb()->getx() > elist[j]->gets()->getx() && elist[j]->gets()->getx() > most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getlb()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getlb()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}
	}


	//handling and making vertical edge + separate horizontal edge
	for (int i = 0; i < n; i++)
	{
		//point -> vertical ray
		if (rset[i].isPoint() == true) {
			//upward
			most = bbox.gett();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getp()->getx() && rset[i].getp()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getp()->gety() < elist[j]->gets()->gety() && elist[j]->gets()->gety() < most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getp()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getp(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//downward
			most = bbox.getb();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getp()->getx() && rset[i].getp()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getp()->gety() > elist[j]->gets()->gety() && elist[j]->gets()->gety() > most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getp()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getp()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}

		//Rect -> vertical ray
		else {
			//upward
			most = bbox.gett();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getrt()->getx() && rset[i].getrt()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getrt()->gety() < elist[j]->gets()->gety() && elist[j]->gets()->gety() < most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getrt()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getrt(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//downward
			most = bbox.getb();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getrb()->getx() && rset[i].getrb()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getrb()->gety() > elist[j]->gets()->gety() && elist[j]->gets()->gety() > most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getrb()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getrb()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}
	}
}

void Cgraph::Cgraphypos(vector<Rect> rset, Rect bbox)
{
	int n = rset.size();
	// make basic vertex, edge set from input
	for (int i = 0; i < n; i++) {
		if (rset[i].isPoint() == true) {
			vlist.push_back(rset[i].getp());
			rset[i].getp()->setid(vcnt++);
		}
		else {
			vlist.push_back(rset[i].getlt());
			vlist.push_back(rset[i].getlb());
			vlist.push_back(rset[i].getrt());
			vlist.push_back(rset[i].getrb());
			rset[i].getlt()->setid(vcnt++);
			rset[i].getlb()->setid(vcnt++);
			rset[i].getrt()->setid(vcnt++);
			rset[i].getrb()->setid(vcnt++);
			elist.push_back(new Edge(rset[i].getlt(), rset[i].getrt()));
			elist.push_back(new Edge(rset[i].getlb(), rset[i].getlt()));
			elist.push_back(new Edge(rset[i].getlb(), rset[i].getrb()));
			elist.push_back(new Edge(rset[i].getrb(), rset[i].getrt()));
		}
	}
	double most; // temp value: x-coordinate first hit by ray
	int index; // temp index: Rect first hit by ray
	// Handling and making horizontal edge
	for (int i = 0; i < n; i++)
	{
		//point -> horizontal ray
		if (rset[i].isPoint() == true) {
			most = bbox.gett();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getp()->getx() && rset[i].getp()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getp()->gety() < elist[j]->gets()->gety() && elist[j]->gets()->gety() < most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getp()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getp(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}

		//Rect -> horizontal ray
		else {
			//left vertical ray
			most = bbox.gett();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getlt()->getx() && rset[i].getlt()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getlt()->gety() < elist[j]->gets()->gety() && elist[j]->gets()->gety() < most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getlt()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getlt(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//right vertical ray
			most = bbox.gett();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getrt()->getx() && rset[i].getrt()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getrt()->gety() < elist[j]->gets()->gety() && elist[j]->gets()->gety() < most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getrt()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getrt(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}
	}


	//handling and making horizontal edge + separate vertical edge
	for (int i = 0; i < n; i++)
	{
		//point -> hor ray
		if (rset[i].isPoint() == true) {
			//rightward
			most = bbox.getr();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getp()->gety() && rset[i].getp()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getp()->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getp()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getp(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//leftward
			most = bbox.getl();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getp()->gety() && rset[i].getp()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getp()->getx() > elist[j]->gets()->getx() && elist[j]->gets()->getx() > most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getp()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getp()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}

		//Rect -> hor ray
		else {
			//rightward
			most = bbox.getr();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getrb()->gety() && rset[i].getrb()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getrb()->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getrb()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getrb(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//leftward
			most = bbox.getl();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getlb()->gety() && rset[i].getlb()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getlb()->getx() > elist[j]->gets()->getx() && elist[j]->gets()->getx() > most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getlb()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getlb()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}
	}
}

void Cgraph::Cgraphyneg(vector<Rect> rset, Rect bbox)
{
	int n = rset.size();
	// make basic vertex, edge set from input
	for (int i = 0; i < n; i++) {
		if (rset[i].isPoint() == true) {
			vlist.push_back(rset[i].getp());
			rset[i].getp()->setid(vcnt++);
		}
		else {
			vlist.push_back(rset[i].getlt());
			vlist.push_back(rset[i].getlb());
			vlist.push_back(rset[i].getrt());
			vlist.push_back(rset[i].getrb());
			rset[i].getlt()->setid(vcnt++);
			rset[i].getlb()->setid(vcnt++);
			rset[i].getrt()->setid(vcnt++);
			rset[i].getrb()->setid(vcnt++);
			elist.push_back(new Edge(rset[i].getlt(), rset[i].getrt()));
			elist.push_back(new Edge(rset[i].getlb(), rset[i].getlt()));
			elist.push_back(new Edge(rset[i].getlb(), rset[i].getrb()));
			elist.push_back(new Edge(rset[i].getrb(), rset[i].getrt()));
		}
	}
	double most; // temp value: x-coordinate first hit by ray
	int index; // temp index: Rect first hit by ray
	// Handling and making horizontal edge
	for (int i = 0; i < n; i++)
	{
		//point -> horizontal ray
		if (rset[i].isPoint() == true) {
			most = bbox.getb();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getp()->getx() && rset[i].getp()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getp()->gety() > elist[j]->gets()->gety() && elist[j]->gets()->gety() > most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getp()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getp()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}

		//Rect -> horizontal ray
		else {
			//above horizontal ray
			most = bbox.getb();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getrb()->getx() && rset[i].getrb()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getrb()->gety() > elist[j]->gets()->gety() && elist[j]->gets()->gety() > most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getrb()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getrb()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//below horizontal ray
			most = bbox.getb();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rset[i].getlb()->getx() && rset[i].getlb()->getx() < elist[j]->gett()->getx()) {
					if (rset[i].getlb()->gety() > elist[j]->gets()->gety() && elist[j]->gets()->gety() > most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rset[i].getlb()->getx(), most));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getlb()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}
	}


	//handling and making vertical edge + separate horizontal edge
	for (int i = 0; i < n; i++)
	{
		//point -> vertical ray
		if (rset[i].isPoint() == true) {
			//rightward
			most = bbox.getr();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getp()->gety() && rset[i].getp()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getp()->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getp()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getp(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//leftward
			most = bbox.getl();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getp()->gety() && rset[i].getp()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getp()->getx() > elist[j]->gets()->getx() && elist[j]->gets()->getx() > most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getp()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getp()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}

		//Rect -> vertical ray
		else {
			//rightward
			most = bbox.getr();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getrt()->gety() && rset[i].getrt()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getrt()->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getrt()->gety()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(rset[i].getrt(), vlist[vcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;

			//leftward
			most = bbox.getl();
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rset[i].getlt()->gety() && rset[i].getlt()->gety() < elist[j]->gett()->gety()) {
					if (rset[i].getlt()->getx() > elist[j]->gets()->getx() && elist[j]->gets()->getx() > most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rset[i].getlt()->getx()));
			vlist[vcnt]->setid(vcnt);
			elist.push_back(new Edge(vlist[vcnt], rset[i].getlt()));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vcnt]));
				elist.push_back(new Edge(vlist[vcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vcnt++;
		}
	}
}
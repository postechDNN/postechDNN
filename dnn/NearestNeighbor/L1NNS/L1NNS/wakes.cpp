#include "Edge.h"
#include "Point.h"
#include "Rectangle.h"
#include "RectangularDomain.h"
#include "Segment.h"
#include "Cgraph.h"
#include<vector>
#include<algorithm>
#include<queue>
using namespace std;

#define INF 100000.0

bool comparel(const Rectangle &a, const Rectangle &b) {
	return a.l < b.l;
}

bool comparer(const Rectangle &a, const Rectangle &b) {
	return a.r > b.r;
}

bool compareb(const Rectangle &a, const Rectangle &b) {
	return a.b < b.b;
}

bool comparet(const Rectangle &a, const Rectangle &b) {
	return a.t > b.t;
}

bool compareid(const Rectangle &a, const Rectangle &b) {
	return a.id < b.id;
}
struct costcompare{
	bool operator()(pair<int, double> a, pair<int, double> b) {
		return a.second < b.second;
	}
};

typedef struct temp2 {
	int l;
	int r;
	int t;
	int b;
}IDX;

int main()
{
	RectangularDomain D;
	Rectangle bbox(-10000.0, 10000.0, 10000.0, -10000.0); // bounding box

	vector<Rectangle> rectset; // input rectangles and points
	vector<Edge*> elist; // list of edges in carrier
	vector<Point*> vlist; // list of vertices in carrier
	int vertexcnt = 0; // # of vertices in carrier

	//input
	int n = 11;
	rectset.push_back(Rectangle(1.6, 5.7, 12.4, 5.9));
	rectset.push_back(Rectangle(3.4, 7.8, 17.0, 13.5));
	rectset.push_back(Rectangle(8.5, 13.4, 9.7, 4.7));
	rectset.push_back(Rectangle(11.2, 15.8, 14.3, 12.3));
	rectset.push_back(Rectangle(16.6, 23.1, 15.5, 14.1));
	rectset.push_back(Rectangle(17.9, 30.0, 12.8, 7.8));
	rectset.push_back(Rectangle(14.9, 20.0, 5.1, 1.5));
	rectset.push_back(Rectangle(15.95, 14.8));
	rectset.push_back(Rectangle(15.98, 13.6));
	rectset.push_back(Rectangle(16.14, 9.9));
	rectset.push_back(Rectangle(16.22, 5.7));
	for (int i = 0; i < n; i++)
		rectset[i].setid(i);

	//n^2 grid decomposition (for query to log n)
	/*
	vector<double> gridX,gridY;
	gridX.push_back(bbox.l);
	gridX.push_back(bbox.r);
	gridY.push_back(bbox.t);
	gridY.push_back(bbox.b);
	for (int i = 0; i < n; i++)
	{
		if (rectset[i].isPoint == false) 
		{
			gridX.push_back(rectset[i].l);
			gridX.push_back(rectset[i].r);
			gridY.push_back(rectset[i].t);
			gridY.push_back(rectset[i].b);
		}
	}
	sort(gridX.begin(), gridX.end());
	sort(gridY.begin(), gridY.end());
	vector<vector<IDX> > gridindex;
	*/

	//make shadow for left side (n^3)
	bool unbounded; // check unboundedness
	sort(rectset.begin(), rectset.end(), comparel);
	for (int i = n - 1; i >= 0; i--)
	{
		if (rectset[i].isPoint == false)
		{
			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].b < rectset[i].t && rectset[i].t < rectset[j].t)
				{
					rectset[i].shadow[0].push_back(Segment(rectset[i].l, rectset[i].t, rectset[j].l, rectset[i].t));
					copy(rectset[j].shadow[0].begin(), rectset[j].shadow[0].end(), back_inserter(rectset[i].shadow[0]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rectset[i].shadow[0].push_back(Segment(rectset[i].l, rectset[i].t, bbox.r, rectset[i].t));

			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].b < rectset[i].b && rectset[i].b < rectset[j].t)
				{
					rectset[i].shadow[1].push_back(Segment(rectset[i].l, rectset[i].b, rectset[j].l, rectset[i].b));
					copy(rectset[j].shadow[1].begin(), rectset[j].shadow[1].end(), back_inserter(rectset[i].shadow[1]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rectset[i].shadow[1].push_back(Segment(rectset[i].l, rectset[i].b, bbox.r, rectset[i].b));
		}
	}
	//make shadow for right side (n^3)
	sort(rectset.begin(), rectset.end(), comparer);
	for (int i = n - 1; i >= 0; i--)
	{
		if (rectset[i].isPoint == false)
		{
			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].b < rectset[i].t && rectset[i].t < rectset[j].t)
				{
					rectset[i].shadow[5].push_back(Segment(rectset[i].r, rectset[i].t, rectset[j].r, rectset[i].t));
					copy(rectset[j].shadow[5].begin(), rectset[j].shadow[5].end(), back_inserter(rectset[i].shadow[5]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rectset[i].shadow[5].push_back(Segment(rectset[i].r, rectset[i].t, bbox.l, rectset[i].t));

			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].b < rectset[i].b && rectset[i].b < rectset[j].t)
				{
					rectset[i].shadow[4].push_back(Segment(rectset[i].r, rectset[i].b, rectset[j].r, rectset[i].b));
					copy(rectset[j].shadow[4].begin(), rectset[j].shadow[4].end(), back_inserter(rectset[i].shadow[4]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rectset[i].shadow[4].push_back(Segment(rectset[i].r, rectset[i].b, bbox.l, rectset[i].b));
		}
	}
	//make shadow for bottom side (n^3)
	sort(rectset.begin(), rectset.end(), compareb);
	for (int i = n - 1; i >= 0; i--)
	{
		if (rectset[i].isPoint == false)
		{
			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].l < rectset[i].l && rectset[i].l < rectset[j].r)
				{
					rectset[i].shadow[6].push_back(Segment(rectset[i].l, rectset[i].b, rectset[i].l, rectset[j].b));
					copy(rectset[j].shadow[6].begin(), rectset[j].shadow[6].end(), back_inserter(rectset[i].shadow[6]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rectset[i].shadow[6].push_back(Segment(rectset[i].l, rectset[i].b, rectset[i].l, bbox.t));

			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].l < rectset[i].r && rectset[i].r < rectset[j].r)
				{
					rectset[i].shadow[7].push_back(Segment(rectset[i].r, rectset[i].b, rectset[i].r, rectset[j].b));
					copy(rectset[j].shadow[7].begin(), rectset[j].shadow[7].end(), back_inserter(rectset[i].shadow[7]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rectset[i].shadow[7].push_back(Segment(rectset[i].r, rectset[i].b, rectset[i].r, bbox.t));
		}
	}
	//make shadow for top side (n^3)
	sort(rectset.begin(), rectset.end(), comparet);
	for (int i = n - 1; i >= 0; i--)
	{
		if (rectset[i].isPoint == false)
		{
			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].l < rectset[i].l && rectset[i].l < rectset[j].r)
				{
					rectset[i].shadow[3].push_back(Segment(rectset[i].l, rectset[i].t, rectset[i].l, rectset[j].t));
					copy(rectset[j].shadow[3].begin(), rectset[j].shadow[3].end(), back_inserter(rectset[i].shadow[3]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rectset[i].shadow[3].push_back(Segment(rectset[i].l, rectset[i].t, rectset[i].l, bbox.b));

			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].l < rectset[i].r && rectset[i].r < rectset[j].r)
				{
					rectset[i].shadow[2].push_back(Segment(rectset[i].r, rectset[i].t, rectset[i].r, rectset[j].t));
					copy(rectset[j].shadow[2].begin(), rectset[j].shadow[2].end(), back_inserter(rectset[i].shadow[2]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rectset[i].shadow[2].push_back(Segment(rectset[i].r, rectset[i].t, rectset[i].r, bbox.b));
		}
	}

	Cgraph xpos, ypos, xneg, yneg;
	/* Carrier Graph from positive */
	// make basic vertex, edge set from input
	for (int i = 0; i < n; i++) {
		if (rectset[i].isPoint == true) {
			vlist.push_back(rectset[i].p);
			ypos.vlist.push_back(rectset[i].p);
			rectset[i].p->setid(vertexcnt++);
		}
		else {
			vlist.push_back(rectset[i].lt);
			vlist.push_back(rectset[i].lb);
			vlist.push_back(rectset[i].rt);
			vlist.push_back(rectset[i].rb);
			ypos.vlist.push_back(rectset[i].lt);
			ypos.vlist.push_back(rectset[i].lb);
			ypos.vlist.push_back(rectset[i].rt);
			ypos.vlist.push_back(rectset[i].rb);
			rectset[i].lt->setid(vertexcnt++);
			rectset[i].lb->setid(vertexcnt++);
			rectset[i].rt->setid(vertexcnt++);
			rectset[i].rb->setid(vertexcnt++);
			elist.push_back(new Edge(rectset[i].lt, rectset[i].rt));
			elist.push_back(new Edge(rectset[i].lb, rectset[i].lt));
			elist.push_back(new Edge(rectset[i].lb, rectset[i].rb));
			elist.push_back(new Edge(rectset[i].rb, rectset[i].rt));
			ypos.elist.push_back(new Edge(rectset[i].lt, rectset[i].rt));
			ypos.elist.push_back(new Edge(rectset[i].lb, rectset[i].lt));
			ypos.elist.push_back(new Edge(rectset[i].lb, rectset[i].rb));
			ypos.elist.push_back(new Edge(rectset[i].rb, rectset[i].rt));
			xneg.elist.push_back(new Edge(rectset[i].lt, rectset[i].rt));
			xneg.elist.push_back(new Edge(rectset[i].lb, rectset[i].lt));
			xneg.elist.push_back(new Edge(rectset[i].lb, rectset[i].rb));
			xneg.elist.push_back(new Edge(rectset[i].rb, rectset[i].rt));
			yneg.elist.push_back(new Edge(rectset[i].lt, rectset[i].rt));
			yneg.elist.push_back(new Edge(rectset[i].lb, rectset[i].lt));
			yneg.elist.push_back(new Edge(rectset[i].lb, rectset[i].rb));
			yneg.elist.push_back(new Edge(rectset[i].rb, rectset[i].rt));
		}
	}
	double most; // temp value: x-coordinate first hit by ray
	int index; // temp index: rectangle first hit by ray
	// Handling and making horizontal edge
	for (int i = 0; i < n; i++)
	{
		//point -> horizontal ray
		if (rectset[i].isPoint == true) {
			most = bbox.r;
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rectset[i].p->gety() && rectset[i].p->gety() < elist[j]->gett()->gety()) {
					if (rectset[i].p->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rectset[i].p->gety()));
			vlist[vertexcnt]->setid(vertexcnt);
			elist.push_back(new Edge(rectset[i].p, vlist[vertexcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vertexcnt]));
				elist.push_back(new Edge(vlist[vertexcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vertexcnt++;
		}

		//rectangle -> horizontal ray
		else {
			//above horizontal ray
			most = bbox.r;
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rectset[i].rt->gety() && rectset[i].rt->gety() < elist[j]->gett()->gety()) {
					if (rectset[i].rt->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rectset[i].rt->gety()));
			vlist[vertexcnt]->setid(vertexcnt);
			elist.push_back(new Edge(rectset[i].rt, vlist[vertexcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vertexcnt]));
				elist.push_back(new Edge(vlist[vertexcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vertexcnt++;

			//below horizontal ray
			most = bbox.r;
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->gety() < rectset[i].rb->gety() && rectset[i].rb->gety() < elist[j]->gett()->gety()) {
					if (rectset[i].rb->getx() < elist[j]->gets()->getx() && elist[j]->gets()->getx() < most)
					{
						most = elist[j]->gets()->getx();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(most, rectset[i].rb->gety()));
			vlist[vertexcnt]->setid(vertexcnt);
			elist.push_back(new Edge(rectset[i].rb, vlist[vertexcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vertexcnt]));
				elist.push_back(new Edge(vlist[vertexcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vertexcnt++;
		}
	}

	//handling and making vertical edge + separate horizontal edge
	for (int i = 0; i < n; i++)
	{
		//point -> vertical ray
		if (rectset[i].isPoint == true) {
			//upward
			most = bbox.t;
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rectset[i].p->getx() && rectset[i].p->getx() < elist[j]->gett()->getx()) {
					if (rectset[i].p->gety() < elist[j]->gets()->gety() && elist[j]->gets()->gety() < most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rectset[i].p->getx(), most));
			vlist[vertexcnt]->setid(vertexcnt);
			elist.push_back(new Edge(rectset[i].p, vlist[vertexcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vertexcnt]));
				elist.push_back(new Edge(vlist[vertexcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vertexcnt++;

			//downward
			most = bbox.b;
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rectset[i].p->getx() && rectset[i].p->getx() < elist[j]->gett()->getx()) {
					if (rectset[i].p->gety() > elist[j]->gets()->gety() && elist[j]->gets()->gety() > most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rectset[i].p->getx(), most));
			vlist[vertexcnt]->setid(vertexcnt);
			elist.push_back(new Edge(vlist[vertexcnt], rectset[i].p));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vertexcnt]));
				elist.push_back(new Edge(vlist[vertexcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vertexcnt++;
		}

		//rectangle -> vertical ray
		else {
			//upward
			most = bbox.t;
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rectset[i].lt->getx() && rectset[i].lt->getx() < elist[j]->gett()->getx()) {
					if (rectset[i].lt->gety() < elist[j]->gets()->gety() && elist[j]->gets()->gety() < most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rectset[i].lt->getx(), most));
			vlist[vertexcnt]->setid(vertexcnt);
			elist.push_back(new Edge(rectset[i].lt, vlist[vertexcnt]));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vertexcnt]));
				elist.push_back(new Edge(vlist[vertexcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vertexcnt++;

			//downward
			most = bbox.b;
			index = -1;
			for (int j = 0; j < elist.size(); j++) {
				if (elist[j]->gets()->getx() < rectset[i].lb->getx() && rectset[i].lb->getx() < elist[j]->gett()->getx()) {
					if (rectset[i].lb->gety() > elist[j]->gets()->gety() && elist[j]->gets()->gety() > most)
					{
						most = elist[j]->gets()->gety();
						index = j;
					}
				}
			}
			vlist.push_back(new Point(rectset[i].lb->getx(), most));
			vlist[vertexcnt]->setid(vertexcnt);
			elist.push_back(new Edge(vlist[vertexcnt], rectset[i].lb));
			if (index != -1) {
				elist.push_back(new Edge(elist[index]->gets(), vlist[vertexcnt]));
				elist.push_back(new Edge(vlist[vertexcnt], elist[index]->gett()));
				elist.erase(elist.begin() + index);
			}
			vertexcnt++;
		}
	}

	/*If we want bound to n^2, adj is needed. */
	/*
	vector<vector<pair<int, double>>> adj(vertexcnt);
	for (int i = 0; i < elist.size(); i++) {
		int u = elist[i]->gets()->getid();
		int v = elist[i]->gett()->getid();
		double c = elist[i]->length();
		adj[u].push_back(make_pair(v, c));
		adj[v].push_back(make_pair(u, c));
	}
	*/

	//Adjacency Matrix
	vector<vector<double>> matrix(vertexcnt, vector<double>(vertexcnt, 0));
	for (int i = 0; i < elist.size(); i++) {
		int u = elist[i]->gets()->getid();
		int v = elist[i]->gett()->getid();
		double c = elist[i]->length();
		matrix[u][v] = c;
		matrix[v][u] = c;
	}
	//Floyd Warshall O(n^3) to compute all distances from ver to ver
	for (int i = 0; i < vertexcnt; i++)
	{
		for (int j = 0; j < vertexcnt; j++)
		{
			if (i == j) matrix[i][j] = 0;
			else if(matrix[i][j] == 0) matrix[i][j] = INF;
		}
	}
	for (int k = 0; k < vertexcnt; k++) {
		for (int i = 0; i < vertexcnt; i++) {
			for (int j = 0; j < vertexcnt; j++) {
				if (matrix[i][j] > matrix[i][k] + matrix[k][j])
					matrix[i][j] = matrix[i][k] + matrix[k][j];
			}
		}
	}

	//Temporary print
	/*
	for (int i = 0; i < elist.size(); i++)
		printf("%lf %lf , %lf %lf\n", elist[i]->gets()->getx(), elist[i]->gets()->gety(), elist[i]->gett()->getx(), elist[i]->gett()->gety());
	for (int i = 0; i < vlist.size(); i++) 
		printf("%lf %lf %d\n",vlist[i]->getx(),vlist[i]->gety(),vlist[i]->getid());
	for (int i = 0; i < vertexcnt; i++) {
		for (int j = 0; j < vertexcnt; j++)
			printf("%lf ", matrix[i][j]);
		printf("\n");
	}

	system("pause");*/

	//이 짓을 4번 해야된다고?

	//Wake에서 가려내야 된다고?
	//reachable checking and has from 
	vector<vector<TMP>> lcheck(n, vector<TMP>(n));// i번의 rectangle의 lwake에 j번 정점이 속하는가?
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			lcheck[i][j].reachable = false;
			lcheck[i][j].ul = INF;
			lcheck[i][j].br = INF;
		}
	}
	for (int i = 0; i < rectset.size(); i++) 
	{
		if (rectset[i].isPoint == false)
		{
			vector<int> templist;
			for (int j = 0; j < rectset.size(); j++)
			{
				//point가 wake 내에 있는지 확인? (shadow[0]의 아래)
				if (rectset[j].isPoint == true)
				{
					for (int k = 0; k < rectset[i].shadow[0].size(); k++)
					{
						if (rectset[i].shadow[0][k].px <= rectset[j].p->getx() && rectset[j].p->getx() <= rectset[i].shadow[0][k].qx)
						{
							if (rectset[i].shadow[0][k].py >= rectset[j].p->gety())
								templist.push_back(j);
						}
					}
				}
			}
			for (int j = 0; j < templist.size(); j++)
			{
				//point가 wake 내에 있는지 확인? (shadow[1]의 위면 lcheck)
				for (int k = 0; k < rectset[i].shadow[1].size(); k++)
				{
					if (rectset[i].shadow[1][k].px <= rectset[templist[j]].p->getx() && rectset[templist[j]].p->getx() <= rectset[i].shadow[1][k].qx)
					{
						if (rectset[i].shadow[1][k].py <= rectset[templist[j]].p->gety())
						{
							//wake check랑 제일 작은거만 골라담기
							lcheck[i][templist[j]].reachable = true;
							lcheck[i][templist[j]].ul = matrix[rectset[i].lt->getid()][templist[j]];
							lcheck[i][templist[j]].br = matrix[rectset[i].lb->getid()][templist[j]];
						}
					}
				}
			}
		}
	}

	//xymonotone 처리해야 한다고? 쉬이펄... 쉣
	// Nonsense_

	//query
	Point query(-3.3,13.6);

	//only monotone
	most = bbox.r;
	index = -1;
	Point *nearest = NULL;
	double dist = INF;
	for (int i = 0; i < n; i++)
	{
		if (rectset[i].isPoint == false)
		{
			if (rectset[i].b <= query.gety() && query.gety() <= rectset[i].t)
			{
				if (query.getx() <= rectset[i].l && rectset[i].l < most)
				{
					most = rectset[i].l;
					index = i;
				}
			}
		}
	}
	for (int i = 0; i < n; i++)
	{
		if (lcheck[index][i].reachable == true)
		{
			if (nearest == NULL || rectset[index].lt->distance(query) + lcheck[index][i].ul < dist)
			{
				nearest = rectset[i].p;
				dist = rectset[index].lt->distance(query) + lcheck[index][i].ul;
			}
			if (nearest == NULL || rectset[index].lb->distance(query) + lcheck[index][i].br < dist)
			{
				nearest = rectset[i].p;
				dist = rectset[index].lb->distance(query) + lcheck[index][i].br;
			}
		}
	}

	for (int i = 0; i < n; i++)
	{
		if (rectset[i].isPoint == true)
		{
			if (lcheck[index][i].reachable == false)
			{
				if (nearest == NULL || rectset[i].p->distance(query) < nearest->distance(query))
					nearest = rectset[i].p;
			}
		}
	}
	
	printf("%lf %lf\n",nearest->getx(),nearest->gety());

	system("pause");
}
#include"RectangularDomain.h"
RectangularDomain::RectangularDomain()
{
	//input()
	/*
	rectset.push_back(Rect(1.6, 5.7, 12.4, 5.9));
	rectset.push_back(Rect(3.4, 7.8, 17.0, 13.5));
	rectset.push_back(Rect(8.5, 13.4, 9.7, 4.7));
	rectset.push_back(Rect(11.2, 15.8, 14.3, 12.3));
	rectset.push_back(Rect(16.6, 23.1, 15.5, 14.1));
	rectset.push_back(Rect(17.9, 30.0, 12.8, 7.8));
	rectset.push_back(Rect(14.9, 20.0, 5.1, 1.5));
	rectset.push_back(Rect(new Point(15.95, 14.8)));
	rectset.push_back(Rect(new Point(15.98, 13.6)));
	rectset.push_back(Rect(new Point(16.14, 9.9)));
	rectset.push_back(Rect(new Point(16.22, 5.7)));
	*/
	//Rect은 lrtb입니다
	/*
	rectset.push_back(Rect(107.136, 161, 410.177, 363.854));
	rectset.push_back(Rect(177.698, 261.725, 456.5, 386.477));
	rectset.push_back(Rect(290.812, 376.994, 408.561, 285.213));
	rectset.push_back(Rect(114.139, 206.784, 333.152, 286.829));
	rectset.push_back(Rect(78.05, 283.81, 259.897, 203.879));
	rectset.push_back(Rect(383.996, 437.321, 343.925, 165.636));
	rectset.push_back(Rect(249.875, 333.903, 180.717, 109.079));
	rectset.push_back(Rect(365.682, 524.58, 127.392, 45.5194));
	rectset.push_back(Rect(560.669, 585.985, 260.436, 75.6831));
	rectset.push_back(Rect(478.258, 529.428, 381.629, 222.731));
	rectset.push_back(Rect(334.441, 569.826, 474.275, 438.725));
	rectset.push_back(Rect(137.3, 200.859, 166.713, 51.4444));
	rectset.push_back(Rect(224.559, 327.978, 86.9945, 26.1284));
	rectset.push_back(Rect(43.0386, 88.8228, 391.325, 299.218));
	rectset.push_back(Rect(45.1931, 100.673, 185.027, 106.385));
	rectset.push_back(Rect(new Point(294.582, 450.036)));
	rectset.push_back(Rect(new Point(412.544, 400.482)));
	rectset.push_back(Rect(new Point(421.162, 371.395)));
	rectset.push_back(Rect(new Point(460.483, 354.159)));
	rectset.push_back(Rect(new Point(455.096, 279.827)));
	rectset.push_back(Rect(new Point(555.283, 312.684)));
	rectset.push_back(Rect(new Point(504.112, 185.027)));
	rectset.push_back(Rect(new Point(545.587, 152.17)));
	rectset.push_back(Rect(new Point(574.674, 28.8216)));
	rectset.push_back(Rect(new Point(396.923, 22.8966)));
	rectset.push_back(Rect(new Point(219.712, 14.2784)));
	rectset.push_back(Rect(new Point(227.791, 110.156)));
	rectset.push_back(Rect(new Point(283.271, 99.3831)));
	rectset.push_back(Rect(new Point(327.439, 225.424)));
	rectset.push_back(Rect(new Point(156.152, 27.7444)));
	rectset.push_back(Rect(new Point(111.984, 88.0718)));
	rectset.push_back(Rect(new Point(72.6636, 60.0626)));
	rectset.push_back(Rect(new Point(23.6476, 231.888)));
	rectset.push_back(Rect(new Point(69.9704, 282.52)));
	rectset.push_back(Rect(new Point(202.475, 276.056)));
	rectset.push_back(Rect(new Point(259.032, 318.07)));
	rectset.push_back(Rect(new Point(217.018, 363.854)));
	construct();*/
}

RectangularDomain::RectangularDomain(vector<Rect> arr)
{
	for (int i = 0; i < arr.size(); i++)
	{
		double ll = arr[i].getl();
		double rr = arr[i].getr();
		double tt = arr[i].gett();
		double bb = arr[i].getb();
		if (ll == rr && tt == bb)
			rectset.push_back(new Point(ll, bb));
		else
			rectset.push_back(Rect(ll, rr, tt, bb));
	}
	construct();
}

void RectangularDomain::construct()
{
	bbox = Rect(-10000.0, 10000.0, 10000.0, -10000.0);
	INF = bbox.gett()*bbox.gett();
	np = 0;
	for (int i = 0; i < rectset.size(); i++)
	{
		rectset[i].setid(i);
		if (rectset[i].isPoint() == true)
			np++;
	}
	n = rectset.size();
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
	xpos = Cgraph(rectset, bbox, 0);
	xneg = Cgraph(rectset, bbox, 1);
	ypos = Cgraph(rectset, bbox, 2);
	yneg = Cgraph(rectset, bbox, 3);

	makeshadow();
	makecheck();
}


Rect RectangularDomain::getboundary()
{
	return this->bbox;
}

int RectangularDomain::rectcnt()
{
	return this->rectset.size();
}

Rect RectangularDomain::getrect(int i)
{
	return this->rectset[i];
}

Cgraph RectangularDomain::getcarrier(int i)
{
	switch(i)
	{
	case 0: return xpos; break;
	case 1: return xneg; break;
	case 2: return ypos; break;
	case 3: return yneg; break;
	}
}

bool RectangularDomain::incidentcheck(int i,int j,int dir)
{
	switch (dir)
	{
	case 0: return lcheck[i][j].reachable; break;
	case 1: return rcheck[i][j].reachable; break;
	case 2: return bcheck[i][j].reachable; break;
	case 3: return tcheck[i][j].reachable; break;
	}
}

bool comparel(Rect &a, Rect &b) {
	return a.getl() < b.getl();
}

bool comparer(Rect &a, Rect &b) {
	return a.getr() > b.getr();
}

bool compareb(Rect &a, Rect &b) {
	return a.getb() < b.getb();
}

bool comparet(Rect &a, Rect &b) {
	return a.gett() > b.gett();
}

bool compareid(Rect &a, Rect &b) {
	return a.getid() < b.getid();
}

void RectangularDomain::makeshadow()
{
	bool unbounded; // check unboundedness
	rup = vector<vector<Segment>>(n);
	urp = vector<vector<Segment>>(n);
	ulp = vector<vector<Segment>>(n);
	lup = vector<vector<Segment>>(n);
	ldp = vector<vector<Segment>>(n);
	dlp = vector<vector<Segment>>(n);
	drp = vector<vector<Segment>>(n);
	rdp = vector<vector<Segment>>(n);
	sort(rectset.begin(), rectset.end(), comparel);
	for (int i = n - 1; i >= 0; i--)
	{
		if (rectset[i].isPoint() == false)
		{
			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].getb() < rectset[i].gett() && rectset[i].gett() < rectset[j].gett())
				{
					rup[rectset[i].getid()].push_back(Segment(rectset[i].getl(), rectset[i].gett(), rectset[j].getl(), rectset[i].gett()));
					copy(rup[rectset[j].getid()].begin(), rup[rectset[j].getid()].end(), back_inserter(rup[rectset[i].getid()]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rup[rectset[i].getid()].push_back(Segment(rectset[i].getl(), rectset[i].gett(), bbox.getr(), rectset[i].gett()));

			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].getb() < rectset[i].getb() && rectset[i].getb() < rectset[j].gett())
				{
					rdp[rectset[i].getid()].push_back(Segment(rectset[i].getl(), rectset[i].getb(), rectset[j].getl(), rectset[i].getb()));
					copy(rdp[rectset[j].getid()].begin(), rdp[rectset[j].getid()].end(), back_inserter(rdp[rectset[i].getid()]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				rdp[rectset[i].getid()].push_back(Segment(rectset[i].getl(), rectset[i].getb(), bbox.getr(), rectset[i].getb()));
		}
	}
	//make shadow for right side (n^3)
	sort(rectset.begin(), rectset.end(), comparer);
	for (int i = n - 1; i >= 0; i--)
	{
		if (rectset[i].isPoint() == false)
		{
			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].getb() < rectset[i].gett() && rectset[i].gett() < rectset[j].gett())
				{
					lup[rectset[i].getid()].push_back(Segment(rectset[j].getr(), rectset[i].gett(), rectset[i].getr(), rectset[i].gett()));
					copy(lup[rectset[j].getid()].begin(), lup[rectset[j].getid()].end(), back_inserter(lup[rectset[i].getid()]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				lup[rectset[i].getid()].push_back(Segment(bbox.getl(), rectset[i].gett(), rectset[i].getr(), rectset[i].gett()));

			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].getb() < rectset[i].getb() && rectset[i].getb() < rectset[j].gett())
				{
					ldp[rectset[i].getid()].push_back(Segment(rectset[j].getr(), rectset[i].getb(), rectset[i].getr(), rectset[i].getb()));
					copy(ldp[rectset[j].getid()].begin(), ldp[rectset[j].getid()].end(), back_inserter(ldp[rectset[i].getid()]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				ldp[rectset[i].getid()].push_back(Segment(bbox.getl(), rectset[i].getb(), rectset[i].getr(), rectset[i].getb()));
		}
	}
	//make shadow for bottom side (n^3)
	sort(rectset.begin(), rectset.end(), compareb);
	for (int i = n - 1; i >= 0; i--)
	{
		if (rectset[i].isPoint() == false)
		{
			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].getl() < rectset[i].getl() && rectset[i].getl() < rectset[j].getr())
				{
					ulp[rectset[i].getid()].push_back(Segment(rectset[i].getl(), rectset[i].getb(), rectset[i].getl(), rectset[j].getb()));
					copy(ulp[rectset[j].getid()].begin(), ulp[rectset[j].getid()].end(), back_inserter(ulp[rectset[i].getid()]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				ulp[rectset[i].getid()].push_back(Segment(rectset[i].getl(), rectset[i].getb(), rectset[i].getl(), bbox.gett()));

			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].getl() < rectset[i].getr() && rectset[i].getr() < rectset[j].getr())
				{
					urp[rectset[i].getid()].push_back(Segment(rectset[i].getr(), rectset[i].getb(), rectset[i].getr(), rectset[j].getb()));
					copy(urp[rectset[j].getid()].begin(), urp[rectset[j].getid()].end(), back_inserter(urp[rectset[i].getid()]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				urp[rectset[i].getid()].push_back(Segment(rectset[i].getr(), rectset[i].getb(), rectset[i].getr(), bbox.gett()));
		}
	}
	//make shadow for top side (n^3)
	sort(rectset.begin(), rectset.end(), comparet);
	for (int i = n - 1; i >= 0; i--)
	{
		if (rectset[i].isPoint() == false)
		{
			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].getl() < rectset[i].getl() && rectset[i].getl() < rectset[j].getr())
				{
					dlp[rectset[i].getid()].push_back(Segment(rectset[i].getl(), rectset[j].gett(), rectset[i].getl(), rectset[i].gett()));
					copy(dlp[rectset[j].getid()].begin(), dlp[rectset[j].getid()].end(), back_inserter(dlp[rectset[i].getid()]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				dlp[rectset[i].getid()].push_back(Segment(rectset[i].getl(), bbox.getb(), rectset[i].getl(), rectset[i].gett()));

			unbounded = true;
			for (int j = i + 1; j < n; j++)
			{
				if (rectset[j].getl() < rectset[i].getr() && rectset[i].getr() < rectset[j].getr())
				{
					drp[rectset[i].getid()].push_back(Segment(rectset[i].getr(), rectset[j].gett(), rectset[i].getr(), rectset[i].gett()));
					copy(drp[rectset[j].getid()].begin(), drp[rectset[j].getid()].end(), back_inserter(drp[rectset[i].getid()]));
					unbounded = false;
					break;
				}
			}
			if (unbounded == true)
				drp[rectset[i].getid()].push_back(Segment(rectset[i].getr(), bbox.getb(), rectset[i].getr(), rectset[i].gett()));
		}
	}
	sort(rectset.begin(), rectset.end(), compareid);
}

//working (rcheck, tcheck, bcheck)
void RectangularDomain::makecheck()
{
	vector<vector<double>> mat;
	//reachable checking and has from 
	lcheck = vector<vector<Wake>>(n, vector<Wake>(n));// i번의 Rect의 lwake에 j번 정점이 속하는가?
	mat = xpos.getmatrix();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			lcheck[i][j].reachable = false;
			lcheck[i][j].ul = INF;
			lcheck[i][j].br = INF;
		}
	}
	for (int i = 0; i < n; i++)
	{
		if (rectset[i].isPoint() == false)
		{
			vector<int> templist;
			for (int j = 0; j < n; j++)
			{
				//point가 wake 내에 있는지 확인?
				if (rectset[j].isPoint() == true)
				{
					for (int k = 0; k < rup[i].size(); k++)
					{
						if (rup[i][k].px <= rectset[j].getp()->getx() && rectset[j].getp()->getx() <= rup[i][k].qx)
						{
							if (rup[i][k].py >= rectset[j].getp()->gety())
								templist.push_back(j);
						}
					}
				}
			}
			for (int j = 0; j < templist.size(); j++)
			{
				//point가 wake 내에 있는지 확인? (shadow[1]의 위면 lcheck)
				for (int k = 0; k < rdp[i].size(); k++)
				{
					if (rdp[i][k].px <= rectset[templist[j]].getp()->getx() && rectset[templist[j]].getp()->getx() <= rdp[i][k].qx)
					{
						if (rdp[i][k].py <= rectset[templist[j]].getp()->gety())
						{
							//wake check랑 제일 작은거만 골라담기
							lcheck[i][templist[j]].reachable = true;
							lcheck[i][templist[j]].ul = mat[rectset[i].getlt()->getid()][rectset[templist[j]].getp()->getid()];
							lcheck[i][templist[j]].br = mat[rectset[i].getlb()->getid()][rectset[templist[j]].getp()->getid()];
						}
					}
				}
			}
		}
	}

	rcheck = vector<vector<Wake>>(n, vector<Wake>(n));// i번의 Rect의 lwake에 j번 정점이 속하는가?
	mat = xneg.getmatrix();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			rcheck[i][j].reachable = false;
			rcheck[i][j].ul = INF;
			rcheck[i][j].br = INF;
		}
	}
	for (int i = 0; i < n; i++)
	{
		if (rectset[i].isPoint() == false)
		{
			vector<int> templist;
			for (int j = 0; j < n; j++)
			{
				//point가 wake 내에 있는지 확인? (shadow[0]의 아래)
				if (rectset[j].isPoint() == true)
				{
					for (int k = 0; k < lup[i].size(); k++)
					{
						if (lup[i][k].px <= rectset[j].getp()->getx() && rectset[j].getp()->getx() <= lup[i][k].qx)
						{
							if (lup[i][k].py >= rectset[j].getp()->gety())
								templist.push_back(j);
						}
					}
				}
			}
			for (int j = 0; j < templist.size(); j++)
			{
				//point가 wake 내에 있는지 확인? (shadow[1]의 위면 lcheck)
				for (int k = 0; k < ldp[i].size(); k++)
				{
					if (ldp[i][k].px <= rectset[templist[j]].getp()->getx() && rectset[templist[j]].getp()->getx() <= ldp[i][k].qx)
					{
						if (ldp[i][k].py <= rectset[templist[j]].getp()->gety())
						{
							//wake check랑 제일 작은거만 골라담기
							rcheck[i][templist[j]].reachable = true;
							rcheck[i][templist[j]].ul = mat[rectset[i].getrt()->getid()][rectset[templist[j]].getp()->getid()];
							rcheck[i][templist[j]].br = mat[rectset[i].getrb()->getid()][rectset[templist[j]].getp()->getid()];
						}
					}
				}
			}
		}
	}

	bcheck = vector<vector<Wake>>(n, vector<Wake>(n));// i번의 Rect의 lwake에 j번 정점이 속하는가?
	mat = ypos.getmatrix();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			bcheck[i][j].reachable = false;
			bcheck[i][j].ul = INF;
			bcheck[i][j].br = INF;
		}
	}
	for (int i = 0; i < n; i++)
	{
		if (rectset[i].isPoint() == false)
		{
			vector<int> templist;
			for (int j = 0; j < n; j++)
			{
				//point가 wake 내에 있는지 확인?
				if (rectset[j].isPoint() == true)
				{
					for (int k = 0; k < urp[i].size(); k++)
					{
						if (urp[i][k].py <= rectset[j].getp()->gety() && rectset[j].getp()->gety() <= urp[i][k].qy)
						{
							if (urp[i][k].px >= rectset[j].getp()->getx())
								templist.push_back(j);
						}
					}
				}
			}
			for (int j = 0; j < templist.size(); j++)
			{
				//point가 wake 내에 있는지 확인? (shadow[1]의 위면 lcheck)
				for (int k = 0; k < ulp[i].size(); k++)
				{
					if (ulp[i][k].py <= rectset[templist[j]].getp()->gety() && rectset[templist[j]].getp()->gety() <= ulp[i][k].qy)
					{
						if (ulp[i][k].px <= rectset[templist[j]].getp()->getx())
						{
							//wake check랑 제일 작은거만 골라담기
							bcheck[i][templist[j]].reachable = true;
							bcheck[i][templist[j]].ul = mat[rectset[i].getrb()->getid()][rectset[templist[j]].getp()->getid()];
							bcheck[i][templist[j]].br = mat[rectset[i].getlb()->getid()][rectset[templist[j]].getp()->getid()];
						}
					}
				}
			}
		}
	}

	tcheck = vector<vector<Wake>>(n, vector<Wake>(n));// i번의 Rect의 lwake에 j번 정점이 속하는가?
	mat = yneg.getmatrix();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			tcheck[i][j].reachable = false;
			tcheck[i][j].ul = INF;
			tcheck[i][j].br = INF;
		}
	}
	for (int i = 0; i < n; i++)
	{
		if (rectset[i].isPoint() == false)
		{
			vector<int> templist;
			for (int j = 0; j < n; j++)
			{
				//point가 wake 내에 있는지 확인? (shadow[0]의 아래)
				if (rectset[j].isPoint() == true)
				{
					for (int k = 0; k < drp[i].size(); k++)
					{
						if (drp[i][k].py <= rectset[j].getp()->gety() && rectset[j].getp()->gety() <= drp[i][k].qy)
						{
							if (drp[i][k].px >= rectset[j].getp()->getx())
								templist.push_back(j);
						}
					}
				}
			}
			for (int j = 0; j < templist.size(); j++)
			{
				//point가 wake 내에 있는지 확인? (shadow[1]의 위면 lcheck)
				for (int k = 0; k < dlp[i].size(); k++)
				{
					if (dlp[i][k].py <= rectset[templist[j]].getp()->gety() && rectset[templist[j]].getp()->gety() <= dlp[i][k].qy)
					{
						if (dlp[i][k].px <= rectset[templist[j]].getp()->getx())
						{
							//wake check랑 제일 작은거만 골라담기
							tcheck[i][templist[j]].reachable = true;
							tcheck[i][templist[j]].ul = mat[rectset[i].getrt()->getid()][rectset[templist[j]].getp()->getid()];
							tcheck[i][templist[j]].br = mat[rectset[i].getlt()->getid()][rectset[templist[j]].getp()->getid()];
						}
					}
				}
			}
		}
	}
}

bool comparedist(const Point_Distance &a, const Point_Distance &b) {
	return a.distance < b.distance;
}

Point_Distance RectangularDomain::NNS(Point query)
{
	return kNNS(query,1)[0];
}

vector<Point_Distance> RectangularDomain::kNNS(Point query,int k)
{
	vector<Point_Distance> result(n);
	double lmost = bbox.getr();
	double rmost = bbox.getl();
	double tmost = bbox.getb();
	double bmost = bbox.gett();
	int lindex = -1;
	int rindex = -1;
	int tindex = -1;
	int bindex = -1;

	//find Rect by ray
	for (int i = 0; i < n; i++)
	{
		result[i].distance = INF;
		if (rectset[i].isPoint() == false)
		{
			if (rectset[i].getb() <= query.gety() && query.gety() <= rectset[i].gett())
			{
				if (query.getx() <= rectset[i].getl() && rectset[i].getl() < lmost)
				{
					lmost = rectset[i].getl();
					lindex = i;
				}
			}
			if (rectset[i].getb() <= query.gety() && query.gety() <= rectset[i].gett())
			{
				if (query.getx() >= rectset[i].getr() && rectset[i].getr() > rmost)
				{
					rmost = rectset[i].getr();
					rindex = i; 
				}
			}
			if (rectset[i].getl() <= query.getx() && query.getx() <= rectset[i].getr())
			{
				if (query.gety() <= rectset[i].getb() && rectset[i].getb() < bmost)
				{
					bmost = rectset[i].getb();
					bindex = i;
				}
			}
			if (rectset[i].getl() <= query.getx() && query.getx() <= rectset[i].getr())
			{
				if (query.gety() >= rectset[i].gett() && rectset[i].gett() > tmost)
				{
					tmost = rectset[i].gett();
					tindex = i;
				}
			}
		}
	}
	for (int i = 0; i < n; i++)
	{
		if (rectset[i].isPoint() == true)
		{
			result[i].p = rectset[i].getp();
			if (lindex != -1 && lcheck[lindex][i].reachable == true)
			{
				if (rectset[lindex].getlt()->distance(query) + lcheck[lindex][i].ul < result[i].distance)
					result[i].distance = rectset[lindex].getlt()->distance(query) + lcheck[lindex][i].ul;
				if (rectset[lindex].getlb()->distance(query) + lcheck[lindex][i].br < result[i].distance)
					result[i].distance = rectset[lindex].getlb()->distance(query) + lcheck[lindex][i].br;
			}
			else if (rindex != -1 && rcheck[rindex][i].reachable == true)
			{
				if (rectset[rindex].getrt()->distance(query) + rcheck[rindex][i].ul < result[i].distance)
					result[i].distance = rectset[rindex].getrt()->distance(query) + rcheck[rindex][i].ul;
				if (rectset[rindex].getrb()->distance(query) + rcheck[rindex][i].br < result[i].distance)
					result[i].distance = rectset[rindex].getrb()->distance(query) + rcheck[rindex][i].br;
			}
			else if (bindex != -1 && bcheck[bindex][i].reachable == true)
			{
				if (rectset[bindex].getrb()->distance(query) + bcheck[bindex][i].ul < result[i].distance)
					result[i].distance = rectset[bindex].getrb()->distance(query) + bcheck[bindex][i].ul;
				if (rectset[bindex].getlb()->distance(query) + bcheck[bindex][i].br < result[i].distance)
					result[i].distance = rectset[bindex].getlb()->distance(query) + bcheck[bindex][i].br;
			}
			else if (tindex != -1 && tcheck[tindex][i].reachable == true)
			{
				if (rectset[tindex].getrt()->distance(query) + tcheck[tindex][i].ul < result[i].distance)
					result[i].distance = rectset[tindex].getrt()->distance(query) + tcheck[tindex][i].ul;
				if (rectset[tindex].getlt()->distance(query) + tcheck[tindex][i].br < result[i].distance)
					result[i].distance = rectset[tindex].getlt()->distance(query) + tcheck[tindex][i].br;
			}
			else
				result[i].distance = rectset[i].getp()->distance(query);
		}
	}

	sort(result.begin(),result.end(),comparedist);
	printf("%lf,%lf\n",result[np-1].p->getx(), result[np - 1].p->gety());

	if (k == -1)
		return vector<Point_Distance>(result.begin() + np - 1, result.begin() + np);
	else
		return vector<Point_Distance>(result.begin(), result.begin() + k);
}
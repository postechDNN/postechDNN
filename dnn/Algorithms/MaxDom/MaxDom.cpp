#include "MaxDom.h"

bool cmp_P(const P* a, const P* b) {
	return a->y > b->y;
}

bool cmp_Q_x(const Q* a, const Q* b) {
	return a->x < b->x;
}

bool cmp_Q_y(const Q* a, const Q* b) {
	return a->y > b->y;
}

pair<int, vector<pair<int, int>>> MaxDom(int k, vector<P*> Ps, vector<Q*> Qs, int type) {
	int p = Ps.size();
	int q = Qs.size();

	Q* myQ = new Q;

	sort(Ps.begin(), Ps.end(), cmp_P);
	sort(Qs.begin(), Qs.end(), cmp_Q_x);

	if (type == 0) {
		myQ->x = Qs[q - 1]->x;
	}

	for (int i = 0; i < q; i++) {
		Qs[i]->pi = i;
	}

	for (int i = 0; i < p; i++) {
		if (type == 0 && Ps[i]->x > myQ->x) myQ->x = Ps[i]->x;
		Ps[i]->binarySearch(Qs, 0, q - 1);
	}

	sort(Qs.begin(), Qs.end(), cmp_Q_y);

	if (type == 0) {
		(myQ->x)++;
		myQ->y = min(Ps[p - 1]->y, Qs[q - 1]->y) - 1;
		myQ->pi = q;
		Qs.push_back(myQ);
	}

	return MaxDominance(k, Ps, Qs);
}

pair<int, vector<pair<int, int>>> MaxDominance(int k, vector<P*> Ps, vector<Q*> Qs) {
	int p = Ps.size();
	int q = Qs.size();
	pair<int, int> max;
	vector<int> ind;
	vector<int> Ts;
	vector<int> temp;
	int k_ = k / 2;
	
	SegmentTree*  s = new SegmentTree;;
	s->root = s->makeSegmentTree(0, q - 1);

	ind.assign(q, 0);
	Ts.assign(q, 0);
	temp.assign(q, 0);

	for (int i = 0; i < k; i++) {
		int m = 0;
		s->clearTree(s->root);
		for (int j = 0; j < q; j++) {
			while (m < p && Ps[m]->y > Qs[j]->y) {
				s->add(s->root, Ps[m]->tau, q - 1, Ps[m]->w);
				m++;
			}
			s->set(s->root, Qs[j]->pi, Ts[j]);
			max = s->max(s->root, 0, Qs[j]->pi);
			if(i < k_) ind[Qs[j]->pi] = max.first;
			else if (i == k_) {
				ind[Qs[j]->pi] = max.first;
				temp[Qs[j]->pi] = max.first;
			}
			else {
				ind[Qs[j]->pi] = temp[max.first];
			}
			Ts[j] = max.second;
		}
		if (i > k_) {
			for (int j = 0; j < q; j++) {
				temp[Qs[j]->pi] = ind[Qs[j]->pi];
			}
		}
	}

	int ans_value = Ts[q - 1];
	int ans_point;
	pair<int, int> ans;
	for (int i = 0; i < q; i++) {
		if (Qs[i]->pi == ind[q - 1]) ans_point = i;
	}
	ans.first = Qs[ans_point]->x;
	ans.second = Qs[ans_point]->y;
	vector<pair<int, int>> ans_points;

	vector<P*> P1, P2;
	vector<Q*> Q1, Q2;

	for (int i = 0; i < p; i++) {
		if (Ps[i]->y > Qs[ans_point]->y) {
			P1.push_back(Ps[i]);
		}
		else if (Ps[i]->x > Qs[ans_point]->x) {
			P2.push_back(Ps[i]);
		}
	}
	for (int i = 0; i <= ans_point; i++) {
		Q1.push_back(Qs[i]);
	}
	for (int i = ans_point + 1; i < q; i++) {
		if (Qs[i]->x > Qs[ans_point]->x)Q2.push_back(Qs[i]);
	}
	if(k_ > 0 && P1.size() > 0 && Q1.size() > 0) ans_points = MaxDom(k_, P1, Q1, 1).second;

	ans_points.push_back(ans);
	
	if (k - k_ - 1 > 0 && P2.size() > 0 && Q2.size() > 0) {
		vector<pair<int, int>> ans2 = MaxDom(k - k_ - 1, P2, Q2, 1).second;

		for (int i = 0; i < ans2.size(); i++) {
			ans_points.push_back(ans2[i]);
		}
	}

	return(pair<int, vector<pair<int, int>>>(ans_value, ans_points));
}

#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "P.h"
#include "Q.h"
#include "../../DataStrcutures/SegTree/SegmentTree.h"

using namespace std;

bool cmp_P(const P* a, const P* b);
bool cmp_Q_x(const Q* a, const Q* b);
bool cmp_Q_y(const Q* a, const Q* b);

pair<int, vector<pair<int, int>>> MaxDom(int k, vector<P*> _Ps, vector<Q*> _Qs, int type);
pair<int, vector<pair<int, int>>> MaxDominance(int k, vector<P*> Ps, vector<Q*> Qs);
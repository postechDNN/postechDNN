/*

if (ps == qs) {
	if (pe == qe) {
		get<2>((*vec)[i]) += qc;
		vec->erase(vec->begin() + i + 1); i--;
	}
	else {
		get<2>((*vec)[i]) += qc;
		get<0>((*vec)[i + 1]) = pe;
	}
}
else if (qs < pe) {
	if (pe > qe) {
		get<0>((*vec)[i]) = qs;
		get<2>((*vec)[i + 1]) += pc;
		vec->insert(vec->begin() + i + 2, quadruple(qe, pe, pc, NULL));
	}
	else if (pe == qe) {
		get<0>((*vec)[i]) = qs;
		get<2>((*vec)[i + 1]) += pc;
	}
	else {
		get<1>((*vec)[i]) = qs;
		get<1>((*vec)[i + 1]) = pe;
		get<2>((*vec)[i + 1]) += pc;
		vec->insert(vec->begin() + i + 2, quadruple(pe, qe, qc, NULL));
	}
}
else if (qs == pe) {
	if (pc == qc) {
		get<1>((*vec)[i]) = qe;
		vec->erase(vec->begin() + i + 1); i--;
	}
}
i++;
	 }

	 */

/*

auto ols = overlap(vec, start, end);
if (ols[0] < 0) {
	vector<tii>* path = new vector<tii>({ make_tuple(x_ind + 1, start), make_tuple(x_ind, start), make_tuple(x_ind, end), make_tuple(x_ind + 1, end) });
	vec->insert(vec->begin() + -ols[0] - 1, (quadruple(start, end, 1, path)));
}

vector<int> units;
for (auto ol : ols) {
	int num = 0;
	int s = get<0>((*vec)[ol]), e = get<1>((*vec)[ol]);


	for (auto elem : ols) elem += num;
}



if (ols.size() == 1) {
	int ol = ols[0];
	int s = get<0>((*vec)[ol]), e = get<1>((*vec)[ol]);
	int count = get<2>((*vec)[ol]);

	if (start < s && s < end) {
		if (e == end) {
			get<2>((*vec)[ol]) += 1; // does this work correctly?
			vec->insert(vec->begin() + ol, quadruple(start, s, 1, NULL));
		}
		else {
			get<0>((*vec)[ol]) = end;
			vec->insert(vec->begin() + ol, quadruple(s, end, count + 1, NULL));
			vec->insert(vec->begin() + ol, quadruple(start, s, 1, NULL));
		}
	}
	else if (start < e && e < end) {
		if (s == start) {
			get<2>((*vec)[ol]) += 1; // does this work correctly?
			vec->insert(vec->begin() + ol + 1, quadruple(e, end, 1, NULL));
		}
		else {
			get<1>((*vec)[ol]) = start;
			vec->insert(vec->begin() + ol + 1, quadruple(start, e, count + 1, NULL));
			vec->insert(vec->begin() + ol + 2, quadruple(e, end, 1, NULL));
		}
	}
	else if (s == start) {
		if (e == end) {
			get<2>((*vec)[ol]) += 1;
		}
		else {
			get<0>((*vec)[ol]) = end;
			vec->insert(vec->begin() + ol, quadruple(start, end, count + 1, NULL));
		}
	}
	else {
		if (e == end) {
			get<1>((*vec)[ol]) = start;
			vec->insert(vec->begin() + ol + 1, quadruple(start, end, count + 1, NULL));
		}
		else {
			get<1>((*vec)[ol]) = start;
			vec->insert(vec->begin() + ol + 1, quadruple(start, end, count + 1, NULL));
			vec->insert(vec->begin() + ol + 2, quadruple(end, e, count, NULL));
		}
	}

	if (start <= s && e <= end) { // contained(including coincidence)
		get<2>((*vec)[ol]) += 1;
	}
	else if (start <= s) { // not contained on the right side
		get<0>((*vec)[ol]) = end;
		vec->insert(vec->begin() + ol, quadruple(s, end, count + 1, NULL));
	}
	else { // not contained on the left side
		get<1>((*vec)[ol]) = start;
		vec->insert(vec->begin() + ol + 1, quadruple(start, e, count + 1, NULL));
	}
}

*/

/*

		if (s == start || e == end) {ret.push_back(i);}
		if (start < s && s < end || start < e && e < end) {}

	// if () {}
	int s1 = lnSearch(vec, start, true);
	int s2 = lnSearch(vec, start, false);
	int e1 = lnSearch(vec, end, true);
	int e2 = lnSearch(vec, end, false);


	if (s2 == e1) {

	}
	else {
		int s_ind = max(), e_ind = min();
	}


		 if (right - left <= 1) {
			 if (get<0>((*vec)[right]) == elem) { return right; }
			 else if (get<0>((*vec)[left]) == elem) { return left; }
			 else return -1;
		 }
		 else if (right - left == 2) {
			 if (get<0>((*vec)[right]) == elem) { return right; }
			 else if (get<0>((*vec)[right - 1]) == elem) { return right - 1; }
			 else if (get<0>((*vec)[left]) == elem) { return left; }
			 else return -1;
		 }


			 while (mid < vec.size() - 1) {
				 if (get<0>(*vec[mid + 1]) == elem) { mid += 1; }
			 }


			bin_search(cur_status, get<1>(ev) - 1);
		 if (right - left <= 1) {
			 if (get<0>(vec[right]) == elem) { return right; }
			 else if (get<0>(vec[left]) == elem) { return left; }
			 else return -1;
		 }
		 else if (right - left == 2) {
			 if (get<0>(vec[right]) == elem) { return right; }
			 else if (get<0>(vec[right - 1]) == elem) { return right - 1; }
			 else if (get<0>(vec[left]) == elem) { return left; }
			 else return -1;
		 }
when the interval should be deleted from vec


 if (dirs[dir]) dirs[dir] = false; else dirs[dir] = true;

new tii(make_tuple(x, y)), new tii(make_tuple(x+1, y)), new tii(make_tuple(x+1, y+1)), new tii(make_tuple(x, y+1))

	if (dir == DIR_LEFT) {if (left) left = false; else left = true;}
	if (dir == DIR_RIGHT) { if (right) right = false; else right = true; }
	if (dir == DIR_LOWER) { if (lower) lower = false; else lower = true; }
	if (dir == DIR_UPPER) { if (upper) upper = false; else upper = true; }


	// last vertex is duplicated

RP::RP(deque<Point*> dq){
	vers = {dq};
}

RP::RP(vector<Point*> vec){
	vers = vec;
	// deque<Point*> dq(vec.begin(), vec.end());
	// vers = {dq};
}
*/

/*



RP* myUnion(vector<List*> Lsts, int lv) {
	vector<deque<tuple<int, int>>> chains;
	vector<tuple<int, int>> ints; // intervals. invariant: chains.size() == ints.size()
	vector<tuple<int, int>> ints_old;
	RP* ret = new RP();
	for (int i = 0; i < Lsts.size(); i++) {
		auto L = Lsts[i];
		if (i == 0) { // at the first iteration
			auto lst = L->getLst();
			for (auto it = lst.begin(); it != lst.end(); it++) { // lst.size() is at least 2
				int start = *it;
				while (it != lst.end()) {
					if (*it > start + 1) { // if it is not connected
						deque<tuple<int, int>> chain = { make_tuple(L->getKey() + 1, start), make_tuple(L->getKey(), start),
													make_tuple(L->getKey(), (*it) + 1), make_tuple(L->getKey() + 1, (*it) + 1) }; // lower path to upper path
						chains.push_back(chain);
						ints_old.push_back(make_tuple(start, (*it) + 1));
						break;
					}
					it++;
				}
				it--;
				deque<tuple<int, int>> chain = { make_tuple(L->getKey() + 1, start), make_tuple(L->getKey(), start),
							make_tuple(L->getKey(), (*it) + 1), make_tuple(L->getKey() + 1, (*it) + 1) };
				chains.push_back(chain);
				ints_old.push_back(make_tuple(start, (*it) + 1));
			}
		}
		else {
			auto lst = L->getLst();
			int index = 0;
			for (auto it = lst.begin(); it != lst.end(); it++) { // lst.size() is at least 2
				int start = *it;
				while (it != lst.end()) {
					if (*it > start + 1) { // if it is not connected
						ints.push_back(make_tuple(start, (*it) + 1));
						break;
					}
					it++;
				}
				it--;
				ints.push_back(make_tuple(start, (*it) + 1));
			}
			// linear traversal on both of ints and ints_old
		}
		int index = 0, index_old = 0;
		while (index <= ints.size() - 1 && index_old <= ints_old.size() - 1) {
			int i1 = get<0>(ints[index]), i2 = get<1>(ints[index]);
			int o1 = get<0>(ints_old[index_old]), o2 = get<1>(ints_old[index_old]); // disjoint
			if (i2 + 1 <= o1) {
				deque<tuple<int, int>> chain = { make_tuple(L->getKey() + 1, i1), make_tuple(L->getKey(), i1),
				make_tuple(L->getKey(), i2 + 1), make_tuple(L->getKey() + 1, i2 + 1) };
				chains.push_back(chain);
				ints.push_back(make_tuple(i1, i2 + 1));
			}
			else {

			}
		}
	}

	for (auto dq : chains) {
		for (auto elem : dq) {
			ret->addPt(new Point(pow(2, lv) * double(get<0>(elem)), pow(2, lv) * double(get<1>(elem))));
		}
	}
	return ret;
}

*/

/*
		auto X_elem = X_lists[lists_ind]->getElem(elems_ind);

		int lists_ind = get<2>(cur_q), elems_ind = get<3>(cur_q);
		if (X_lists[lists_ind]->getChecked(elems_ind)) continue;

		X_lists[lists_ind]->check(elems_ind); // if not checked, then check and perform operations

	// queue<tuple<int, int, int, int>> Q; // x_ind, y_ind, Lsts_ind, lst_ind
	// int x = X_lists[0]->getKey(), y = X_lists[0]->getElem(0)->getSV();
	// Q.push(make_tuple(x, y, 0, 0)); // Lsts[0]->check(0);

// Q.push(make_tuple(x_ind, y_ind));} // for left and right, first try brute force.
		// int x_ind = get<0>(cur_q), y_ind = get<1>(cur_q),

vector<ElemList*>,

	if (dir == DIR_LEFT || dir == DIR_RIGHT) {
		if (dir == DIR_LEFT) {
			cur
		}
	}
	else if (dir == DIR_LOWER || dir == DIR_UPPER) {
	}


		if (Lsts[0]->getKeyX()) {throw invalid_argument("error");}

vector<List*> union_core(vector<i_quad*> _Qs, bool key_x) {
	vector<i_quad*> Qs = _Qs;

	vector<List*> ret;
	int lv = Qs[0]->getLv();

	vector<tuple<int, int>> cores = {};
	for (auto Q : Qs) {
		int x_gi = getGridIndex(Q->getX(), lv); // computes x- and y-indices on (lv)-th grid
		int y_gi = getGridIndex(Q->getY(), lv);

		for (int i = 1; i < 3; i++) {
			for (int j = 1; j < 3; j++) {
				if (key_x) cores.push_back(make_tuple(x_gi + i, y_gi + j));
				else cores.push_back(make_tuple(y_gi + i, x_gi + j));
			}
		}
	}

	sort(cores.begin(), cores.end());
	for (auto it = cores.begin(); it != cores.end(); it++) {
		if (it == cores.begin()) { // first element
			List* temp = new List(get<0>(*it), { get<1>(*it) });
			ret.push_back(temp);
		}
		else { // compare current element with the previous one
			auto it_copy = it; --it_copy;

			if (get<0>(*it) == get<0>(*it_copy)) {
				if (ret[ret.size() - 1]->getLst().back() != get<1>(*it)) ret.back()->add(get<1>(*it));
			}
			else {
				List* temp = new List(get<0>(*it), { get<1>(*it) });
				ret.push_back(temp);
			}
		}
	}

	return ret;
}


vector<List*> union_normal(vector<i_quad*> _Qs, bool key_x) {
	// do not explicitly compute cores.
	// sort Qs
	vector<i_quad*> Qs = _Qs;
	sort(Qs.begin(), Qs.end(), sortbyYIncr);
	stable_sort(Qs.begin(), Qs.end(), sortbyXIncr);

	vector<List*> ret;
	int lv = Qs[0]->getLv();

	vector<tuple<int, int>> cores = {};
	for (auto Q : Qs) {
		int x_gi = getGridIndex(Q->getX(), lv); // computes x- and y-indices on (lv)-th grid
		int y_gi = getGridIndex(Q->getY(), lv);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (key_x) cores.push_back(make_tuple(x_gi + i, y_gi + j));
				else cores.push_back(make_tuple(y_gi + i, x_gi + j));
			}
		}
	}

	sort(cores.begin(), cores.end());
	for (auto it = cores.begin(); it != cores.end(); it++) {
		if (it == cores.begin()) { // first element
			List* temp = new List(get<0>(*it), { get<1>(*it) });
			ret.push_back(temp);
		}
		else { // compare current element with the previous one
			auto it_copy = it; --it_copy;

			if (get<0>(*it) == get<0>(*it_copy)) {
				if (ret[ret.size() - 1]->getLst().back() != get<1>(*it)) ret.back()->add(get<1>(*it));
			}
			else {
				List* temp = new List(get<0>(*it), { get<1>(*it) });
				ret.push_back(temp);
			}
		}
	}

	return ret;
}
*/
/*

// vector<List*> union_core(vector<i_quad*>, bool);
// vector<List*> union_normal(vector<i_quad*>, bool);

	bool TF[2] = {true, false};
	for (auto key_x : TF) {
bool XkeyXIncr(Elem* a, Elem* b);
bool XkeyYIncr(Elem* a, Elem* b);
bool YkeyXIncr(Elem* a, Elem* b);
bool YkeyYIncr(Elem* a, Elem* b);

bool XkeyYIncr(Elem* a, Elem* b) {

}
bool YkeyXIncr(Elem* a, Elem* b) {

}
bool YkeyYIncr(Elem* a, Elem* b) {
	return XkeyXIncr(a, b);
}
*/

/*


std::vector<List*> union_core(std::vector<i_quad*> _Qs) {
	std::vector<i_quad*> Qs = _Qs;
	// sort(Qs.begin(), Qs.end(), sortbyYIncr);
	// stable_sort(Qs.begin(), Qs.end(), sortbyXIncr);

	std::vector<List*> ret;
	int lv = Qs[0]->getLv();

	vector<tuple<int, int>> cores = {};
	for (auto Q : Qs) {
		int x_gi = getGridIndex(Q->getX(), lv); // computes x- and y-indices on (lv)-th grid
		int y_gi = getGridIndex(Q->getY(), lv);

		for (int i = 1; i < 3; i++) {
			for (int j = 1; j < 3; j++) {
				cores.push_back(make_tuple(x_gi + i, y_gi + j));
			}
		}
	}

	sort(cores.begin(), cores.end());
	for (auto it = cores.begin(); it != cores.end(); it++) {
		if (it == cores.begin()) { // first element
			List* temp = new List(get<0>(*it), { get<1>(*it)});
			ret.push_back(temp);
		}
		else { // compare current element with the previous one
			auto it_copy = it; --it_copy;

			if (get<0>(*it) == get<0>(*it_copy)) {
				if(ret[ret.size()-1]->getLst().back() != get<1>(*it)) ret.back()->add(get<1>(*it));
			}
			else {
				List* temp = new List(get<0>(*it), { get<1>(*it) });
				ret.push_back(temp);
			}
		}
	}

	return ret;
}
*/

/*

		vector<List*>::iterator it;
		vector<int>::iterator it2;
		int num = -1;
		vector<int> vec;
		for (int i=0; i<4; i++) { // for each element in core. later, we wish to push them coupled.
			x_gi = get<0>(core[i]);
			y_gi = get<1>(core[i]);


					make_tuple(L->getKey() + 1, start);
					make_tuple(L->getKey(), start);
					make_tuple(L->getKey(), (*it) + 1);
					make_tuple(L->getKey() + 1, (*it) + 1);

if (ret.empty()) {
	List* temp_lst = new List(x_gi, { y_gi, y_gi + 1 });
	ret.insert(ret.begin(), temp_lst); i++;
	continue;
}

bool out = false;
// if (i % 2 == 0) {
for (it = ret.begin(); it != ret.end(); it++) { // find where to push. bin search needed
	int x_ind = (*it)->getKey();

	if (x_ind > x_gi) {
		List* temp_lst = new List(x_gi, { y_gi });
		ret.insert(++it, temp_lst);
		num = temp_lst->getLst().back();
		out = true;
	}
	else if (x_ind == x_gi) {
		vec = (*it)->getLst();
		for (it2 = vec.begin(); it2 != vec.end(); it2++) { // bin search needed.
			if (*it2 > y_gi) {
				auto it2_temp = it2;
				if (++it2_temp == vec.begin()) {
					vec.insert(++it2, y_gi);
					out = true; break;
				}
				else continue;
			}
			else if (*it2 == y_gi) break; // already in
			else // (*it2 < y_gi)
				vec.insert(it2, y_gi);
			out = true; break;
		}
	}
	else { // if (x_ind < x_gi) { // insert just before x_ind becomes larger
		auto it_temp = it;
		if (++it_temp == ret.end()) {
			List* temp_lst = new List(x_gi, { y_gi });
			ret.insert(++it, temp_lst);
			num = temp_lst->getLst().back();
			out = true;
		}
		else continue;
	}

	if (out) break;
}
*/

/*
else { // already in ret, with iterator it preserved
	if (it2 != vec.end() && *(++it2) != y_gi) {
		vec.insert(--it2, y_gi);
	}
	else if (it2 == vec.end()) {
		vec.insert(vec.end(), y_gi);
	}
}
*/


/*



	// compute side length
	double sl = sqrs[0]->getLeft()->getLength();

	// divide 'sqrs' into connected components

	// for each connected component
	RectPoly* ret = new RectPoly();
	double x_pos = DBL_MIN;
	std::vector<std::tuple<Point*, int, bool>> events = {}; // store events. true for upper left point, false for upper right point
	for (Square* sqr : sqrs) {
		events.push_back(std::make_tuple(sqr->getUl(), sqr->getIndex(), true));
		events.push_back(std::make_tuple(sqr->getUr(), sqr->getIndex(), false));
	}

	sort(events.begin(), events.end(), sortEventbyYDecr);
	stable_sort(events.begin(), events.end(), sortEventbyXIncr);

	// it only stores
	// later, we have to ensure O(1)-time deletion operation by implementing cur_list with a linked list
	priority_queue<tuple<double, double>> cur_list = {};  // stores
	bool first = true;
	for (auto event : events) {

		if (get<2>(event)) { // in case of start event
		// int idx = std::get<1>(event);

			if (first) { // first event.
				double cur_x = get<0>(event)->getX(), cur_y = get<0>(event)->getY();
				Point* tmp = new Point(cur_x, cur_y - sl);
				ret->addEdge_back(new Edge(get<0>(event), tmp)); first = false;
				cur_list.push(make_tuple(cur_y, cur_x));
				continue;
			}
			else {

			}
		}

		else { // in case of end event

		}
	}

	// set vertical sweep line



	// vector<tuple<double, double>> cur_list = {};


// https://gmlwjd9405.github.io/2018/08/31/algorithm-union-find.html
int my_find(int x, int* root, int count) {
	// 루트 노드는 부모 노드 번호로 자기 자신을 가진다.
	if (root[x] == x) {
		return x;
	}
	else {
		// "경로 압축(Path Compression)"
		// find 하면서 만난 모든 값의 부모 노드를 root로 만든다.
		return root[x] = my_find(root[x], root, count);
	}
}

// union1(x, y): union-by-rank 최적화 
int my_union(int x, int y, int* root, int* rank, int count) {
	x = my_find(x, root, count);
	y = my_find(y, root, count);

	// 두 값의 root가 같으면(이미 같은 트리) 합치지 않는다.
	if (x == y)
		return -1; // debug 필요. 맞나?

	// "union-by-rank 최적화"
	// 항상 높이가 더 낮은 트리를 높이가 높은 트리 밑에 넣는다. 즉, 높이가 더 높은 쪽을 root로 삼음
	if (rank[x] < rank[y]) {
		root[x] = y; // x의 root를 y로 변경
		return y; // return the new root
	}
	else {
		root[y] = x; // y의 root를 x로 변경
		return x; // return the new root

		if (rank[x] == rank[y])
			rank[x]++; // 만약 높이가 같다면 합친 후 (x의 높이 + 1)
	}
}

double my_log2(double x) { return log(x) / log(2); }

bool sortbyX(i_quad& a, i_quad& b) {
	return (a.getX() < b.getX());
}

bool sortbyY(i_quad& a, i_quad& b) {
	return (a.getY() < b.getY());
}

*/
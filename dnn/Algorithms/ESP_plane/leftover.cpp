
/*
void conforming_subdivision::draw_SPBD(i_quad* q) { // draw simple boundary
	double unit_length = pow(2, q->getLv());
	// double x_min = q->ll.k * unit_length, y_min = q->ll.l * unit_length;

	for (__int64 i = 0; i < 4; i++) {
		// BD.push_back(line_segment{ Point{x_min + i * unit_length, y_min} , Point{x_min + (i + 1) * unit_length, y_min} }); // 아래쪽
		// BD.push_back(line_segment{ Point{x_min + i * unit_length, y_min + 3 * unit_length}, Point{x_min + (i + 1) * unit_length, y_min + 3 * unit_length} }); // 위쪽
		// BD.push_back(line_segment{ Point{x_min, y_min + i * unit_length}, Point{x_min, y_min + (i + 1) * unit_length} }); // 왼쪽
		// BD.push_back(line_segment{ Point{x_min + 3 * unit_length, y_min + i * unit_length}, Point{x_min + 3 * unit_length, y_min + (i + 1) * unit_length} }); // 오른쪽
	}
}
*/

/*
	bool conforming_subdivision::contained(i_box* B, vector<core> R1) {
		assert(B.i == R1[0].i);
		for (core C : R1) {
			int x_diff = B.k - C.ll.k, y_diff = B.l - C.ll.l;
			if (0 <= x_diff && x_diff <= 1 && 0 <= y_diff && y_diff <= 1) { return true; }
		}
		return false;
	}

	bool conforming_subdivision::contained(i_box* B, vector<i_quad*> Qs) {
		assert(B.i == Qs[0].i);

		for (auto Q : Qs) {
			int x_diff = B.k - Q.ll.k, y_diff = B.l - Q.ll.l;
			if (0 <= x_diff && x_diff <= 3 && 0 <= y_diff && y_diff <= 3) { return true; }
		}

		return false;
	}

	void conforming_subdivision::draw_CXBD(i_box* B) { // draw complex boundary

		double unit_length = pow(2, B.i);

		// BD.push_back(line_segment{ Point{unit_length * B.k, unit_length * B.l}, Point{unit_length * (double(B.k) + 1), unit_length * B.l} }); // 아래쪽
		// BD.push_back(line_segment{ Point{unit_length * B.k, unit_length * (double(B.l) + 1)}, Point{unit_length * (double(B.k) + 1), unit_length * (double(B.l) + 1)} }); // 위쪽
		// BD.push_back(line_segment{ Point{unit_length * B.k, unit_length * B.l}, Point{unit_length * B.k, unit_length * (double(B.l) + 1)} }); // 왼쪽
		// BD.push_back(line_segment{ Point{unit_length * (double(B.k) + 1), unit_length * B.l}, Point{unit_length * (double(B.k) + 1), unit_length * (double(B.l) + 1)} }); // 오른쪽
	}

	void conforming_subdivision::fill_R1R2(vector<core> R1, vector<i_quad> R2) {
		// R1의 level = R2의 level + 2
		// R1과 R2에서 같은 index의 원소들은 1대1 대응 관계임
		for (core r1 : R1) {
			for (int j = 0; j < 8; j++) {
				for (int ind3 = 0; ind3 < 8; ind3++) {
					i_box B = { 4 * r1.ll.k + j, 4 * r1.ll.l + ind3, r1.i - 2 };
					if (!contained(B, R2)) { draw_CXBD(B); } // draw edges. 4개의 edge를 추가함으로써 반영.
				}
			}
		}
	}


	void conforming_subdivision::fill_R1S(vector<core> R1, vector<i_quad> S_region) {

		for (i_quad q : S_region) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					i_box B = { q.ll.k + i, q.ll.l + j, q.i };
					if (!contained(B, R1)) { draw_CXBD(B); } // intersection 여부를 모든 core에 대해 검사해야 함.
				}
			}
		}

	}
	*/


/*

for (vector<int> S : equiv_classes(Q_i)) {
	vector<i_quad> S_region = {};
	for (int num : S) {
		S_region.push_back(Q_i[num]);
	} // 아래 R2 계산에서도 쓰임.

	vector<i_quad> S_prime = {};
	for (i_quad* q : Q_im2) {
		vector<i_quad>::iterator it = find(S_region.begin(), S_region.end(), *q.G);
		if (it != S_region.end()) { S_prime.push_back(q); }
	}

	if (S_prime.size() > 1) { // (* S is complex *)


		// 아래 R1과 (특히) R2의 자료형은 vector<i_quad>로 두고, 실제로는 union처럼 다룸.

		// (* R1 := union of cores of growth(q) *)
		vector<core> R1 = {};
		for (i_quad q : S_prime) {
			core temp = get_core(*q.G);
			vector<core>::iterator it = find(R1.begin(), R1.end(), temp);
			if (it == R1.end()) { R1.push_back(temp); }
		}

		// (* R2 := the region covered by q. *)
		vector<i_quad> R2 = S_prime;

		fill_R1R2(R1, R2);

		fill_R1S(R1, S_region);
	}

}
*/

// auto ret = Union(vec, 4);

/*
auto ret = union_core(vec);
for (auto elem : get<0>(ret)) {elem->print();} cout << "\n";
auto ret2 = union_normal(vec);
for (auto elem : get<1>(ret2)) { elem->print(); }
// for (auto elem : get<1>(ret)) { elem->print(); }


auto ret = union_normal(vec, true);
for (auto elem: ret) elem->print(); cout << "\n";
auto ret2 = union_normal(vec, false);
for (auto elem : ret2) elem->print(); cout << "\n";
*/
// ret = union_normal(vec);
// for (auto elem : ret) elem->print();
// auto ret2 = myUnion(ret, 3);
// ret2->print();

/*




	list<Point*> vers; // vertices
	// RP(deque<Point*>);
	RP(list<Point*>);


		if (last_column) {
			pind = 0;
			continue;
		}

				if (p1 == c1 && p2 == c2) {
					temp_cur[cind]->top_path = temp_prev[pind]->top_path;
					int tsz = temp_cur[cind]->top_path.size();
					get<0>(temp_cur[cind]->top_path[tsz-1]) += 1;
					temp_cur[cind]->bot_path = temp_prev[pind]->bot_path;
					int bsz = temp_cur[cind]->bot_path.size();
					get<0>(temp_cur[cind]->bot_path[bsz - 1]) += 1;
				}

				// how to maintain only one from left?


	vector<STS*> temp_prev;
	for (int i = 0; i < prev_status.size(); i++) {
		temp_prev.push_back(new STS(*(prev_status[i])));
	}
	i = 0;
	while (i <= int(temp_prev.size()) - 2) {
		if (temp_prev[i]->end == temp_prev[i + 1]->start) {
			temp_prev[i]->end = temp_prev[i + 1]->end; temp_prev.erase(temp_prev.begin() + i + 1); i--;
		}
		i++;
	}


// RP* myUnion(vector<List*>,int);
bool exist(Elem*, Dir);
void extend(RP*, Elem*, Dir); // left : 0, right : 1, lower : 2, upper : 3
RP* traverseUnion(vector<ElemList*>, vector<ElemList*>, int);

RP* myUnion(vector<i_quad*>);
int bin_search(vector<tuple<int, int>*>,int);
int bin_insert(vector<tuple<int, int>>*, int);
int bin_delete(vector<tuple<int, int>>*, int);

RP* mymyUnion(vector<i_quad*>);
int mySearch(vector<quadruple>*, int, bool);
int lnSearch(vector<quadruple>*, int, bool);
void myInsert(vector<quadruple>*, int, int, int);
void myDelete(vector<quadruple>*, int, int, int);
vector<int> overlap(vector<quadruple>*, int, int);

void extend(list<Point*> rp, Elem* elem, Dir dir) {
	if (dir == DIR_LEFT) { // left

	}
	else if (dir == DIR_LOWER) { // lower
		if (elem->getPt(0)->getHEdgeType() == 3 && elem->getPt(1)->getHEdgeType() == 2) {
			elem->getPt(0);
		}
		else if () {

		}
		else if () {

		}
		else if () {

		}
	}
}


RP* traverseUnion(vector<ElemList*> X_lists, vector<ElemList*> Y_lists, int lv) {
	queue<Elem*> Q;
	Q.push(X_lists[0]->getElem(0));

	int x = Q.front()->getKey(), y = Q.front()->getSV();
	// is it a correct way to initialize a list?
	list<Point*> plst = { new Point(x, y, lv), new Point(x + 1, y, lv), new Point(x + 1, y + 1, lv), new Point(x, y + 1, lv) };
	Q.front()->setHEdge(DIR_LEFT, true); Q.front()->setHEdge(DIR_RIGHT, true); Q.front()->setHEdge(DIR_LOWER, true); Q.front()->setHEdge(DIR_UPPER, true);
	auto it = plst.begin(); Q.front()->setPt(LOWER_LEFT, *it); (*it)->setHEdgeType(3);
	advance(it, 1); Q.front()->setPt(LOWER_RIGHT, *it); (*it)->setHEdgeType(2);
	advance(it, 1); Q.front()->setPt(UPPER_LEFT, *it); (*it)->setHEdgeType(1);
	advance(it, 1); Q.front()->setPt(UPPER_RIGHT, *it); (*it)->setHEdgeType(0);

	while (!Q.empty()) { // contains elems only from X_lists
		auto X_elem = Q.front(); Q.pop();
		if (X_elem->getChecked()) continue;
		X_elem->check();
		auto Y_elem = X_elem->getTwin();
		
		// if (exist(Y_elem, DIR_LEFT)) { extend(plst, DIR_LEFT); Q.push(Y_elem->getPrev()->getTwin());}
		// if (exist(Y_elem, DIR_RIGHT)) { extend(plst, DIR_RIGHT); Q.push(Y_elem->getNext()->getTwin());}
		// if (exist(X_elem, DIR_LOWER)) { extend(plst, DIR_LOWER); Q.push(X_elem->getPrev());}
		// if (exist(X_elem, DIR_RIGHT)) { extend(plst, DIR_UPPER); Q.push(X_elem->getNext());}
		
	}

	// transform list of tuples into list of Points

	RP* ret = new RP(plst);
	return ret;
}

RP* myUnion(vector<i_quad*> _Qs) { // plane sweep
	vector<i_quad*> Qs = _Qs;
	int lv = Qs[0]->getLv();

	vector<tuple<int, int, bool>> eventQ; // stores events
	for (auto Q : Qs) {
		int x_gi = getGridIndex(Q->getX(), lv); // computes x- and y-indices on (lv)-th grid
		int y_gi = getGridIndex(Q->getY(), lv);

		eventQ.push_back(make_tuple(x_gi, y_gi, true));
		eventQ.push_back(make_tuple(x_gi + 2, y_gi, false));
	}

	sort(eventQ.begin(), eventQ.end(), greater<>());
	vector<tii>* prev_status = new vector<tii>;
	vector<tii>* cur_status = new vector<tii>; // maintains current intervals. (y_ind, count)

	bool first_column = true;
	int first_x = get<0>(eventQ[eventQ.size() - 1]), prev_x = first_x;

	while (!eventQ.empty()) {

		// there are at least 2 events(1 insert, 1 delete)
		cur_status = prev_status; // duplicate status
		auto ev = eventQ[eventQ.size() - 1]; // last event 
		while (eventQ.size() >= 1 && prev_x == get<0>(ev)) { // if all events are processed, we finish the while loop
			if (get<2>(ev)) { // insert event
				bin_insert(cur_status, get<1>(ev));
				bin_insert(cur_status, get<1>(ev) + 1);
			}
			else { // delete event
				bin_delete(cur_status, get<1>(ev));
				bin_delete(cur_status, get<1>(ev) + 1);
			}
			prev_x = get<0>(ev);
			eventQ.pop_back();

			if (eventQ.empty()) break;
			ev = eventQ[eventQ.size() - 1]; // last event 
		}

		// not the first event, and the x-coordinate of the event is different from the previous one 
		if (first_column) {
			first_column = false; prev_x = get<0>(ev); continue;
		}

		// compare two statuses and draw RP correspondingly
		int pind = 0, cind = 0;
		while (pind < prev_status->size() || cind < cur_status->size()) {

		}

		prev_x = get<0>(ev);
		prev_status = cur_status; // update the previous status
	}

	return NULL;
}

// returns the index corresponding to the one when elem is inserted to vec
// returns last index with the same value 
int bin_search(vector<tuple<int, int>*> vec, int elem) {
	int left = 0, right = vec.size() - 1;
	while (left <= right) {
		if (right - left <= 1) {
			if (get<0>(*vec[right]) == elem) { return right; }
			else if (get<0>(*vec[left]) == elem) { return left; }
			else return -1;
		}
		else if (right - left == 2) {
			if (get<0>(*vec[right]) == elem) { return right; }
			else if (get<0>(*vec[right - 1]) == elem) { return right - 1; }
			else if (get<0>(*vec[left]) == elem) { return left; }
			else return -1;
		}

		int mid = (left + right) / 2;
		if (get<0>(*vec[mid]) < elem) {
			left = mid + 1;
		}
		else if (get<0>(*vec[mid]) > elem) {
			right = mid - 1;
		}
		else {
			while (mid < vec.size() - 1) {
				if (get<0>(*vec[mid + 1]) == elem) { mid += 1; }
			}
			return mid;
		}
	}
}

int bin_insert(vector<tuple<int, int>>* vec, int elem) {
	if (vec->empty()) {
		vec->push_back(make_tuple(elem, 1));
		return 0;
	}

	if (get<0>((*vec)[0]) > elem) {
		vec->insert(vec->begin(), make_tuple(elem, 1));
		return 0;
	}

	if (get<0>((*vec)[vec->size() - 1]) < elem) {
		vec->insert(vec->end(), make_tuple(elem, 1));
		return vec->size() - 1;
	}

	int left = 0, right = vec->size() - 1;
	while (left <= right) {

		int mid = (left + right) / 2;
		if (get<0>((*vec)[mid]) < elem) {
			left = mid + 1;
		}
		else if (get<0>((*vec)[mid]) > elem) {
			right = mid - 1;
		}
		else {
			(*vec)[mid] = make_tuple(get<0>((*vec)[mid]), get<1>((*vec)[mid]) + 1);
			return mid;
		}
	}

	// if elem is not found
	int i;
	for (i = right; i <= left; i++) {
		if (get<0>((*vec)[i]) < elem) {
			vec->insert(vec->begin() + i, make_tuple(elem, 1));
			return i;
		}
	}
	vec->insert(vec->begin() + i, make_tuple(elem, 1));
	return i;
}

int bin_delete(vector<tuple<int, int>>* vec, int elem) {
	// the element should be found

	int left = 0, right = vec->size() - 1;
	while (left <= right) {

		int mid = (left + right) / 2;
		if (get<0>((*vec)[mid]) < elem) {
			left = mid + 1;
		}
		else if (get<0>((*vec)[mid]) > elem) {
			right = mid - 1;
		}
		else {
			if (get<1>((*vec)[mid]) == 1) { vec->erase(vec->begin() + mid); return mid; }
			else {
				(*vec)[mid] = make_tuple(get<0>((*vec)[mid]), get<1>((*vec)[mid]) - 1);
				return mid;
			}
		}
	}
}

RP* mymyUnion(vector<i_quad*> _Qs) { // plane sweep
	vector<i_quad*> Qs = _Qs;
	int lv = Qs[0]->getLv();

	vector<tuple<int, int, bool>> eventQ; // stores events
	for (auto Q : Qs) {
		int x_gi = getGridIndex(Q->getX(), lv); // computes x- and y-indices on (lv)-th grid
		int y_gi = getGridIndex(Q->getY(), lv);

		eventQ.push_back(make_tuple(x_gi, y_gi, true));
		eventQ.push_back(make_tuple(x_gi + 2, y_gi, false));
	}

	sort(eventQ.begin(), eventQ.end(), greater<>());
	vector<quadruple>* prev_status = new vector<quadruple>; // quadruple: start, end, count, path
	vector<quadruple>* cur_status = new vector<quadruple>; // maintains current intervals. (y_ind, count)


	bool last_column = false;
	// int first_x = get<0>(eventQ[eventQ.size() - 1]); int prev_x = first_x;
	int cur_x = get<0>(eventQ[eventQ.size() - 1]), next_x = -1;
	int p1, p2, c1, c2, cind, pind;
	while (!eventQ.empty()) {

		// there are at least 2 events(1 insert, 1 delete)
		cur_status = prev_status; // duplicate status
		auto ev = eventQ[eventQ.size() - 1]; // last event. important!!!!
		while (eventQ.size() >= 1 && cur_x == get<0>(ev)) { // if all events with the same x-coordinate are processed, we finish the while loop
			if (get<2>(ev)) { // insert event
				myInsert(cur_status, get<1>(ev), get<0>(ev), 2);
			}
			else { // delete event
				myDelete(cur_status, get<1>(ev), get<0>(ev), 2);
			}
			// cur_x = get<0>(ev);
			eventQ.pop_back();

			if (eventQ.empty()) break;
			ev = eventQ[eventQ.size() - 1]; // last event 
		}
		if (eventQ.size() >= 1) cur_x = get<0>(ev); // need to debug
		else last_column = true;

		// simplify statuses. for temp_cur and temp_prev, count (which can be obtained by get<2>()) has no meaning.
		
		auto temp_cur = cur_status;
		int i = 0;
		while (i <= int(temp_cur->size()) - 2) {
			if (get<1>((*temp_cur)[i]) == get<0>((*temp_cur)[i + 1])) {
				get<1>((*temp_cur)[i]) = get<1>((*temp_cur)[i+1]); temp_cur->erase(temp_cur->begin() + i+1); i--;
		   }
		   i++;
		}

		auto temp_prev = prev_status;
		int i = 0;
		while (i <= int(temp_prev->size()) - 2) {
			if (get<1>((*temp_prev)[i]) == get<0>((*temp_prev)[i + 1])) {
				get<1>((*temp_prev)[i]) = get<1>((*temp_prev)[i + 1]); temp_prev->erase(temp_prev->begin() + i + 1); i--;
			}
			i++;
		}

		if (last_column) {
			pind = 0;
			continue;
		}

		pind = 0, cind = 0;
		while (pind < temp_prev->size() || cind < temp_cur->size()) {
			if (pind >= temp_prev->size()) { // draw RP portions from temp_cur. only emerge
			   c1 = get<0>((*temp_cur)[cind]), c2 = get<1>((*temp_cur)[cind]);
			   // from below to above. path is ordered clockwise.
			   vector<tii>* path = new vector<tii>;
			   *path = {tii(next_x, c1), tii(cur_x, c1), tii(cur_x, c2), tii(next_x, c2)};
			   get<3>((*temp_cur)[cind]) = path;// update path
		   }
			else if (cind >= temp_cur->size()) { // draw RP portions from temp_prev. only finish (no need to insert a vertex)
				p1 = get<0>((*temp_prev)[pind]), p2 = get<1>((*temp_prev)[pind]);
			}
			else {
				p1 = get<0>((*temp_prev)[pind]), p2 = get<1>((*temp_prev)[pind]);
				c1 = get<0>((*temp_cur)[cind]), c2 = get<1>((*temp_cur)[cind]);
				// how to maintain only one from left?

				if (c2 <= p1) {

				}
				else if (p2 <= c1) {

				}
				else if () {

				}

				if (p1 == c1 && p2 == c2) {
				   auto ptr = get<3>((*temp_cur)[cind]);
				   ptr->push_back(tii(next_x, c2)); ptr->insert((*ptr).begin(), tii(cur_x, c2));
				   get<3>((*temp_cur)[cind]) = ptr;
				}
				else if () {

				}

				if (c1 < p1 && p1 < c2 && c2 < p2) {
				   get<3>((*temp_cur)[cind]) push_back()
				   push_back()
				   insert()
				}
			}
		}

		// not the first event, and the x-coordinate of the event is different from the previous one


		// compare two statuses and draw RP correspondingly

		


		prev_status = cur_status; // update the previous status
	}

	return NULL;
}

int mySearch(vector<quadruple>* vec, int elem, bool left_check) {
	if (vec->empty()) { return -1; }

	if (left_check) {
		if (get<0>(vec->front()) > elem) { return -1; }
		else if (get<0>(vec->back()) > elem) { return -(int(vec->size()) + 1); }
	}
	else {
		if (get<1>(vec->front()) > elem) { return -1; }
		else if (get<1>(vec->back()) > elem) { return -(int(vec->size()) + 1); }
	}

	int left = 0, right = vec->size() - 1;
	while (left <= right) {

		int mid = (left + right) / 2;

		if (left_check) {
			if (get<0>((*vec)[mid]) == elem) { return mid; }
			else if (get<0>((*vec)[mid]) > elem) { right = mid - 1; }
			else { left = mid + 1; }
		}
		else {
			if (get<1>((*vec)[mid]) == elem) { return mid; }
			else if (get<1>((*vec)[mid]) > elem) { right = mid - 1; }
			else { left = mid + 1; }
		}

	}
}

int lnSearch(vector<quadruple>* vec, int elem, bool left) {
	if (left) {
		if (get<0>(vec->front()) > elem) { return -1; }
		if (get<0>(vec->front()) == elem) { return 0; }
		if (get<0>(vec->back()) < elem) { return -(int(vec->size()) + 1); }
		if (get<0>(vec->back()) == elem) { return vec->size() - 1; }
		for (int i = 1; i < vec->size(); i++) {
			if (get<0>((*vec)[i]) == elem) { return i; }
			if (get<0>((*vec)[i - 1]) < elem && get<0>((*vec)[i]) > elem) { return -(i + 1); }
		}
	}
	else {
		if (get<1>(vec->front()) > elem) { return -1; }
		if (get<1>(vec->front()) == elem) { return 0; }
		if (get<1>(vec->back()) < elem) { return -(int(vec->size()) + 1); }
		if (get<1>(vec->back()) == elem) { return vec->size() - 1; }
		for (int i = 1; i < vec->size(); i++) {
			if (get<1>((*vec)[i]) == elem) { return i; }
			if (get<1>((*vec)[i - 1]) < elem && get<1>((*vec)[i]) > elem) { return -(i + 1); }
		}
	}
}

vector<int> overlap(vector<quadruple>* vec, int start, int end) { // ln search after bin search possible, will implement later
	vector<int> ret;
	for (int i = 0; i < vec->size(); i++) {
		int s = get<0>((*vec)[i]), e = get<1>((*vec)[i]);
		if (!(end <= s || start >= e)) { ret.push_back(i); }
	}

	if (!ret.empty()) return ret;
	else {
		for (int i = 0; i <= vec->size() - 2; i++) {
			if (get<1>((*vec)[i]) <= start && end <= get<0>((*vec)[i + 1])) { return { -(i + 2) }; }
		}
	}
	throw invalid_argument("error");
}

void myInsert(vector<quadruple>* vec, int elem, int x_ind, int step) {
	int start = elem, end = elem + step;

	for (int i = start; i < end; i++) {
		vec->push_back(quadruple(i, i + 1, 1, NULL));
	}

	sort(vec->begin(), vec->end());

	int i = 0;
	// postprocessing
	while (i <= int(vec->size()) - 2) {

		int ps = get<0>((*vec)[i]), pe = get<1>((*vec)[i]), pc = get<2>((*vec)[i]);
		int qs = get<0>((*vec)[i + 1]), qe = get<1>((*vec)[i + 1]), qc = get<2>((*vec)[i + 1]);

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


}

void myDelete(vector<quadruple>* vec, int elem, int x_ind, int step) {
	int start = elem, end = elem + step;

	for (int i = start; i < end; i++) {
		vec->push_back(quadruple(i, i + 1, -1, NULL));
	}

	sort(vec->begin(), vec->end());

	int i = 0;
	// postprocessing
	while (i <= int(vec->size()) - 2) {

		int ps = get<0>((*vec)[i]), pe = get<1>((*vec)[i]), pc = get<2>((*vec)[i]);
		int qs = get<0>((*vec)[i + 1]), qe = get<1>((*vec)[i + 1]), qc = get<2>((*vec)[i + 1]);

		if (pc == -1) { // ps == qs
			if (pe < qe) {
				if (qc == 1) {
					vec->erase(vec->begin() + i); get<0>((*vec)[i]) = pe; i--;
				}
				else {
					get<2>((*vec)[i]) = qc - 1;
					get<0>((*vec)[i + 1]) = pe;
				}
			}
			else { // pe == qe
				if (qc == 1) {
					vec->erase(vec->begin() + i); vec->erase(vec->begin() + i); i--; // i--;
				}
				else {
					vec->erase(vec->begin() + i); get<2>((*vec)[i]) -= 1; i--;
				}
			}
		}
		else if (qc == -1 && pe > qs) { // ps < qs
			if (qe == pe) {
				if (pc == 1) {
					get<0>((*vec)[i]) = qs;
					vec->erase(vec->begin() + i + 1); i--;
				}
				else {
					get<0>((*vec)[i]) = qs;
					get<2>((*vec)[i + 1]) = pc - 1;
				}
			}
			else { // qe < pe
				if (pc == 1) {
					get<0>((*vec)[i]) = qs;
					get<0>((*vec)[i + 1]) = qe;
					get<1>((*vec)[i + 1]) = pe;
					get<2>((*vec)[i + 1]) = pc;
				}
				else {
					get<0>((*vec)[i]) = qs;
					get<2>((*vec)[i + 1]) = pc - 1;
					vec->insert(vec->begin() + i + 2, quadruple(qe, pe, pc, NULL));
				}
			}
		}
		i++;
	}
}

*/


/*

		 if (first_column) {
			cind = 0;
			while (pind < temp_prev->size() || cind < temp_cur->size()) {
				c1 = get<0>((*temp_cur)[cind]), c2 = get<1>((*temp_cur)[cind]);
				vector<tii>* path = new vector<tii>;
				*path = { tii(next_x, c1), tii(cur_x, c1) };
				get<3>((*temp_cur)[cind]) = path;// update path
			}
			 first_column = false;
			 cur_x = get<0>(ev); // may need to debug
			 continue;
		 }

	 // bool first_column = true;


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
				ret->addHEdge_back(new HEdge(get<0>(event), tmp)); first = false;
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
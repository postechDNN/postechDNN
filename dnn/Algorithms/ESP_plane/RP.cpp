#include "RP.h"
#include <tuple>
#include <queue>
#include <stdexcept>

using namespace std;

RP::RP(){}
RP::~RP(){}
RP::RP(list<Point*> _vers) {vers = _vers;}
void RP::print() {
	for (auto elem : vers) {
			elem->print();
	}
}
void RP::addPt(Point* pt) {vers.push_back(pt);}

bool exist(Elem* cur, Dir dir) {
	if (dir == DIR_LEFT || dir == DIR_LOWER) {
		if (cur->getPrev() == NULL || cur->getSV() > cur->getPrev()->getSV() + 1) return false;
		else return true;
	}
	else if (dir == DIR_RIGHT || dir == DIR_UPPER) {
		if (cur->getNext() == NULL || cur->getSV() + 1 < cur->getNext()->getSV()) return false;
		else return true;
	}
}
/*
void extend(list<Point*> rp, Elem* elem, Dir dir) {	
	if (dir == DIR_LEFT) { // left
		
	}
	else if (dir == DIR_LOWER) { // lower
		if (elem->getPt(0)->getEdgeType() == 3 && elem->getPt(1)->getEdgeType() == 2) {
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
*/

RP* traverseUnion(vector<ElemList*> X_lists, vector<ElemList*> Y_lists, int lv) {
	queue<Elem*> Q;
	Q.push(X_lists[0]->getElem(0)); 

	int x = Q.front()->getKey(), y = Q.front()->getSV();
	// is it a correct way to initialize a list?
	list<Point*> plst= { new Point(x, y, lv), new Point(x+1, y, lv), new Point(x+1, y+1, lv), new Point(x, y+1, lv) };
	Q.front()->setEdge(DIR_LEFT, true); Q.front()->setEdge(DIR_RIGHT, true); Q.front()->setEdge(DIR_LOWER, true); Q.front()->setEdge(DIR_UPPER, true);
	auto it = plst.begin(); Q.front()->setPt(LOWER_LEFT, *it); (*it)->setEdgeType(3);
	advance(it, 1); Q.front()->setPt(LOWER_RIGHT, *it); (*it)->setEdgeType(2);
	advance(it, 1); Q.front()->setPt(UPPER_LEFT, *it); (*it)->setEdgeType(1);
	advance(it, 1); Q.front()->setPt(UPPER_RIGHT, *it); (*it)->setEdgeType(0);

	while (!Q.empty()) { // contains elems only from X_lists
		auto X_elem = Q.front(); Q.pop();
		if (X_elem->getChecked()) continue;
		X_elem->check();
		auto Y_elem = X_elem->getTwin();
		/*
		if (exist(Y_elem, DIR_LEFT)) { extend(plst, DIR_LEFT); Q.push(Y_elem->getPrev()->getTwin());}
		if (exist(Y_elem, DIR_RIGHT)) { extend(plst, DIR_RIGHT); Q.push(Y_elem->getNext()->getTwin());}
		if (exist(X_elem, DIR_LOWER)) { extend(plst, DIR_LOWER); Q.push(X_elem->getPrev());}
		if (exist(X_elem, DIR_RIGHT)) { extend(plst, DIR_UPPER); Q.push(X_elem->getNext());}
		*/
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
		eventQ.push_back(make_tuple(x_gi+2, y_gi, false));
	}

	sort(eventQ.begin(), eventQ.end(), greater<>());
	vector<tii> *prev_status = new vector<tii>;
	vector<tii> *cur_status = new vector<tii>; // maintains current intervals. (y_ind, count)

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
	int left = 0, right = vec.size()-1;
	while (left <= right) {
		if (right - left <= 1) {
			if (get<0>(*vec[right]) == elem) { return right; }
			else if (get<0>(*vec[left]) == elem) {return left;}
			else return -1;
		}
		else if (right - left == 2) {
			if (get<0>(*vec[right]) == elem) { return right; }
			else if (get<0>(*vec[right-1]) == elem) { return right-1;}
			else if (get<0>(*vec[left]) == elem) { return left; }
			else return -1;
		}

		int mid = (left + right)/2;
		if (get<0>(*vec[mid]) < elem) {
			left = mid + 1;
		}
		else if (get<0>(*vec[mid]) > elem) {
			right = mid - 1;
		}
		else {
			while (mid < vec.size() - 1) {
				if (get<0>(*vec[mid+1]) == elem) { mid += 1;}
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

	if (get<0>((*vec)[vec->size()-1]) < elem) {
		vec->insert(vec->end(), make_tuple(elem, 1));
		return vec->size()-1;
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
			vec->insert(vec->begin()+i, make_tuple(elem, 1));
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
			if (get<1>((*vec)[mid]) == 1) {vec->erase(vec->begin()+mid); return mid;}
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

	 bool first_column = true;
	 int first_x = get<0>(eventQ[eventQ.size() - 1]), prev_x = first_x;

	 while (!eventQ.empty()) {

		 // there are at least 2 events(1 insert, 1 delete)
		 cur_status = prev_status; // duplicate status
		 auto ev = eventQ[eventQ.size() - 1]; // last event 
		 while (eventQ.size() >= 1 && prev_x == get<0>(ev)) { // if all events are processed, we finish the while loop
			 if (get<2>(ev)) { // insert event
				 myInsert(cur_status, get<1>(ev), get<0>(ev), 2);
			 }
			 else { // delete event
				 myDelete(cur_status, get<1>(ev), get<0>(ev), 2);
			 }
			 prev_x = get<0>(ev);
			 eventQ.pop_back();

			 if (eventQ.empty()) break;
			 ev = eventQ[eventQ.size() - 1]; // last event 
		 }

		 // simplify
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

		 int pind = 0, cind = 0;
		 while (pind < prev_status->size() || cind < cur_status->size()) {
			 if (pind >= prev_status->size()) { // draw RP portions from cur_status. only emerge
				tii* path = new tii({new tii(), new tii(), new tii(), new tii()}):
				get<3>(cur_status[cind]) = 
			}
			 else if (cind >= cur_status->size()) { // draw RP portions from prev_status. only finish

			 }
			 else {

			 }
		 }

		 // not the first event, and the x-coordinate of the event is different from the previous one 
		 if (first_column) {

			
			 first_column = false; prev_x = get<0>(ev); continue;
		 }

		 // compare two statuses and draw RP correspondingly
		 /*

		 */

		 prev_x = get<0>(ev);
		 prev_status = cur_status; // update the previous status
	 }

	 return NULL;
 }

 int mySearch(vector<quadruple>* vec, int elem, bool left_check) {
	if (vec->empty()) { return -1;}

	if (left_check) {
		if (get<0>(vec->front()) > elem) { return -1;}
		else if (get<0>(vec->back()) > elem) { return -(int(vec->size())+1);}
	}
	else {
		if (get<1>(vec->front()) > elem) { return -1; }
		else if (get<1>(vec->back()) > elem) { return -(int(vec->size()) + 1); }
	}

	 int left = 0, right = vec->size() - 1;
	 while (left <= right) {

		 int mid = (left + right) / 2;
		
		 if (left_check) {
			 if (get<0>((*vec)[mid]) == elem) { return mid;}
			 else if (get<0>((*vec)[mid]) > elem) { right = mid-1;}
			 else { left = mid+1;}
		 }
		 else {
			 if (get<1>((*vec)[mid])== elem) { return mid;}
			 else if (get<1>((*vec)[mid]) > elem) { right = mid - 1; }
			 else { left = mid + 1; }
		 }

	 }
 }

 int lnSearch(vector<quadruple>* vec, int elem, bool left) {
	if (left) {
		if (get<0>(vec->front()) > elem) { return -1; }
		if (get<0>(vec->front()) == elem)  {return 0;}
		if (get<0>(vec->back()) < elem) { return -(int(vec->size())+1);}
		if (get<0>(vec->back()) == elem) { return vec->size()-1; }
		for (int i = 1; i < vec->size(); i++) {
			if (get<0>((*vec)[i]) == elem) { return i;}
			if (get<0>((*vec)[i-1]) < elem && get<0>((*vec)[i]) > elem) { return -(i+1);}
		}
	}
	else {
		if (get<1>(vec->front()) > elem) { return -1; }
		if (get<1>(vec->front()) == elem) { return 0; }
		if (get<1>(vec->back()) < elem) { return -(int(vec->size()) + 1); }
		if (get<1>(vec->back()) == elem) { return vec->size() - 1; }
		for (int i = 1; i < vec->size(); i++) {
			if (get<1>((*vec)[i]) == elem) { return i; }
			if (get<1>((*vec)[i - 1]) < elem && get<0>((*vec)[i]) > elem) { return -(i + 1); }
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
		for (int i = 0; i <= vec->size()-2; i++) {
			if (get<1>((*vec)[i]) <= start && end <= get<0>((*vec)[i+1])) { return {-(i+2)}; }
		}
	}
	throw invalid_argument("error");
 }

 void myInsert(vector<quadruple>* vec, int elem, int x_ind, int step) {
	int start = elem, end = elem + step;
	
	for (int i = start; i < end; i++) {
		vec->push_back(quadruple(i, i+1, 1, NULL));
	}

	sort(vec->begin(), vec->end());

	int i = 0;
	// postprocessing
	while(i <= int(vec->size())-2){

		int ps = get<0>((*vec)[i]), pe = get<1>((*vec)[i]), pc = get<2>((*vec)[i]);
		int qs = get<0>((*vec)[i+1]), qe = get<1>((*vec)[i+1]), qc = get<2>((*vec)[i+1]);

		if (ps == qs) {
			if (pe == qe) { 
				get<2>((*vec)[i]) += qc;
				vec->erase(vec->begin() + i + 1); i--;
			}
			else {
				get<2>((*vec)[i]) += qc;
				get<0>((*vec)[i+1]) = pe;
			}
		}
		else if (qs < pe) {
			if (pe > qe) {
				get<0>((*vec)[i]) = qs;
				get<2>((*vec)[i+1]) += pc;
				vec->insert(vec->begin() + i+2, quadruple(qe, pe, pc, NULL));
			}
			else if (pe == qe) {
				get<0>((*vec)[i]) = qs;
				get<2>((*vec)[i+1]) += pc;
			}
			else {
				get<1>((*vec)[i]) = qs;
				get<1>((*vec)[i+1]) = pe;
				get<2>((*vec)[i + 1]) += pc;
				vec->insert(vec->begin() + i+2, quadruple(pe, qe, qc, NULL));
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
					 get<2>((*vec)[i]) = qc -1;
					 get<0>((*vec)[i + 1]) = pe;
				 }
			}
			 else { // pe == qe
				 if (qc == 1) {
					 vec->erase(vec->begin()+i); vec->erase(vec->begin() + i); i--; // i--;
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
					 vec->erase(vec->begin() + i+1); i--;
				}
				 else {
					 get<0>((*vec)[i]) = qs;
					 get<2>((*vec)[i+1]) = pc-1;
				 }
			}
			 else { // qe < pe
				 if (pc == 1) {
					 get<0>((*vec)[i]) = qs;
					 get<0>((*vec)[i+1]) = qe;
					 get<1>((*vec)[i + 1]) = pe;
					 get<2>((*vec)[i + 1]) = pc;
				}
				 else {
					 get<0>((*vec)[i]) = qs;
					 get<2>((*vec)[i + 1]) = pc-1;
					 vec->insert(vec->begin() + i + 2, quadruple(qe, pe, pc, NULL));
				 }
			 }
		 }
		i++;
	}
 }
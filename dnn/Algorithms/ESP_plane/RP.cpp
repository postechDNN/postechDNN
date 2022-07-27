#include "RP.h"
#include <tuple>
#include <queue>
#include <stdexcept>

using namespace std;
typedef tuple<int, int> tii;
typedef tuple<int, int, int, tuple<int, int>> quadruple;

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
	 vector<quadruple>* prev_status = new vector<quadruple>;
	 vector<quadruple>* cur_status = new vector<quadruple>; // maintains current intervals. (y_ind, count)

	 bool first_column = true;
	 int first_x = get<0>(eventQ[eventQ.size() - 1]), prev_x = first_x;

	 while (!eventQ.empty()) {

		 // there are at least 2 events(1 insert, 1 delete)
		 cur_status = prev_status; // duplicate status
		 auto ev = eventQ[eventQ.size() - 1]; // last event 
		 while (eventQ.size() >= 1 && prev_x == get<0>(ev)) { // if all events are processed, we finish the while loop
			 if (get<2>(ev)) { // insert event
				 // bin_insert(cur_status, get<1>(ev));
			 }
			 else { // delete event
				 // bin_delete(cur_status, get<1>(ev));
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

 /*
 int bin_insert(vector<tuple<int, int, int, tuple<int, int>>>* vec, int elem) {
	int start = elem, end = elem + 2;

	return 0;
 }
 */
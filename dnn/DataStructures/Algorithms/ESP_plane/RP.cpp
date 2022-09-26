#include "RP.h"
#include <tuple>
#include <queue>
#include <stdexcept>

using namespace std;

RP::RP() {}
RP::~RP() {}
RP::RP(vector<Point*> _vers) { vers = _vers; }
// RP::RP(list<Point*> _vers) { vers = _vers; }
void RP::print() {
	for (auto elem : vers) {
		elem->print();
	}
}
void RP::addPt(Point* pt) { vers.push_back(pt); }

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

RP* Union(vector<i_quad*> _Qs, int step) { // plane sweep
	vector<i_quad*> Qs = _Qs;
	int lv = Qs[0]->getLv();

	vector<tuple<int, int, bool>> eventQ; // stores events
	for (auto Q : Qs) {
		int x_gi = getGridIndex(Q->getX(), lv); // computes x- and y-indices on (lv)-th grid
		int y_gi = getGridIndex(Q->getY(), lv);

		eventQ.push_back(make_tuple(x_gi, y_gi, true));
		eventQ.push_back(make_tuple(x_gi +  step, y_gi, false));
	}

	sort(eventQ.begin(), eventQ.end(), greater<>());
	vector<STS*> prev_status; // quadruple: start, end, count, path
	vector<STS*> cur_status; // maintains current intervals. (y_ind, count)

	vector<STS*> Final = {}, temp_prev = {}, temp_cur = {};
	bool last_column = false;
	// int first_x = get<0>(eventQ[eventQ.size() - 1]); int prev_x = first_x;
	int prev_x = -1; int cur_x = get<0>(eventQ[eventQ.size() - 1]); // , next_x = -1;
	int p1, p2, c1, c2, cind, pind;
	while (!eventQ.empty()) {

		// there are at least 2 events(1 insert, 1 delete)
		cur_status = prev_status; // duplicate status
		auto ev = eventQ[eventQ.size() - 1]; // last event. important!!!!
		while (eventQ.size() >= 1 && cur_x == get<0>(ev)) { // if all events with the same x-coordinate are processed, we finish the while loop
			if (get<2>(ev)) { // insert event
				myInsert(cur_status, get<1>(ev), get<0>(ev), step);
			}
			else { // delete event
				myDelete(cur_status, get<1>(ev), get<0>(ev), step);
			}
			// cur_x = get<0>(ev);
			eventQ.pop_back();
			prev_x = get<0>(ev);

			if (eventQ.empty()) break;
			ev = eventQ[eventQ.size() - 1]; // last event 
		}
		if (eventQ.size() >= 1) cur_x = get<0>(ev); // need to debug
		else last_column = true;

		// simplify statuses. for temp_cur and temp_prev, count (which can be obtained by get<2>()) has no meaning.

		temp_cur.clear(); // cur_status.clear();

		for (int i = 0; i < cur_status.size(); i++) {
			temp_cur.push_back(new STS(*(cur_status[i])));
		}
		int i = 0;
		while (i <= int(temp_cur.size()) - 2) {
			if (temp_cur[i]->end == temp_cur[i + 1]->start) {
				temp_cur[i]->end = temp_cur[i + 1]->end; temp_cur.erase(temp_cur.begin() + i + 1); i--;
			}
			i++;
		}

		pind = 0, cind = 0;
		// tell if previous top_path and bot_path are already used
		bool top_used = false, bot_used = false;
		while (pind < temp_prev.size() || cind < temp_cur.size()) {
			if (pind >= temp_prev.size()) { // draw RP portions from temp_cur. only emerge
				c1 = temp_cur[cind]->start, c2 = temp_cur[cind]->end;
				// from below to above. path is ordered clockwise.
				temp_cur[cind]->conn = true;
				// rightward. because we want to use push_back to insert new vertices
				temp_cur[cind]->bot_path = { tii(prev_x, c1), tii(cur_x, c1),  }; 
				temp_cur[cind]->top_path = { tii(prev_x, c2), tii(cur_x, c2) };
				cind++;
			}
			else if (cind >= temp_cur.size()) { // draw RP portions from temp_prev. only finish 
																  // no need to insert a vertex. just set conn = true.
				temp_prev[pind]->conn = true;
				// temp_cur[cind] = temp_prev[pind];
				pind++;
			}
			else {
				p1 = temp_prev[pind]->start, p2 = temp_prev[pind]->end;
				c1 = temp_cur[cind]->start, c2 = temp_cur[cind]->end;

				if (c2 <= p1) {
					temp_cur[cind]->conn = true;
					// rightward. because we want to use push_back to insert new vertices
					temp_cur[cind]->bot_path = { tii(prev_x, c1), tii(cur_x, c1), };
					temp_cur[cind]->top_path = { tii(prev_x, c2), tii(cur_x, c2) };
					cind++; continue;
				}

				if (p2 <= c1) {
					temp_prev[pind]->conn = true;
					// temp_cur[cind] = temp_prev[pind];
					pind++; continue;
				}

				// ensures that temp_prev[pind] and temp_cur[cind] always overlap. 

				// process c1
				if (c1 < p1 ) {
					temp_cur[cind]->bot_path = temp_prev[pind]->bot_path;
					temp_cur[cind]->bot_path.push_back(tii(prev_x, c1));
					temp_cur[cind]->bot_path.push_back(tii(cur_x, c1));
				}
				else if (c1 == p1) {
					temp_cur[cind]->bot_path = temp_prev[pind]->bot_path;
					temp_cur[cind]->bot_path.pop_back();
					temp_cur[cind]->bot_path.push_back(tii(cur_x, c1));
				}
				else { // p1 < c1  && c1 < p2
					if (!bot_used) {
						bot_used = true;
						temp_cur[cind]->bot_path = temp_prev[pind]->bot_path;
					}
					temp_cur[cind]->bot_path.push_back(tii(prev_x, c1));
					temp_cur[cind]->bot_path.push_back(tii(cur_x, c1));
				}

				// process c2
				if (c2 > p2) {
					temp_cur[cind]->top_path = temp_prev[pind]->top_path;
					temp_cur[cind]->top_path.push_back(tii(prev_x, c2));
					temp_cur[cind]->top_path.push_back(tii(cur_x, c2));
					top_used = false; bot_used = false; pind++;
				}
				else if (c2 == p2) {
					temp_cur[cind]->top_path = temp_prev[pind]->top_path;
					temp_cur[cind]->top_path.pop_back();
					temp_cur[cind]->top_path.push_back(tii(cur_x, c2));
					top_used = false; bot_used = false; pind++; cind++;
				}
				else { // p1 < c2 && c2 < p2 
					if (!top_used) {
						top_used = true;
						temp_cur[cind]->top_path = temp_prev[pind]->top_path;
					}
					temp_cur[cind]->top_path.push_back(tii(prev_x, c2));
					temp_cur[cind]->top_path.push_back(tii(cur_x, c2));
					cind++;
				}
			}
		}
		Final = temp_prev;
		temp_prev = temp_cur;
		prev_status = cur_status; // update the previous status
	}

	// merge paths two form a rectilinear polygon (vertices in clockwise direction)
	vector<Point*> vers;
	for (auto elem : Final) {
		auto temp = tii2Point(elem->top_path, pow(2, lv));
		vers.insert(vers.begin(), temp.begin(), temp.end());
		reverse(elem->bot_path.begin(), elem->bot_path.end());
		auto temp2 = tii2Point(elem->bot_path, pow(2, lv));
		vers.insert(vers.begin(), temp2.begin(), temp2.end());
	}
	RP* ret = new RP(vers);

	return ret;
}

int mySearch(vector<STS*>& vec, int elem, bool left_check) {
	int sz = int(vec.size());
	if (vec.empty()) { return -1; }

	if (left_check) {
		if (vec[0]->start > elem) { return -1; }
		else if (vec[sz - 1]->start > elem) { return -(sz + 1); }
	}
	else {
		if (vec[0]->end > elem) { return -1; }
		else if (vec[sz - 1]->end > elem) { return -(sz + 1); }
	}

	int left = 0, right = sz - 1;
	while (left <= right) {

		int mid = (left + right) / 2;

		if (left_check) {
			if (vec[mid]->start == elem) { return mid; }
			else if (vec[mid]->start > elem) { right = mid - 1; }
			else { left = mid + 1; }
		}
		else {
			if (vec[mid]->end == elem) { return mid; }
			else if (vec[mid]->end > elem) { right = mid - 1; }
			else { left = mid + 1; }
		}

	}
}

int lnSearch(vector<STS*>& vec, int elem, bool left) {
	int sz = int(vec.size());
	if (left) {
		if (vec[0]->start > elem) { return -1; }
		if (vec[0]->start == elem) { return 0; }
		if (vec[sz - 1]->start < elem) { return -(sz + 1); }
		if (vec[sz - 1]->start == elem) { return sz - 1; }
		for (int i = 1; i < sz; i++) {
			if (vec[i]->start == elem) { return i; }
			if (vec[i - 1]->start < elem && vec[i]->start > elem) { return -(i + 1); }
		}
	}
	else {
		if (vec[0]->end > elem) { return -1; }
		if (vec[0]->end == elem) { return 0; }
		if (vec[sz - 1]->end < elem) { return -(sz + 1); }
		if (vec[sz - 1]->end == elem) { return sz - 1; }
		for (int i = 1; i < sz; i++) {
			if (vec[i]->end == elem) { return i; }
			if (vec[i - 1]->end < elem && vec[i]->end > elem) { return -(i + 1); }
		}
	}
}

vector<int> overlap(vector<STS*>& vec, int start, int end) { // ln search after bin search possible, will implement later
	vector<int> ret;
	int sz = int(vec.size());
	for (int i = 0; i < sz; i++) {
		int s = vec[i]->start, e = vec[i]->end;
		if (!(end <= s || start >= e)) { ret.push_back(i); }
	}

	if (!ret.empty()) return ret;
	else {
		for (int i = 0; i <= sz - 2; i++) {
			if (vec[i]->end <= start && end <= vec[i + 1]->start) { return { -(i + 2) }; }
		}
	}
	throw invalid_argument("error");
}

void myInsert(vector<STS*>& vec, int elem, int x_ind, int step) {

	int start = elem, end = elem + step;

	for (int i = start; i < end; i++) {
		vec.push_back(new STS(i, i + 1, 1));
	}

	// sort(vec.begin(), vec.end(), sortSTS); // may cause problem. need to debug.

	int i = 0;
	// postprocessing
	while (i <= int(vec.size()) - 2) {
		sort(vec.begin(), vec.end(), sortSTS);

		int ps = vec[i]->start, pe = vec[i]->end, pc = vec[i]->count;
		int qs = vec[i + 1]->start, qe = vec[i + 1]->end, qc = vec[i + 1]->count;

		if (ps == qs) {
			if (pe == qe) {
				vec[i]->count += qc;
				vec.erase(vec.begin() + i + 1); i--;
			}
			else {
				vec[i]->count += qc;
				vec[i + 1]->start = pe;
			}
		}
		else if (qs < pe) {
			if (pe > qe) {
				vec[i]->start = qs;
				vec[i + 1]->count += pc;
				vec.insert(vec.begin() + i + 2, new STS(qe, pe, pc));
			}
			else if (pe == qe) {
				vec[i]->start = qs;
				vec[i + 1]->count += pc;
			}
			else {
				vec[i]->end = qs;
				vec[i + 1]->end = pe;
				vec[i + 1]->count += pc;
				vec.insert(vec.begin() + i + 2, new STS(pe, qe, qc));
			}
		}
		else if (qs == pe) {
			if (pc == qc) {
				vec[i]->end = qe;
				vec.erase(vec.begin() + i + 1); i--;
			}
		}
		i++;
	}


}

void myDelete(vector<STS*>& vec, int elem, int x_ind, int step) {
	int start = elem, end = elem + step;

	for (int i = start; i < end; i++) {
		vec.push_back(new STS(i, i + 1, -1));
	}

	// sort(vec.begin(), vec.end(), sortSTS);

	int i = 0;
	// postprocessing
	while (i <= int(vec.size()) - 2) {
		sort(vec.begin(), vec.end(), sortSTS); // need to debug.

		int ps = vec[i]->start, pe = vec[i]->end, pc = vec[i]->count;
		int qs = vec[i + 1]->start, qe = vec[i + 1]->end, qc = vec[i + 1]->count;

		if (pc == -1) { // ps == qs
			if (pe < qe) {
				if (qc == 1) {
					vec.erase(vec.begin() + i); vec[i]->start = pe; i--;
				}
				else {
					vec[i]->count = qc - 1;
					vec[i + 1]->start = pe;
				}
			}
			else { // pe == qe
				if (qc == 1) {
					vec.erase(vec.begin() + i); vec.erase(vec.begin() + i); i--;
				}
				else {
					vec.erase(vec.begin() + i); vec[i]->count -= 1; i--;
				}
			}
		}
		else if (qc == -1 && pe > qs) { // ps < qs
			if (qe == pe) {
				if (pc == 1) {
					vec[i]->start = qs;
					vec.erase(vec.begin() + i + 1); i--;
				}
				else {
					vec[i]->start = qs;
					vec[i + 1]->count = pc - 1;
				}
			}
			else { // qe < pe
				if (pc == 1) {
					vec[i]->start = qs;
					vec[i + 1]->start = qe;
					vec[i + 1]->end = pe;
					vec[i + 1]->count = pc;
				}
				else {
					vec[i]->start = qs;
					vec[i + 1]->count = pc - 1;
					vec.insert(vec.begin() + i + 2, new STS(qe, pe, pc));
				}
			}
		}
		i++;
	}
}

STS::STS() {}
STS::STS(int s, int e, int c) { start = s; end = e; count = c; }
STS::STS(int s, int e, int c, vector<tii> t, vector<tii> b) { start = s; end = e; count = c; t = top_path; bot_path = b; }

bool sortSTS(STS* a, STS* b) {
	if (a->start < b->start) { return true; }
	else if (a->start > b->start) { return false; }

	if (a->end < b->end) { return true; }
	else if (a->end > b->end) { return false; }

	if (a->count < b->count) { return true; }
	else if (a->count > b->count) { return false; }

	return true;
}

vector<Point*> tii2Point(vector<tii> ts, int lv) {
	vector<Point*> ret;
	for (auto elem : ts) {
		Point* temp = new Point(get<0>(elem) * lv, get<1>(elem) * lv);
		ret.push_back(temp);
	}

	return ret;
}
#include "i_quad.h"
// #include "Point.h" // recently added
#include <algorithm>
#include "Point.h"
#include <iostream>
#include <tuple>

using namespace std;

i_quad::i_quad() {}
i_quad::~i_quad() {}
i_quad::i_quad(int _lv, int _x, int _y) { lv = _lv; x = _x; y = _y; }
i_quad::i_quad(int _lv, double _x, double _y) {lv = _lv; x = _x; y = _y;}
bool operator>(i_quad& a, i_quad& b) { // consider only the y-coordinate, for plane-sweep.
	if (a.getY() > b.getY()) { return true; }
	else { return false; }
}
bool operator==(i_quad& a, i_quad& b) {
	return (abs(a.getX() - b.getX()) < EPS) && (abs(a.getY() - b.getY()) < EPS) && a.getLv() == b.getLv();
}
int i_quad::getLv() { return lv; }
int i_quad::getX() { return x; }
int i_quad::getY() { return y; }
void i_quad::setG(i_quad* _G) { G = _G; }
i_quad* i_quad::getG() { return G; }
int i_quad::getIndex() { return index; }
void i_quad::setIndex(int _index) { index = _index; }
void i_quad::addTouching(i_quad* _q) { touching.push_back(_q); }
bool i_quad::getVisited() { return visited; }
void i_quad::setVisited(bool _visited) { visited = _visited; }
vector<i_quad*> i_quad::getTouching() { return touching; }
// int i_quad::getEquiClass() { return equi_class; }
// void i_quad::setEquiClass(int _e) { equi_class = _e; }

bool operator<(i_quad& a, i_quad& b) { return a.getY() < b.getY(); }

List::List(){}
List::~List(){}
List::List(int _key, vector<int> _lst){
	key = _key;
	lst = _lst;
	checked.assign(lst.size(), false);
}
int List::getKey(){ return key;}
vector<int> List::getLst(){ return lst;}
void List::add(int num){ lst.push_back(num);}
void List::check(int ind) { checked[ind] = true;}
bool List::getChecked(int ind) { return checked[ind];}

void List::print() {
	cout << key << endl;
	for (auto elem : lst) {
		cout << elem << " ";
	}
	cout << "\n" << endl;
}

ElemList::ElemList(){}
ElemList::ElemList(bool _key_x, int _key, Elem* _elem){key_x = _key_x; key = _key; elems = {_elem}; }
void ElemList::addElem(Elem* elem) {elems.push_back(elem);}
int ElemList::getKey() { return key;}
bool ElemList::getKeyX() { return key_x;}
Elem* ElemList::getElem(int index) { return elems[index];}
// Elem* ElemList::getElem(0) { return elems[0]; }
// Elem* ElemList::getLastElem() {return elems[elems.size()-1];}

tuple<vector<ElemList*>, vector<ElemList*>> union_core(vector<i_quad*> _Qs) {
	vector<i_quad*> Qs = _Qs;

	vector<ElemList*> X_ret, Y_ret;
	int lv = Qs[0]->getLv();

	vector<Elem*> X_cores = {};
	vector<Elem*> Y_cores = {};
	for (auto Q : Qs) {
		int x_gi = getGridIndex(Q->getX(), lv); // computes x- and y-indices on (lv)-th grid
		int y_gi = getGridIndex(Q->getY(), lv);

		for (int i = 1; i < 3; i++) {
			for (int j = 1; j < 3; j++) {
				X_cores.push_back(new Elem(x_gi + i, y_gi + j));
				Y_cores.push_back(new Elem(y_gi + i, x_gi + j));
				Y_cores[Y_cores.size()-1]->setTwin(X_cores[X_cores.size()-1]);
				X_cores[X_cores.size() - 1]->setTwin(Y_cores[Y_cores.size() - 1]);
			}
		}
	}

	sort(X_cores.begin(), X_cores.end(), sortElems);
	sort(Y_cores.begin(), Y_cores.end(), sortElems);

	for (auto it = X_cores.begin(); it != X_cores.end(); it++) {
		if (it == X_cores.begin()) { // first element
			X_ret.push_back(new ElemList(true, (*it)->getKey(), (*it)));
		}
		else { // compare current element with the previous one
			auto it_copy = it; --it_copy;

			if ((*it)->getKey() == (*it_copy)->getKey()) {
				int sz = X_ret[X_ret.size() - 1]->getElems().size();
				if (X_ret[X_ret.size() - 1]->getElem(sz-1)->getSV() != (*it)->getSV()) X_ret.back()->addElem(*it);
			}
			else {
				X_ret.push_back(new ElemList(true, (*it)->getKey(), (*it)));
			}
		}
	}

	for (auto xc : X_ret) {
		auto elems = xc->getElems();
		for (unsigned int i = 0; i < elems.size(); i++) { // always more than 2 elements
			if (i == 0) {
				elems[i]->setNext(elems[i+1]);
			}
			else if (i == elems.size() - 1) {
				elems[i]->setPrev(elems[i-1]);
			}
			else {
				elems[i]->setPrev(elems[i-1]);
				elems[i]->setNext(elems[i+1]);
			}
		}
	}

	for (auto it = Y_cores.begin(); it != Y_cores.end(); it++) {
		if (it == Y_cores.begin()) { // first element
			Y_ret.push_back(new ElemList(true, (*it)->getKey(), (*it)));
		}
		else { // compare current element with the previous one
			auto it_copy = it; --it_copy;

			if ((*it)->getKey() == (*it_copy)->getKey()) {
				int sz = Y_ret[Y_ret.size() - 1]->getElems().size();
				if (Y_ret[Y_ret.size() - 1]->getElem(sz-1)->getSV() != (*it)->getSV()) Y_ret.back()->addElem(*it);
			}
			else {
				Y_ret.push_back(new ElemList(true, (*it)->getKey(), (*it)));
			}
		}
	}

	for (auto yc : Y_ret) {
		auto elems = yc->getElems();
		for (unsigned int i = 0; i < elems.size(); i++) { // always more than 2 elements
			if (i == 0) {
				elems[i]->setNext(elems[i + 1]);
			}
			else if (i == elems.size() - 1) {
				elems[i]->setPrev(elems[i - 1]);
			}
			else {
				elems[i]->setPrev(elems[i - 1]);
				elems[i]->setNext(elems[i + 1]);
			}
		}
	}

	return make_tuple(X_ret, Y_ret);
}

tuple<vector<ElemList*>, vector<ElemList*>> union_normal(vector<i_quad*> _Qs) {
	vector<i_quad*> Qs = _Qs;

	vector<ElemList*> X_ret, Y_ret;
	int lv = Qs[0]->getLv();

	vector<Elem*> X_cores = {};
	vector<Elem*> Y_cores = {};
	for (auto Q : Qs) {
		int x_gi = getGridIndex(Q->getX(), lv); // computes x- and y-indices on (lv)-th grid
		int y_gi = getGridIndex(Q->getY(), lv);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				X_cores.push_back(new Elem(x_gi + i, y_gi + j));
				Y_cores.push_back(new Elem(y_gi + i, x_gi + j));
				Y_cores[Y_cores.size() - 1]->setTwin(X_cores[X_cores.size() - 1]);
				X_cores[X_cores.size() - 1]->setTwin(Y_cores[Y_cores.size() - 1]);
			}
		}
	}

	sort(X_cores.begin(), X_cores.end(), sortElems);
	sort(Y_cores.begin(), Y_cores.end(), sortElems);

	for (auto it = X_cores.begin(); it != X_cores.end(); it++) {
		if (it == X_cores.begin()) { // first element
			X_ret.push_back(new ElemList(true, (*it)->getKey(), (*it)));
		}
		else { // compare current element with the previous one
			auto it_copy = it; --it_copy;

			if ((*it)->getKey() == (*it_copy)->getKey()) {
				int sz = X_ret[X_ret.size() - 1]->getElems().size();
				if (X_ret[X_ret.size() - 1]->getElem(sz-1)->getSV() != (*it)->getSV()) X_ret.back()->addElem(*it);
			}
			else {
				X_ret.push_back(new ElemList(true, (*it)->getKey(), (*it)));
			}
		}
	}

	for (auto it = Y_cores.begin(); it != Y_cores.end(); it++) {
		if (it == Y_cores.begin()) { // first element
			Y_ret.push_back(new ElemList(true, (*it)->getKey(), (*it)));
		}
		else { // compare current element with the previous one
			auto it_copy = it; --it_copy;

			if ((*it)->getKey() == (*it_copy)->getKey()) {
				int sz = Y_ret[Y_ret.size() - 1]->getElems().size();
				if (Y_ret[Y_ret.size() - 1]->getElem(sz-1)->getSV() != (*it)->getSV()) Y_ret.back()->addElem(*it);
			}
			else {
				Y_ret.push_back(new ElemList(true, (*it)->getKey(), (*it)));
			}
		}
	}

	return make_tuple(X_ret, Y_ret);
}

Elem::Elem() {}
Elem::Elem(int _key, int _sv) {key = _key; second_val = _sv; checked = false; twin = NULL; 
												next = prev = NULL;  memset(dirs, false, sizeof(bool) * 4);
												// pts = {NULL, NULL, NULL, NULL};}
												memset(pts, NULL, sizeof(Point*) * 4);}
int Elem::getKey() { return key; }
int Elem::getSV() { return second_val;}
void Elem::setNext(Elem* _next) { next = _next;}
Elem* Elem::getNext() { return next; }
void Elem::setPrev(Elem* _prev) { prev = _prev;}
Elem* Elem::getPrev() { return prev; }
bool Elem::getChecked() {return checked;}
void Elem::setTwin(Elem* _twin) { twin = _twin; }
Elem* Elem::getTwin() {return twin;}
void Elem::check() { checked = true;}
void Elem::setEdge(Dir dir, bool isEdge) {
	dirs[dir] = isEdge;
}
void Elem::setPt(Dir2 dir, Point* ptr) {
	pts[dir] = ptr;
}
Point* Elem::getPt(int index) {return pts[index];}

void ElemList::print() {
	cout << key << endl;
	for (auto elem : elems) {
		cout << elem->getSV() << " ";
	}
	cout << "\n" << endl;
}
bool ElemList::getChecked(int ind) {
	return elems[ind]->getChecked(); 
}
void ElemList::check(int ind) { elems[ind]->check(); }
vector<Elem*> ElemList::getElems() { return elems; }

bool sortElems(Elem* a, Elem* b) {
	if (a->getKey() < b->getKey()) { return true; }
	else if (a->getKey() > b->getKey()) { return false;}
	else return a->getSV() < b->getSV();
}

bool sortbyXIncr(i_quad* a, i_quad* b) {
	return ((*a).getX() < (*b).getX());
}

bool sortbyYDecr(i_quad* a, i_quad* b) {
	return ((*a).getY() > (*b).getY());
}

bool sortbyYIncr(i_quad* a, i_quad* b) {
	return ((*a).getY() < (*b).getY());
}
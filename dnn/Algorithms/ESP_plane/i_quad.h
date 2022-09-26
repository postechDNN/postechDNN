#pragma once
#include <vector>
#define EPS 0.000001
typedef struct List List;
#include "Point.h" // recently added

using namespace std;

class i_quad {
private:
	int lv; // level
	double x; // its lower-left corner lies at a point (x, y) which equals (a2^(lv), b2^(lv)) for some integers a, b.
	double y; // should be int?
	i_quad* G; // growth
	bool alone;
	int index;
	vector<i_quad*> touching; // store i_quads each of whose closure intersects with this i_quad.
	bool visited;
	// int equi_class;
	// core는 get_core()를 통해서 필요할 때마다 i_quad로부터 뽑아내면 됨.
public:
	i_quad();
	~i_quad();
	// i_quad(int, int);
	// i_quad(double, double);
	i_quad(int,int,int);
	i_quad(int, double, double);
	friend bool operator>(i_quad& a, i_quad& b);
	friend bool operator==(i_quad& a, i_quad& b);
	int getLv();
	int getX();
	int getY();
	i_quad* getG();
	void setG(i_quad*);
	int getIndex();
	void setIndex(int);
	// int getEquiClass();
	// void setEquiClass(int);
	void addTouching(i_quad*);
	bool getVisited();
	void setVisited(bool);
	vector<i_quad*> getTouching();
	i_quad* operator=(const i_quad* q) { 
		// lv = q.lv; x = q.x; y = q.y; G = q.G; alone = q.alone; 
		lv = q->lv; x = q->x; y = q->y; G = q->G; alone = q->alone;
		return this; 
	}
};

enum Dir {
	DIR_LEFT,
	DIR_RIGHT,
	DIR_LOWER,
	DIR_UPPER,
};

enum Dir2 {
	LOWER_LEFT,
	LOWER_RIGHT,
	UPPER_LEFT,
	UPPER_RIGHT,
};

class Elem {
private:
	int key;
	int second_val;
	Elem *next, *prev, *twin;
	bool checked;
	bool dirs[4];
	// vector<Point*> pts;
	Point *pts[4]; // *ll, *lr, *ul, *ur;
public:
	Elem();
	Elem(int, int);
	int getKey();
	int getSV();
	void setNext(Elem*);
	Elem* getNext();
	void setPrev(Elem*);
	Elem* getPrev();
	bool getChecked();
	void setTwin(Elem*);
	Elem* getTwin();
	void check();
	void setEdge(Dir, bool);
	void setPt(Dir2, Point*);
	Point* getPt(int);
};

class ElemList {
private:
	bool key_x;
	int key;
	vector<Elem*> elems;
public:
	ElemList();
	ElemList(bool, int, Elem*);
	void addElem(Elem*);
	int getKey();
	bool getKeyX();
	Elem* getElem(int);
	// Elem* getElem(0);
	// Elem* getLastElem();
	void print();
	bool getChecked(int);
	void check(int);
	vector<Elem*> getElems();
};

class List {
private:
	int key;
	vector<int> lst;
	vector<bool> checked;
public:
	List();
	~List();
	List(int, vector<int>);
	int getKey();
	vector<int> getLst();
	void add(int);
	void check(int);
	bool getChecked(int);
	void print();
};

tuple<vector<ElemList*>, vector<ElemList*>> union_core(vector<i_quad*> _Qs);
tuple<vector<ElemList*>, vector<ElemList*>> union_normal(vector<i_quad*> _Qs);


bool sortElems(Elem* a, Elem* b);
bool sortbyXIncr(i_quad* a, i_quad* b);
bool sortbyYDecr(i_quad* a, i_quad* b);
bool sortbyYIncr(i_quad* a, i_quad* b);
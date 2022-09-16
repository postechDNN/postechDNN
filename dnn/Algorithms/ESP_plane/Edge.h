#pragma once
#include "Point.h"

class Face;

class Edge { // assume that each edge is oriented clockwise.
public:
	Point* p1;
	Point* p2;
	int lv; // length of the 'line_segment' equals the unit length in level lv
	double length;
	Face* f1; 
	Face* f2;
	Edge* next;
	Edge* prev;
public:
	Edge();
	~Edge();
	Edge(Point*, Point*);
	void editP1(Point*);
	void editP2(Point*);
	Point* getP1();
	Point* getP2();
	void setNext(Edge*);
	Edge* getNext();
	void getPrev(Edge*);
	Edge* getPrev();
	Face* getTwin(Face*);
	double getLength();
	void print();
};

// bool sortbylength(Edge& a, Edge& b);
bool sortbylength(Edge* a, Edge* b);
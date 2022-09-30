#pragma once
#include "Point.h"
#define ERR 1e-6
#include <utility>

class Face;

	class Edge { // assume that each edge is oriented clockwise.
	public:
		Point* p1;
		Point* p2;
		int lv; // length of the 'line_segment' equals the unit length in level lv
		double Length;
		double Length_inf;
		Face* f1; 
		Face* f2;
		Edge* next;
		Edge* prev;

		Point s, t;
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
		double getLength_inf();
		void print();

		Edge(const Point& _s, const Point& _t);
		Edge(const Edge&);
		bool operator==(Edge&);
		bool on(Point& p);
		Edge* crossing(Edge&, bool);
		Point gets();
		Point gett();
		void sets(Point&);
		void sett(Point&);
		double length();
	};

	// bool sortbylength(Edge& a, Edge& b);

bool sortbylength(Edge* a, Edge* b);
bool sortbylength_inf(Edge* a, Edge* b);
std::pair<double, double> inline find_mid_points(double num1, double num2, double num3, double num4);
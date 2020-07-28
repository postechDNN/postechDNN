#include "PolygonClipping.h"

PolygonClipping::PolygonClipping(){}

PolygonClipping::~PolygonClipping(){}

//A(P,Q,R) = (Q.x - P.x) (R.y - P.y) - (Q.y - P.y)(R.x - P.x)
//It is the function that computes twice the signed area of the triange [PQR]
inline double PolygonClipping::A_func(Point P, Point Q, Point R) {
	return (Q.getx() - P.getx()) * (R.gety() - P.gety()) - (Q.gety() - P.gety()) * (R.getx() - P.getx());
}

inline void PolygonClipping::circularNext(std::list<__Node>& l, std::list<__Node>::iterator& it) {
	it++;
	if (it == l.end()) it = l.begin();
}

inline void PolygonClipping::circularPrev(std::list<__Node>& l, std::list<__Node>::iterator& it) {
	if (it == l.begin()) it = l.end();
	it--;
}

//There is 3 type of intersection
//X-intersection, T-intersection, V-intersection
//Note that two half edge is half open edge
void PolygonClipping::intersection_phase(std::list<__Node>& P, std::list<__Node>& Q) {
	Point P1, P2, Q1, Q2;	//[P1,P2) , [Q1,Q2) Half open edge
	for (auto it_i = P.begin();it_i != P.end();it_i++) {
		P1 = it_i->p;
		auto next_it_i = it_i;
		circularNext(P, next_it_i);
		P2 = next_it_i->p;
		for (auto it_j = Q.begin();it_j != Q.end(); it_j++) {
			Q1 = it_j->p;
			auto next_it_j = it_j;
			circularNext(Q, next_it_j);
			Q2 = next_it_j->p;
			//If both edges are parallel, then they can intersect or rather overlap
			if (abs(A_func(P1, Q1, Q2)) < 1e-6 && abs(A_func(P2, Q1, Q2)) < 1e-6 && abs(A_func(Q1, P1, P2)) < 1e-6 && abs(A_func(Q2, P1, P2)) < 1e-6) {
				overlap_case(P, it_i, Q, it_j);
			}
			if (abs(A_func(P1, Q1, Q2) - A_func(P2, Q1, Q2)) > 1e-6 && abs((A_func(Q1, P1, P2) - A_func(Q2, P1, P2))) > 1e-6) {
				double alpha = A_func(P1, Q1, Q2) / (A_func(P1, Q1, Q2) - A_func(P2, Q1, Q2));
				double beta = A_func(Q1, P1, P2) / (A_func(Q1, P1, P2) - A_func(Q2, P1, P2));
				if (-1e-6 <= alpha && alpha < 1- 1e-6 && -1e-6 <= beta && beta < 1- 1e-6) {
					intersection_case(P, it_i, Q, it_j);
				}
			}
		}
	}
}

void PolygonClipping::intersection_case(std::list<__Node>& P, std::list<__Node>::iterator e_p, std::list<__Node>& Q, std::list<__Node>::iterator e_q) {

	Point P1, P2, Q1, Q2;	//[P1,P2) , [Q1,Q2) Half open edge

	P1 = e_p->p;
	auto next_e_p = e_p;
	circularNext(P, next_e_p);
	P2 = next_e_p->p;
	Q1 = e_q->p;
	auto next_e_q = e_q;
	circularNext(Q, next_e_q);
	Q2 = next_e_q->p;
	//the dominator of alpha and beta do not vanish as long as the two half open edges are parallel.
	double alpha = A_func(P1, Q1, Q2) / (A_func(P1, Q1, Q2) - A_func(P2, Q1, Q2));
	double beta = A_func(Q1, P1, P2) / (A_func(Q1, P1, P2) - A_func(Q2, P1, P2));
	Point I((P1 * (1 - alpha)) + (P2 * alpha));

	/*-----------TEST-------------------
	std::cout << alpha << ' ' << beta << std::endl;
	std::cout << Edge(P1,P2) << " " << Edge(Q1,Q2)<< std::endl;
	std::cout << I << std::endl;
	
	*///-----------TEST-------------------

	//X-intersection
	//We can add I to polygon P and Polygon Q and link the two copies
	if (1e-6 < alpha && alpha < 1- 1e-6 && 1e-6 < beta && beta < 1- 1e-6) {
		e_p++, e_q++;
		P.insert(e_p, __Node(I, true));
		Q.insert(e_q, __Node(I, true));
		e_p--,e_q--;
		e_p->link = e_q;
		e_q->link = e_p;

		//std::cout << "X intersection " << I<<std::endl;
	}

	//T-intersection
	//p1 lies on the edge [q1,q2] but does not coincide with q1 or q2
	//Add a copy of p1 to polygon Q, and link it with p1
	if (abs(alpha) < 1e-6 && 1e-6 < beta && beta < 1- 1e-6 && e_p->intersect == false) {
		e_q++;
		Q.insert(e_q, __Node(P1, true));
		e_p->intersect = true;
		e_q--;
		e_p->link = e_q;
		e_q->link = e_p;

		//std::cout << "T1 intersection "<<P1<<std::endl;
	}

	//T-intersection
	//q1 lies on the edge [p1,p2] but does not coincide with p1 or p2
	//Add a copy of q1 to polygon P, and link it with q1
	if (1e-6 < alpha && alpha < 1- 1e-6 && abs(beta) < 1e-6 && e_q->intersect == false) {
		e_p++;
		P.insert(e_p, __Node(Q1, true));
		e_q->intersect = true;
		e_p--;
		e_p->link = e_q;
		e_q->link = e_p;
		//std::cout << "T2 intersection "<<Q1 << std::endl;
	}

	//V-intersection
	//both edges intersect at p1== q1
	//We link p1 with q1
	if (abs(alpha) < 1e-6 && abs(beta) < 1e-6) {
		e_p->intersect = true;e_q->intersect = true;
		e_p->link = e_q;
		e_q->link = e_p;

		//std::cout << "V intersection "<<P1<<std::endl;
	}
}

void PolygonClipping::overlap_case(std::list<__Node>& P, std::list<__Node>::iterator e_p, std::list<__Node>& Q, std::list<__Node>::iterator e_q) {
	Point P1, P2, Q1, Q2;	//[P1,P2) , [Q1,Q2) Half open edge

	P1 = e_p->p;
	auto next_e_p = e_p;
	circularNext(P, next_e_p);
	P2 = next_e_p->p;
	Q1 = e_q->p;
	auto next_e_q = e_q;
	circularNext(Q, next_e_q);
	Q2 = next_e_q->p;

	double alpha = ((Q1 - P1).dot_product(P2 - P1)) / ((P2 - P1).dot_product(P2 - P1));
	double beta = ((P1 - Q1).dot_product(Q2 - Q1)) / ((Q2 - Q1).dot_product(Q2 - Q1));

	//X-overlap
	//wee add p1 to Q linked with p1, and q1 to P linked with q1.
	if (1e-6 < alpha && alpha < 1- 1e-6 && 1e-6 < beta && beta < 1- 1e-6 && e_p->intersect == false && e_q->intersect == false) {
		e_q++; //Q2
		Q.insert(e_q, __Node(P1, true));
		e_q--; //i point
		e_p->intersect = true;
		e_p->link = e_q; e_q->link = e_p;
		e_q--;	//e_q lies on Q1

		e_p++;	//P2
		P.insert(e_p, __Node(Q1, true));
		e_p--; //i point
		e_q->intersect = true;
		e_q->link = e_p; e_p->link = e_q;

		//std::cout << "X overlap "<<P1<<Q1<<std::endl;
	}

	//T-overlap
	//we add p1 to Q linked with p1
	if ((alpha < -1e-6 || alpha >= 1- 1e-6) && 1e-6 < beta && beta < 1- 1e-6 && e_p->intersect == false) {
		e_q++;
		Q.insert(e_q, __Node(P1, true));
		e_q--;
		e_p->intersect = true;
		e_p->link = e_q;e_q->link = e_p;

		//std::cout << "T1 overlap "<<P1 << std::endl;
	}

	//T-overlap
	//we add q1 to P linked with q1
	if ((beta < -1e-6 || beta >= 1- 1e-6) && 1e-6 < alpha && alpha < 1- 1e-6 && e_q->intersect == false) {
		e_p++;
		P.insert(e_p, __Node(Q1, true));
		e_p--;
		e_q->intersect = true;
		e_q->link = e_p;e_p->link = e_q;

		//std::cout << "T2 overlap "<<Q1 <<std::endl;
	}

	//V-overlap
	//p1 = q1 we link p1 with q1
	if (abs(alpha) < 1e-6 && abs(beta) < 1e-6) {
		e_p->intersect = true; e_q->intersect = true;
		e_p->link = e_q;e_q->link = e_p;

		//std::cout << "V overlap "<< P1 << std::endl;
	}
}

//Mark intersection vertices as entry or exit points.
//Output) -1 : disjoint, 0 : Include(Q includes P), 1 : intersection
int PolygonClipping::labeling_phase(SimplePolygon& poly_Q, std::list<__Node>& P, std::list<__Node>& Q) {
	int crossing = 0;

	for (auto e_p = P.begin();e_p != P.end(); e_p++) {
		if (e_p->intersect == true){
			auto e_q = e_p->link;
			Point I = e_p->p;
			circularPrev(P, e_p);
			Point P1 = e_p->p;
			circularNext(P, e_p);circularNext(P, e_p);
			Point P2 = e_p->p; circularPrev(P, e_p);

			circularPrev(Q, e_q);
			Point Q1 = e_q->p;
			circularNext(Q, e_q);circularNext(Q, e_q);
			Point Q2 = e_q->p;

			int t = turn_direction(Q1, P1, I, P2)* turn_direction(Q2, P1, I, P2);
			if (t < 0) {  	//Crossing
				e_p->sub_label.type = __Type::CROSSING;
				crossing = 1;
			}
			else if (t > 0)	//Bouncing
				e_p->sub_label.type = __Type::BOUNCING;
			else	//Sublabelling
				e_p->sub_label = sublabelling(P1, P2, I, Q1, Q2);
		}
	}

	for (auto e_p = P.begin(); e_p != P.end();e_p++) {
		if (e_p->sub_label.type == __Type::DELAYED) {
			auto start = e_p, end = e_p;
			do {
				circularPrev(P, start);
			} while (start->sub_label.type == __Type::DELAYED && start->sub_label.d2 == __Delayed::ON && e_p != start);
			
			if (e_p == start) {	//polygon P == polygon Q , All intersection points are bouncing
				for (auto it = P.begin();it != P.end();it++)
					it->sub_label.type = __Type::BOUNCING;
				break;
			}
			//Now P != Q
			circularNext(P, start);

			do {
				circularNext(P, end);
			} while (end->sub_label.type == __Type::DELAYED && end->sub_label.d1 == __Delayed::ON);
			circularPrev(P, end);

			if (start->sub_label.d1 == end->sub_label.d2) {
				while (start != end) {
					start->sub_label.type = __Type::BOUNCING;
					circularNext(P,start);
				}
				end->sub_label.type = __Type::BOUNCING;

			}
			else{
				while (start != end) {
					start->sub_label.type = __Type::BOUNCING;
					circularNext(P, start);
				}
				end->sub_label.type = __Type::CROSSING;
				crossing = 1;
			}
			e_p = end;
		}
	}

	//If there is no CROSSING vertex, two polygons are disjoint or one includes other.



	//Find start vertex which does not lie on Q
	//Start vertex can be the vertex which does not lies on Q
	auto cur = P.begin();
	Point start_point(cur->p);
	for (auto it = P.begin();it != P.end();it++) {
		if (it->intersect == false) {
			cur = it;
			start_point = it->p;
			break;
		}
		else if (it->sub_label.d1 != __Delayed::ON) {	//d1 = NONE(NOT DELAYED),LEFT,RIGHT
			Point cur_point = it->p;
			cur = it;
			circularPrev(P, it);
			start_point = it->p;
			if (it->intersect == true)
				start_point = cur_point * 0.5 + start_point * 0.5;
			break;
		}
	}

	//NONCROSSING : Deal with the degenerate Case
	//P = Q : cur == P.end()
	//P and Q are disjoint : inout = outside
	//P includes Q : inout = outside (DOES NOT CHECK)
	//Q includes P : cur == P.end() or inout = inside
	
	int inout = poly_Q.inPolygon(start_point); //-1 : outside, 1 : inside

	if (crossing == 0) {
		if (inout == -1)	//P and Q are disjoint or Q < P
			return -1;
		else if (inout == 0)	//P == Q
			return 0;
		else  return 0;	//P < Q
	}


	//Label the CROSSING vertex
	int P_list_len = P.size();
	for (int i = 0; i < P_list_len; i++, circularNext(P,cur)) {
		if (cur->sub_label.type == __Type::CROSSING) {
			if (inout == 1) {		//Exit
				cur->label = -1;
				inout = -1;
			}
			else {					//Entry
				cur->label = 1;
				inout = 1;
			}
		}
	}
	return 1;
}

//Determine whether Q lies to the left of to the right of the polygonal chain (P1,P2,P3)
//Output) Left : -1, On : 0, Right = 1
int PolygonClipping::turn_direction(Point Q, Point P1, Point P2, Point P3) {
	double s1 = A_func(Q, P1, P2), s2 = A_func(Q, P2, P3), s3 = A_func(P1, P2, P3);

	if (s3 > 1e-6) {	//Left turn polygonal chain
		if (s1 > 1e-6 && s2 > 1e-6) return -1;
		else if (s1 < -1e-6 || s2 < -1e-6) return 1;
		else return 0;
	}
	else if (s3 < -1e-6) { //Right turn polygonal chain
		if (s1 > 1e-6 || s2 > 1e-6) return -1;
		else if (s1 < -1e-6 && s2 < -1e-6) return 1;
		else return 0;
	}
	else {	//Straight polygonal chain
		if (s1 > 1e-6) return -1;
		else if (s1 < -1e-6) return 1;
		else return 0;
	}

}
__SubLabel PolygonClipping::sublabelling(Point P1, Point P2,Point I, Point Q1, Point Q2){
	__SubLabel ret;
	ret.type = __Type::DELAYED;

	if ((P2 == Q2 && turn_direction(Q1, P1, I, P2) > 0) || (P2 == Q1 && turn_direction(Q2, P1, I, P2) > 0)) {	//Left/On
		ret.d1 = __Delayed::LEFT;
		ret.d2 = __Delayed::ON;
	}
	if ((P2 == Q2 && turn_direction(Q1, P1, I, P2) < 0) || (P2 == Q1 && turn_direction(Q2, P1, I, P2) < 0)) { //Right/On
		ret.d1 = __Delayed::RIGHT;
		ret.d2 = __Delayed::ON;
	}
	if ((P2 == Q2 && P1 == Q1) || (P2 == Q1 && P1 == Q2)) { //On/On
		ret.d1 = __Delayed::ON;
		ret.d2 = __Delayed::ON;
	}
	if ((P1 == Q1 && turn_direction(Q2, P1, I, P2) > 0) || (P1 == Q2 && turn_direction(Q1, P1, I, P2) > 0)) { //On/Left
		ret.d1 = __Delayed::ON;
		ret.d2 = __Delayed::LEFT;
	}
	if ((P1 == Q1 && turn_direction(Q2, P1, I, P2) < 0) || (P1 == Q2 && turn_direction(Q1, P1, I, P2) < 0)) { //On/Right
		ret.d1 = __Delayed::ON;
		ret.d2 = __Delayed::RIGHT;
	}
	return ret;
}
//Trace all points and return the intersection polygons 
std::vector<SimplePolygon*> PolygonClipping::tracing_phase(std::list<__Node>& P, std::list<__Node>& Q) {
	std::vector<SimplePolygon*> ret;
	for (auto start = P.begin();start != P.end();start++) {
		int dir = 1;	//1: forward = entry , 0 : backward = exit
		auto it = start;
		for (;it != P.end(); it++) {
			if (it->label == 1 && it->visit == false) {//Entry
				start = it;
				break;
			}
			/*
			if (it->label == -1 && it->visit == false) { // Exit
				start = it; dir = 0;
				break;
			}*/
		}
		if (it == P.end())	//There is no non-visited crossing anymore
			break;

		std::vector<Point> pts;

		int poly_flag = 0; //0 : P, 1 : Q
		std::list<__Node>* poly_list[2] = { &P,&Q };

		auto cur = start;
		
		do {
			if (dir) circularNext(*poly_list[poly_flag], cur);
			else circularPrev(*poly_list[poly_flag], cur);
			pts.push_back(cur->p);
			if (cur->intersect && cur->link->sub_label.type == __Type::CROSSING) {	//Encounter
				cur->visit = true;
				cur = cur->link;
				cur->visit = true;
				poly_flag ^= 1;
				if (cur->label == 1) dir = 1;
				else if (cur->label == -1) dir = 0;
			}
		} while (cur->p != start->p);
		
		ret.push_back(new SimplePolygon(pts,false));
	}
	
	//If all vertices are

	return ret;
}

//Weiler Atherton Polygon Clipping Algorithm
//Do not consider Degenerate Case
std::vector<SimplePolygon*> PolygonClipping::clipping(SimplePolygon& P, SimplePolygon& Q) {
	std::list<__Node> list_P, list_Q;

	//1 : Construct List Phase
	for (auto it : P.getVertices()) 
		list_P.push_back(__Node(it, false));
	for (auto it : Q.getVertices())
		list_Q.push_back(__Node(it, false));
	
	//2 : Intersection Phase
	intersection_phase(list_P, list_Q);

	//3 : Labelling Phase
	int r1 = labeling_phase(Q, list_P, list_Q);
	int r2 = labeling_phase(P, list_Q, list_P);

	std::vector<SimplePolygon*> ret;
	if (r1 == 0) { //P < Q
		std::vector<Point> P_vertices = P.getVertices();
		ret.push_back(new SimplePolygon(P_vertices));

		//std::cout << "RETURN P\n";
		return ret;
	}
	else if (r2 == 0) { //Q < P
		std::vector<Point> Q_vertices = Q.getVertices();
		ret.push_back(new SimplePolygon(Q_vertices));
		//std::cout << "RETURN Q\n";
		return ret;
	}
	else if (r1 == -1 && r2 == -1) {	//P and Q are disjoint
		//std::cout << "DISJOINT\n";
		return ret;
	}
	//std::cout << "INTERSECTION\n";
	//4 : Tracing Phase
	return tracing_phase(list_P, list_Q);
}
#ifndef __L1VORONOI_H__
#define __L1VORONOI_H__

#include "Point.h"
#include "Polygon.h"
#include "PolygonClipping.h"
#include <iostream>
#include <algorithm>
#include <vector>

class L1_Bisector {
protected:
	std::vector<Point>* vertice;
	int Case, Subcase;
	int n_vertice;
	double values[14];
	enum{ P_X, P_Y, Q_X, Q_Y,MP_X,MQ_X,MP_Y,MQ_Y, MIN_X, MIN_Y, MAX_X,MAX_Y};


	//M_X, M_Y = the middle coordinate between p and q
	//MP_X , MP_Y = the point which divide p and q which is more closer to p than q
	const std::pair<int, int> vertex_seq[4][8][7] = {	//Case, Subcase, Vertices
		{	//Case 0 : they lie on the vertical line
			{	//Subcase 0 : p_y > q_y
				{MIN_X,MP_Y},{MAX_X,MP_Y},{MAX_X,MAX_Y},{MIN_X,MAX_Y},},
			{	//Subcase 1 : p_y < q_y
				{MAX_X,MP_Y},{MIN_X,MP_Y},{MIN_X,MIN_Y},{MAX_X,MIN_Y},},
		},
		{	//Case 1 : they lie on the horizontal line
			{	//Subcase 0 : p_x > q_x
				{MP_X,MAX_Y},{MP_X, MIN_Y},{MAX_X,MIN_Y},{MAX_X,MAX_Y},},
			{	//Subcase 1 : p_x < q_x
				{MP_X,MIN_Y},{MP_X, MAX_Y},{MIN_X,MAX_Y},{MIN_X,MIN_Y},},
		},
		{	//Case 2 : they are the diagonal points of regular structure
			{	//Subcase 0 : p_x < q_x && p_y < q_y
				{MAX_X,P_Y},{Q_X,P_Y},{P_X,Q_Y},{P_X,MAX_Y},{MIN_X,MAX_Y},{MIN_X,MIN_Y},{MAX_X,MIN_Y}},
			{	//Subcase 1 : p_x < q_x && p_y > q_y
				{P_X,MIN_Y},{P_X,Q_Y},{Q_X,P_Y},{MAX_X,P_Y},{MAX_X,MAX_Y},{MIN_X,MAX_Y},{MIN_X,MIN_Y}},
			{	//Subcase 2 : p_x > q_x&& p_y < q_y
				{P_X,MAX_Y},{P_X,Q_Y},{Q_X,P_Y},{MIN_X,P_Y},{MIN_X,MIN_Y},{MAX_X,MIN_Y},{MAX_X,MAX_Y}},
			{	//Subcase 3 : p_x > q_x && p_y > q_y
				{MIN_X,P_Y},{Q_X,P_Y},{P_X,Q_Y},{P_X,MIN_Y},{MAX_X,MIN_Y},{MAX_X,MAX_Y},{MIN_X,MAX_Y}},
		},
		{	//CASE 3 : they are the diagonal points of rectangular
			{	//Subcase 0 : abs(p_x - q_x) > abs(p_y - q_y) && p_x < q_x && p_y < q_y 
				{MQ_X,MIN_Y},{MQ_X,P_Y},{MP_X,Q_Y},{MP_X,MAX_Y},{MIN_X,MAX_Y},{MIN_X,MIN_Y},},
			{	//Subcase 1 : abs(p_x - q_x) > abs(p_y - q_y) && p_x < q_x && p_y > q_y 
				{MP_X,MIN_Y},{MP_X,Q_Y},{MQ_X,P_Y},{MQ_X,MAX_Y},{MIN_X,MAX_Y},{MIN_X,MIN_Y},},
			{	//Subcase 2 : abs(p_x - q_x) > abs(p_y - q_y) && p_x > q_x && p_y < q_y 
				{MP_X,MAX_Y},{MP_X,Q_Y},{MQ_X,P_Y},{MQ_X,MIN_Y},{MAX_X,MIN_Y},{MAX_X,MAX_Y},},
			{	//Subcase 3 : abs(p_x - q_x) > abs(p_y - q_y) && p_x > q_x && p_y > q_y 
				{MQ_X,MAX_Y},{MQ_X,P_Y},{MP_X,Q_Y},{MP_X,MIN_Y},{MAX_X,MIN_Y},{MAX_X,MAX_Y},},
			{	//Subcase 4 : abs(p_x - q_x) < abs(p_y - q_y) && p_x < q_x && p_y < q_y 
				{MAX_X,MP_Y},{Q_X,MP_Y},{P_X,MQ_Y},{MIN_X,MQ_Y},{MIN_X,MIN_Y},{MAX_X,MIN_Y},},
			{	//Subcase 5 : abs(p_x - q_x) < abs(p_y - q_y) && p_x < q_x && p_y > q_y 
				{MIN_X,MQ_Y},{P_X,MQ_Y},{Q_X,MP_Y},{MAX_X,MP_Y},{MAX_X,MAX_Y},{MIN_X,MAX_Y},},
			{	//Subcase 6 : abs(p_x - q_x) < abs(p_y - q_y) && p_x > q_x && p_y < q_y 
				{MAX_X,MQ_Y},{P_X,MQ_Y},{Q_X,MP_Y},{MIN_X,MP_Y},{MIN_X,MIN_Y},{MAX_X,MIN_Y},},
			{	//Subcase 7 : abs(p_x - q_x) < abs(p_y - q_y) && p_x > q_x && p_y > q_y 
				{MIN_X,MP_Y},{Q_X,MP_Y},{P_X,MQ_Y},{MAX_X,MQ_Y},{MAX_X,MAX_Y},{MIN_X,MAX_Y},},
		}
	};
	void ConstructVertice();

	void DetermineCase(double p_x, double p_y, double q_x, double q_y, double dx, double dy);
public:
	L1_Bisector(Point& p, Point& q, double minX, double maxX, double minY, double maxY);
	~L1_Bisector();
	std::vector<Point> getVertice();
	SimplePolygon getPolygon();
};

class L1_voronoi {
private:
	double minX;
	double maxX;
	double minY;
	double maxY;
	struct __Cell {
		Point site;
		SimplePolygon cell;
		__Cell(Point site, SimplePolygon cell) :site(site), cell(cell) {}
	};
	int n_site;
	std::vector<__Cell> diagram;
public:
	L1_voronoi(std::vector<Point>& vertices, double len_inf_edge = 50.);
	~L1_voronoi();
	std::vector<Edge> getBoundary();
};

#endif
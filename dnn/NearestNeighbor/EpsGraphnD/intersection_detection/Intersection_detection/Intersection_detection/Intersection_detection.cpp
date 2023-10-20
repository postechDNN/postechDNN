#include<stdio.h>
#include<iostream>
#include<Eigen/Dense>
#include<vector>
#include "Point.h"
#include "optimization.h"
#include "Polytope.h"
#include <stdlib.h>
#include <math.h>


using Eigen::MatrixXd;
using namespace std;
using namespace alglib;

int main(int argc, char** argv)
{
    vector<double> p1 = { 0,0 };
    vector<double> p2 = { 2,0 };
    vector<double> p3 = { 0,2 };
    vector<double> p4 = { 2,2 };
    Point* t1 = new Point(p1);
    Point* t2 = new Point(p2);
    Point* t3 = new Point(p3);
    Point* t4 = new Point(p4);
    vector<Point*> pol1 = { t1, t2, t3};
    vector<Point*> pol2 = { t2, t3, t4 };
    simplices S1(2, pol1);
    simplices S2(2, pol2);

    vector<double> p5 = { 1.001,1.001 };
    *t1 = p5;
    cout << S1.isIn(t1) << endl;
    vector<double> ap1 = { -1,-1,-1 };
    vector<double> ap2 = { 6,0,0 };
    vector<double> ap3 = { 0,6,0 };
    vector<double>  ap4 = { 0,0,6 };

    Point* k1 = new Point(ap1);
    Point* k2 = new Point(ap2);
    Point* k3 = new Point(ap3);
    Point* k4 = new Point(ap4);
    pol2 = { k1, k2, k3, k4 };
    //S2(3, pol2);
    
    //cout << S1.intersect(S2) << endl;
}

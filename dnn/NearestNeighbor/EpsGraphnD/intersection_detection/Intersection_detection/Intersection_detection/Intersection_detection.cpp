#include<stdio.h>
#include<iostream>
#include<Eigen/Dense>
#include<vector>
#include "Point.h"
#include "optimization.h"
#include <stdlib.h>
#include <math.h>


using Eigen::MatrixXd;
using namespace std;
using namespace alglib;

int main(int argc, char** argv)
{
    real_2d_array a = "[[2,1],[1,1]]";
    real_1d_array al = "[-1,0.0000001]";
    real_1d_array au = "[+inf,+inf]";
    real_1d_array c = "[0,0]";
    real_1d_array s = "[1,1]";
    real_1d_array bndl = "[-inf,-inf]";
    real_1d_array bndu = "[+inf,+inf]";
    double asdf[] = { INFINITY, INFINITY };
    bndu.setcontent(2, asdf);
    real_1d_array x;
    minlpstate state;
    minlpreport rep;

    minlpcreate(2, state);
    minlpsetcost(state, c);
    minlpsetbc(state, bndl, bndu);
    minlpsetlc2dense(state, a, al, au, 2);
    minlpsetscale(state, s);

    // Solve
    minlpoptimize(state);
    minlpresults(state, x, rep);
    cout << x[0] << endl;
    return 0;
}

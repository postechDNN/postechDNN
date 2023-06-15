#include<stdio.h>
#include<iostream>
#include<Eigen/Dense>
#include<vector>
#include "Point.h"

using Eigen::MatrixXd;
using namespace std;

int main()
{
    Eigen::MatrixXd A(3,3);
    A << 1, 1, 0,
        2, 2, 0,
        3, 3, 0;
    
    cout << "Here is the matrix A:\n" << A << endl;
    Eigen::FullPivLU<Eigen::MatrixXd> lu_decomp(A);
    MatrixXd T = lu_decomp.kernel();
    cout << "Here is a matrix whose columns form a basis of the null-space of A:\n"
        << T << std::endl;
    cout << "Here is a matrix whose columns form a basis of the column-space of A:\n"
        << lu_decomp.image(A) << std::endl; // yes, have to pass the original A
}
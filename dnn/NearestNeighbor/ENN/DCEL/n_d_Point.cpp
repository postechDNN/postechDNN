#include <stdio.h>
#include "n_d_Point.h"
#include <math.h>

//this class defines a point in arbitrary dimensional space

n_d_Point::n_d_Point(int dim, double* input_coor) {
	this->dim = dim;
	this->coor = input_coor;
}

n_d_Point::n_d_Point(n_d_Point* _p) {
	this->dim = _p->dim;
	this->coor = _p->coor;
}

n_d_Point::~n_d_Point() {
}

double n_d_Point::getproj(int i) {
	return this->coor[i-1];
}

bool n_d_Point::operator==(n_d_Point _p) {
	int standard = this->dim;
	if (standard == _p.dim)
	{
		for (int i = 1; i < standard + 1; i++)
		{
			if (getproj(i) != _p.getproj(i))
			{
				return false;
			}
		}
		return true;
	}
	else { return false; }
}

double n_d_Point::distance(n_d_Point _p) {
	int standard = this->dim;
	double squared_value = 0;
	if (standard == _p.dim)
	{
		for (int i = 1; i < standard + 1; i++)
		{
			squared_value += pow(this->coor[i - 1] - _p.coor[i - 1], 2);
		}
		return sqrt(squared_value);
	}
	else { return -1; }
}
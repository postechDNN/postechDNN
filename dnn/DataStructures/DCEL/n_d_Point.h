#pragma once

class n_d_Point
{
private:
	double* coor; // coordinates
	int dim;
public:
	n_d_Point(int, double*);
	n_d_Point(n_d_Point*);
	~n_d_Point();
	double getproj(int);
	bool operator==(n_d_Point);
	double distance(n_d_Point);
};
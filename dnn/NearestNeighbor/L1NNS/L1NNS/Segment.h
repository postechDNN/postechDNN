#pragma once

class Segment {
public:
	double px, py;
	double qx, qy;
	Segment(double x1, double x2, double x3, double x4)
	{
		this->px = x1;
		this->py = x2;
		this->qx = x3;
		this->qy = x4;
	}
};
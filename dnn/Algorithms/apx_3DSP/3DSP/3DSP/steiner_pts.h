#pragma once

double dist(PolyDomain PD, Point P, bool isVertex);
double radius(PolyDomain PD, Point P, bool isVertex);
double* radius_e(PolyDomain PD, Segment S);
Point perpen(Point A, Point B, Point P);
vector<Segment> MarkPoints(PolyDomain PD, Point A, Point B, Point C, Point D, int Tet_index, int sg_num);
vector<Segment> MarkPoints(PolyDomain PD);


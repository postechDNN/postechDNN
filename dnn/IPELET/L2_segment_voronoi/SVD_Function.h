#pragma once

#include "Segment.h"
#include "SimplePolygon.h"

#include <vector>

//Check ipe input
int checkInput(std::vector<ISegment> *input);

//Compute boundary of voronoi diagram
void ComputeBoundary(double *boundary, std::vector<ISegment> inputSegments);

//Brute Force algorithm
void BFAlgorithm(std::vector<ISegment> *inputSegments, double boundary[], std::vector<std::vector<SimplePolygon*>*>* Cells);

//Compute interesction region of all cells
void CellsIntersection(std::vector<std::vector<SimplePolygon*>*>* Cells, std::vector<SimplePolygon> *resultCells);
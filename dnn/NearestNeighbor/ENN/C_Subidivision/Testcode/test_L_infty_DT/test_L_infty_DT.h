#include "../../DCEL/DCEL.h"
#include "../../DCEL/Point.h"
#include "../../DCEL/Edge.h"
#include "../../DCEL/Vector.h"
#include <vector>

//Graph is an adjacency list of Delaunay triangulation which is an undirected graph whose vertices are stored in sites.
//Return true if it is a L-infty Delaunay triangulation, otherwise return false.
bool test_L_infty_DT(std::vector<Point> sites, std::vector< std::vector<int>> graph);
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Testcode/test_csdiv_pts/test_csdiv_pts.h"
#include "Testcode/test_L_infty_DT/test_L_infty_DT.h"

int main(){
    std::vector<Point> pts = {Point(0,0), Point(4,1), Point(2,2)};
    std::vector<std::vector<int>> graph(3);
    graph[0].push_back(1); graph[0].push_back(2);
    graph[1].push_back(0); graph[1].push_back(2);
    graph[2].push_back(1); graph[2].push_back(0);
    if(test_L_infty_DT(pts,graph))
        std::cout <<"pass L-infinity Delaunay triangulation test!"<<std::endl;
    return 0;
}
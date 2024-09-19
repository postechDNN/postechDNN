#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Testcode/test_csdiv_pts/test_csdiv_pts.h"
#include "CS_Free.h"
#include "WF_propagation.h"
#include "SPM.h"

int main(){
    //test_csdiv_pts();

    // To test CS_Free construction
    Point src(0, 0);
    std::vector<Point> pts1 = { Point(4,0), Point(0,2), Point(2,6) };
    std::vector<Point> pts2 = { Point(5, 1), Point(5.5, 5),Point(8.5,6), Point(8,3) };
    std::vector<SimplePolygon> obstacles = { SimplePolygon(pts1), SimplePolygon(pts2) };
    std::cout << "Construct the conforming subdivision of free space start\n";
    CS_Free cs_free(src, obstacles);
    std::cout << "Construct the conforming subdivision of free space done\n";

    Vertex srcV(src);
    std::cout << "Wave front propagation start\n";
    WF_propagation wf_propagation(&srcV, cs_free);
    std::cout << "Wave front propagation done\n";


    std::cout << "Wave front propagation start\n";
    SPM spm(&srcV, wf_propagation, obstacles);
    std::cout << "Wave front propagation done\n";

    return 0;
}
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Testcode/test_csdiv_pts/test_csdiv_pts.h"
#include "CS_Free.h"

int main(){
    //test_csdiv_pts();

    // To test CS_Free construction
    Point src(0,0);
    std::vector<Point> pts1 = {Point(4,0), Point(0,2), Point(2,6)};
    std::vector<Point> pts2 = {Point(5,1), Point(5.5,5),Point(8.5,5), Point(8.5,3)};
    std::vector<SimplePolygon> obstacles={SimplePolygon(pts1), SimplePolygon(pts2)};
    std::cout << "cs free construction start";
    CS_Free cs(src, obstacles);
    std::cout << "cs free construction done";
    return 0;
}
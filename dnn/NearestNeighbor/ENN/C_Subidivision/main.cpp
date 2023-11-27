#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Testcode/test_csdiv_pts/test_csdiv_pts.h"
#include "CS_Free.h"

int main(){
    test_csdiv_pts();

    // To test CS_Free construction
    Point src;
    std::vector<SimplePolygon> obstacles;
    
    //CS_Free cs(src, obstacles);

    return 0;
}
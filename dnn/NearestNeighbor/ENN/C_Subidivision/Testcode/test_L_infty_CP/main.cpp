#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Testcode/test_csdiv_pts/test_csdiv_pts.h"
#include "Testcode/test_L_infty_DT/test_L_infty_DT.h"
#include "Testcode/test_L_infty_CP/test_L_infty_CP.h"

int main(){
    std::vector<Point> pts = {Point(0,0), Point(4,1), Point(2,2)};
    if(test_L_infty_CP(pts,1,0))
        std::cout <<"pass L-infinity closest pair test!"<<std::endl;
    return 0;
}
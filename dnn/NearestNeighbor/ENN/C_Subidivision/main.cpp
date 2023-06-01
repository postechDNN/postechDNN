#include <iostream>
#include <algorithm>
#include <vector>
#include "C_Subdivision.h"
#include "DCEL/DCEL_IO.h"

int main(){
    std::vector<Point> pts = {Point(0.1,0.1),Point(0.1,0.17),Point(0.3,0.27),Point(200,200.27)};
    C_Subdivision csubdiv(pts);

    std::cout<<"scale_factor: "<<csubdiv.scale_factor<<std::endl;
    std::cout<<"translation_x_factor: "<<csubdiv.tr_x_factor<<std::endl;
    std::cout<<"translation_y_factor: "<<csubdiv.tr_y_factor<<std::endl;

    std::cout<<csubdiv.scale_factor <<' '<<csubdiv.tr_x_factor<<' '<<csubdiv.tr_y_factor<<std::endl;
    for(auto p : csubdiv.sites){
        std::cout<<p<<std::endl;
    }

    DCEL dcel = csubdiv.build_d_subdivision(1);
    DCEL_IO::print_DCEL(dcel);

    return 0;
}

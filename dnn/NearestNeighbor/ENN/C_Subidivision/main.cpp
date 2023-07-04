#include <iostream>
#include <algorithm>
#include <vector>
#include "C_Subdivision.h"
#include "DCEL/DCEL_IO.h"
#include "DCEL/Vector.h"
#include "WC_region.h"
#include "CS_valid_check.h"

int main(){
    std::vector<Point> pts = {Point(0.5,0.5),Point(4.5,4.5)};
    C_Subdivision csubdiv(pts);

    std::cout<<"scale_factor: "<<csubdiv.scale_factor<<std::endl;
    std::cout<<"translation_x_factor: "<<csubdiv.tr_x_factor<<std::endl;
    std::cout<<"translation_y_factor: "<<csubdiv.tr_y_factor<<std::endl;

    for(auto p : csubdiv.sites){
        std::cout<<p<<std::endl;
    }
    int alpha = 1;

    DCEL dcel = csubdiv.build_d_subdivision(alpha);
    //DCEL_IO::print_DCEL(dcel);
    std::string he_key = "e2_1";

    HEdge *he = dcel.getHedge(he_key);
    if(!he){
        std::cout <<"There is no key of half edges in DCEL"<<std::endl;
        return 0;
    }
    WC_region wc(dcel, he);
    
    std::cout << "Well-covering region of "<<he_key<<": ";
    for(auto f:wc.regions){
        std::cout<<f->getKey()<<' ';
    }
    std::cout<<std::endl;
    std::cout << "Boundary of this well-covering region is: ";
    for(auto he:wc.boundary){
        std::cout<<he->getKey()<<' ';
    }
    std::cout<<std::endl;
    std::cout <<"W1: "<<check_w1(dcel)<<std::endl;
    std::cout <<"W2: "<<check_w2(dcel,alpha)<<std::endl;
    //std::cout <<"W3: "<<check_w3(dcel,alpha)<<std::endl;
    std::cout<<"A1: "<<check_a1(dcel)<<std::endl;
    std::cout<<"A4: "<<check_a4(dcel,alpha)<<std::endl;
    return 0;
}
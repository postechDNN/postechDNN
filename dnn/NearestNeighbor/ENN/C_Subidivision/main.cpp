#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "C_Subdivision.h"
#include "DCEL/DCEL_IO.h"
#include "DCEL/Vector.h"
#include "WC_region.h"
#include "CS_valid_check.h"

//Test function
bool test_csdiv_pts(std::string fname){
    std::cout << "test conforoming subdivision with filename="<<fname<<std::endl;
    std::ifstream fin(fname);
    std::string line;
    int n,alpha;
    std::vector<Point> pts;
    if(fin.is_open()){
        fin >> n >> alpha;
        double x,y;
        for(int i = 0 ; i< n ; i++){
            fin >>x >> y;
            pts.push_back(Point(x,y));
        }
        C_Subdivision csubdiv(pts);
        std::cout<<"number of points: "<<n << " alpha: "<<alpha<<std::endl;
        std::cout<<"scale_factor: "<<csubdiv.scale_factor<<std::endl;
        std::cout<<"translation_x_factor: "<<csubdiv.tr_x_factor<<std::endl;
        std::cout<<"translation_y_factor: "<<csubdiv.tr_y_factor<<std::endl;
        DCEL dcel = csubdiv.build_alpha_subdivision(alpha);

        bool pass = true;
        std::cout <<"Strong alpha-conforming subdivision space test\n";
        if(!check_strong_csdiv(pts,dcel,alpha))
            pass = false;

        std::cout <<"\nAdditional properties of alpha-conforming subdivision test\n";
        if(!check_add_properties(pts,dcel,alpha))
            pass = false;
        return pass;
    }
    else{
        std::cout <<"error: file is not opened\n";
        return false;
    }
}

int main(){
    std::string folder_name = "./testcase/csdiv_pts/";
    std::vector<std::string> fnames= {"t1.txt","t2.txt","t3.txt","t4.txt"};
    for(auto fname :fnames){
        if(test_csdiv_pts(folder_name+fname))
            std::cout << "pass all tests\n";
        else
            std::cout<< "fail some test\n";
    }

    return 0;
}
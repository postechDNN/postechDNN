#include <iostream>
#include <fstream>
#include "Gen_geom_data.h"
#include "../../DCEL/Point.h"
#include "../../DCEL/Polygon.h"

int main(){
    int n_files = 13;
    std::vector<int> n_pts={1,5,10,50,100,500,1000,5000,10000,50000,100000,500000,1000000};
    Point left_bottom = Point(0,0), right_upper = Point(100000,100000);
    Gen_geom_data gen_geom(left_bottom,right_upper);

    int i =1;
    for(auto n : n_pts){
        std::vector<Point> pts = gen_geom.gen_points_uniform(n);
        std::string fname= "t"+std::to_string(i)+".txt";
        std::ofstream file(fname);
        if(file.is_open()){
            file << n<<std::endl;
            for(int j = 0 ;j<n;j++){
                file << pts[j].getx() <<' '<<pts[j].gety()<<std::endl;
            }
            file.close();
        }
        i++;
    }
    return 0;
}
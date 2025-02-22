#include <iostream>
#include <fstream>
#include "Gen_geom_data.h"
#include "../../ENN/C_Subidivision/DCEL/Point.h"
#include "../../ENN/C_Subidivision/DCEL/Polygon.h"

/*
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
}*/

int main() {
    int n = 20;
    int m = 10;
    int k = 60;
    Point left_bottom = Point(0, 0), right_upper = Point(10000, 10000);
    Gen_geom_data gen_geom(left_bottom, right_upper);
    std::vector<SimplePolygon> obstacles = gen_geom.gen_polygonal_domain(k,m);
    std::vector<Point> srcs = gen_geom.gen_sources(obstacles, n);
    Point q = srcs.back();
    srcs.pop_back();
    std::ofstream file("input.txt");
    file << n << " " << m << std:: endl;
    for (size_t i = 0; i < srcs.size(); i++) {
        file << srcs[i].getx() << " " << srcs[i].gety() << std::endl;
    }
    for (size_t i = 0; i < obstacles.size(); i++) {
        std::vector<Point> temp = obstacles[i].getVertices();
        file << temp.size() << std::endl;
        for (size_t j = 0; j < temp.size(); j++) {
            file << temp[j].getx() << " " << temp[j].gety() << std::endl;
        }
    }
    file << q.getx() << " " << q.gety() << std::endl;
    file.close();
    return 0;
}
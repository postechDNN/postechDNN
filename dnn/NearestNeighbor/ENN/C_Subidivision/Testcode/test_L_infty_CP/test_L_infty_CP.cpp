#include "test_L_infty_CP.h"
#define tolerance 1e-6

double inline L_infty_dist(Point& p1, Point& p2){
    return std::max(std::abs(p1.getx()- p2.getx()), std::abs(p1.gety()- p2.gety()));
}

bool test_L_infty_CP(std::vector<Point> &pts, int cp1, int cp2){
    if(cp1 < 0 || cp1 >= pts.size() || cp2 <0 || cp2 >= pts.size()){
        std::cout <<"the index error: cp1 or cp2 do not lie in the range of point list\n";
        std::cout <<"the size of point list: "<<pts.size()<<", cp1: "<< cp1 <<", cp2: "<< cp2 <<std::endl;
        return false;
    }
    double claim_ans = L_infty_dist(pts[cp1], pts[cp2]);
    double min_dist = std::numeric_limits<double>::max();
    for(int i = 0; i<pts.size();i++){
        for(int j = i+1 ; j<pts.size();j++){
            double dist = L_infty_dist(pts[i], pts[j]);
            min_dist = min_dist > dist ? dist : min_dist; 
        }
    }
    if(std::abs(claim_ans - min_dist) < tolerance)
        return true;

    std::cout <<"correctness error: the minimum L infinity distance differs from your answer\n";
    std::cout <<"the minimum L infinity dist: "<<min_dist <<", your answer: "<<claim_ans<<std::endl;
    return false;
}
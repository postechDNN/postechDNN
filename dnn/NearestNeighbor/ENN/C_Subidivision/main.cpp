#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Testcode/test_csdiv_pts/test_csdiv_pts.h"
#include "Testcode/test_L_infty_DT/test_L_infty_DT.h"
#include "Testcode/test_L_infty_CP/test_L_infty_CP.h"
#include "Testcode/Gen_geom_data/Gen_geom_data.h"

int main() {
    Gen_geom_data geom_data(Point(0,0),Point(30,30));
    DCEL ret = geom_data.gen_polygonal_domain(100);
    return 0;
}

// bool compare_by_x(Point a, Point b) {
//     return a.getx() > b.getx();
// }

// bool compare_by_y(Point a, Point b) {
//     return a.gety() > b.gety();
// }

// double get_dist(Point a, Point b) {
//     double dx = std::abs(a.getx()-b.getx());
//     double dy = std::abs(a.gety()-b.gety());
//     double min = std::max(dx, dy); 
//     return dx + dy;
// }

// double compare_mid(std::vector<Point> mid_y, double min_left_right) {
//     int min_mid = min_left_right;

//     for(int i = 0; i < mid_y.size(); i++) {
//         for(int j = i + 1; j < mid_y.size(); j++) {
//             if(get_dist(mid_y[i], mid_y[j]) < min_mid) {
//                 min_mid = get_dist(mid_y[i], mid_y[j]);
//             }
//         }
//     }
//     return min_mid;
// }

// double closest_dist(std::vector<Point> sites_x, std::vector<Point> sites_y){
//     if(sites_x.size() == 1){

//         return std::numeric_limits<double>::infinity();

//     } else if(sites_x.size() == 2){

//         return get_dist(sites_x[0], sites_x[1]);

//     } else if(sites_x.size() <= 3){

//         double dist1 = get_dist(sites_x[0], sites_x[1]);
//         double dist2 = get_dist(sites_x[0], sites_x[2]);
//         double dist3 = get_dist(sites_x[2], sites_x[1]);

//         return std::min(dist1, std::min(dist2, dist3));
//     }

//     int mid_idx_x = sites_x.size() / 2;

//     std::vector<Point> left_y;
//     std::vector<Point> right_y;

//     for (int i = 0; i < sites_y.size(); i++) {

//         if(sites_y[i].getx() < sites_x[mid_idx_x].getx()) {
//             left_y.push_back(sites_y[i]);
//         } else {
//             right_y.push_back(sites_y[i]);
//         }
//     }


//     std::vector<Point> left_x;  
//     for (int i = 0; i < mid_idx_x; i++) {
//         left_x.push_back(sites_x[i]);
//     }
//     std::vector<Point> right_x;  
//     for (int i = mid_idx_x; i < sites_x.size(); i++) {
//         right_x.push_back(sites_x[i]);
//     }

//     double min_left_right = std::numeric_limits<double>::infinity();


//     if(left_x.size() == 1) {
//         min_left_right = closest_dist(right_x, right_y);
//     } else if(right_x.size() == 1) {
//         min_left_right = closest_dist(left_x, left_y);
//     } else {
//         min_left_right = std::min(closest_dist(left_x, left_y), closest_dist(right_x, right_y));
//     }

//     std::vector<Point> mid_y;

//     for(int i = 0; i < sites_x.size(); i++) {
//         if (std::abs(sites_y[i].getx() - sites_x[mid_idx_x].getx() ) < min_left_right) {
//             mid_y.push_back(sites_y[i]);
//         }
//     }

//     double min_mid = compare_mid(mid_y, min_left_right);


//     double min = std::min(min_mid, min_left_right);
//     return min;
// }

// int main(){
//     std::string folder_name = "./Testcode/test_L_infty_CP/testcase/";
//     int t = 13;
//     for(int i = 2 ;i<=t;i++){
//         std::string fname = folder_name+ "t"+std::to_string(i)+".txt";
//         std::ifstream fin(fname);
//         std::vector<Point> pts;
//         if(fin.is_open()){
//             int n_pts;
//             fin>> n_pts;
//             for(int j = 0;j<n_pts;j++){
//                 double x,y;
//                 fin >>x >> y;
//                 pts.push_back(Point(x,y));
//             }
//             std::vector<Point> sites_x(pts.begin(), pts.end());
//             std::vector<Point> sites_y(pts.begin(), pts.end());
//             std::sort(sites_x.begin(), sites_x.end(), compare_by_x);
//             std::sort(sites_y.begin(), sites_y.end(), compare_by_y);
//             std::cout <<i<<std::endl;
//             // double min_dist = closest_dist(sites_x, sites_y);
//             // if(test_L_infty_CP(pts,min_dist))
//             //    std::cout <<"pass L-infinity closest pair test!"<<std::endl;

//         }
//     }
//     // std::vector<Point> pts = {Point(0,0), Point(4,1), Point(2,2)};
//     // std::vector<Point> sites_x(pts.begin(), pts.end());
//     // std::vector<Point> sites_y(pts.begin(), pts.end());
//     // sort(sites_x.begin(), sites_x.end(), compare_by_x);
//     // sort(sites_y.begin(), sites_y.end(), compare_by_y);
//     // double min_dist = closest_dist(sites_x, sites_y);
//     // std::cout <<"mindist: "<<min_dist<<std::endl;
//     // if(test_L_infty_CP(pts,min_dist))
//     //     std::cout <<"pass L-infinity closest pair test!"<<std::endl;

//     return 0;
// }
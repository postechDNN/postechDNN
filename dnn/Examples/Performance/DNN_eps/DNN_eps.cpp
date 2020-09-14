#include "../../../NearestNeighbor/EpsGraph/eps_graph.h"
#include "../../../NearestNeighbor/EpsGraph/point.h"
#include "../../../NearestNeighbor/EpsGraph/polygon.h"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <list>
#include <cmath>
/*
class __Point{
public:
    double x,y;
    __Point(double x,double y) : x(x),y(y) {}
};
class __Polygon{
public:
    std::vector<Point> vertices;
    __Polygon(std::vector<Point> &vertices){
        for(auto v :vertices) this->vertices.push_back(v);
    }
};*/

/* Example of execution
./a.out input_file_name query_file_name
There are two arguments
*/
int main(int argc, char **argv){
    std::string input_fname(argv[1]);
    std::string query_fname(argv[2]);
    std::string out_fname(argv[3]);
    //std::cout<<input_fname<<std::endl;
    //std::cout <<query_fname << std::endl;
    std::ifstream input_file(input_fname);
    std::ifstream query_file(query_fname);
    std::ofstream out_file(out_fname);

    int n_pts, n_polys;
    std::list<Free_Point> pts;
    std::vector<_Polygon> polys;
    input_file >>n_pts>>n_polys;
    for(int i = 0 ;i<n_pts;i++){
        double x,y;
        input_file >> x >> y;
        pts.push_back(Free_Point(x,y));
    }
    for(int i = 0 ; i<n_polys;i++){
        double x,y;
        int n_vertices;
        input_file >> n_vertices;
        std::vector<Point> vertices;
        for(int j = 0 ; j < n_vertices;j++){
            input_file>>x>>y;
            vertices.push_back(Point(x,y));
        }
        polys.push_back(_Polygon(vertices,i));
    }
    double epsilon = 10;
    int K = 1;
    Eps_Graph eps_graph(pts,polys,epsilon);
    
    int n_query;
    query_file >> n_query;
    out_file << n_query <<" "<<K<< std::endl;

    for(int i = 0 ; i < n_query;i++){
        double x,y;
        query_file >> x>>y;
        Free_Point query_point(x,y);
        clock_t startTime = clock();
        std::vector<Free_Point> results = eps_graph.kNN(query_point,K);
        clock_t endTime = clock();
        int ticks = endTime - startTime;
        out_file << ticks << std::endl;
        int k = 0;
        for(auto j : results){
		    Grid_Point anchor_end = eps_graph.query_anchor(j);
		    Grid_Point anchor_start = eps_graph.query_anchor(query_point);
		    double dist_end = sqrt(pow(anchor_end.x - j.x,2) +pow(anchor_end.y - j.y ,2));
		    double dist_start = sqrt(pow((anchor_start.x - query_point.x), 2) + pow(anchor_start.y - query_point.y, 2));
            out_file << j.x <<" " << j.y <<" " << dist_start+dist_end + eps_graph.NN_dist[k++] *epsilon <<std::endl;
        }
    }
    input_file.close();
    query_file.close();
    out_file.close();

    return 0;
}
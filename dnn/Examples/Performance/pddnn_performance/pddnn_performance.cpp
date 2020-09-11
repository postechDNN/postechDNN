#include "../../../NearestNeighbor/PDDNN/PDgraph.h"
#include "../../../DataStructures/DCEL/PolygonalDomain.h"
#include "../../../DataStructures/DCEL/SimplePolygon.h"
#include "../../../DataStructures/DCEL/Point.h"
#include "../../../DataStructures/DCEL/Edge.h"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <list>
#include <cmath>

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
    std::vector<Point*> pts;
    std::vector<SimplePolygon*> polys;
    input_file >>n_pts>>n_polys;
    for(int i = 0 ;i<n_pts;i++){
        double x,y;
        input_file >> x >> y;
        pts.push_back(new Point(x,y));
    }
    for(int i = 0 ; i<n_polys;i++){
        double x,y;
        int n_vertices;
        input_file >> n_vertices;
        std::vector<Point*> vertices;
        for(int j = 0 ; j < n_vertices;j++){
            input_file>>x>>y;
            vertices.push_back(new Point(x,y));
        }
        polys.push_back(new SimplePolygon(&vertices));
    }
    PolygonalDomain PD(&polys);
    PDgraph pd_graph(&PD, &pts);

    int K = 1;
    int n_query;
    query_file >> n_query;
    out_file << n_query <<" "<<K<< std::endl;

    for(int i = 0 ; i < n_query;i++){
        double x,y;
        query_file >> x>>y;
        Point query_point(x,y);
        clock_t startTime = clock();
        std::vector<std::pair<Point*, double>>* results = pd_graph.knn(&query_point,K);
        clock_t endTime = clock();
        int ticks = endTime - startTime;
        out_file << ticks << std::endl;
        int k = 0;
        for(auto j : *results){
            out_file << j.first->getx() <<" " << j.first->gety() <<" " << j.second <<std::endl;
        }
    }
    input_file.close();
    query_file.close();
    out_file.close();

    return 0;
}
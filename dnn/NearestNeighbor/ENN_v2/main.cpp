#include "../ENN/C_Subidivision/DCEL/Polygon.h"
#include "Space.h"
#include "Space_IO.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>


int main(){
    //test_csdiv_pts();

    // To test CS_Free construction
    Point q(0, 0);
    std::vector<Point> srcs; //= { Point(4,4), Point(2,8), Point(7,1) };
    //std::vector<Point> pts1 = { Point(4,0), Point(0,2), Point(2,6) };
    //std::vector<Point> pts2 = { Point(5, 1), Point(5.5, 5), Point(8.5,6), Point(9,4.3), Point(8,3) };
    std::vector<SimplePolygon> obstacles; //= { SimplePolygon(pts1), SimplePolygon(pts2) };
    std::cout << "Construct the conforming subdivision of free space start\n";
    std::cout << "Construct the conforming subdivision of free space done\n";
    text_input(srcs, obstacles, q, "input.txt");
    VisGraph s(srcs, obstacles);
    //s.Dijkstra();
    tuple<Point, int, double> answer = s.query(q);
    print_result(s, q, answer, "output.txt");
    cout << get<0>(answer).getx() << ' ' << get<0>(answer).gety() << endl;
    cout << get<2>(answer) << endl;

    //Arrangement arr(srcs);
    return 0;
}


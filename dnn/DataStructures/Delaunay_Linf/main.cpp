#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <queue> 
#include "delaunay.h"


#include "gnuplot-iostream.h"

#include "./externals/quadedge-basic/quadedge.h"

int main(int argc, char** argv){

  
  vector<Point2d> sList;
	Point2d s1(0,0), s2(20,20), s3(40,30);
  sList.push_back(s1);
  sList.push_back(s2);
  sList.push_back(s3);

  vector<Point2d> test1; 
  Point2d p1(10, 0), p2(0, 20), p3(20, 50), p4(40, 30), p5(30, 7), p6(50, 10), p7(60, 40), p8(50, -10), p9(60, -20);
  test1.push_back(p1);
  test1.push_back(p2);
  //test1.push_back(p3);
  test1.push_back(p4);
  test1.push_back(p5);
  test1.push_back(p6);
  test1.push_back(p7);
	//Subdivision mesh(p1, p2, p3);

  vector<Point2d> test2; 
  test2.push_back(p1);
  test2.push_back(p2);
  //test1.push_back(p3);
  test2.push_back(p4);
  test2.push_back(p5);
  test2.push_back(p8);
  test2.push_back(p9);

  vector<Point2d> test3;
  Point2d t1(0, 0), t2(20, 40), t3(10, 60), t4(20, 80), t5(0, 100), t6(40, 20), t7(60, 50), t8(40, 90), t9(60, 120);
  test3.push_back(t1);
  test3.push_back(t2);
  test3.push_back(t3);
  test3.push_back(t4);
  test3.push_back(t5);
  test3.push_back(t6);
  test3.push_back(t7);
  test3.push_back(t8);
  test3.push_back(t9);

  vector<Point2d> test4;
  Point2d q1(0, 20), q2(10, 0), q3(30, 0), q4(40, 40), q5(20, 60);
  test4.push_back(q1);
  test4.push_back(q2);
  test4.push_back(q3);
  test4.push_back(q4);
  test4.push_back(q5);

  vector<Point2d> test5;
  Point2d w1(0, 20), w2(0, 40), w3(0, 50), w4(0, 70), w5(0, 90), w6(10, 30);
  test5.push_back(w1);
  test5.push_back(w2);
  test5.push_back(w3);
  test5.push_back(w4);
  //test5.push_back(w5);
  test5.push_back(w6);


  vector<Point2d> test = test5;
  sort(test.begin(), test.end(), cmp);
  Edge2d* sEdge = delaunay(test);

  printf("Draw in main \n");
  static unsigned int timestamp = 0;

	std::vector<std::vector<std::pair<double, double>>> segments;
	std::vector<std::pair<int, int> > edges;
  if (++timestamp == 0)
    timestamp = 1;
  sEdge->Draw(1000, segments, edges);

  printf("Edges using node indices \n");
  for (auto e:edges){
    printf("%d -> %d \n", e.first, e.second);
  }


  Gnuplot gp;

  gp << "set xrange [-50:150]\nset yrange [-50:150]\n";
  gp << "plot '-' with linespoints\n";
  gp.send2d(segments); //NOTE: send2d is used here, rather than send1d.  This puts a blank line between segments.
  

  

  
  // Creates the window and
  // sets the title
  //glutDisplayFunc(myDisplay);
  //myInit();

  // Additional initializations
  //glutMainLoop();

  
  


	return 0;

}


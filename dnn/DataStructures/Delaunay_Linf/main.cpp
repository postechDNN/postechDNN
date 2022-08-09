#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <queue> 

#include "./externals/quadedge-basic/quadedge.h"

//#include "quadedge.cpp"
#include <GLUT/glut.h>
//#include <GL/GL.h>
#include "delaunay.h"

#include <vector>
#include "gnuplot-iostream.h"


using namespace std; 
#define EPS 1e-6

#define INSIDE 0
#define OUTSIDE 1
#define BEFORE 2
#define AFTER 3

#define ONBEFORE 4
#define ONAFTER 5
#define ON 6

int mystamp = 50;

bool cmp(Point2d a, Point2d b){
  return a.x<b.x;
}
bool cmp_x_inc(Point2d a, Point2d b){
  return a.x<b.x;
}
bool cmp_x_dec(Point2d a, Point2d b){
  return a.x>b.x;
}
bool cmp_y_inc(Point2d a, Point2d b){
  return a.y<b.y;
}
bool cmp_y_dec(Point2d a, Point2d b){
  return a.y>b.y;
}

int inInfCircle(const Point2d& p, const Point2d& a, const Point2d& b){
  if(a==b) {
    printf("Two points are same. Exit in inInfCircle \n");
    exit(1);
  }

	Point2d *da, *db;
  da = new Point2d(a), db = new Point2d(b);
  
  Edge2d* e = MakeEdge();
	e->EndPoints(da, db);

  bool rightof = RightOf(p, e);

  if(a.y < b.y){
    if(a.x < b.x){
      if((a.x < p.x) && (b.y > p.y)) return INSIDE;
      if((a.x == p.x) && (p.y < a.y)) return INSIDE;
      if((b.y == p.y) && (b.x < p.x)) return INSIDE;
    }
    else if(a.x > b.x){
      if((a.y<p.y) && (b.x<p.x)) return INSIDE;
      if((a.y==p.y) && (a.x < p.x)) return INSIDE;
      if((b.x==p.x) && (b.y<p.y)) return INSIDE;
    }
    else{
      if(a.x < p.x) return INSIDE;
      if((a.x == p.x) && (a.y<p.y) && (p.y<b.y)) return INSIDE;
    }
  }
  else if(a.y > b.y){
    if(b.x < a.x){
      if((p.x < a.x) && (b.y < p.y)) return INSIDE; 
      if((a.x == p.x) && (a.y < p.y)) return INSIDE;
      if((b.y==p.y) && (p.x < b.x)) return INSIDE;
    }
    else if(b.x > a.x){
      if((p.y < a.y) && (p.x < b.x)) return INSIDE;
      if((a.y == p.y) && (p.x < a.x)) return INSIDE;
      if((b.x == p.x) && (p.y < b.y)) return INSIDE;
    }
    else{
      if(p.x < a.x) return INSIDE;
      if((p.x==a.x)&&(b.y < p.y) && (p.y < a.y)) return INSIDE;
    }

  }
  else{
    if (a.x < b.x){
      if(p.y < a.y) return INSIDE;
      if ((a.y == p.y) &&(a.x < p.x) && (p.x < b.x)) return INSIDE;
    }
    else{
      if(a.y < p.y) return INSIDE;
      if((a.y == p.y) &&(b.x < p.x) && (p.x < a.x)) return INSIDE;
    }
  }

  if(!rightof) return OUTSIDE;

  if((p-a).norm() < (p-b).norm()) return BEFORE;
  else return AFTER;

}

Edge2d* computeSmallDelaunay(vector<Point2d>& sList, int num){
  if (num>3){
    printf("This point set is not small \n");
    exit(1);
  }

  if(num == 2){
    Subdivision sub(sList[0], sList[1]);
    return sub.startingEdge;
  }

  else if (num==3){
    // Check it all three edges are support edges 
    for (int i=0; i<3; i++){
      int a = i; 
      int b = (i+1) % 3;
      int c = (i+2) % 3;
      int iic1 = inInfCircle(sList[b], sList[a], sList[c]);
      int iic2 = inInfCircle(sList[b], sList[c], sList[a]);

      //1) Point b is included in INSIDE of ac. 
      if((iic1==INSIDE) && (iic2 == INSIDE)){
        // Make edges, ab and bc, not ac. 
        Subdivision sub(sList[a], sList[b], sList[c], 0);
        return sub.startingEdge;
      }
    }

    //2) All three edges can make a triange
    Subdivision sub(sList[0], sList[1], sList[2]);
    return sub.startingEdge;
  } 

  else{
    printf("computeSamllDelaunay: wrong input number \n");
    exit(1);
  }
}


Edge2d* LowerSupport(Edge2d* left, Edge2d* right){
  if(LowerThan(left->Org2d(), right->Org2d())){
    // Back up right pointer until infinite circle contains no points lying CW from it or wraps around back 
    while((inInfCircle(right->Rnext()->Org2d(), left->Org2d(), right->Org2d()) == INSIDE)&&
          (LessThan(right->Rnext()->Org2d(), right->Org2d()))){

            right = right->Rnext(); //CHECK
    }

    // Move CCW around both support hulls 
    while(1){
      if(inInfCircle(right->Dest2d(), left->Org2d(), right->Org2d()) == INSIDE){
        right = right->Rprev(); //CHECK
      }
      else if(inInfCircle(left->Dest2d(), left->Org2d(), right->Org2d()) == INSIDE){
        left = left->Rprev(); //CHECK
      }
      else{
        return Connect(right->Sym(), left->Oprev());
      }
    }
    


  }
  else{ // CHECK
    printf("Lower Support: right one is lower. Not sure\n");

    // Back up left pointer until infinite circle contains no points lying CW from it or wraps around back 
    while((inInfCircle(left->Rnext()->Org2d(), left->Org2d(), right->Org2d()) == INSIDE) &&
          (LessThan(left->Rnext()->Org2d(), left->Org2d()))){
            left = left->Rnext();
    }

    // Move CCW around both support hulls 
    while(1){
      if(inInfCircle(left->Dest2d(), left->Org2d(), right->Org2d()) == INSIDE){
        left = left->Rprev();
      }
      else if (inInfCircle(right->Dest2d(), left->Org2d(), right->Org2d()) == INSIDE){
        right = right->Rprev(); 
      }
      else{
        return Connect(right->Onext()->Sym(), left->Oprev());
      }

    }
    exit(1);
  }

}

/* Test if circle grwing through global var base hits e.Dest */
bool Valid(Edge2d* e, Edge2d* base){
  return (inInfCircle(e->Dest2d(), base->Org2d(), base->Dest2d()) == INSIDE);
}

Edge2d* ProdONext(Edge2d* e){
  return e->Onext(); //CHECK
}

Edge2d* ProdOPrev(Edge2d* e){
  return e->Oprev(); //CHECK
}

bool onBoundary(const Point2d& a, float l, float r, float d, float t){
  return ((l <= a.x) && (a.x <=r) && ((a.y == d) || (a.y == t))) ||
          ((d <= a.y) && (a.y <= t) && ((a.x == l)||(a.x == r)));
}

int MyInCircle(const Point2d& a, const Point2d& b, const Point2d& c,const Point2d& d){
  //Preconditions: InInfCircle(c, a, b) = INSIDE and InInfCircle(c, b, a) is not INSIDE 
  if(!((inInfCircle(c, a, b) == INSIDE) && (inInfCircle(c, b, a) != INSIDE))){
    printf("MyInCircle: Precondition does not hold. \n");
    exit(1);
  }

  

  vector<Point2d> pset;
  pset.push_back(a);
  pset.push_back(b);
  pset.push_back(c);
  //pset.push_back(d);

  sort(pset.begin(), pset.end(), cmp_x_inc);
  float bLeft = pset[0].x;
  float bRight = pset[pset.size()-1].x;
  sort(pset.begin(), pset.end(), cmp_y_inc);
  float bDown = pset[0].y;
  float bTop = pset[pset.size()-1].y;
  float boundingX = abs(bRight-bLeft);
  float boundingY = abs(bTop - bDown);

  //side length of a smallest square touching a, b, c
  float side = (boundingX > boundingY) ? boundingX : boundingY;

  //Compute the largest boundary 
  if(boundingX > boundingY){
    //lift bTop up if possible (if there is no point on bTop)
    if (onBoundary(a, bLeft, bRight, bDown, bTop + 1) && onBoundary(b, bLeft, bRight, bDown, bTop+1) && onBoundary(c, bLeft, bRight, bDown, bTop+1))
      bTop = bDown + side; 
    //lower bDown if possible (if there is no point on bDown)
    if(onBoundary(a, bLeft, bRight, bDown -1, bTop) && onBoundary(b, bLeft, bRight, bDown -1, bTop) && onBoundary(c, bLeft, bRight, bDown -1, bTop))
      bDown = bTop -side; 
  }
  if(boundingX < boundingY){
    // move bLeft more left if possible 
    if(onBoundary(a, bLeft -1, bRight, bDown, bTop) && onBoundary(b, bLeft -1, bRight, bDown, bTop) && onBoundary(c, bLeft -1, bRight, bDown, bTop))
      bLeft = bRight - side; 
    // move bRight more right if possible 
    if(onBoundary(a, bLeft, bRight + 1, bDown, bTop) && onBoundary(b, bLeft, bRight + 1, bDown, bTop) && onBoundary(c, bLeft, bRight+1, bDown, bTop))
      bRight = bLeft + side; 
  }

  // Return values
  if ((bLeft < d.x) && (d.x < bRight) && (bDown < d.y) && (d.y < bTop)) return INSIDE; 
  if (!onBoundary(d, bLeft, bRight, bDown, bTop)) return OUTSIDE; // check using updated boundaries 
  
	Edge2d* e = MakeEdge();
	Point2d *da, *db, *d_ad, *d_bd; 
	da = new Point2d(a.x, a.y), db = new Point2d(b.x, b.y);
  d_ad = new Point2d((a.x + d.x)/2, (a.y + d.y)/2); //middle point bw a and d
  d_bd = new Point2d((b.x + d.x)/2, (b.y + d.y)/2); //middle point bw b and d
  e->EndPoints(da, db);
  if(!RightOf(d, e) && onBoundary(*d_ad, bLeft, bRight, bDown, bTop)) return ONBEFORE; 
  if(!RightOf(d, e) && onBoundary(*d_bd, bLeft, bRight, bDown, bTop)) return ONAFTER; 

  return ON; 
  
}

Edge2d* ComputeLcand(Edge2d* base){
  Edge2d* lcand = base->Rprev();

  /* First eliminate edges crossing infinite circle */
  if (inInfCircle(lcand->Dest2d(), base->Org2d(), base->Dest2d()) == BEFORE){
    lcand = ProdONext(lcand);
    while (inInfCircle(lcand->Dest2d(), base->Org2d(), base->Dest2d()) == BEFORE){
      DeleteEdge(lcand->Oprev());
      lcand = ProdONext(lcand);
    }

    //Last edge saved if removal would disconnect diagram
    if (Valid(lcand, base)) DeleteEdge(lcand->Oprev());
    else lcand = lcand->Oprev();
  }

  /* Eliminate edges whose Delaunayhood is destroyed */
  Edge2d* current; 
  Edge2d* top;
  if (Valid(lcand, base)){
    current = lcand->Onext();
    top = lcand;

    int ic;
    while(1){
      ic = MyInCircle(lcand->Org2d(), lcand->Dest2d(), base->Org2d(), current->Dest2d());

      Edge2d* t; 
      if(ic == INSIDE){
        //Update lcand, unless wrapped around 
        Edge2d* temp = MakeEdge();
        if (LeftOf(current->Dest2d(), lcand)){
          //Make upper and lower bundles if needed 
          
          //Eliminate lcand and get next candidate 
          t = ProdONext(lcand); 

          lcand->EdgeDraw();
          ProdONext(lcand)->EdgeDraw();

          DeleteEdge(lcand);
          lcand = t; 
          current = lcand->Onext(); 
          top = lcand; 
        }
        else break; // If wrapped, have lcand
      }
      else if(ic == ONBEFORE){
        // Update current, unless wrapped around
        if (ccw(current->Dest2d(), current->Org2d(), lcand->Dest2d())) // IF LeftOf[current.Dest, current.Org, lcand.Dest]
          current = current->Rprev();

        // If wrapped, have lcand
        else break; 
      }
      else if(ic == ONAFTER){
        // Delete current edge and continue
        top = current->Dnext(); 
        t = current; 
        current = current->Onext(); 
        DeleteEdge(current);
      }
      else break;
    }
  }

  return lcand; 
  


}

Edge2d* ComputeRcand(Edge2d* base){
  Edge2d* rcand = base->Oprev();
  printf("Initial rcand \n");
  rcand->EdgeDraw(); 
  rcand->Oprev()->EdgeDraw(); 

  /* First eliminate edges crossing infinite circle */
  if (inInfCircle(rcand->Dest2d(), base->Org2d(), base->Dest2d()) == AFTER){
    printf("ComputeRcand 1st step \n");
    rcand = ProdOPrev(rcand);
    while (inInfCircle(rcand->Dest2d(), base->Org2d(), base->Dest2d()) == AFTER){
      DeleteEdge(rcand->Onext());
      printf("Delete Edge 3\n");
      rcand = ProdOPrev(rcand);
    }

    //Last edge saved if removal would disconnect diagram
    if (Valid(rcand, base)) {
      DeleteEdge(rcand->Onext());
      printf("Delete Edge 4\n");
    }
    else rcand = rcand->Onext();
  }

  /* Eliminate edges whose Delaunayhood is destroyed */
  Edge2d* current; 
  Edge2d* top;
  if (Valid(rcand, base)){
    current = rcand->Oprev();
    top = rcand;

    int ic;
    while(1){
      //ic = MyInCircle(lcand->Org2d(), lcand->Dest2d(), base->Org2d(), current->Dest2d());
      ic = MyInCircle(rcand->Dest2d(), rcand->Org2d(), base->Dest2d(), current->Dest2d());

      Edge2d* t; 
      if(ic == INSIDE){
        //Update rcand, unless wrapped around 
        Edge2d* temp = MakeEdge();
        if (RightOf(current->Dest2d(), rcand)){
          //Make upper and lower bundles if needed 
          ;
          
          //Eliminate rcand and get next candidate 
          t = ProdOPrev(rcand); 
          DeleteEdge(rcand);
          
          printf("Delete Edge 5\n");
          rcand = t; 
          current = rcand->Oprev(); 
          top = rcand; 
        }
        else break; // If wrapped, have lcand
      }
      else if(ic == ONAFTER){
        // Update current, unless wrapped around
        // if LeftOf[current.Dest, current.Org, lcand.Dest] in ComputeLcand
        // if RightOf[current.Dest, current.Org, rcand.Dest]
        if (ccw(current->Dest2d(), rcand->Dest2d(), current->Org2d())) //CHECK
          current = current->Lnext();

        // If wrapped, have lcand
        else break; 
      }
      else if(ic == ONBEFORE){
        // Delete current edge and continue
        top = current->Dprev(); 
        t = current; 
        current = current->Oprev(); 
        DeleteEdge(current);
        printf("Delete Edge 6\n");
      }
      else break;
    }
  }

  return rcand; 
  


}

Edge2d* ConnectLeft(Edge2d* lcand, Edge2d* base){
  //UnBudleAll(lcand); //CHECK
  return Connect(base->Sym(), lcand->Sym());
}

Edge2d* ConnectRight(Edge2d* rcand, Edge2d* base){
  //UnBudleAll(rcand); //CHECK
  return Connect(rcand, base->Sym());
}


//sList: A sorted point list
Edge2d* delaunay(vector<Point2d>& S){ 

  // Small Delaunay (n<=3)
  if(S.size()<=3){
    return computeSmallDelaunay(S, S.size());
  }

  // Divide sList into L and R 
  int halfSize = ceil((float)S.size()/2);
  vector<Point2d> L(S.begin(), S.begin() + halfSize);
  vector<Point2d> R(S.begin()+halfSize, S.end());

  // Delaunay for L and R
  Edge2d * lleft = delaunay(L);
  Edge2d * lright = delaunay(R);

  printf("leftsize: %d, rightsize: %d \n", halfSize, S.size()-halfSize);

  /*
  printf("Draw L \n");
  
	std::vector<std::vector<std::pair<double, double>>> temp;
  lleft->Draw(mystamp++, temp);

  printf("Draw R \n");
  lright->Draw(mystamp++, temp);
  */

  //Create common base edge and remember lowest edge 
  Edge2d* base = LowerSupport(lleft, lright);
  bool leftLower = LowerThan(lleft->Org2d(), lright->Org2d());

  Edge2d* lower; 
  if (leftLower) {
    if (lleft->Org2d() == base->Dest2d())
      lower = base->Sym();
    else
      lower = lleft;
  }
  else lower = lright->Rnext(); //Back up to an edge that can't be deleted

  Edge2d* lcand;
  Edge2d* rcand; 

  printf("Compute lcand and rcand \n");

  while(1){
    printf("Draw base \n");
    base->EdgeDraw();

    lcand = ComputeLcand(base);
    printf("Draw Lcand \n");
    lcand->EdgeDraw();

    rcand = ComputeRcand(base);
    printf("Draw Rcand \n");
    rcand->EdgeDraw();

    /* Figure out what is next cross edge */
    if (Valid(lcand, base) && Valid(rcand, base)){
      int ic = MyInCircle(base->Org2d(), base->Dest2d(), lcand->Dest2d(), rcand->Dest2d());
      if (ic == INSIDE) base = ConnectRight(rcand, base);
      else if(ic == OUTSIDE)  base = ConnectLeft(lcand, base);
      else{ // ic == ON or ONBEFORE or ONAFTER 
        if (ccw(rcand->Dest2d(), lcand->Org2d(), lcand->Dest2d())) base = ConnectRight(rcand, base);
        else base = ConnectLeft(lcand, base);
      }

    }
    else if (Valid(lcand, base)) base = ConnectLeft(lcand, base);
    else if (Valid(rcand, base)) base = ConnectRight(rcand, base);
    else if (inInfCircle(rcand->Dest2d(), base->Org2d(), base->Dest2d()) == AFTER){
      //Infinite excursion right
      while(inInfCircle(lcand->Dest2d(), lcand->Org2d(), rcand->Dest2d()) == INSIDE) 
        lcand = lcand->Rprev(); 
      base = Connect(rcand, lcand->Oprev());
      DeleteEdge(rcand); //Left side broke through edge 
    }
    else if (inInfCircle(lcand->Dest2d(), base->Org2d(), base->Dest2d()) == AFTER){
      //Infinite excursion left 
      while(inInfCircle(rcand->Dest2d(), lcand->Dest2d(), rcand->Org2d()) == INSIDE) 
        rcand = rcand->Lnext();
      base = Connect(rcand->Lprev(), lcand->Sym());
      DeleteEdge(lcand); //Left side broke through edge 
    }
    else break; 

  }

  if(!leftLower) lower = lower->Rprev();
  

  printf(" == \n");

  return lower; 

}

Point2d pointToPoint2d(Point& p){
  Point2d newP = Point2d(p.getX(), p.getY());
  newP.point = &p;
  return newP;
}

void ptsToDelaunay(vector<Point>& ptS, Graph* G){

  vector<Point2d> S;
  for (auto i=0; i<ptS.size(); i++){
    S.push_back(pointToPoint2d(ptS[i]));
  }
  
  sort(S.begin(), S.end(), cmp);
  Edge2d* sEdge = delaunay(S);

	std::vector<std::vector<std::pair<double, double>>> edges;
  std::vector<std::pair<Point*, Point*>> gedges;
  sEdge->Draw(++mystamp, edges, *G);

  return;
}


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

	std::vector<std::vector<std::pair<double, double>>> edges;
  if (++timestamp == 0)
    timestamp = 1;
  
  Graph* tempG = new Graph();
  sEdge->Draw(1000, edges, *tempG);


  Gnuplot gp;

  gp << "set xrange [-50:150]\nset yrange [-50:150]\n";
  gp << "plot '-' with linespoints\n";
  gp.send2d(edges); //NOTE: send2d is used here, rather than send1d.  This puts a blank line between segments.
  

  

  
  // Creates the window and
  // sets the title
  //glutDisplayFunc(myDisplay);
  //myInit();

  // Additional initializations
  //glutMainLoop();

  
  


	return 0;

}


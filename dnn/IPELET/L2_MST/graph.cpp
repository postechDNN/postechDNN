// --------------------------------------------------------------------
// Ipelet for creating a graph 
// --------------------------------------------------------------------
#include "ipelet.h"
#include "ipepath.h"
#include "ipepage.h"
#include "ipetext.h"
#include "ipereference.h"
#include "ipebase.h"
#include "ipeattributes.h"
#include "ipeshape.h"
#include "ipeIO.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <queue> 

using namespace std;
using namespace ipe;

#define maxSelNum 100
#define maxVNum 100

int parent[maxVNum];

double lenOfCurveSegment(CurveSegment a);

double lenOfCurveSegment(CurveSegment a){
  /* Length of a*/
  Vector aStart(a.cp(0).x, a.cp(0).y);
  Vector aEnd(a.cp(1).x, a.cp(1).y);

  aStart.operator-=(aEnd);
  double aLen = aStart.len();

  return aLen;
}

int numOfVertex(const vector<Vector>& Vertices, Vector v);

int numOfVertex(const vector<Vector>& Vertices, Vector v){
  for(int i=0; i<Vertices.size(); i++){
    if ((Vertices[i].x == v.x) && (Vertices[i].y == v.y))
      return i;
  }
  return -1; 
}

int Find(int x){
    if (x==parent[x]) return x;
    else {
        int y = Find(parent[x]);
        parent[x] = y;
        return y;
    }
}

void Union (int x, int y){
    y = Find(y);
    x = Find(x);
    if (x!=y){
        parent[y] = x;
    }
}


class Edge {
  public:
      int node[2];
      int distance;

      Edge(int a,int b,int distance){
          this->node[0] = a;
          this->node[1] = b;
          this->distance = distance;
      }

      //연산자 오버로딩
      bool operator<(const Edge &edge) const {
          return (this->distance) < (edge.distance);
      }
};

// --------------------------------------------------------------------
class GraphIpelet : public Ipelet {
public:
  virtual int ipelibVersion() const { return IPELIB_VERSION; }
  virtual bool run(int, IpeletData *data, IpeletHelper *helper);
};
// --------------------------------------------------------------------
bool GraphIpelet::run(int, IpeletData *data, IpeletHelper *helper)
{
  Page *page = data->iPage;
  // int selNum = page->primarySelection()+1;
  // if (selNum < 1) {
  //   helper->message("No selection");
  //   return false;
  // }

  /* Store Vertices and Edges */
  bool verticeExist, edgeExist; 
  int verticeNum, edgeNum; 
  vector<Vector> Vertices;
  
  vector<CurveSegment> Edges;

  verticeExist = Get_points(data, helper, Vertices);
  if (!verticeExist){
    helper->message("No Vertices");
    return false;
  } 
  verticeNum = Vertices.size();

  edgeExist = Get_segments(data, helper, false, Edges);
  if (!edgeExist){
    helper->message("No Edges");
    return false;
  } 
  edgeNum = Edges.size();


  char msg[100];
  char msg_temp[100];


  /* Find Real Vertices and Edges */
  // std::vector<CurveSegment>::iterator eIter; 
  // std::vector<Vector>::iterator vIter; 

  // eIter = Edges.begin();

  // while(eIter != Edges.end()){

  //   int intersectNum = 0;
  //   vIter = Vertices.begin();

  //   while(vIter != Vertices.end()){
  //     Vector endPoint1 = (*eIter).cp(0);
  //     Vector endPoint2 = (*eIter).cp(1);
  //     if((*vIter).operator==(endPoint1) || (*vIter).operator==(endPoint2)) intersectNum++;
  //     vIter++;
  //   }

  //   if (intersectNum<2) eIter = Edges.erase(eIter);
  //   else eIter ++;
  // }


  /* MST using Kruskal's algorithm */
  vector<Edge> newFormEdges;
  vector<Edge> edgesInMst;

  


  bool inMst[verticeNum];


  //Precondition: All vertices in Vertices should be connected & Edges contain each edge, not a connected polyline
  /* Make a new set of edges with new class form */ 
  // Edge(vNum1, vNum2, len)
  for(int i=0; i<edgeNum; i++){
    int startV = numOfVertex(Vertices, Edges[i].cp(0));
    int endV = numOfVertex(Vertices, Edges[i].cp(1));
    double len = lenOfCurveSegment(Edges[i]);

    newFormEdges.push_back(Edge(startV, endV, len));
  }

  sort(newFormEdges.begin(), newFormEdges.end());

  for (int i=0;i<Vertices.size();i++){
        parent[i] = i;
    } 

  for (int i=0;i<newFormEdges.size();i++){
    if (Find(newFormEdges[i].node[0]) != Find(newFormEdges[i].node[1])){
          Union(newFormEdges[i].node[0], newFormEdges[i].node[1]);
          edgesInMst.push_back(newFormEdges[i]);
          cout<< "Union %d \n", i;
        }
    }

  
  // /* Draw Edges */
  IPEIO edge;

  edge.set_color(255,0,0);
  edge.set_arrow(false);
  edge.set_dash(3);
  edge.set_pen(3);

  for(int i=0; i<edgesInMst.size(); i++){
    edge.Draw_segment(data,helper,Vertices[edgesInMst[i].node[0]], Vertices[edgesInMst[i].node[1]]);
  }
  
  
  strcpy(msg, "Graph: Selected ");
  strcat(msg, itoa(Vertices.size(), msg_temp, 10));
  strcat(msg, itoa(edgeNum, msg_temp, 10));
  
  strcat(msg, itoa(edgesInMst.size(), msg_temp, 10));
  strcat(msg, " ");

  for(int i=0; i<Vertices.size();i++){
    strcat(msg, itoa(Find(i), msg_temp, 10));
    strcat(msg, "/");
  }

  helper->message(msg);

  return false;



  
}
// --------------------------------------------------------------------
IPELET_DECLARE Ipelet *newIpelet()
{
  return new GraphIpelet;
}
// --------------------------------------------------------------------


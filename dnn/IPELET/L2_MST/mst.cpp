// --------------------------------------------------------------------
// Ipelet for drawing a MST 
// (Note that ipeIO.cpp was edited for this code.)
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


double lenOfCurveSegment(CurveSegment a){
  /* Length of a*/
  Vector aStart(a.cp(0).x, a.cp(0).y);
  Vector aEnd(a.cp(1).x, a.cp(1).y);

  aStart.operator-=(aEnd);
  double aLen = aStart.len();

  return aLen;
}


int idxOfVertex(const vector<Vector>& Vertices, Vector v){
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

void Union(int x, int y){
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

  /* Store vertices and edges */
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

  /* MST using Kruskal's algorithm */
  vector<Edge> newFormEdges;
  vector<Edge> edgesInMst;

  bool inMst[verticeNum];

  /* Make new set of edges with new class form */ 
  //Precondition: All vertices in Vertices should be connected & Edges contain each edge, not a connected polyline
  for(int i=0; i<edgeNum; i++){
    int startV = idxOfVertex(Vertices, Edges[i].cp(0));
    int endV = idxOfVertex(Vertices, Edges[i].cp(1));
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

  
  /* Draw MST */
  IPEIO edge;

  edge.set_color(255,0,0);
  edge.set_dash(3);
  edge.set_pen(3);

  for(int i=0; i<edgesInMst.size(); i++){
    edge.Draw_segment(data,helper,Vertices[edgesInMst[i].node[0]], Vertices[edgesInMst[i].node[1]]);
  }
  
  
  strcpy(msg, "MST");
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


#ifndef DCEL
#define DCEL
#include <list>
#include <vector>
#include <iterator>

using namespace std;

struct Vertex{
	double x,y;
	list<HEdge*> el;
	Vertex():x(0),y(0){};
	Vertex(double _x,double _y):x(x),y(_y){};
	Vertex(Vertex &q){
		x=q.x;y=q.y;el=q.el;
	}
	Vertex operator+(Vertex &q){
		return Vertex(x+q.x,y+q.y);
	}
	Vertex operator-(Vertex &q){
		return Vertex(x-q.x,y-q.y);
	}
	Vertex& operator=(Vertex &q){
		x=q.x;y=q.y;el=q.el;
		return *this;
	}
	bool operator==(Vertex &q){
		return x==q.x && y==q.y;
	}
};
struct HEdge{
	Vertex *start,*end;
	HEdge *twin,*next,*prev;
	Face *inface;
	bool inf;//infinity edge,reverse direction
/*	HEdge(){
		twin=next=prev=NULL;
		inface=NULL;

	}*/
	HEdge(Vertex *s,Vertex *e,bool _inf=false){
		twin=next=prev=NULL;
		inface=NULL;
		inf=_inf;
		start=s;end=e;
	
	}
};
struct Face{
	HEdge *out_compo;
	bool bounded;//false:unbounded
	list<HEdge*> in_compo;
	int num_in;
	Face():out_compo(NULL),bounded(true),num_in(0){}
	//Face(HEdge &e)
};

namespace DCEL{
	struct VertexT;
	struct HEdgeT;
	struct FaceT;

	class Dcel{
		list<Vertex*> vertices;
		list<HEdge*> edges;
		list<Face*> faces;

	public:
		Dcel();
		Dcel(vector<Vertex*> vv);
		~Dcel();
		void initialize(vector<Vertex*> vv);
		void addVertex(Vertex *v);
		bool deleteVertex(int i);
		bool deleteVertex(Vertex *v);
		void addEdge(Vertex *s,Vertex *e,bool inf=true);
		bool deleteEdge(int i);
		bool deleteEdge(Vertex *s,Vertex *e);
		bool deleteEdge(HEdge *e);

	};
	
}
#endif
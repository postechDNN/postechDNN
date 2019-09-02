#include <stdio.h>
#include "compressed_quadtree.h"
#include <stdlib.h>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <ctime>
using namespace std;



struct Point {
    double coords[2];
	double speed;
	size_t id;
    Point() {};
    
	Point(const Point &other)
    {
        coords[0] = other.coords[0];
        coords[1] = other.coords[1];
		
        speed = other.speed;
		id= other.id;
	}
	


    double operator[](size_t idx) const {return coords[idx];}
    double &operator[](size_t idx) {return coords[idx];}
	size_t operator++() {return id;}	
	
	double dist(Point b){
		return sqrt((coords[0]-b[0])*(coords[0]-b[0])+(coords[1]-b[1])*(coords[1]-b[1]));
	}

	double dist(double x,double y){
		return sqrt((coords[0]-x)*(coords[0]-x)+(coords[1]-y)*(coords[1]-y));
	}

	double cross(Point b){
		double sus=speed+b.speed;
		return this->dist(b)/sus;
	}
	
	double cover(CompressedQuadtree<Point>::Node * nu){
		double t1=this->dist(nu->mid[0]+nu->radius,nu->mid[1]+nu->radius)/this->speed;
		double t2=this->dist(nu->mid[0]+nu->radius,nu->mid[1]-nu->radius)/this->speed;
		double t3=this->dist(nu->mid[0]-nu->radius,nu->mid[1]+nu->radius)/this->speed;
		double t4=this->dist(nu->mid[0]-nu->radius,nu->mid[1]-nu->radius)/this->speed;

		return max(max(t1,t2),max(t3,t4));
	}

};



typedef CompressedQuadtree<Point>::Node Node;


int main(){
	time_t st,et;
	size_t n;	
	size_t d=2;
	scanf("%u",&n);	
	Point* pt =new Point[n];
	double* times = new double[n];
	int* removed = new int[n];
	for(int i=0;i<n;i++){
		scanf("%lf %lf %lf",&pt[i][0],&pt[i][1],&(pt[i].speed));
		pt[i].id=i;
	}
	CompressedQuadtree<Point> CQ(d,pt,n);

	double maxV=-std::numeric_limits<double>::max();
	double minV=std::numeric_limits<double>::max();
	for(int i=0;i<n;i++){
		if(pt[i].speed>maxV) maxV=pt[i].speed;
		if(pt[i].speed<minV) minV=pt[i].speed;
	}
	
	double delta=maxV/minV;
	
	

	CQ.CalCand(delta);

	CQ.root->D=0;
	for(int i=0;i<n;i++){
		Node* travel=CQ.Getleaf(pt[i].id);
		times[i]=std::numeric_limits<double>::max();
		while(travel!=CQ.root && (travel->nodes.size()==1  || times[i]>pt[i].cover(travel))){

			travel->D=i;
			set<size_t>::iterator it;
			set<size_t> cand=travel->Cand;
			for(it=cand.begin();it!=cand.end();++it){
				Node* Node_=CQ.GetNode(*it);
				if(Node_->D==-1)
					Node_->Cand.insert(travel->id);
				else {
					double time_tmp=pt[i].cross(pt[Node_->D]);
					if(time_tmp<times[i]&&time_tmp<times[Node_->D]){
						times[i]=time_tmp;
						removed[i]=Node_->D;
						}
				}

			}
			travel=travel->parrent;
		}
	}
	time(&et);
	for(int i=0;i<n;i++){
		printf("%lf\n",times[i]);
		}

}



#ifndef DDCEL_UTIL
#define DDCEL_UTIL

#include <algorithm>
#include <cassert>

#include "dPoint.h"
#include "dEdge.h"


constexpr double ERR = 1e-6;

using namespace std;

template <typename BinaryOperation>
auto pts_fcn(dPoint*, dPoint*, BinaryOperation);

auto pts_sum(dPoint* , dPoint*);
auto pts_dif(dPoint* , dPoint*);
bool same_vec(dPoint*, dPoint*);

bool dCoplanar(dEdge* , dEdge*);
bool dCoplanar(vector<dEdge*>);
bool dCoplanar(dPoint*, dPoint*, dPoint*, dPoint*);
bool dCoplanar(vector<dPoint*>);

//require {min, max, <} operations
template <typename T>
bool is_between(T,T,T);

template <typename BinaryOperation>
auto pts_fcn(dPoint* p1, dPoint* p2, BinaryOperation binary_op){
    assert(p1->getd()==p2->getd());
    auto *tmp= new vector<double>;
    tmp->reserve(p1->getd());
    transform(p1->cbegin(), p1->cend(), p2->cbegin(), 
                   back_inserter(*tmp), binary_op);
    return new dPoint(tmp);
}
auto pts_sum(dPoint* p1, dPoint* p2){
    return pts_fcn(p1,p2,std::plus<double>());
}

auto pts_dif(dPoint* p1, dPoint* p2){
    return pts_fcn(p1,p2,std::minus<double>());
}

bool same_vec(dPoint* p1, dPoint* p2){
    const int d = p1->getd();
    auto base1 = p1->getc(0);
    auto base2 = p2->getc(0);
    for(int i=1;i<d;i++){
        if(abs(p1->getc(i)*base2-p2->getc(i)*base1)>ERR)
            return false;
    }
    return true;
}
bool dCoplanar(dEdge* e1, dEdge* e2){
    assert(e1!=NULL && e2!=NULL);
    return dCoplanar(e1->get_s(),e1->get_t(),e2->get_s(),e2->get_t());
}
bool dCoplanar(vector<dEdge*> es){
    assert(es.size()>1);
    for (int i=1;i<es.size();i++){
        if (!dCoplanar(es[0],es[i])) return false;
    }
    return true;
}
//core
bool dCoplanar(dPoint* p1, dPoint* p2, dPoint* p3, dPoint* p4){
    assert(p1!=NULL && p2!=NULL && p3!=NULL && p4!=NULL);
    const int d = p1->getd();
    double **mat2d;
	mat2d=new double*[3];
	for(int i=0; i<3; i++){
		mat2d[i]=new double[d];
	}
    for(int i = 0; i<d;i++){
        double fixed=p4->getc(i);
        mat2d[0][i]=p1->getc(i)-fixed;
        mat2d[1][i]=p2->getc(i)-fixed;
        mat2d[2][i]=p3->getc(i)-fixed;   
    }
    //calculate the rank of mat2d, it must less than 3 to be coplanar
    //using gaussian elimination, O(d^2)
    int rank = 0;
    bool row_selection[]{false,false,false};
    for(int i=0;i<d;i++){
        int j;
        for(j=0;j<3;j++) if(!row_selection[j] && abs(mat2d[j][i])>ERR) break;
        if(j!=d){
            rank++;
            row_selection[j]=true;
            for(int p=i+1;p<d;p++) mat2d[j][p]/=mat2d[j][i];
            for(int k=j+1;k<3;k++){
                if(abs(mat2d[k][i])>ERR){
                    for(int p=i+1;p<d;p++) mat2d[k][p]-=mat2d[j][p]*mat2d[k][i];
                }
            }
        }
    }
	for(int i=0; i<3; i++) delete mat2d[i];
	delete mat2d;
    return rank<3;
}
bool dCoplanar(vector<dPoint*> pts){
    assert(pts.size()>3);
    for(int i=3;i<pts.size();i++){
        if (!dCoplanar(pts[0],pts[1],pts[2],pts[i])) return false;
    }
    return true;
}

template <typename T>
bool is_between(T a,T b,T x){
    if (x<min(a,b) || max(a,b)<x) return false;
    return true;
}

#endif
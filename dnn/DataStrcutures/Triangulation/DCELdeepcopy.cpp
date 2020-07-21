#include "DCELdeepcopy.h"
#include <map>
#include <set>

class DCELdeepcopyContext{
	public :
		std::map<Face *,Face *> fm;
		std::map<HEdge *,HEdge *> em;
		std::map<Vertex *, Vertex *> vm;
		std::set<Face *> cf;
		std::set<HEdge * >ce;
		std::set<Vertex *> cv;
		

		bool check(Vertex * v){
			if(cv.find(v)!=cv.end()) return true;
			else {
				cv.insert(v);
				return false;
			}
		}

		bool check(HEdge * e){
			if(ce.find(e)!=ce.end()) return true;
			else {
				ce.insert(e);
				return false;
			}
		}
	
		bool check(Face * f){
			if(cf.find(f)!=cf.end()) return true;
			else {
				cf.insert(f);
				return false;
			}
		}

		
		Face * find(Face *f){
			auto it=fm.find(f); 
			if(it!=fm.end()) return it->second;
			else {
				fm[f]=new Face();
				return fm[f];
			}
		}

		HEdge* find(HEdge *e){
			auto it=em.find(e); 
			if(it!=em.end()) return it->second;
			else {
				em[e]=new HEdge();
				return em[e];
			}
		}
		
		Vertex* find(Vertex *v){
			auto it=vm.find(v);
			if(it!=vm.end()) return it->second;
			else {
				vm[v]=new Vertex(v);
				return vm[v];
			}
		}

		
		HEdge * DCE(HEdge * e){
			if(check(e)) return find(e);
			HEdge * te=find(e);
			te->setOrigin(find(e->getOrigin()));
			te->setNext(find(e->getNext()));
			te->setPrev(find(e->getPrev()));
			te->setTwin(find(e->getTwin()));
			te->setIncidentFace(find(e->getIncidentFace()));
			return te;
		}


		Face * DCF(Face * f){
			if(check(f)) return find(f);	
			Face * tf=find(f);
			HEdge *te=f->getOuter();
			do{
				DCE(te);
				te=te->getNext();
			}while(te!=tf->getOuter());
			
			tf->setOuter(DCE(f->getOuter()));

			for(auto it=f->getInners()->begin();it!=f->getInners()->end();it++){
				te=*it;
				do{
					DCE(te);
					te=te->getNext();
				}while(te!=*it);
				
				tf->addInner(DCE(*it));	
			}
			return tf;
		}



};



DCEL * DCELdeepcopy(DCEL * o){
	DCELdeepcopyContext DC;
	DCEL * result=new DCEL();
}



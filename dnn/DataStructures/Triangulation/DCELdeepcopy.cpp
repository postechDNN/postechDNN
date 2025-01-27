#include "DCELdeepcopy.h"
#include <map>
#include <set>
#include <vector>

class DCELdeepcopyContext{
	public :
		std::map<Face *,Face *> fm;
		std::map<HEdge *,HEdge *> em;
		std::map<Vertex *, Vertex *> vm;
		std::set<Face *> cf;
		std::set<HEdge * >ce;


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
				vm[v]=new Vertex(*v);
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

			for(auto it=f->getInners().begin();it!=f->getInners().end();it++){
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



DCELFaceMap * DCELdeepcopy(DCEL * o){
	DCELdeepcopyContext DC;
	DCEL * result=new DCEL();
	
	DCELFaceMap * ret= new DCELFaceMap(result); 

	auto af= o->getFaces();
	std::vector<Face*>* nfs =new std::vector<Face*>();
	for(auto it=af.begin(); it!=af.end();it++){
		nfs->push_back(DC.DCF(*it));
	}
	o->setFaces(*nfs);


	auto ae= o->getHedges();
	std::vector<HEdge*>* nes =new std::vector<HEdge*>();
	for(auto it=ae.begin(); it!=ae.end();it++){
		nes->push_back(DC.DCE(*it));
	}
	o->setHedges(*nes);

	
	auto av= o->getVertices();
	std::vector<Vertex*>* nvs =new std::vector<Vertex*>();
	for(auto it=av.begin(); it!=av.end();it++){
		nvs->push_back(DC.find(*it));
	}
	o->setVertices(*nvs);
	ret->map_result=DC.fm;
	return ret;
}



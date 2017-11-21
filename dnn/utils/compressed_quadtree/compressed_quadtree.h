#include <

/*
Copyright (c) 2010 Daniel Minor 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef COMPRESSED_QUADTREE_H_
#define COMPRESSED_QUADTREE_H_

/*
    Compressed Quadtree implementation supporting approximate nearest neighbour
    queries, based upon the description in:  
    
    Eppstein, D., Goodrich, M. T., Sun, J. Z. (2008) The Skip Quadtree:
    A Simple Dynamic Data Structure for Multidimensional Data,
    Int. Journal on Computational Geometry and Applications, 18(1/2), pp. 131 - 160
*/

#include <algorithm>
#include <limits>
#include <list>
#include <vector>
#include <iostream>
#include <cstring>
#include <queue>
#include <set>
#include <cmath>
#include <map>

template<class Point> class CompressedQuadtree {

    public:

        //Nodes of the quadtree
        struct Node { 
			private:
			static size_t ident;
			public :

			Node * parrent; 	//parrent
			std::vector<Node *> nodes;       //children
            Point mid;          //midpoint
            double radius;      //half side length
			Point *pt;
			size_t id;
			int D;
			std::set<size_t> Cand;
			Node(){
				id=ident;
				ident++;
				D=-1;
			}

		};	
	
		std::map<size_t,Node *> map;
		std::map<size_t,Node *> leaf;
		
		Node* Getleaf(size_t id){
			return leaf[id];
		}

		Node* GetNode(size_t id){
			return map[id];
		}

		class NodePointerCmp
		{
  			public:
    		NodePointerCmp(){}
		 	 bool operator() (Node* lhs,Node* rhs)
		    {
				return lhs->radius<rhs->radius;
			}
		};

		double dist(Node *a,Node* b){
			double ra=a->radius;
			double rb=b->radius;
			bool left = a->mid[0]+ra < b->mid[0]-rb;
			bool right = a->mid[0]-ra > b->mid[0]+rb;
			bool top = a->mid[1]-ra > b->mid[1]+rb;
			bool bottom = a->mid[1]+ra < b->mid[1]-rb;

			if(left && top) return sqrt((a->mid[0]+ra-b->mid[0]+rb)*(a->mid[0]+ra-b->mid[0]+rb)+(a->mid[1]-b->mid[1]-ra-rb)*(a->mid[1]-b->mid[1]-ra-rb));
			if(left && bottom) return sqrt((a->mid[0]+ra-b->mid[0]+rb)*(a->mid[0]+ra-b->mid[0]+rb)+(a->mid[1]-b->mid[1]+ra+rb)*(a->mid[1]-b->mid[1]+ra+rb));
			if(right && top) return sqrt((a->mid[0]-ra-b->mid[0]-rb)*(a->mid[0]-ra-b->mid[0]-rb)+(a->mid[1]-b->mid[1]-ra-rb)*(a->mid[1]-b->mid[1]-ra-rb));
			if(right && bottom) return sqrt((a->mid[0]-ra-b->mid[0]-rb)*(a->mid[0]-ra-b->mid[0]-rb)+(a->mid[1]-b->mid[1]+ra+rb)*(a->mid[1]-b->mid[1]+ra+rb));

			if(top) return abs(a->mid[1]-b->mid[1]-ra-rb);
			if(bottom) return abs(a->mid[1]-b->mid[1]+ra+rb);
			if(left) return abs(a->mid[0]+ra-b->mid[0]+rb);
			if(right) return abs(a->mid[0]-b->mid[0]-ra-rb);
			return 0.0;
		}

		bool Candcheck(Node *a, Node* b,double delta){
				double ra=a->radius;
				double rb=b->radius;
				Node* tmp;
				if(ra>rb) return false;
				if(rb/ra <= 4*delta){
					if(dist(a,b)<=4*sqrt(2)*(ra+rb))
						return true;
					else return false;
				}
				else if(b->nodes.size()<2){
					if(b->nodes.size()==1){
						tmp=b->nodes[0];	
						if(tmp->radius>4*delta*ra) return false;	
					}
					if(dist(a,b)<=4*sqrt(2)*ra*(1+4*delta)) return true;
				}
				return false;
		}

		void CalCand(double delta){
			int count=0;
			std::priority_queue<Node*,std::vector<Node*>,NodePointerCmp> bfsQ;
			bfsQ.push(root);
			map[root->id]=root;
			while(!bfsQ.empty()){
				Node* tmp= bfsQ.top();
				bfsQ.pop();	
          		if (tmp!=root) {
					Node * pa=tmp->parrent;
					std::set<size_t>::iterator it;
					for(it=pa->Cand.begin();it!=pa->Cand.end();it++){
						if(Candcheck(tmp,map[*it],delta)){
							tmp->Cand.insert(*it);
							map[*it]->Cand.insert(tmp->id);
						}
					}
				}
		
				for (size_t i=0; i<tmp->nodes.size(); ++i) {
					bfsQ.push(tmp->nodes[i]);
					map[tmp->nodes[i]->id]=tmp->nodes[i];
					for (size_t j=i+1;j<tmp->nodes.size();++j){
						tmp->nodes[i]->Cand.insert(tmp->nodes[j]->id);
						tmp->nodes[j]->Cand.insert(tmp->nodes[i]->id);
					}
				}
			}
		}

        CompressedQuadtree(size_t dim, Point *pts, size_t n) : dim(dim), nnodes(1 << dim)//, locate_eps(0.001)
        { 
            //calculate bounds
            Point bounds[2];
            for (size_t d = 0; d < dim; ++d) {
                bounds[0][d] = std::numeric_limits<double>::max(); 
                bounds[1][d] = -std::numeric_limits<double>::max(); 
            }

            for (size_t d = 0; d < dim; ++d) {
                for (size_t i = 0; i < n; ++i) {
                    if (pts[i][d] < bounds[0][d]) bounds[0][d] = pts[i][d];
                    if (pts[i][d] > bounds[1][d]) bounds[1][d] = pts[i][d];
                }
            }
 
            //calculate mid point and half side length
            Point mid, side; 
            double radius = 0;
            for (size_t d = 0; d < dim; ++d) {
                mid[d] = (bounds[0][d]+bounds[1][d]) / 2;
                double side = (bounds[1][d]-bounds[0][d]) / 2;
                if (side > radius) radius = side;
            } 

            //set up points vector 
            std::vector<Point *> pts_vector;
            for (size_t i = 0; i < n; ++i) {
                pts_vector.push_back(&pts[i]);
            }

            root = worker(mid, radius, pts_vector,NULL);
        }

        virtual ~CompressedQuadtree()
        {
           if (root) delete_worker(root);  
        }

        Node *root;
		
    private:
        size_t dim; 
        size_t nnodes;


        Node *worker(const Point &mid, double radius, std::vector<Point *> &pts,Node* parrent)
        {
            Node *node = new Node; 
			node->parrent=parrent;
			
			for (size_t d = 0; d < dim; ++d) {
                node->mid[d] = mid[d];
            }
            node->radius = radius; 

            if (pts.size() == 1) {
                node->pt = pts[0];
				//printf("%d ",node->pt->id);
				leaf[node->pt->id]=node;
            } else { 
                node->pt = 0;

                //divide points between the nodes 
                std::vector<Point *> *node_pts = new std::vector<Point *>[nnodes];
                for (typename std::vector<Point *>::iterator itor = pts.begin(); itor != pts.end(); ++itor) {

                    //determine node index based upon which which side of midpoint for each dimension
                    size_t n = 0;
                    for (size_t d = 0; d < dim; ++d) {
                        if ((*(*itor))[d] > mid[d]) n += 1 << d; 
                    } 

                    node_pts[n].push_back(*itor);
                } 

                //create new nodes recursively
                size_t n_nodes = 0;
                for (size_t n = 0; n < nnodes; ++n) {

                    if (node_pts[n].size()) {

                        Point nbounds[2];
                        Point new_mid;
                        double new_radius = radius / 2.0;
                        for (size_t d = 0; d < dim; ++d) { 
                            if (n & (1 << d)) {
                                new_mid[d] = mid[d] + new_radius;
                            } else { 
                                new_mid[d] = mid[d] - new_radius;
                            }
                        }
						Node * tmp = worker(new_mid, new_radius, node_pts[n],node);

                        node->nodes.push_back(tmp);
						}
                }

                delete[] node_pts;
                
				//compress if less than 2 interesting nodes
                if (node->nodes.size() == 1) {
					if (node->nodes[0]) {
						if(node->nodes[0]->nodes.size()==1){
                            Node *re = node->nodes[0];
                            Node *ad=node->nodes[0]->nodes[0];
							node->nodes[0]=ad;
							ad->parrent=node;
                            delete re;
						}
                    }
                }
            } 

            return node;
        } 

        void delete_worker(Node *node)
        {
        	for (size_t n = 0; n < node->nodes.size(); ++n) { 
                	delete_worker(node->nodes[n]); 
            }
            delete node; 
        }    

};

template<class Point>
size_t CompressedQuadtree<Point>::Node::ident = 0;

#endif


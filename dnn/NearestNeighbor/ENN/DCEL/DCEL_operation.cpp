#include "DCEL_operation.h"
#include "DCEL.h"

Point* HEdgeContainer::sweep_p = nullptr;

HEdgeContainer::HEdgeContainer(){}

HEdgeContainer::HEdgeContainer(int k,HEdge *hedge) : key(k),hedge(hedge){}

HEdgeContainer::HEdgeContainer(const HEdgeContainer& op){
    this->hedge = op.hedge;
    this->key = op.key;
}

double HEdgeContainer::getKey() const{
    Edge e = this->hedge->getEdge();
    Point s = e.gets(); 
    Point t = e.gett();  
    double s_x = s.getx(), t_x = t.getx();
    double s_y = s.gety(), t_y = t.gety();

    double x_val = sweep_p->getx(), y_val = sweep_p->gety();
    double dx = t_x - s_x, dy = t_y - s_y;

    if(std::abs(dy) < tolerance) return x_val;

    return (dx*y_val - dx*s_y + dy*s_x + tolerance*x_val*dx) / (dy + tolerance * dx);
}

bool HEdgeContainer::operator<(const HEdgeContainer& op) const{
    return this->getKey() < op.getKey();
}

bool HEdgeContainer::operator==(const HEdgeContainer& op) const{
    return this->key == op.key;
}

intersectEvent::intersectEvent(const Point& p, HEdgeContainer& hec ,EVENT ev) : p(p),hec(hec), ev(ev){}

bool intersectEvent::operator<(const intersectEvent op) const{
    Point p1 = this->p, p2 = op.p;
    double diff = p1.gety()- p2.gety();
    if(diff < -tolerance) return true;
    else if(std::abs(diff) < tolerance) return p1.getx() < p2.getx();
    else return false;
}
bool intersectEvent::operator==(intersectEvent& op){ //굳이 필요한가?
    return this->p == op.p && this->ev == op.ev;
}


FaceNode::FaceNode(HEdge* he, bool is_outer = false):he(he),is_outer(is_outer) {}


void FindNewEvent(HEdgeContainer& hec_1, HEdgeContainer& hec_2, std::priority_queue<intersectEvent> &pq){
    Edge e_1 = align_edge(hec_1.hedge->getEdge()), e_2 = align_edge(hec_2.hedge->getEdge());
    //int k1 = ec_1.key, k2 = ec_2.key;
    Point* crossPoint = e_1.crossing(e_2,true);
    if(!crossPoint) return;


    Point sweep_p = *hec_1.sweep_p;
    double x = crossPoint->getx(), y = crossPoint->gety();
    double x_val = sweep_p.getx(), y_val = sweep_p.gety();
    bool is_below= false; 
    if (std::abs(y - y_val) < tolerance && x < x_val ) is_below = true;
    else if(y < y_val) is_below = true;
    //if(__DEBUG_MODE__) std::cout << "FIND NEW EVENT: "<< *crossPoint <<is_below << std::endl;
    
    if(is_below){
        //Except CrossPoint Event if it is on the lower endpoint(gett).
        if(!(e_1.gett() == *crossPoint)){
            intersectEvent iev1(*crossPoint,hec_1, intersectEvent::EVENT::CROSS);
            pq.push(iev1);
        }
        if(!(e_2.gett() == *crossPoint)){
            intersectEvent iev2(*crossPoint,hec_2, intersectEvent::EVENT::CROSS);
            pq.push(iev2);
        }

    }
    delete crossPoint;
}

Edge align_edge(Edge e){
    Point s = e.gets(), t = e.gett(); 
    double dy = s.gety() - t.gety(), dx = s.getx() - t.getx();
    bool need_swap = false;
    if(std::abs(dy) < tolerance && dx < 0) need_swap = true;
    if(dy < 0) need_swap = true; 

    if(need_swap) return Edge(t,s);
    else return e;
}


std::vector<Face*> ConstructFaces(std::vector<HEdge*> &hedges){
    
    //Constructing HEdge Container 
    std::vector<HEdgeContainer> hedge_containers;
    int num_edges= 0;
    for(auto he: hedges){
        Vertex *v = he->getOrigin(), *twin_v = he->getTwin()->getOrigin();
        //Remove the duplicate of hedge by reminaing only a half edge whose origin is larger than twin own.
        bool is_larger = false;
        is_larger |= (std::abs(v->gety() - twin_v->gety())< tolerance && v->getx() > twin_v->getx());
        is_larger |= (v->gety() > twin_v->gety());
        if(is_larger) {
            hedge_containers.push_back(HEdgeContainer(num_edges,he));
            num_edges++;
        }
        //NEED TO BE CERTIFICATED
    }

    //Push the start and end events into the priority queue.
    Point sweep_p;
    AVLTree<HEdgeContainer> T;
    std::priority_queue<intersectEvent> pq; 
    std::map<Vertex*, HEdge*> left_hedges;

    HEdgeContainer::sweep_p = &sweep_p;

    for (auto hec : hedge_containers){
        Edge e =align_edge(hec.hedge->getEdge());                                       //REMOVE THE ALIGN EDGE PROCEDURE
        intersectEvent ev_start(e.gets(),hec,intersectEvent::EVENT::START); 
        intersectEvent ev_end(e.gett(),hec,intersectEvent::EVENT::END); 
        pq.push(ev_start);
        pq.push(ev_end);
    }

    while(!pq.empty()){
        std::set<int> start_e_set, end_e_set;
        //update sweep line status
        Point ev_p = pq.top().p;
        HEdge *ev_he = pq.top().hec.hedge;
        Vertex *ev_v = *(ev_he->getOrigin()) == ev_p ? ev_he->getOrigin() : ev_he->getTwin()->getOrigin();  
        sweep_p.setx(ev_p.getx());
        sweep_p.sety(ev_p.gety());

        while(!pq.empty() && ev_p == pq.top().p){
            intersectEvent iev = pq.top();
            HEdgeContainer hec = pq.top().hec;
            int key = hec.key;
            if(iev.ev == intersectEvent::EVENT::START)
                start_e_set.insert(key);
            else if(iev.ev == intersectEvent::EVENT::END)
                end_e_set.insert(key);
            pq.pop();
        }

        //Delete the segments in L(p) 
        for(auto k:end_e_set)
            T.pop(hedge_containers[k]);

        //Find the edge left to the vertex
        double x = ev_p.getx(), y = ev_p.gety();
        Vertex v1(x ,y+1.), v2(x,y-1.); 
        HEdge dum_he(&v1,&v2);
        HEdgeContainer dum_line(-1,&dum_he);
        auto leftNode = T.getLeftNode(dum_line);

        if(leftNode)
            left_hedges[ev_v] = leftNode->value.hedge;
        else
            left_hedges[ev_v] = nullptr;
            

        for(auto k:start_e_set)
            T.insert(hedge_containers[k]);

        if(__DEBUG_MODE__){
            std::cout << "<ConstructFaces> p:"<<ev_p <<std::endl;
            std::cout << "U(p) :";
            for(auto it:start_e_set)
                std::cout << hedge_containers[it].hedge->getKey()<<' ';
            std::cout <<std::endl;

            std::cout << "L(p) :";
            for(auto it:end_e_set)
                std::cout << hedge_containers[it].hedge->getKey()<<' ';
            std::cout <<std::endl;
        }
    }

    std::map<HEdge*, int> visit;    //int: FaceNode key in face_nodes
    std::vector<FaceNode> face_nodes;
    std::vector<std::vector<int> > adj_list;

    face_nodes.push_back(FaceNode(nullptr,true)); //It is outmost face node whose key is 0.
    adj_list.push_back(std::vector<int>());

    auto get_vec=[](HEdge *he){
        Vertex *org = he->getOrigin();
        Vertex *dest = he->getTwin()->getOrigin(); 
        return Point(dest->getx()- org->getx(), dest->gety()- org->gety());
    };

    auto cross_prod =[](Point &a, Point &b){
        return a.getx() * b.gety() - a.gety() * b.getx();
    };

    //Construct Face Nodes in Graph
    for(auto he:hedges){
        if(visit.find(he) != visit.end()) //Visited
            continue;

        int fn_key = face_nodes.size();
        face_nodes.push_back(FaceNode(he));
        adj_list.push_back(std::vector<int>());

        HEdge *lb_he = he; //whose origin is left bottom most vertex 

        //Traverse the chain
        auto cur = he;
        do{
            visit[cur] = fn_key;
            Vertex *org = cur->getOrigin();
            Vertex *lb_v = lb_he->getOrigin();
            if(org->getx() < lb_v->getx()) lb_he = cur;
            else if(std::abs(org->getx() - lb_v->getx()) < tolerance 
            && org->gety() < lb_v->gety()) lb_he = cur;
            cur = cur->getNext();
        }while(cur != he);

        //Determine whether it is hole boundary or outer boundary. 
        HEdge *prev_lb = lb_he->getPrev();
        Point vec_a = get_vec(prev_lb),vec_b = get_vec(lb_he);
        double cross_vec_ab = cross_prod(vec_a, vec_b);
        //double cross_vec_ab = vec_a.getx() * vec_b.gety() -vec_a.gety() * vec_b.getx();

        if(prev_lb == lb_he->getTwin() || cross_vec_ab < 0){  //Right turn = clockwise traverse = hole boundary
            face_nodes[fn_key].is_outer = false;
        }
        else if(cross_vec_ab > 0){   //Left turn = counterclockwise traverse = outer boundary
            face_nodes[fn_key].is_outer = true;
        }
    }

    //Construct the edge relation between Face Nodes in Graph
    int key=0;
    for(auto fn : face_nodes){
        key++;
        if(fn.is_outer == true) continue; //Process only for hole boundary

        HEdge *lb_he = fn.he; //Since face node store the information of a half-edge whose origin is the left-bottommost vertex in the boundary. 
        Vertex *lb_v = lb_he->getOrigin();
        HEdge *left_he = left_hedges[lb_v];

        if(left_he == nullptr){ // It is outmost face.
            adj_list[0].push_back(key);
            adj_list[key].push_back(0);
        }
        else{
            //Check if the left_he has same incident face.
            Point vec_a = get_vec(left_he);
            Vertex *dest_v = left_he->getTwin()->getOrigin();
            Point vec_b(lb_v->getx() - dest_v->getx(), lb_v->gety() - dest_v->gety());
            double cross_vec_ab = cross_prod(vec_a,vec_b); 
            if(cross_vec_ab < 0 )  //Right turn = They do not share same incident face
                left_he = left_he->getTwin();
            //cross_vec_ab = 0, it is the horizontal edge whose expanded line passes through left-bottommost vertex. 
            //It does not happen here, since the edge can not be appointed by a left edge of lb_v when sweep point encounter the vertex.

            //We only need to connect the face node to other
            adj_list[visit[left_he]].push_back(key);
            adj_list[key].push_back(visit[left_he]);
        }
    }


    //Traverse a graph for face nodes and determine the number of faces and set incident face of each half edge.
    
    //1. Mark the index of equivalent class of the graph starting to the number zero.
    std::vector<int> visit_fnode(face_nodes.size(),-1); //visit_fnode record the index of equivalent class. 0 ~ n_f.
    int num_f = 0;
    for(int i = 0;i <face_nodes.size();i++){
        if(visit_fnode[i] >=0 ) continue;

        //Traverse by BFS
        std::queue<int> Q; 
        Q.push(i);
        while(!Q.empty()){
            int u = Q.front();
            Q.pop();
            if(visit_fnode[u] >= 0 ) continue;

            visit_fnode[u] = num_f;
            for(auto v:adj_list[u]){
                if(visit_fnode[v]<0) 
                    Q.push(v);
            }
        }
        num_f++;
    }

    //2. Make a face record using the equivalence class.

    std::vector<Face*> faces(num_f);
    for(int i = 0 ; i< faces.size();i++) faces[i]=new Face();

    for(int fn_k = 0; fn_k< face_nodes.size();fn_k++){
        int face_key = visit_fnode[fn_k];

        if(face_nodes[fn_k].is_outer == true){ //outer boundary 
            faces[face_key]->setOuter(face_nodes[fn_k].he);
        }
        else{   //Hole boundary
            faces[face_key]->addInner(face_nodes[fn_k].he);
        }
    }

    return faces;
}



DCEL DCEL::merge(DCEL &op){
    //Constructing HEdge Container 
    int num_edges_1=0, num_edges_2=0;
    std::vector<HEdgeContainer> hedge_containers;
    std::set<HEdge*> he_set; 

    for(auto it: this->hedges){
        HEdge *he = it.second;
        HEdge *twin = he->getTwin();
        if(he_set.end() == he_set.find(twin)) {
            hedge_containers.push_back(HEdgeContainer(num_edges_1,he));
            he_set.insert(he);
            num_edges_1++;
        }
    }
    for(auto it: op.hedges){
        HEdge *he = it.second;
        HEdge *twin = he->getTwin();
        if(he_set.end() == he_set.find(twin)) {
            hedge_containers.push_back(HEdgeContainer(num_edges_1+num_edges_2,he));
            he_set.insert(he);
            num_edges_2++;
        }
    }

    //for(auto it:hedge_containers)
    //    std::cout<<it.hedge->getKey()<<" "<<it.key<< std::endl;

    Point sweep_p;
    AVLTree<HEdgeContainer> T;
    std::priority_queue<intersectEvent> pq; 

    HEdgeContainer::sweep_p = &sweep_p;
    
    //An immediate previous vertex on the edge which is above the sweep line. 
    std::vector<int> on_pts_key(hedge_containers.size());
    std::vector<Vertex*> ret_vertices;
    std::vector<HEdge*> ret_hedges;
    std::vector<std::pair<int, HEdge*> > origin_hedges;

    //Push the start and end events into the priority queue.
    for (auto hec : hedge_containers){
        Edge e =align_edge(hec.hedge->getEdge());
        intersectEvent ev_start(e.gets(),hec,intersectEvent::EVENT::START); 
        intersectEvent ev_end(e.gett(),hec,intersectEvent::EVENT::END); 
        pq.push(ev_start);
        pq.push(ev_end);
    }

    while(!pq.empty()){
        std::set<int> start_e_set, end_e_set, cross_e_set;
        //update sweep line status
        Point ev_p = pq.top().p;
        sweep_p.setx(ev_p.getx());
        sweep_p.sety(ev_p.gety());

        while(!pq.empty() && ev_p == pq.top().p){
            intersectEvent iev = pq.top();
            HEdgeContainer hec = pq.top().hec;
            int key = hec.key;
            if(iev.ev == intersectEvent::EVENT::START)
                start_e_set.insert(key);
            else if(iev.ev == intersectEvent::EVENT::END)
                end_e_set.insert(key);
            else
                cross_e_set.insert(key);
            pq.pop();
        }

        std::set<int> SC_union;
        SC_union.insert(start_e_set.begin(),start_e_set.end());
        SC_union.insert(cross_e_set.begin(),cross_e_set.end());
        std::set<int> EC_union;
        EC_union.insert(end_e_set.begin(),end_e_set.end());
        EC_union.insert(cross_e_set.begin(),cross_e_set.end());

        //Delete the segments in L(p) U C(p) 
        for(auto k:EC_union)
            T.pop(hedge_containers[k]);
        
        //Find segments passing through the event point
        //There is at most one additional passing edge in the AVL tree
        double x = ev_p.getx(), y = ev_p.gety();
        Vertex v1(x-tolerance ,y+1.), v2(x-tolerance,y-1.); 
        Vertex v3(x+tolerance ,y+1.), v4(x+tolerance,y-1.); 
        HEdge left_dum_he(&v1,&v2), right_dum_he(&v3,&v4);
        HEdgeContainer left_dum_line(-1,&left_dum_he),right_dum_line(-1,&right_dum_he);
        auto leftNode = T.getLeftNode(right_dum_line);
        
        if(leftNode && std::abs(leftNode->value.getKey()- x)<tolerance){
            //std::cout<<"Passing :"<<leftNode->value.hedge->getKey()<<std::endl;
            T.pop(leftNode->value);
            cross_e_set.insert(leftNode->value.key); //ONLY FOR DEBUGGING
            EC_union.insert(leftNode->value.key);
            SC_union.insert(leftNode->value.key);
        }



        //Construct half edges at the event point.
        Vertex *new_v = new Vertex(ev_p);
        int new_key = ret_vertices.size();
        ret_vertices.push_back(new_v);
        
        for(auto k:EC_union){   //k -> key of edge container.
            int prev_v_key = on_pts_key[k];
            Vertex *prev_v = ret_vertices[prev_v_key];
            //Make a half-edge having p as origin.
            HEdge *he = new HEdge(), *twin = new HEdge();
            he->setTwin(twin), twin->setTwin(he);
            he->setOrigin(new_v);
            twin->setOrigin(prev_v);
            new_v->setIncidentEdge(he);
            prev_v->setIncidentEdge(twin);
            ret_hedges.push_back(he);
            ret_hedges.push_back(twin);

            origin_hedges.push_back(std::pair(new_key, he));
            origin_hedges.push_back(std::pair(prev_v_key,twin));
        }
        //std::cout <<std::endl;
        for(auto k:SC_union)
            on_pts_key[k] = new_key;
        //Next() and Prev() are determined later.
 
        if(__DEBUG_MODE__){
            std::cout << "p:"<<ev_p <<std::endl;
            std::cout << "U(p) :";
            for(auto it:start_e_set)
                std::cout << hedge_containers[it].hedge->getKey()<<' ';
            std::cout <<std::endl;

            std::cout << "L(p) :";
            for(auto it:end_e_set)
                std::cout << hedge_containers[it].hedge->getKey()<<' ';
            std::cout <<std::endl;

            std::cout << "C(p) :";
            for(auto it:cross_e_set)
                std::cout << hedge_containers[it].hedge->getKey() <<' ';
            std::cout <<std::endl;
        }


        //Insert the segments in U(p) U C(p)
        if(SC_union.empty()){
            auto leftNode = T.getLeftNode(left_dum_line);
            auto rightNode = T.getRightNode(right_dum_line);
            if(leftNode && rightNode)
                FindNewEvent(leftNode->value, rightNode->value ,pq);
        }
        else{
            sweep_p.setx(sweep_p.getx() - tolerance);

            int min_k=*SC_union.begin();
            int max_k=*SC_union.begin();
            for(auto k :SC_union){
                if(hedge_containers[k] < hedge_containers[min_k]) min_k = k;
                if(hedge_containers[max_k] < hedge_containers[k]) max_k = k;
            }
            auto leftNode = T.getLeftNode(hedge_containers[min_k]);
            auto rightNode = T.getRightNode(hedge_containers[max_k]);
            if(leftNode)
                FindNewEvent(leftNode->value,hedge_containers[min_k], pq);
            if(rightNode)
                FindNewEvent(rightNode->value,hedge_containers[max_k],pq);

            for(auto k : SC_union)
                T.insert(hedge_containers[k]);
        }
    }

    //TODO : key 설정 안했음
    
    //Process of classification according to origin.
    std::vector<std::vector<HEdge*> > hedges_origin_v(ret_vertices.size()); 
    for(auto it : origin_hedges){
        HEdge *he = it.second;
        hedges_origin_v[it.first].push_back(he);
    }

    //Sort in circular order.

    auto angle_cmp = [](HEdge* a, HEdge* b){
        Vertex *origin = a->getOrigin();
        Vertex *p = a->getTwin()->getOrigin();
        Vertex *q = b->getTwin()->getOrigin(); 
        double px =p->getx() - origin->getx(), py = p->gety() - origin->gety();
        double qx =q->getx() - origin->getx(), qy = q->gety() - origin->gety();
        double dp = std::sqrt(px*px + py*py), dq = std::sqrt(qx*qx + qy*qy);
        px /= dp, py /= dp;
        qx /= dq, qy /= dq;
        if(py < 0) px = -3 - px;
        if(qy < 0) qx = -3 - qx;
        //std::cout <<dp << ' '<<dq<<std::endl;
        return px > qx;
    };

    for(auto it : hedges_origin_v){
        //Sort hedges in clockwise order
        std::sort(it.begin(),it.end(),angle_cmp);

        //set Next and Prev (It also operates when there is only one edge incident to v)
        for(int i = 0 ; i<it.size();i++){
            HEdge *he =it[i]; 
            int j = i < it.size()-1 ? i+1: 0;
            HEdge *prev = it[j]->getTwin();
            prev->setNext(he);
            he->setPrev(prev);
        }
    }

    //Now we remain to set incident face of half edges and construct faces. 
    
    std::vector<Face*> ret_faces = ConstructFaces(ret_hedges);
    DCEL ret;
    ret.setVertices(ret_vertices);
    ret.setHedges(ret_hedges);
    ret.setFaces(ret_faces);

    return ret;
}

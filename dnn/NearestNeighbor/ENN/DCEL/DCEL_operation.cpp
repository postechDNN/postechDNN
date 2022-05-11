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

DCEL DCEL::merge(DCEL &op){

    //Constructing HEdge Container 
    int num_edges_1=0, num_edges_2=0;
    std::vector<HEdgeContainer> hedge_containers;
    DCEL ret;
    std::set<HEdge*> he_set; 
    for(auto he: this->hedges){
        HEdge *twin = he->getTwin();
        if(he_set.end() == he_set.find(twin)) {
            hedge_containers.push_back(HEdgeContainer(num_edges_1,he));
            he_set.insert(he);
            num_edges_1++;
        }
    }
    for(auto he: op.hedges){
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
    //std::vector<int> on_pts_key(hedges.size()); 

    for (auto hec : hedge_containers){
        Edge e =align_edge(hec.hedge->getEdge());
        intersectEvent ev_start(e.gets(),hec,intersectEvent::EVENT::START); 
        intersectEvent ev_end(e.gett(),hec,intersectEvent::EVENT::END); 
        //std::cout <<"START: "<<e.gets() << " END: "<<e.gett() << std::endl;
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

        //Make graph
        //int g_key = this->graph.insert_vertex(ev_p);
        //for(auto k : EC_union)
        //    this->graph.insert_edge(on_pts_key[k],g_key);
        //for(auto k : SC_union) on_pts_key[k]=g_key;

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
        //if(T.getRoot())
        //    std::cout << "Root: "<<T.getRoot()->value.hedge->getEdge()<<std::endl;
        if(leftNode && std::abs(leftNode->value.getKey()- x)<tolerance){
            //std::cout<<"Passing :"<<leftNode->value.hedge->getKey()<<std::endl;
            T.pop(leftNode->value);
            SC_union.insert(leftNode->value.key); 
             if(__DEBUG_MODE__) cross_e_set.insert(leftNode->value.key);
        }

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
                //if(HEdgeContainer(k,&this->edges,&(this->sweep_p)) < HEdgeContainer(min_k,&this->edges,&(this->sweep_p))) min_k = k;
                //if(HEdgeContainer(max_k,&this->edges,&(this->sweep_p)) < HEdgeContainer(k,&this->edges,&(this->sweep_p))) max_k = k;
            }
            //HEdgeContainer min_ec(min_k, &this->edges, &(this->sweep_p));
            //HEdgeContainer max_ec(max_k, &this->edges, &(this->sweep_p));
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

    return ret;
}
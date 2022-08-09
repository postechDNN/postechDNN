#include "EdgesIntersection.h"

EdgeContainer::EdgeContainer(){}

EdgeContainer::EdgeContainer(int k,std::vector<Edge>* edges, Point *sweep_p) : key(k),edges(edges), sweep_p(sweep_p){}

EdgeContainer::EdgeContainer(const EdgeContainer& op){
    this->edges = op.edges;
    this->sweep_p= op.sweep_p;
    this->key = op.key;
}

double EdgeContainer::getKey() const{
    Edge e = (*this->edges)[this->key];
    Point s = e.gets(); 
    Point t = e.gett();  
    double s_x = s.getx(), t_x = t.getx();
    double s_y = s.gety(), t_y = t.gety();

    double x_val = sweep_p->getx(), y_val = sweep_p->gety();
    double dx = t_x - s_x, dy = t_y - s_y;

    if(std::abs(dy) < tolerance) return x_val;

    return (dx*y_val - dx*s_y + dy*s_x + tolerance*x_val*dx) / (dy + tolerance * dx);
}

bool EdgeContainer::operator<(const EdgeContainer& op) const{
    return this->getKey() < op.getKey();
}

bool EdgeContainer::operator==(const EdgeContainer& op) const{
    Edge e_1 = (*this->edges)[this->key], e_2 = (*this->edges)[op.key];
    return e_1 == e_2;
}

intersectEvent::intersectEvent(const Point& p, int key ,EVENT ev) : p(p),key(key), ev(ev){}

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

void EdgesIntersection::FindNewEvent(EdgeContainer ec_1, EdgeContainer ec_2){
    Edge e_1 = (*ec_1.edges)[ec_1.key], e_2 = (*ec_2.edges)[ec_2.key];
    int k1 = ec_1.key, k2 = ec_2.key;
    Point* crossPoint = e_1.crossing(e_2,true);
    if(!crossPoint) return;
    double x = crossPoint->getx(), y = crossPoint->gety();
    double x_val = sweep_p.getx(), y_val = sweep_p.gety();
    bool is_below= false; 
    if (std::abs(y - y_val) < tolerance && x < x_val) is_below = true;
    else if(y < y_val) is_below = true;
    //if(__DEBUG_MODE__) std::cout << "FIND NEW EVENT: "<< *crossPoint <<is_below << std::endl;
    
    if(is_below){
        intersectEvent iev1(*crossPoint,k1, intersectEvent::EVENT::CROSS);
        intersectEvent iev2(*crossPoint,k2, intersectEvent::EVENT::CROSS);
        this->pq.push(iev1);
        this->pq.push(iev2);
    }
    delete crossPoint;
}
EdgesIntersection::EdgesIntersection(std::vector<Edge> edges){
    this->edges = edges;
    std::vector<int> on_pts_key(edges.size()); 

    int k = 0;
    for (auto e : this->edges){ 
        Point s = e.gets(), t = e.gett(); 
        double dy = s.gety() - t.gety(), dx = s.getx() - t.getx();
        if(std::abs(dy) < tolerance && dx < 0) std::swap(s,t);
        if(dy < 0) std::swap(s,t); //s = start point t = end point
        intersectEvent ev_start(s,k,intersectEvent::EVENT::START); 
        intersectEvent ev_end(t,k,intersectEvent::EVENT::END); 
        this->pq.push(ev_start);
        this->pq.push(ev_end);
        k++;
    }

    while(!this->pq.empty()){
        std::set<int> start_e_set, end_e_set, cross_e_set;
        //update sweep line status
        Point ev_p = pq.top().p;
        this->sweep_p.setx(ev_p.getx());
        this->sweep_p.sety(ev_p.gety());

        while(!this->pq.empty() && ev_p == pq.top().p){
            intersectEvent iev = this->pq.top();
            int key = iev.key;
            if(iev.ev == intersectEvent::EVENT::START)
                start_e_set.insert(key);
            else if(iev.ev == intersectEvent::EVENT::END)
                end_e_set.insert(key);
            else
                cross_e_set.insert(key);
            this->pq.pop();
        }

        if(__DEBUG_MODE__){
            std::cout << "p:"<<ev_p <<std::endl;
            std::cout << "U(p) :";
            for(auto it:start_e_set)
                std::cout << it<<' ';
            std::cout <<std::endl;

            std::cout << "L(p) :";
            for(auto it:end_e_set)
                std::cout << it<<' ';
            std::cout <<std::endl;

            std::cout << "C(p) :";
            for(auto it:cross_e_set)
                std::cout << it <<' ';
            std::cout <<std::endl;
        }

        std::set<int> SC_union;
        SC_union.insert(start_e_set.begin(),start_e_set.end());
        SC_union.insert(cross_e_set.begin(),cross_e_set.end());
        std::set<int> EC_union;
        EC_union.insert(end_e_set.begin(),end_e_set.end());
        EC_union.insert(cross_e_set.begin(),cross_e_set.end());

        //Make graph
        int g_key = this->graph.insert_vertex(ev_p);
        for(auto k : EC_union)
            this->graph.insert_edge(on_pts_key[k],g_key);
        for(auto k : SC_union) on_pts_key[k]=g_key;

        //Delete the segments in L(p) U C(p) 
        for(auto k:EC_union){
            this->T.pop(EdgeContainer(k,&this->edges,&(this->sweep_p)));
        }
        //Insert the segments in U(p) U C(p)
        if(SC_union.empty()){
            double x = ev_p.getx(), y = ev_p.gety();
            this->edges.push_back(Edge(Point(x,y+1.), Point(x,y-1.)));//dummy vertical edge
            EdgeContainer v_line(this->edges.size()-1,&this->edges,&(this->sweep_p));
            auto leftNode = T.getLeftNode(v_line);
            auto rightNode = T.getRightNode(v_line);
            if(leftNode && rightNode)
                FindNewEvent(leftNode->value, rightNode->value);
        }
        else{
            this->sweep_p.setx(this->sweep_p.getx() - tolerance);

            int min_k=*SC_union.begin();
            int max_k=*SC_union.begin();
            for(auto k :SC_union){
                if(EdgeContainer(k,&this->edges,&(this->sweep_p)) < EdgeContainer(min_k,&this->edges,&(this->sweep_p))) min_k = k;
                if(EdgeContainer(max_k,&this->edges,&(this->sweep_p)) < EdgeContainer(k,&this->edges,&(this->sweep_p))) max_k = k;
            }
            EdgeContainer min_ec(min_k, &this->edges, &(this->sweep_p));
            EdgeContainer max_ec(max_k, &this->edges, &(this->sweep_p));
            auto leftNode = T.getLeftNode(min_ec);
            auto rightNode = T.getRightNode(max_ec);
            if(leftNode)
                FindNewEvent(leftNode->value,min_ec);
            if(rightNode)
                FindNewEvent(rightNode->value,max_ec);

            for(auto k : SC_union)
                this->T.insert(EdgeContainer(k,&this->edges,&(this->sweep_p)));
        }
    }
    return;
}

Graph<Point> EdgesIntersection::getGraph(){
    return this->graph;
}
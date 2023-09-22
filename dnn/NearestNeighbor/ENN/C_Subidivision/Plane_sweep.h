#pragma once

#include "C_Subdivision.h"

#include <algorithm>

// CAUTION: The original vector Q is modified, that is sorted!
std::vector<std::vector<int>> compute_adj_list(std::vector<Quad*>& Q){
    std::vector<std::vector<int>> adj_list(Q.size());

    /* Plane Sweep for O(nlogn) */

    std::vector<Quad*> & Q_sorted = Q; // Use S_sorted.assign(S.begin(), S.end()) if deepcopy is needed;
    
    auto comp_r = [](Quad* lhs, Quad* rhs) { 
        if (lhs->c == rhs->c && lhs->r == rhs->r) return std::addressof(*lhs) < std::addressof(*rhs); 
        else if (lhs->r == rhs->r) return lhs->c < rhs->c; 
        else return (lhs -> r) < (rhs -> r); 
    };

    std::sort(Q_sorted.begin(), Q_sorted.end(), comp_r);

    


    // Make a Status Tree
    struct comp_c
    {
        bool operator()(Quad* lhs, Quad *rhs) const
        {
            if (lhs->c == rhs->c) {
                if (lhs->r == rhs->r) {
                    // CAUTION 
                    //return false;
                    //std::cout <<"ADDRESSOF ";
                    return std::addressof(*lhs) < std::addressof(*rhs);
                }
                else return lhs->r < rhs->r;
            }
    
            return lhs->c < rhs->c;
        }
    };
    
    std::multiset<Quad*, comp_c> tree;

    // Handle events (Moving the sweepline rightwards)
    for(int i=0; i<Q_sorted.size(); i++){
        
        //std::cout << "IDX i: " <<  i << std::endl;
        Quad* quadNew = Q_sorted[i];
        auto sweepline = quadNew-> r; // r-coordinate of the sweepline

        // Remove Quads which are not intersected by the sweepline
        while(!tree.empty() && ((*tree.begin())->r + 4 < sweepline)) {
            tree.extract(tree.begin());
            
        }

        
        // Insert a new Quad 
        tree.insert(quadNew);

        /* Debug*/
        // bool cnt = tree.count(quadNew);
        // if (cnt > 0) std::cout << "DUPLICATES!!!!!!!!!!" << std::endl;


        // Find a starting position to look at 
        auto pos = tree.lower_bound(quadNew);
        auto left = pos;
        auto right = pos;

        // First, check leftside of pos 
        while(C_Subdivision::is_intersect_quad(quadNew, *left)){

            auto idx = std::lower_bound(Q_sorted.begin(), Q_sorted.end(), *left, comp_r) - Q_sorted.begin(); 

            if (idx != i){
                adj_list[idx].push_back(i);
                adj_list[i].push_back(idx);
            } 


            if (left == tree.begin()) break;
            else left = prev(left);
        }
        

        // Then, check rightside of pos
        while(C_Subdivision::is_intersect_quad(quadNew, *right)){
            int idx = std::lower_bound(Q_sorted.begin(), Q_sorted.end(), *right, comp_r) - Q_sorted.begin(); 

            
            if (idx != i){
                adj_list[idx].push_back(i);
                adj_list[i].push_back(idx);
            } 


            if (std::next(right) == tree.end()) break;
            else right = next(right);
        }
    }

    return adj_list;

}
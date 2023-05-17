#pragma once
#include <unordered_map>
#include <vector>

template <typename T>
class Disjoint_Set{
protected:
    int n;
    std::unordered_map<T,int> rank;
    std::unordered_map<T,T> parent;
public:

    //Make n sets, each containing a single item.
    Disjoint_Set(std::vector<T> items){
        //TODO
    }
    ~Disjoint_Set(){}
    
    //Find a parent of x
    T parent_find(T x){
        //TODO
    }

    //Union two sets containing x and y.
    void set_union(T x, T y){
        //TODO
        //Caution: deal with the case that x and y are already contained in the same set.
    }

    //Return all sets, each put in a sequence container (std::vector <T>)
    std::vector<std::vector <T>> get_all_sets(){
        std::vector<std::vector <T>> ret;
        //TODO
        return ret;
    }
};
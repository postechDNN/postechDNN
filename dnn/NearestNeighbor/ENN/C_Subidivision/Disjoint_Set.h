#pragma once

#include <unordered_map>
#include <vector>
#include <stack>

template <typename T>
class Disjoint_Set {
protected:
    std::unordered_map<T, int> rank;
    std::unordered_map<T, T> parent;
    
public:
    //Make n sets, each containing a single item.
    Disjoint_Set(std::vector<T> items) {
        for (T x : items) {
            parent[x] = x;
            rank[x] = 0;
        }
    }
    ~Disjoint_Set() {}

    //Find a parent of x
    T find(T x) {
        if (x != parent[x])
            parent[x] = find(parent[x]);

        return parent[x];
    }

    //Union two sets containing x and y.
    void set_union(T x, T y) {
        x = find(x);
        y = find(y);

        if (x != y) {
            if (rank[x] < rank[y]) {
                parent[x] = y;
            }
            else {
                parent[y] = x;
                if (rank[x] == rank[y])
                    rank[x]++;
            }
        }
    }

    //Return all sets, each put in a sequence container (std::vector <T>)
    std::vector<std::vector<T>> get_all_sets() {
        std::vector<std::vector<T>> ret;
        std::unordered_map<T, int> index;
        int count = 0;

        for (auto [key, value] : parent) {
            index[key] = -1;
        }

        for (auto [key, value] : index) {
            if (value == -1) {
                T p = find(key);

                if (index[p] == -1) {
                    index[p] = count;
                    ret.push_back(std::vector<T>({p}));
                    count++;
                }
                
                if (p != key) {
                    ret[index[p]].push_back(key);
                }
            }
        }

        return ret;
    }
};
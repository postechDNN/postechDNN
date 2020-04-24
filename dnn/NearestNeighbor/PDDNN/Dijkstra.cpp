#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include "Edge.h"
#include "Point.h"
#define INF 1e9
using namespace std;

void Dijkstra(Edge* _e[]) {
    const int E = sizeof(_e) / sizeof(Edge*);
  
    vector <pair<Point*, int>> dist;
    
    for (int i = 0; i < dist.size(); i++) {
        dist[i].second = INF;
    }

    priority_queue <pair<Point*, int>, vector <pair<Point*, int>>, greater <pair<Point*, int>>> qu;
    
    qu.push({ _e[0]->gets(), _e[0]->length});
    dist.push_back({ _e[0]->gets(),0 });

    while (!qu.empty()) {
        Point* here = qu.top.first;
        int cost = qu.top.second;
        
        qu.pop();
      
        for (int i = 0; i < E; i++) {
            if (_e[i]->gets() == here) {
                Point* next = _e[i]->gett();
                int nextcost = _e[i]->length();

                vector <pair<Point*, int>>::iterator it1 = find(dist.begin(), dist.end(), next);
                vector <pair<Point*, int>>::iterator it2 = find(dist.begin(), dist.end(), here);

                
                if (it1->second > it2->second + nextcost) {
                    it1->second = it2->second + nextcost;
                    qu.push({ next, it1->second });
                }
            }
        }
    }
}
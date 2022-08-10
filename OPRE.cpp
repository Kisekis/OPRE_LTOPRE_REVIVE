#include "OPRE.h"
#include <vector>
#include <iostream>
#include "my_random.h"
using namespace std;



bool OPRE_o(Graph &G, int rounds, int depth,int u,int v) {
    vector<int> max_cap_cycle = {};
    double max_cap = 0,cap = 0;

    while (rounds--) {
        auto data = randomWalk(G,u, v, depth);
        auto cycle = data.first;
        cap = data.second;
        if(cap > max_cap) {
            max_cap = cap;
            max_cap_cycle = cycle;
        }
    }
    double delta = max_cap;
    if(!max_cap_cycle.empty()) {
        int a,b;
        for(int i = 0; i < max_cap_cycle.size() - 1; i++) {
            a = max_cap_cycle[i];
            b = max_cap_cycle[i + 1];
            G.Hl[a][b]-=delta;
            G.Hr[a][b]+=delta;
            G.Hl[b][a]+=delta;
            G.Hr[b][a]-=delta;
        }
        return true;
    }
    return false;
}

pair<vector<int>,double> randomWalk(const Graph &G,int source,int dest,int L) {
    int u = source;
    int v;
    vector<int> path;
    vector<int> p_neigh;
    path.push_back(source);
    double min_cap = abs(G.Hl[dest][source]-G.Hr[dest][source])/2;
    while(L--) {
        if(u == dest) {
            path.push_back(source);
            return make_pair(path,min_cap);
        }
        for(auto x : G.neigh[u]) {
            if(G.Hl[u][x] > G.Hr[u][x]) {
                p_neigh.push_back(x);
            }
        }
        if(p_neigh.empty()) {
            return {};
        }else {
            v = p_neigh[randomNum(0,p_neigh.size()-1)];
            min_cap = min(min_cap,(G.Hl[u][v]-G.Hr[u][v])/2);
            p_neigh.clear();
            path.push_back(v);
        }
        u = v;
    }
    return {};
}

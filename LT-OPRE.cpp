#include "LT-OPRE.h"
#include <vector>
#include "my_random.h"
using namespace std;

double computeCred(const Graph &G, double delta, const vector<int> &cycle) {
    if(cycle.empty()) {
        return 0;
    }
    int a,b;
    double cred = 0;
    double Bu,Bv;
    for(int i = 0; i < cycle.size() - 1; i++) {
        a = cycle[i];
        b = cycle[i + 1];
        Bu = G.Hl[a][b];
        Bv = G.Hr[a][b];
        cred += 4*delta*(Bu-Bv-delta)/((Bu+Bv)*(Bu+Bv));
    }
    return cred;
}

bool LT_OPRE_o(Graph &G,int rounds,int depth,int u,int v) {
    double delta = G.alpha*(G.Hl[v][u]+G.Hr[v][u]) - G.Hr[v][u];
    vector<int> max_cred_cycle = {};
    double max_cred = 0,cred = 0;
    while (rounds--) {
        auto cycle = randomWalk(G,u, v, depth,delta);
        cred = computeCred(G,delta,cycle);
        if(cred > max_cred) {
            max_cred_cycle = cycle;
            max_cred = cred;
        }
    }
    if(!max_cred_cycle.empty()) {
        int a,b;
        for(int i = 0; i < max_cred_cycle.size() - 1; i++) {
            a = max_cred_cycle[i];
            b = max_cred_cycle[i + 1];
            G.Hl[a][b]-=delta;
            G.Hr[a][b]+=delta;
            G.Hl[b][a]+=delta;
            G.Hr[b][a]-=delta;
        }
        return true;
    }
    return false;
}

vector<int> randomWalk(const Graph &G, int source,int dest,int L,double delta) {
    int u = source;
    int v;
    vector<int> path;
    vector<int> delta_neigh;
    path.push_back(source);
    while(L--) {
        if(u == dest) {
            path.push_back(source);
            return path;
        }
        for(auto x : G.neigh[u]) {
            if(G.Hl[u][x] - G.Hr[u][x] > 0 && (G.Hl[u][x] - G.Hr[u][x]) / 2 > delta) {
                delta_neigh.push_back(x);
            }
        }
        if(delta_neigh.empty()) {
            return {};
        }else {
            v = delta_neigh[randomNum(0,delta_neigh.size()-1)];
            delta_neigh.clear();
            path.push_back(v);
        }
        u = v;
    }
    return {};
}
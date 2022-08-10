#include <random>

#include "my_random.h"

int randomNum(int a,int b) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(a,b);
    return dist(rng);
}
void randomDirection(const int N, vector<vector<bool>> &direction) {
    long count = 0;
    for(int i = 0;i<N;i++) {
        long r = randomNum(0,pow(2,30)-1);
        count = 0;
        for(int j = i+1;j<N;j++) {
            count++;
            if(count == 30) {
                r = randomNum(0,pow(2,30)-1);
                count = 0;
            }
            if(r&1U) {
                direction[i][j] = true;
                direction[j][i] = false;
            }else {
                direction[i][j] = false;
                direction[j][i] = true;
            }
            r>>=1U;
        }
    }
}
pair<int,int> selectChannel(const Graph &G) {
    int u = randomNum(0,G.N-1);
    while(G.neigh[u].empty()) {
        u = randomNum(0,G.N-1);
    }
    int neigh_size = G.neigh[u].size();
    int rand_neigh = randomNum(0,neigh_size-1);
    return make_pair(u,G.neigh[u][rand_neigh]);
}
pair<int,int> selectExhaustedChannel(const Graph &G) {
    pair<int,int> edge;
    while(true) {
        edge = selectChannel(G);
        double gr = max(G.Hl[edge.first][edge.second], G.Hr[edge.first][edge.second]);
        double ls = min(G.Hl[edge.first][edge.second], G.Hr[edge.first][edge.second]);
        if (G.alpha * (gr + ls) - ls > 0) {
            if (G.Hl[edge.first][edge.second] > G.Hr[edge.first][edge.second]) {
                return make_pair(edge.first, edge.second);
            } else {
                return make_pair(edge.second, edge.first);
            }
        }
    }
}
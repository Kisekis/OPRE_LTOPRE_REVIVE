#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <queue>
#include "my_random.h"
#include "Revive.h"
#include "LT-OPRE.h"
#include "OPRE.h"
#include "Graph.h"
extern "C"{
#include "glpk.h"
}
using namespace std;

//计算每条边Unbalanced Rate之和
double computeUnbRate(const Graph &G) {
    double rate = 0;
    double bu,bv;
    for(int i = 0;i<G.N;i++) {
        for(int j: G.neigh[i]) {
            bu = G.Hl[i][j];
            bv = G.Hr[i][j];
            if(bu>bv) {
                rate += (bu-bv)*(bu-bv)/((bu+bv)*(bu+bv));
            }
        }
    }
    return rate;
}

//在u v节点间进行交易(BFS搜索一条路)，尝试转移金额C，成功则返回true
bool transact(Graph &G,const int u,const int v,const double C,const vector<vector<bool>> &direction) {
    bool* vis = new bool[G.N];
    int* pi = new int[G.N];
    queue<int> q;
    vis[u] = true;
    q.push(u);
    bool found = false;
    while (!q.empty()) {
        int next = q.front();
        q.pop();
        for (auto it : G.neigh[next]) {
            {
                if(direction[min(next,it)][max(next,it)]) {
                    if (!vis[it] && G.Hl[next][it] > C) {
                        vis[it] = true;
                        pi[it] = next;
                        if(it == v) {
                            found = true;
                            break;
                        }
                        q.push(it);
                    }
                }
            }
        }
        if(found) break;
    }
    if(found) {
        int cur = v;
        while(cur!=u) {
            G.Hl[pi[cur]][cur] -= C;
            G.Hr[pi[cur]][cur] += C;
            G.Hr[cur][pi[cur]] -= C;
            G.Hl[cur][pi[cur]] += C;
            cur = pi[cur];
        }
    }
    delete []vis;
    delete []pi;
    return found;
}

int main() {
    Graph G1;
    auto edge = selectExhaustedChannel(G1);
    cout<<computeUnbRate(G1)<<endl;

    //交易部分
    vector<vector<bool>> direction (G1.N,vector<bool>(G1.N,false));
    randomDirection(G1.N, direction);
    for(int j = 0;j<100;j++) {
        int u = randomNum(0,G1.N-1);
        int v = randomNum(0,G1.N-1);

        for(int i = 0;i<100;i++) {
            transact(G1,u,v,0.01,direction);
        }
    }
    double after_transact = computeUnbRate(G1);
    cout<<"After Transact : " <<after_transact<<endl;
    cout<<"----------"<<endl;
    //G1 G2 G3一样,拷贝,对G1 G2 G3分别运行OPRE LT-OPRE Revive
    Graph G2(G1);
    Graph G3(G1);

    //OPRE部分
    clock_t start0 = clock();
    if(OPRE_o(G1,50,G1.N/2,edge.second,edge.first)) {
        cout<<"OPRE Finished"<<endl;
    }
    clock_t end0 = clock();
    cout<<"After OPRE : " <<after_transact<<" ---> "<<computeUnbRate(G1)<<endl;
    cout<<"OPRE total time : "<<end0-start0<<"ms"<<endl;
    cout<<"----------"<<endl;
    //LT-OPRE部分
    clock_t start1 = clock();
    if(LT_OPRE_o(G2,50, G2.N /2, edge.second, edge.first)) {
        cout<<"LT-OPRE Finished"<<endl;
    }
    clock_t end1 = clock();
    cout<<"After LT-OPRE : " <<after_transact<<" ---> "<<computeUnbRate(G2)<<endl;
    cout << "LT-OPRE total time : " << end1 - start1 << "ms" << endl;
    cout<<"----------"<<endl;

    //Revive部分
    clock_t start2 = clock();
    if(revive(G3)) {
        cout<<"Revive Finished"<<endl;
    }
    clock_t end2 = clock();
    cout<<"After Revive : " <<after_transact<<" ---> "<<computeUnbRate(G3)<<endl;
    cout<<"Revive total time : "<<end2-start2<<"ms"<<endl;
    cout<<"----------"<<endl;
}



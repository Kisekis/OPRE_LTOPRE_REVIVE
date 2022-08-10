#include <iostream>
#include <vector>
#include "Graph.h"
using namespace std;
Graph::Graph() {
    if(alpha<=0 || alpha >=0.5) {
        alpha = 0.25;
    }
    ifstream infile("subgraph.txt");
    if (!infile.is_open())
        cout << "failed to open file\n";
    int n,u,v;
    double bu,bv;
    infile>>n;
    cout<<"Graph size is : "<<n<<endl;
    N = n;
    Hl = new double*[N];
    for(int i = 0; i < N; ++i) {
        Hl[i] = new double[N]{0};
    }
    Hr = new double*[N];
    for(int i = 0; i < N; ++i) {
        Hr[i] = new double[N]{0};
    }
    reviveDelta = new double*[N];
    for(int i = 0; i < N; ++i) {
        reviveDelta[i] = new double[N]{0};
    }
    neigh.resize(N);

    while(infile>>u>>v>>bu>>bv) {
        Hl[u][v] = bu;
        Hr[u][v] = bv;
        Hl[v][u] = bv;
        Hr[v][u] = bu;
        neigh[u].push_back(v);
        neigh[v].push_back(u);
        channelSize++;
    }
}
Graph::~Graph() {
    for(int i = 0; i < N; ++i) {
        delete [] Hl[i];
    }
    delete [] Hl;
    for(int i = 0; i < N; ++i) {
        delete [] Hr[i];
    }
    delete [] Hr;
    for(int i = 0; i < N; ++i) {
        delete [] reviveDelta[i];
    }
    delete [] reviveDelta;
};
Graph::Graph(const Graph &G) {
    N = G.N;
    alpha = G.alpha;
    neigh = G.neigh;
    channelSize = G.channelSize;
    Hl = new double*[N];
    for(int i = 0; i < N; ++i) {
        Hl[i] = new double[N]{0};
    }
    Hr = new double*[N];
    for(int i = 0; i < N; ++i) {
        Hr[i] = new double[N]{0};
    }
    reviveDelta = new double*[N];
    for(int i = 0; i < N; ++i) {
        reviveDelta[i] = new double[N]{0};
    }
    for(int i = 0;i<N;i++) {
        for(int j = 0;j<N;j++) {
            Hl[i][j] = G.Hl[i][j];
            Hr[i][j] = G.Hr[i][j];
            reviveDelta[i][j] = G.reviveDelta[i][j];
        }
    }
}


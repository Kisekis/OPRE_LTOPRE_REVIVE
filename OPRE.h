#include <vector>
#include "Graph.h"
using namespace std;
#ifndef EP_OPRE__H
#define EP_OPRE__H

//OPRE，输入轮数，深度，枯竭的边u v(Bu<Bv,从u开始到v)
bool OPRE_o(Graph &G, int rounds, int depth,int u,int v);

//OPRE的randomWalk
pair<vector<int>,double> randomWalk(const Graph &G,int source,int dest,int L);


#endif //EP_OPRE__H

#include <vector>
#include "Graph.h"
using namespace std;

#ifndef EP_LT_OPRE_H
#define EP_LT_OPRE_H

//LT-OPRE，输入轮数，深度，枯竭的边uv（Bu < Bv,算法从u开始找到v）
bool LT_OPRE_o(Graph &G,int rounds,int depth,int u,int v);

//LT-OPRE的randomWalk，输入源节点，目的节点，深度L，delta）
vector<int> randomWalk(const Graph &G,int source,int dest,int L,double delta);

//给定一个randomwalk的cycle，计算cred之和
double computeCred(const Graph &G, double delta, const vector<int> &cycle);
#endif //EP_LT_OPRE_H


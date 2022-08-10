#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
#ifndef EP_GRAPH_H
#define EP_GRAPH_H

//图结构
class Graph {
public:
    int N{0}; //节点数
    double alpha{0}; //alpha，LT-OPRE中的阈值
    double **Hl{nullptr}; //邻接矩阵，Hl[u][v]代表边u v上u的余额
    double **Hr{nullptr}; //邻接矩阵，Hr[u][v]代表边u v上v的余额
    vector<vector<int>> neigh{1}; //邻接链表，neigh[u]包含了u的所有邻居
    double **reviveDelta{nullptr}; //revive中每条边的delta值
    int channelSize{0}; //边数
    //构造函数
    Graph();

    ~Graph();

    //拷贝构造
    Graph(const Graph &G);

};

#endif //EP_GRAPH_H

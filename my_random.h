#include <vector>
#include<cmath>
#include "Graph.h"
using namespace std;
#ifndef EP_MY_RANDOM_H
#define EP_MY_RANDOM_H

//随机返回[a,b]间的整数
int randomNum(int a,int b);
//随机为无向图生成一个方向，用于交易
void randomDirection(const int N, vector<vector<bool>> &direction);
//随机在G中选择一条边
pair<int,int> selectChannel(const Graph &G);
//随机在G中选择一条枯竭的边
pair<int,int> selectExhaustedChannel(const Graph &G);
#endif //EP_MY_RANDOM_H

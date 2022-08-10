import networkx as nx
import os
import json
import random


# read file
allNode = []
path = './nodes/'
for file_name in [file for file in os.listdir(path)]:
    with open(path + file_name) as json_file:
        data = json.load(json_file)
        allNode.append(data)


#create 2 graphs,directed graph "G" and undirected graph "undirected_G",assign random balance for both sides
G = nx.DiGraph()
undirected_G = nx.Graph()
for node in allNode:
    for channel in node["channels"]:
        if G.has_edge(node["id"],channel["neightbor"]) or G.has_edge(channel["neightbor"],node["id"]):
            continue
        capacity = float(channel["capacity"][:-4])
        balance1 = random.uniform(0,capacity)
        balance2 = capacity-balance1
        if balance1 > balance2:
            G.add_edge(node["id"],channel["neightbor"],capacity = capacity,balance1 = balance1,balance2 = balance2)
            undirected_G.add_edge(node["id"],channel["neightbor"],capacity = capacity,balance1 = balance1,balance2 = balance2)
        else:
            G.add_edge(channel["neightbor"],node["id"],capacity = capacity,balance1 = balance2,balance2 = balance1)
            undirected_G.add_edge(channel["neightbor"],node["id"],capacity = capacity,balance1 = balance2,balance2 = balance1)


# bfs from source on undirected_G
def bfs(source, N):
    visited = []
    q = []
    visited.append(source)
    q.append(source)

    while q and len(visited) < N:
        s = q.pop(0)
        count = 0
        for node in undirected_G.neighbors(s):
            count += 1
            if node not in visited and count < N / 10 and len(visited) < N:
                visited.append(node)
                q.append(node)
    return visited


# get induced subgraph of bfs tree nodes
def generateSubgraph(source, N):
    subgraph_nodes = bfs(source, N)
    H = G.subgraph(subgraph_nodes)  # induced subgraph
    if len(list(H.nodes())) != N:
        return generateSubgraph(random.sample(list(G.nodes), 1)[0], N)
    return H


N = 30
source = random.sample(list(G.nodes), 1)[0]
H = generateSubgraph(source,N)
H = nx.convert_node_labels_to_integers(H,first_label=0)


with open('subgraph.txt', 'w') as f:
    f.write(str(N))
    f.write('\n')
    for edge in list(H.edges()):
        ed = str(edge[0]) + " " + str(edge[1]) + " " + '{:f}'.format(H[edge[0]][edge[1]]["balance1"]) + " " + '{:f}'.format(H[edge[0]][edge[1]]["balance2"])
        f.write(ed)
        f.write('\n')




#pragma once

#include "undirectedGraph.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>

class dynamicUndirectedGraph : public UndirectedGraph {
    private:

        //std::unordered_map<int, std::unordered_set<Vertex*>> adjList_; // 邻接表，为动态图类特有
        //std::unordered_set<Edge*> edges_;// 边集合，为动态图类特有
        //std::vector<Vertex*> vertices_; // 顶点集合，为动态图类特有
        
    public:
        //~dynamicUndirectedGraph();
        bool removeVertex( int v );
        bool removeEdge( Edge* e );
        dynamicUndirectedGraph edgesSwitch(const std::vector<Edge*> &edges);
        dynamicUndirectedGraph getComplement();
};
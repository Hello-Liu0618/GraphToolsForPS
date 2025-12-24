#pragma once

#include "directedGraph.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>

class dynamicDirectedGraph : public DirectedGraph {
    private:
    
        //std::unordered_map<int, std::unordered_set<Vertex*>> adjList_;
        //std::unordered_set<Edge*> edges_;
        //std::vector<Vertex*> vertices_;
        
    
    public:
        //~dynamicDirectedGraph();
        bool removeVertex( int v );
        bool removeEdge( Edge* e );
        dynamicDirectedGraph edgesSwitch(const std::vector<Edge*> &edges);
        dynamicDirectedGraph getComplement();
};
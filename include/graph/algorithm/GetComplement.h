#pragma once

#include "graphAlgorithm.h"
#include "../dynamicDirectedGraph.h"
#include "../dynamicUndirectedGraph.h"

class GetComplement : public graphAlgorithm {
    public:
        inline AlgorithmResult run(const Graph& graph) const override {
            bool isDynamicDirected = dynamic_cast<const dynamicDirectedGraph*>(&graph)!= nullptr;
            bool isDynamicUndirected = dynamic_cast<const dynamicUndirectedGraph*>(&graph)!= nullptr;
            if ( !isDynamicDirected && !isDynamicUndirected ) {
                throw std::invalid_argument("GetComplement: graph is not dynamic");
            }
            if ( graph.numVertices() == 0 ) {
                throw std::invalid_argument("GetComplement: graph has no vertices");
            }
            AlgorithmResult result;
            if ( isDynamicDirected ) {
                dynamicDirectedGraph graph0 = static_cast<const dynamicDirectedGraph&>(graph);
                dynamicDirectedGraph resultGraph = graph0.getComplement();
                result.setData("complementGraph", resultGraph);
                return result;
            }
            else if ( isDynamicUndirected ) {
                dynamicUndirectedGraph graph0 = static_cast<const dynamicUndirectedGraph&>(graph);
                dynamicUndirectedGraph resultGraph = graph0.getComplement();
                result.setData("complementGraph", resultGraph);
                return result;
            }
            return result;
        }
};
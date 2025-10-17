#pragma once

#include "graphAlgorithm.h"
#include "../dynamicDirectedGraph.h"
#include "../dynamicUndirectedGraph.h"

#include <vector>

class EdgesSwitch : public graphAlgorithm {
    private:
        std::vector<Edge*> edges;//传入边集合
    public:
        inline explicit EdgesSwitch(const std::vector<Edge*> &edges) : edges(edges) {
            if ( edges.empty() ) {
                throw std::invalid_argument("EdgesSwitch: edges is empty");
            }
        }

        inline AlgorithmResult run(const Graph& graph) const override {
            bool isDynamicDirected = dynamic_cast<const dynamicDirectedGraph*>(&graph)!= nullptr;
            bool isDynamicUndirected = dynamic_cast<const dynamicUndirectedGraph*>(&graph)!= nullptr;
            if ( !isDynamicDirected && !isDynamicUndirected ) {
                throw std::invalid_argument("EdgesSwitch: graph is not dynamic");
            }
            for ( Edge* edge : edges ) {
                if ( edge == nullptr ) {
                    throw std::invalid_argument("EdgesSwitch: edge is nullptr");
                }
            }
            AlgorithmResult result;
            if ( isDynamicDirected ) {
                dynamicDirectedGraph graph0 = static_cast<const dynamicDirectedGraph&>(graph);
                dynamicDirectedGraph resultGraph = graph0.edgesSwitch(edges);
                result.setData("switchedGraph", resultGraph);
                return result;
            }
            else if ( isDynamicUndirected ) {
                dynamicUndirectedGraph graph0 = static_cast<const dynamicUndirectedGraph&>(graph);
                dynamicUndirectedGraph resultGraph = graph0.edgesSwitch(edges);
                result.setData("switchedGraph", resultGraph);
                return result;
            }
            return result;
        }
};
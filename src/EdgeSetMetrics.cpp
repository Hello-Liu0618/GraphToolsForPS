#include "../include/graph/algorithm/EdgeSetMetrics.h"

#include <vector>

AlgorithmResult EdgeSetMetrics::run(const Graph& graph) const {
    if ( graph.numVertices() == 0 ) {
        throw std::invalid_argument("EdgeSetMetrics : graph is empty");
    }
    if ( other == nullptr ) {
        throw std::invalid_argument("EdgeSetMetrics : other graph is empty");
    }
    if (  graph.isDirected() ^ other->isDirected() ) {
        throw std::invalid_argument("EdgeSetMetrics : the two graphs must be the same type (directed or undirected)");
    }
    std::vector<Edge*> graph_edges = graph.edges();
    std::vector<Edge*> other_edges = other->edges();
    if ( graph.isDirected() && other->isDirected() ) {//directed
        int intersection = 0;
        for ( auto e : graph_edges ) {
            if ( other->findEdge(e->getU(), e->getV()) != nullptr) {
                intersection++;
            }
        }
        int union_size = graph_edges.size() + other_edges.size() - intersection;
        AlgorithmResult result;
        result.setData("intersection", intersection);
        result.setData("union_size", union_size);
        return result;
    }
    else if ( !graph.isDirected() && !other->isDirected()  ) {//undirected
        int intersection = 0;
        for ( auto e : graph_edges ) {
            if ( other->findEdge(e->getU(), e->getV()) != nullptr) {
                intersection++;
            }
        }
        intersection /= 2;
        int union_size = (graph_edges.size() + other_edges.size()) / 2 - intersection;
        AlgorithmResult result;
        result.setData("intersection", intersection);
        result.setData("union_size", union_size);
        return result;
    }
    else {
        throw std::runtime_error("EdgeSetMetrics : unknown error");
    }
}
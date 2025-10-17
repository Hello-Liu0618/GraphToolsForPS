#include "../include/graph/algorithm/DFSCV.h"

#include <vector>

void isCutVertex(Graph* copyGraph, int u, int& time) {
    time++;
    copyGraph->setVertexAttr(u, "d", time);
    copyGraph->setVertexAttr(u, "low", copyGraph->getVertexAttr(u, "d"));
    copyGraph->setVertexAttr(u, "DFSCV_visited", true);
    std::vector<int> neighbors = copyGraph->neighbors(u);
    int childrenCount = 0;
    for ( int v : neighbors ) {
        if ( false == std::any_cast<bool>(copyGraph->getVertexAttr(v, "DFSCV_visited")) ) {
            copyGraph->setVertexAttr(v, "DFSCV_parent", u);
            childrenCount++;
            isCutVertex(copyGraph, v, time);
            int low_v = std::any_cast<int>(copyGraph->getVertexAttr(v, "low"));
            int low_u = std::any_cast<int>(copyGraph->getVertexAttr(u, "low"));
            if ( low_v < low_u ) {
                low_u = low_v;
                copyGraph->setVertexAttr(u, "low", low_u);
            }
            if ( -1 == std::any_cast<int>(copyGraph->getVertexAttr(u, "DFSCV_parent")) && childrenCount >= 2 ) {
                copyGraph->setVertexAttr(u, "isCutVertex", true);
            }
            else if ( -1 != std::any_cast<int>(copyGraph->getVertexAttr(u, "DFSCV_parent")) && std::any_cast<int>(copyGraph->getVertexAttr(v, "low")) >= std::any_cast<int>(copyGraph->getVertexAttr(u, "d"))) {
                copyGraph->setVertexAttr(u, "isCutVertex", true);
            }
        }
        else if ( v != std::any_cast<int>(copyGraph->getVertexAttr(u, "DFSCV_parent")) ) {
            int d_v = std::any_cast<int>(copyGraph->getVertexAttr(v, "d"));
            int low_u = std::any_cast<int>(copyGraph->getVertexAttr(u, "low"));
            if ( d_v < low_u ) {
                low_u = d_v;
                copyGraph->setVertexAttr(u, "low", low_u);
            }
        }
    }
    return;
}


AlgorithmResult DFSCV::run(const Graph& graph) const {
    if ( graph.vertices().empty() ) {
        throw std::invalid_argument("DFSCV : Graph is empty!");
    }
    Graph* copyGraph = graph.clone();
    if ( copyGraph->isDirected() ) {
        delete copyGraph;
        throw std::invalid_argument("DFSCV : Graph is directed!");
    }
    try{
        //Initializations
        std::vector<int> vertices = copyGraph->vertices();
        for ( int v : vertices ){
            copyGraph->setVertexAttr(v, "DFSCV_visited", false);
            copyGraph->setVertexAttr(v, "DFSCV_parent", -1);
            //copyGraph->setVertexAttr(v, "DFSCV_children", 0);
            copyGraph->setVertexAttr(v, "isCutVertex", false);
            copyGraph->setVertexAttr(v, "d", -1);
            copyGraph->setVertexAttr(v, "low", -1);
        }
        for ( int u : vertices ) {
            if ( std::any_cast<bool>(copyGraph->getVertexAttr(u, "DFSCV_visited")) == false ) {
                int t = 0;
                isCutVertex(copyGraph, u, t);
            }
        }
        //Output
        std::vector<int> cutVertices;
        for ( int v : vertices ) {
            if ( true == std::any_cast<bool>(copyGraph->getVertexAttr(v, "isCutVertex")) ) {
                cutVertices.push_back(v);
            }
        }
        AlgorithmResult result;
        if ( cutVertices.empty() ) {
            cutVertices.push_back(-1);
            result.setData("cutVertices", cutVertices);
            delete copyGraph;
            return result;
        }
        result.setData("cutVertices", cutVertices);
        delete copyGraph;
        return result;
    }
    catch ( const std::bad_any_cast& e ) {
        delete copyGraph;
        throw std::runtime_error("DFSCV: attribute type mismatch: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        delete copyGraph;
        throw std::runtime_error("DFSCV: runtime error: " + std::string(e.what()));
    }
}
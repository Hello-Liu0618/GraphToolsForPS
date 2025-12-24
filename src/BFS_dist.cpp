#include "../include/graph/algorithm/BFS_dist.h"

#include <queue>

AlgorithmResult BFS_dist::run(const Graph &graph) const {
    if ( graph.findVertex(this->start) == nullptr || graph.findVertex(this->v) == nullptr ) {
        throw std::invalid_argument(
            "BFS_dist: start vertex (" + std::to_string(this->start) + 
            ") or target vertex (" + std::to_string(this->v) + ") not found"
        );
    }

    Graph* copyGraph = graph.clone();
    if ( copyGraph == nullptr ) {
        throw std::bad_alloc();
    }
    try{
    std::queue<int> Ids;
    Ids.push(this->start);
    for ( int id : copyGraph->vertices() ) {
        copyGraph->setVertexAttr(id, "distance", -1);
        copyGraph->setVertexAttr(id, "visited", false);
    }
    copyGraph->setVertexAttr(this->start, "distance", 0);
    copyGraph->setVertexAttr(this->start, "visited", true);

    while (!Ids.empty()) {
        int current = Ids.front();
        Ids.pop();
        int crntDist = std::any_cast<int>(copyGraph->getVertexAttr(current, "distance"));
        if (current == this->v) {
            break;
        }
        for (int neighbor : copyGraph->neighbors(current)) {
            if (std::any_cast<int>(copyGraph->getVertexAttr(neighbor, "distance")) == -1 ){
                copyGraph->setVertexAttr(neighbor, "distance", crntDist + 1);
            }
            if (std::any_cast<bool>(copyGraph->getVertexAttr(neighbor, "visited")) == false) {
                Ids.push(neighbor);
                copyGraph->setVertexAttr(neighbor, "visited", true);
            }
            else {
                continue;
            }
        }
    }

    AlgorithmResult result;
    result.setData("distance", copyGraph->getVertexAttr(this->v, "distance"));
    delete copyGraph;
    return result;
    }
    catch ( const std::bad_any_cast& e ) {
        delete copyGraph;
        throw std::runtime_error("BFS_dist: attribute type mismatch: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        delete copyGraph;
        throw std::runtime_error("BFS_dist: runtime error: " + std::string(e.what()));
    }
}
#include "../include/graph/algorithm/BinaryGraphCheck.h"

#include <queue>

AlgorithmResult BinaryGraphCheck::run(const Graph& graph) const {
    Graph* copyGraph = graph.clone();
    if (copyGraph == nullptr) {
        delete copyGraph;
        throw std::bad_alloc();
    }
    if (copyGraph->numVertices() == 0) {
        AlgorithmResult result;
        result.setData("isBinary", true);
        delete copyGraph;
        return result;
    }
    try {
        for ( int id : copyGraph->vertices() ) {
                copyGraph->setVertexAttr(id, "visited", false);
                copyGraph->setVertexAttr(id, "color", 0);//初始颜色为0
            }
        for ( int startId : copyGraph->vertices() ) {//处理多联通分量
            if ( std::any_cast<bool>(copyGraph->getVertexAttr(startId, "visited")) == true ) {
                continue;
            }
            std::queue<int> Ids;
            Ids.push(startId);
            copyGraph->setVertexAttr(startId, "visited", true);
            copyGraph->setVertexAttr(startId, "color", 1);
            while (!Ids.empty() ) {
                int crnt = Ids.front();
                Ids.pop();
                int crntColor = std::any_cast<int>(copyGraph->getVertexAttr(crnt, "color"));
                for ( int neighbor : copyGraph->neighbors(crnt) ) {
                   int neighborColor = std::any_cast<int>(copyGraph->getVertexAttr(neighbor, "color"));
                    if ( neighborColor == 0 ) {
                        copyGraph->setVertexAttr(neighbor, "color", crntColor * -1);
                    }
                    else if ( neighborColor == crntColor ) {
                        AlgorithmResult result;
                       result.setData("isBinary", false);
                       delete copyGraph;
                       return result;
                   }
                   if ( std::any_cast<bool>(copyGraph->getVertexAttr(neighbor, "visited")) == false ) {
                        Ids.push(neighbor);
                       copyGraph->setVertexAttr(neighbor, "visited", true);
                    }
                    else {
                        continue;
                 }
            }
        }
        }
        AlgorithmResult result;
        result.setData("isBinary", true);
        delete copyGraph;
        return result;
    }
    catch ( const std::bad_any_cast& e ) {
        delete copyGraph;
        throw std::runtime_error("BinaryGraphCheck: attribute type mismatch: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        delete copyGraph;
        throw std::runtime_error("BinaryGraphCheck: runtime error: " + std::string(e.what()));
    }
}
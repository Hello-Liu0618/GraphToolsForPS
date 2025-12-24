#include "../include/graph/algorithm/DFSBlk.h"

#include <stack>
#include <vector>
#include <set>

void subDFSBlk(Graph* copyGraph, int u, int& time, std::stack<std::set<int>>& S, std::vector<std::set<int>>& ans) {
    time++;
    copyGraph->setVertexAttr(u, "d", time);
    copyGraph->setVertexAttr(u, "low", copyGraph->getVertexAttr(u, "d"));
    copyGraph->setVertexAttr(u, "DFSBlk_visited", true);
    std::vector<int> neighbors = copyGraph->neighbors(u);
    int childrenCount = 0;
    for ( int v : neighbors ) {
        if ( false == std::any_cast<bool>(copyGraph->getVertexAttr(v, "DFSBlk_visited")) ) {
            S.push({u, v});
            copyGraph->setVertexAttr(v, "DFSBlk_parent", u);
            childrenCount++;
            subDFSBlk(copyGraph, v, time, S, ans);
            int low_u = std::any_cast<int>(copyGraph->getVertexAttr(u, "low"));
            int low_v = std::any_cast<int>(copyGraph->getVertexAttr(v, "low"));
            if ( low_v < low_u ) {
                low_u = low_v;
                copyGraph->setVertexAttr(u, "low", low_u);
            }
            if ( (-1 == std::any_cast<int>(copyGraph->getVertexAttr(u, "DFSBlk_parent")) 
                && childrenCount >= 2 ) 
                ||
                (-1 != std::any_cast<int>(copyGraph->getVertexAttr(u, "DFSBlk_parent")) 
                && std::any_cast<int>(copyGraph->getVertexAttr(v, "low")) >= std::any_cast<int>(copyGraph->getVertexAttr(u, "d")) ) ) {
                    std::set<int> subAns;
                    while ( true ) {
                        std::set<int> top = S.top();
                        S.pop();
                        for ( int n : top ) {
                            subAns.insert(n);
                        }
                        std::set<int> t = {u, v};
                        if ( top == t ) {
                            break;
                        }
                    }
                    ans.push_back(subAns);
                }
        }
        else if ( v != std::any_cast<int>(copyGraph->getVertexAttr(u, "DFSBlk_parent")) ) {
            int d_u = std::any_cast<int>(copyGraph->getVertexAttr(u, "d"));
            int d_v = std::any_cast<int>(copyGraph->getVertexAttr(v, "d"));
            if ( d_u > d_v ) {
                S.push({u, v});
            }
            int low_u = std::any_cast<int>(copyGraph->getVertexAttr(u, "low"));
            if ( d_v < low_u ) {
                low_u = d_v;
                copyGraph->setVertexAttr(u, "low", low_u);
            }
        }
    }
    return;
}

AlgorithmResult DFSBlk::run( const Graph& graph ) const {
    if ( graph.vertices().empty() ) {
        throw std::invalid_argument("DFSBlk: Graph is empty!");
    }
    Graph* copyGraph = graph.clone();
    if ( copyGraph->isDirected() ) {
        delete copyGraph;
        throw std::invalid_argument("DFSBlk: Graph is directed!");
    }
    try{
        //Initializations
        std::vector<int> vertices = copyGraph->vertices();
        for ( int v : vertices ) {
            copyGraph->setVertexAttr( v, "DFSBlk_visited", false );
            copyGraph->setVertexAttr( v, "DFSBlk_parent", -1 );
            copyGraph->setVertexAttr( v, "d", -1 );
            copyGraph->setVertexAttr( v, "low", -1 );
        }
        std::vector<std::set<int>> ans;
        for ( int u : vertices ) {
            if ( false == std::any_cast<bool>(copyGraph->getVertexAttr(u, "DFSBlk_visited")) ) {
                std::stack<std::set<int>> S;
                int time = 0;
                subDFSBlk(copyGraph, u, time, S, ans);
                if ( !S.empty() ) {
                    std::set<int> subAns;
                    while ( !S.empty() ) {
                        std::set<int> top = S.top();
                        S.pop();
                        for ( int n : top ) {
                            subAns.insert(n);
                        }
                    }
                    ans.push_back(subAns);
                }
            }
        }
        std::vector<std::vector<int>> vecResult;
        for ( std::set<int> subAns : ans ) {
            std::vector<int> vecSubAns;
            for ( int n : subAns ) {
                vecSubAns.push_back(n);
            }
            vecResult.push_back(vecSubAns);
        }
        AlgorithmResult result;
        result.setData("biconnectedComponents", vecResult);
        result.setData("biconnectedComponentsCount", vecResult.size());
        delete copyGraph;
        return result;
    }
    catch ( const std::bad_any_cast& e ) {
        delete copyGraph;
        throw std::runtime_error("DFSBlk: attribute type mismatch: " + std::string(e.what()));
    }
    catch (const std::exception& e) {
        delete copyGraph;
        throw std::runtime_error("DFSBlk: runtime error: " + std::string(e.what()));
    }
}
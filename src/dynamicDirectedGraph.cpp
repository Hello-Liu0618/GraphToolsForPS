#include "../include/graph/dynamicDirectedGraph.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <stack>
/*
dynamicDirectedGraph::~dynamicDirectedGraph() {
    for ( Vertex* vertex : this->vertices_ ) {
        delete vertex;
    }
    for ( Edge* edge : this->edges_ ) {
        delete edge;
    }
}
*/
bool dynamicDirectedGraph::removeVertex( int v ) {
    if ( this->findVertex(v) == nullptr ) {
        return false;//顶点不存在
    }
    else {
        std::stack<Edge*> edgesToRemove;
        Vertex* vertex = this->findVertex(v);
        for ( Edge* edge : this->edges_ ) {
            if ( edge->getV() == v ) {
                this->adjList_[edge->getU()].erase(vertex);
                //this->edges_.erase(edge);
                edgesToRemove.push(edge);
            }
            else if ( edge->getU() == v ) {
                //this->adjList_[v].erase(this->findVertex(edge->getV()));
                //this->edges_.erase(edge);
                edgesToRemove.push(edge);
            }
        }
        while (!edgesToRemove.empty() ) {
            Edge* edge = edgesToRemove.top();
            edgesToRemove.pop();
            this->edges_.erase(edge);
            delete edge;
        }
        this->adjList_.erase(v);
        auto it = std::find(this->vertices_.begin(), this->vertices_.end(), vertex);
        if (it != this->vertices_.end()) {
            this->vertices_.erase(it);
        }
        return true;
    }
}

bool dynamicDirectedGraph::removeEdge(Edge* edge) {
    if ( edge == nullptr ) {
        return false;//指针为空
    }
    int u = edge->getU();
    int v = edge->getV();
    Edge* edgeToRemove = this->findEdge(u, v);
    if ( edgeToRemove == nullptr ) {
        return false;//边不存在
    }
    this->adjList_[u].erase(this->findVertex(v));
    this->edges_.erase(edgeToRemove);
    delete edgeToRemove;
    return true;
}

dynamicDirectedGraph dynamicDirectedGraph::edgesSwitch(const std::vector<Edge*> &edges) {
    dynamicDirectedGraph deepCopy;
    for ( Vertex* vertex : this->vertices_ ) {
        deepCopy.addVertex(vertex->getId());
        deepCopy.findVertex(vertex->getId())->setAttributesMap(vertex->getAttributesMap());
    }
    for ( Edge* edge : this->edges_ ) {
        deepCopy.addEdge(edge->getU(), edge->getV());
        deepCopy.findEdge(edge->getU(), edge->getV())->setAttributesMap(edge->getAttributesMap());
        deepCopy.findEdge(edge->getU(), edge->getV())->setId(edge->getId());
    }
    //下面这段代码我怀疑并没有必要，因为addEdge中实现了对adjList_的更新，但是由于insert并不会重复添加，所以我选择保留
    for ( auto it = this->adjList_.begin() ; it!= this->adjList_.end() ; it++ ) {
        int u = it->first;
        for ( Vertex* vertex : it->second ) {
            deepCopy.adjList_[u].insert(deepCopy.findVertex(vertex->getId()));
        }
    }
    for ( Edge* edge : edges ) {
        if ( edge == nullptr ) {
            throw std::invalid_argument("nullptr edge");
        }//指针为空
        int u = edge->getU();
        int v = edge->getV();
        if ( deepCopy.findVertex(u) == nullptr || deepCopy.findVertex(v) == nullptr ) {
            throw std::runtime_error("edge not in graph");//顶点不存在
        }
    }
    for ( Edge* edge : edges ) {
        int u = edge->getU();
        int v = edge->getV();
        if ( deepCopy.findEdge(u, v) == nullptr ) {
            //addEdge中确保为单向边添加
            if ( !deepCopy.addEdge(u, v) ) {
                throw std::runtime_error("add edge failed");
            }
        }
        else {
            //removeEdge中确保为单向边删除
            if (!deepCopy.removeEdge(edge)) {
                throw std::runtime_error("remove edge failed");
            }
        }
    }
    return deepCopy;
}

dynamicDirectedGraph dynamicDirectedGraph::getComplement() {
    dynamicDirectedGraph complement;
    for ( Vertex* vertex : this->vertices_ ) {
        complement.addVertex(vertex->getId());
    }
    for ( int i = 0 ; i < this->vertices_.size() ; i++ ) {
        for ( int j = 0 ; j < this->vertices_.size() ; j++ ) {
            if ( i == j ) {
                continue;
            }
            int u = this->vertices_[i]->getId();
            int v = this->vertices_[j]->getId();
            if ( this->findEdge(u, v) == nullptr ) {
                complement.addEdge(u, v);//addEdge中确保为单向边添加
            }
        }
    }
    return complement;
}
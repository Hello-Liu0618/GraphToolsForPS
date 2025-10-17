#include "../include/graph/dynamicUndirectedGraph.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
/*
dynamicUndirectedGraph::~dynamicUndirectedGraph() {
    for ( Vertex* vertex : this->vertices_ ) {
        delete vertex;
    }
    for ( Edge* edge : this->edges_ ) {
        delete edge;
    }
}
*/
bool dynamicUndirectedGraph::removeVertex(int v) {
    if ( this->findVertex(v) == nullptr ) {
        return false;// 顶点不存在
    }
    else {
        Vertex* vertex = this->findVertex(v);
        std::vector<int> neighbors = this->neighbors(v);
        for ( int i : neighbors ) {
            this->adjList_[i].erase(vertex);
            Edge* e1 = this->findEdge(i, v);
            Edge* e2 = this->findEdge(v, i);
            this->edges_.erase(e1);
            this->edges_.erase(e2);
            delete e1;
            delete e2;
        }
        this->adjList_.erase(v);
        this->vertices_.erase(std::find(this->vertices_.begin(), this->vertices_.end(), vertex));
        delete vertex;
        return true;
    }
}

bool dynamicUndirectedGraph::removeEdge(Edge* edge) {
    if ( edge == nullptr ) {
        return false;//指针为空
    }
    int u = edge->getU();
    int v = edge->getV();
    if ( this->findEdge(u, v) == nullptr ) {
        return false;// 边不存在
    }
    this->adjList_[u].erase(this->findVertex(v));
    this->adjList_[v].erase(this->findVertex(u));
    Edge* edge1 = this->findEdge(u, v);
    Edge* edge2 = this->findEdge(v, u);
    this->edges_.erase(edge1);
    this->edges_.erase(edge2);
    delete edge1;
    delete edge2;
    return true;
}


dynamicUndirectedGraph dynamicUndirectedGraph::edgesSwitch(const std::vector<Edge*> &edges) {
    dynamicUndirectedGraph deepCopy;
    for ( Vertex* vertex : this->vertices_ ) {
        deepCopy.addVertex(vertex->getId());
        deepCopy.findVertex(vertex->getId())->setAttributesMap(vertex->getAttributesMap());
    }
    for ( Edge* edge : this->edges_ ) {
        deepCopy.addEdge(edge->getU(), edge->getV());//addEdge中已经实现了双向边添加
        deepCopy.findEdge(edge->getU(), edge->getV())->setAttributesMap(edge->getAttributesMap());
        deepCopy.findEdge(edge->getV(), edge->getU())->setAttributesMap(edge->getAttributesMap());
        deepCopy.findEdge(edge->getU(), edge->getV())->setId(edge->getId());
        deepCopy.findEdge(edge->getV(), edge->getU())->setId(edge->getId());
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
            throw std::invalid_argument("nullptr edge");// 指针为空
        }
        int u = edge->getU();
        int v = edge->getV();
        if ( deepCopy.findVertex(u) == nullptr || deepCopy.findVertex(v) == nullptr ) {
            throw std::invalid_argument("edge not in graph");// 顶点不存在
        }
    }
    for ( Edge* edge : edges ) {
        int u = edge->getU();
        int v = edge->getV();
        if ( deepCopy.findEdge(u, v) == nullptr ) {
            //this->addEdge(u, v);//addEdge中已经实现了双向边添加
            if ( !deepCopy.addEdge(u, v) ) {
                throw std::runtime_error("add edge failed");// 边添加失败
            }
        }
        else {
            //this->removeEdge(edge);//removeEdge中已经实现了双向边删除
            if (!deepCopy.removeEdge(edge)) {
                throw std::runtime_error("remove edge failed");// 边删除失败
            }
        }
    }
    return deepCopy;
}

dynamicUndirectedGraph dynamicUndirectedGraph::getComplement() {
    dynamicUndirectedGraph complement;
    for ( Vertex* vertex : this->vertices_ ) {
        complement.addVertex(vertex->getId());
    }
    for ( int i = 0 ; i < this->vertices_.size() ; i++ ) {
        for ( int j = i+1 ; j < this->vertices_.size() ; j++ ) {
            if ( i == j ) {
                continue;
            }
            int u = this->vertices_[i]->getId();
            int v = this->vertices_[j]->getId();
            if ( this->findEdge(u, v) == nullptr ) {
                complement.addEdge(u, v);//addEdge中已经实现了双向边添加
            }
        }
    }
    return complement;
}
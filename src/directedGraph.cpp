#include "../include/graph/directedGraph.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>

DirectedGraph::~DirectedGraph() {
    // 1. 释放 vertices_ 中的 Vertex
    for (Vertex* v : vertices_) {
        delete v;
    }
    // 2. 释放 edges_ 中的 Edge
    for (Edge* e : edges_) {
        delete e;
    }
}

bool DirectedGraph::addVertex(int v) {//确保顶点编号大于0且确保不重复，实现了对vertices_和adjList_的同步更新
    if ( v <= 0 ) {
        return false;
    }
    if ( this->adjList_.count(v) ) {
        return false;//顶点已存在
    }
    this -> vertices_.push_back(new Vertex(v));
    this -> adjList_[v] = std::unordered_set<Vertex*>();
    return true;
}

bool DirectedGraph::addEdge(int u, int v) {//确保了两个顶点编号均大于0，通过检查邻接表确保两个点都在图中，实现了对adjList_和edges_的同步更新，确保为有向边的实现方式，保证了边的去重
    if ( u <= 0 || v <= 0 ) {
        return false;
    }
    //在邻接表中查找而不是在顶点集合中查找，因为顶点集合中元素为对象指针，需要调用成员函数，加大代码复杂度
    if ( !(this->adjList_.count(u)) || !(this->adjList_.count(v)) ) {
        throw std::invalid_argument("vertices not exist");
        return false;// 顶点不存在
    }
    //addVertex中保持了adjList_与vertices_的一致性，因此这里直接操作adjList_即可，不必担心[]操作对adjList_的改变
    //确保为有向边的实现方式
    if ( this->findEdge(u, v) != nullptr ) {
        return false;//边已存在，无需重复添加
    }
    Vertex* v_vertex = this->findVertex(v);//寻找v的地址
    this -> adjList_[u].insert(v_vertex);//v插入adjList_[u]中
    std::pair<int, int> edge = std::make_pair(u, v);
    this -> edges_.insert(new Edge(edge));
    return true;
}

bool DirectedGraph::addEdge(int u, int v, int weight) {
    if ( u <= 0 || v <= 0 ) {
        return false;// 顶点编号必须大于0
    }
    //在邻接表中查找而不是在顶点集合中查找，因为顶点集合中元素为对象指针，需要调用成员函数，加大代码复杂度
    if ( !(this->adjList_.count(u)) || !(this->adjList_.count(v)) ) {
        throw std::invalid_argument("vertices not exist");
        return false;// 顶点不存在
    }
    //addVertex中保持了adjList_与vertices_的一致性，因此这里直接操作adjList_即可，不必担心[]操作对adjList_的改变
    //确保为有向边的实现方式
    if ( this->findEdge(u, v) != nullptr ) {
        return false;//边已存在，无需重复添加
    }
    Vertex* v_vertex = this->findVertex(v);//寻找v的地址
    this -> adjList_[u].insert(v_vertex);//v插入adjList_[u]中
    std::pair<int, int> edge = std::make_pair(u, v);
    this -> edges_.insert(new Edge(edge, weight));
    return true;
}

bool DirectedGraph::setEdgeId(Edge* e, int id) {
    if (e == nullptr || id < 0) {
        throw std::invalid_argument(id < 0 ? "edge ID must be non-negative" : "edge pointer is null");
    }
    int u = e->getU();
    int v = e->getV();
    for (Edge* edge : this->edges_) {
        if ( edge->getId() == id ) {
            return false;//边id已存在
        }
    }
    bool updated = false;
    for (Edge* edge : this->edges_) {
        // 同时匹配 u→v 和 v→u 边
        if (edge->getU() == u && edge->getV() == v) {
            edge->setId(id);
            updated = true;
        }
    }
    if (!updated) {
        throw std::invalid_argument("edge not found in the graph");
    }
    return true;
}

int DirectedGraph::degree(int v) const{
    if ( !(this ->adjList_.count(v)) ) {
        throw std::invalid_argument("vertices not exist");
        return -1; // 顶点不存在
    }
    return this -> adjList_.at(v).size();
}

Graph* DirectedGraph::subgraph_by_vertices(const std::vector<int>& vertices) const {
    DirectedGraph* subgraph = new DirectedGraph();
    for ( int v : vertices ) {
        if ( this->adjList_.count(v) ) {
            subgraph->addVertex(v);
            Vertex* v_vertex = subgraph->findVertex(v);
            v_vertex->setAttributesMap(this->findVertex(v)->getAttributesMap());
        }
    }
    std::vector<int> vs = subgraph->vertices();
    for ( int i = 0 ; i < vs.size() ; i++ ) {
        for ( int j = 0 ; j < vs.size() ; j++ ) {
            if ( i == j ) {
                continue;
            }
            int u = vs[i];
            int v = vs[j];
            if ( this->adjList_.at(u).count(this->findVertex(v)) ) {//存在从u到v的边
                subgraph->addEdge(u, v);//addEdge中会检查边是否重复添加且能正确处理有向边的添加
                Edge* edge = subgraph->findEdge(u, v);
                edge->setId(this->findEdge(u, v)->getId());
                edge->setAttributesMap(this->findEdge(u, v)->getAttributesMap());
            }
        }
    }
    //subgraph->original_graph_ = const_cast<DirectedGraph*>(this);
    return subgraph;
}

Graph* DirectedGraph::subgraph_by_edges(const std::vector<Edge*>& edges) const {
    DirectedGraph* subgraph = new DirectedGraph();
    for ( Edge* e : edges ) {
        int u = e->getU();
        int v = e->getV();
        if ( this->findEdge(u, v) == nullptr ) {
            throw std::invalid_argument("edge not exist in the original graph");
        }
        int w = e->getWeight();
        subgraph->addVertex(u);
        subgraph->addVertex(v);
        subgraph->addEdge(u, v, w);
        Vertex* v_vertex = subgraph->findVertex(v);
        v_vertex->setAttributesMap(this->findVertex(v)->getAttributesMap());
        Vertex* u_vertex = subgraph->findVertex(u);
        u_vertex->setAttributesMap(this->findVertex(u)->getAttributesMap());
        Edge* new_edge = subgraph->findEdge(u, v);
        new_edge->setId(e->getId());
        new_edge->setAttributesMap(e->getAttributesMap());
    }
    //subgraph->original_graph_ = const_cast<DirectedGraph*>(this);
    return subgraph;
}

int DirectedGraph::numVertices() const{
    return this -> vertices_.size();
}

int DirectedGraph::numEdges() const {
    return this->edges_.size();
}

bool DirectedGraph::isDirected() const {
    return this->is_directed_;
}

std::vector<int> DirectedGraph::neighbors(int v) const {
    if ( !this->adjList_.count(v) ) {
        throw std::invalid_argument("vertices not exist");
    }
    std::vector<int> neighbors;
    for ( Vertex* w : this->adjList_.at(v) ) {
        neighbors.push_back(w->getId());
    }
    return neighbors;
}

std::vector<Edge*> DirectedGraph::edges() const {
    std::vector<Edge*> edges;
    for ( Edge* e : this->edges_ ) {
        edges.push_back(e);
    }
    return edges;
}

std::vector<int> DirectedGraph::vertices() const {
    std::vector<int> vertices;
    for ( Vertex* v : this->vertices_ ) {
        vertices.push_back(v->getId());
    }
    return vertices;
}

Vertex* DirectedGraph::findVertex(int id) const {
    for ( Vertex* v : this->vertices_ ) {
        if ( v->getId() == id ) {
            return v;
        }
    }
    return nullptr;
}

Edge* DirectedGraph::findEdge(int u, int v) const {
    for ( Edge* e : this->edges_ ) {
        if ( e->getU() == u && e->getV() == v ) {
            return e;
        }
    }
    return nullptr;
}

Edge* DirectedGraph::findEdge(int id) const {
    for ( Edge* e : this->edges_ ) {
        if ( e->getId() == id ) {
            return e;
        }
    }
    return nullptr;
}

std::any DirectedGraph::getVertexAttr(int id, std::string key) const {
    Vertex* v = this->findVertex(id);
    if (v == nullptr) {
        throw std::invalid_argument("vertex not exist");
    }
    return v->getAttribute(key);
}

std::any DirectedGraph::getEdgeAttr(int u, int v, std::string key) const {
    Edge* e = this->findEdge(u, v);
    if (e == nullptr) {
        throw std::invalid_argument("edge not exist");
    }
    return e->getAttribute(key);
}

std::any DirectedGraph::getEdgeAttr(int id, std::string key) const {
    Edge* e = this->findEdge(id);
    if (e == nullptr) {
        throw std::invalid_argument("edge not exist");
    }
    return e->getAttribute(key);
}

bool DirectedGraph::setVertexAttr(int id, std::string key, std::any value) {
    Vertex* v = this->findVertex(id);
    if (v == nullptr) {
        return false;
    }
    return v->setAttribute(key, value);
}

bool DirectedGraph::setEdgeAttr(int u, int v, std::string key, std::any value) {
    Edge* e = this->findEdge(u, v);
    if (e == nullptr) {
        return false;
    }
    return e->setAttribute(key, value);
}

bool DirectedGraph::setEdgeAttr(int id, std::string key, std::any value) {
    Edge* e = this->findEdge(id);
    if (e == nullptr) {
        return false;
    }
    return e->setAttribute(key, value);
}

bool DirectedGraph::removeVertexAttr(int id, std::string key) {
    Vertex* v = this->findVertex(id);
    if (v == nullptr) {
        return false;
    }
    return v->deleteAttribute(key);
}

bool DirectedGraph::removeEdgeAttr(int u, int v, std::string key) {
    Edge* e = this->findEdge(u, v);
    if (e == nullptr) {
        return false;
    }
    return e->deleteAttribute(key);
}

bool DirectedGraph::removeEdgeAttr(int id, std::string key) {
    Edge* e = this->findEdge(id);
    if (e == nullptr) {
        return false;
    }
    return e->deleteAttribute(key);
}

bool DirectedGraph::clearVertexAttrs(int v) {
    Vertex* vertex = this->findVertex(v);
    if (vertex == nullptr) {
        return false;
    }
    return vertex->clearAttributes();
}

bool DirectedGraph::clearEdgeAttrs(int u, int v) {
    Edge* edge = this->findEdge(u, v);
    if (edge == nullptr) {
        return false;
    }
    return edge->clearAttributes();
}

bool DirectedGraph::clearEdgeAttrs(int id) {
    Edge* edge = this->findEdge(id);
    if (edge == nullptr) {
        return false;
    }
    return edge->clearAttributes();
}

Graph* DirectedGraph::clone() const {
    DirectedGraph* clonedGraph = new DirectedGraph();
    for ( Vertex* v : this->vertices_ ) {
        clonedGraph->addVertex(v->getId());
        Vertex* clonedVertex = clonedGraph->findVertex(v->getId());
        clonedVertex->setAttributesMap(v->getAttributesMap());
    }
    for ( Edge* e : this->edges_ ) {
        int u = e->getU();
        int v = e->getV();
        int w = e->getWeight();
        clonedGraph->addEdge(u, v, w);
        Edge* clonedEdge = clonedGraph->findEdge(u, v);
        clonedEdge->setId(e->getId());
        clonedEdge->setAttributesMap(e->getAttributesMap());
    }
    return clonedGraph;
}
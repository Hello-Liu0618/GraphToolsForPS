#include "../include/graph/undirectedGraph.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>

UndirectedGraph::~UndirectedGraph() {
    // 1. 释放 vertices_ 中的 Vertex
    for (Vertex* v : vertices_) {
        delete v;
    }
    // 2. 释放 edges_ 中的 Edge
    for (Edge* e : edges_) {
        delete e;
    }
}

bool UndirectedGraph::addVertex(int v) {//确保顶点编号大于0且确保不重复，实现了对vertices_和adjList_的同步更新
    if ( v <= 0 ) {
        return false; // 顶点编号必须大于0
    }
    if (this->adjList_.count(v)) {
        return false; // 顶点已存在
    }
    this -> vertices_.push_back(new Vertex(v));//加入顶点集合中
    this -> adjList_[v] = std::unordered_set<Vertex*>(); // 初始化邻接表为空集；由此，邻接表中key与vertices_中的元素一一对应，因此可用于不用访问vertices_的情况下确定顶点的存在性
    return true;
}

bool UndirectedGraph::addEdge(int u, int v) {//确保两个顶点编号均大于0，通过检查邻接表确保两个点都在图中，实现了对adjList_和edges_的同步更新，通过edges_中加入两个有向边实现无向边，保证了边的去重
    if ( u <= 0 || v <= 0 ) {
        return false; // 顶点编号必须大于0
    }
    //在邻接表中查找而不是在顶点集合中查找，因为顶点集合中元素为对象指针，需要调用成员函数，加大代码复杂度
    if ( this -> adjList_.find(u) == this -> adjList_.end() || this -> adjList_.find(v) == this -> adjList_.end() ) {
        throw std::invalid_argument("顶点不存在");
        return false; // 顶点不存在
    }
    // addEdge 中添加边存在性检查
    if (this->findEdge(u, v) != nullptr) {
        return false; // 边已存在，无需重复添加
    }
    // 再执行后续 new Edge 和插入逻辑
    //addVertex中保持了adjList_与vertices_的一致性，因此这里直接操作adjList_即可，不必担心[]操作对adjList_的改变
    //用两个方向的有向边来表示无向边
    Vertex* v_vertex = this->findVertex(v); // 获取 vertices_ 中已有的 v 指针
    Vertex* u_vertex = this->findVertex(u); // 获取 vertices_ 中已有的 u 指针
    this->adjList_[u].insert(v_vertex); // 插入已有指针，而非新建
    this->adjList_[v].insert(u_vertex);
    std::pair<int, int> edge0 = std::make_pair(u, v);
    std::pair<int, int> edge1 = std::make_pair(v, u);
    this -> edges_.insert(new Edge(edge0));
    this -> edges_.insert(new Edge(edge1));
    return true;
}

bool UndirectedGraph::addEdge(int u, int v, int weight) {//确保两个顶点编号均大于0，通过检查邻接表确保两个点都在图中，实现了对adjList_和edges_的同步更新，通过edges_中加入两个有向边实现无向加权边
    if ( u <= 0 || v <= 0 ) {
        return false; // 顶点编号必须大于0
    }
    //在邻接表中查找而不是在顶点集合中查找，因为顶点集合中元素为对象指针，需要调用成员函数，加大代码复杂度
    if ( this -> adjList_.find(u) == this -> adjList_.end() || this -> adjList_.find(v) == this -> adjList_.end() ) {
        throw std::invalid_argument("顶点不存在");
        return false; // 顶点不存在
    }
    // addEdge 中添加边存在性检查
    if (this->findEdge(u, v) != nullptr) {
        return false; // 边已存在，无需重复添加
    }
    // 再执行后续 new Edge 和插入逻辑
    //addVertex中保持了adjList_与vertices_的一致性，因此这里直接操作adjList_即可，不必担心[]操作对adjList_的改变
    //用两个方向的有向边来表示无向边
    Vertex* v_vertex = this->findVertex(v); // 获取 vertices_ 中已有的 v 指针
    Vertex* u_vertex = this->findVertex(u); // 获取 vertices_ 中已有的 u 指针
    this->adjList_[u].insert(v_vertex); // 插入已有指针，而非新建
    this->adjList_[v].insert(u_vertex);
    std::pair<int, int> edge0 = std::make_pair(u, v);
    std::pair<int, int> edge1 = std::make_pair(v, u);
    this -> edges_.insert(new Edge(edge0, weight));
    this -> edges_.insert(new Edge(edge1, weight));
    return true;
}

bool UndirectedGraph::setEdgeId(Edge* e, int id) {
    if (e == nullptr || id < 0) {
        throw std::invalid_argument(id < 0 ? "边id必须非负" : "边指针为空");
    }
    for ( Edge* edge : this->edges_ ) {
        if ( edge->getId() == id ) {
            return false; // 边id已存在
        }
    }
    int u = e->getU();
    int v = e->getV();
    bool updated = false;
    for (Edge* edge : this->edges_) {
        // 同时匹配 u→v 和 v→u 边
        if ((edge->getU() == u && edge->getV() == v) || (edge->getU() == v && edge->getV() == u)) {
            edge->setId(id);
            updated = true;
        }
    }
    if (!updated) {
        throw std::invalid_argument("边不存在于图中");
    }
    return true;
}

int UndirectedGraph::degree(int v) const {//通过检查邻接表确保顶点存在，返回顶点v的度
    if ( this ->adjList_.find(v) == this -> adjList_.end() ) {
        throw std::invalid_argument("顶点不存在");
        return -1; // 顶点不存在
    }
    return this -> adjList_.find(v)->second.size();
}

Graph* UndirectedGraph::subgraph_by_vertices(const std::vector<int>& vertices) const {
    UndirectedGraph* subgraph = new UndirectedGraph();
    for ( int v : vertices ) {
        if ( this -> adjList_.find(v) != this -> adjList_.end() ) {
            subgraph -> addVertex(v);//addVertex函数中会自动判断顶点是否重复，因此无需判断
            Vertex* v_vertex = subgraph -> findVertex(v);
            v_vertex->setAttributesMap(this->findVertex(v)->getAttributesMap());//复制属性
        }
    }
    std::vector<int> vs = subgraph -> vertices();
    for ( int i = 0 ; i < vs.size() ; i++ ) {
        for ( int j = i + 1 ; j < vs.size() ; j++ ) {
            //子图中任意两点
            int u = vs[i];
            int v = vs[j];
            //确认两点间是否有边
            if ( this->adjList_.at(u).count(this->findVertex(v)) ) {//由于无向边，故u,v可以互换，因此不必关心u,v的顺序，此处为了鲁棒性
                subgraph -> addEdge(u, v, this->findEdge(u, v) -> getWeight());//addEdge函数中会自动判断边是否重复，因此无需判断,且addEdge函数中会自动处理无向边的表示方式
                Edge* e0 = subgraph -> findEdge(u, v);
                Edge* e1 = subgraph -> findEdge(v, u);
                e0->setId(this->findEdge(u, v)->getId());
                e1->setId(this->findEdge(v, u)->getId());
                e0->setAttributesMap(this->findEdge(u, v)->getAttributesMap());//复制属性
                e1->setAttributesMap(this->findEdge(v, u)->getAttributesMap());//复制属性
            }
        }
    }
    return subgraph;
}

Graph* UndirectedGraph:: subgraph_by_edges(const std::vector<Edge*>& edges) const {//遍历输入边集，将边的两个端点通过addVertex加入子图，再通过addEdge加入子图
    UndirectedGraph* subgraph = new UndirectedGraph();
    for ( Edge* e : edges ) {
        int u = e -> getU();
        int v = e -> getV();
        if (this->findEdge(u, v) == nullptr) {
            throw std::invalid_argument("输入边不属于原图");
        }
        int w = e -> getWeight();
        subgraph -> addVertex(u);//addVertex自动处理邻接表的创建以及vertices_的添加，且自动保证去重
        subgraph -> addVertex(v);//addVertex自动处理邻接表的创建以及vertices_的添加，且自动保证去重
        subgraph -> addEdge(u, v, w);//addEdge自动处理邻接表的更新以及edges_的添加，且自动保证去重以及用两个有向边表示无向边
        Vertex* v_vertex = subgraph -> findVertex(v);
        v_vertex->setAttributesMap(this->findVertex(v)->getAttributesMap());//复制属性
        Vertex* u_vertex = subgraph -> findVertex(u);
        u_vertex->setAttributesMap(this->findVertex(u)->getAttributesMap());//复制属性
        Edge* e0 = subgraph -> findEdge(u, v);
        Edge* e1 = subgraph -> findEdge(v, u);
        e0->setId(this->findEdge(u, v)->getId());
        e1->setId(this->findEdge(v, u)->getId());
        e0->setAttributesMap(this->findEdge(u, v)->getAttributesMap());//复制属性
        e1->setAttributesMap(this->findEdge(v, u)->getAttributesMap());//复制属性
    }
    return subgraph;
}

int UndirectedGraph::numVertices() const {
    return this -> vertices_.size();
}

int UndirectedGraph::numEdges() const {
    return this -> edges_.size() / 2; //由于无向边在edges_中存储了两次，因此除以2
}

bool UndirectedGraph::isDirected() const {
    return this->is_directed_;
}

std::vector<int> UndirectedGraph::neighbors(int v_id) const {//通过邻接表获取顶点v的邻居，返回邻居指针STL容器
    // 用 adjList_ 判断顶点是否存在，效率更高
    if (this->adjList_.find(v_id) == this->adjList_.end()) {
        throw std::invalid_argument("顶点不存在");
    }
    std::vector<int> neighbors;
    for (Vertex* w : this->adjList_.at(v_id)) {
        neighbors.push_back(w->getId());
    }
    return neighbors;
}

std::vector<Edge*> UndirectedGraph::edges() const {//将edges_中的边转化为STL容器返回
    std::vector<Edge*> edges;
    for ( Edge* e : this -> edges_ ) {  
        edges.push_back(e);
    }
    return edges;
}

std::vector<int> UndirectedGraph::vertices() const {//将vertices_中的顶点转化为STL容器返回
    std::vector<int> vertices;
    for ( Vertex* v : this -> vertices_ ) {
        vertices.push_back(v->getId());
    }
    return vertices;
}

Vertex* UndirectedGraph::findVertex(int id) const {//通过遍历vertices_查找顶点，返回顶点指针，若不存在则返回空指针
    for ( Vertex* v : this -> vertices_ ) {
        if ( v -> getId() == id ) {
            return v;
        }
    }
    return nullptr; // 顶点不存在，返回空指针
}

Edge* UndirectedGraph::findEdge(int u, int v) const {//通过遍历edges_查找边，返回边指针，若不存在则返回空指针
    for ( Edge* e : this -> edges_ ) {//对于此处，由于addVertex中保证了id与顶点的双射关系，因此可用id直接查找
        if ( e -> getU() == u  && e -> getV() == v  ) {
            return e;
        }
    }
    return nullptr; // 边不存在，返回空指针
}

Edge* UndirectedGraph::findEdge(int id) const {//通过遍历edges_查找边，返回边指针，若不存在则返回空指针
    for ( Edge* e : this -> edges_ ) {
        if ( e -> getId() == id ) {
            return e;
        }
    }
    return nullptr; // 边不存在，返回空指针
}

std::any UndirectedGraph::getVertexAttr(int id, std::string key) const {
    Vertex* v = this->findVertex(id);
    if (v == nullptr) {
        throw std::invalid_argument("顶点不存在");
    }
    return v->getAttribute(key);
}

std::any UndirectedGraph::getEdgeAttr(int u, int v, std::string key) const {
    Edge* e = this->findEdge(u, v);
    if (e == nullptr) {
        throw std::invalid_argument("边不存在");
    }
    return e->getAttribute(key);
}

std::any UndirectedGraph::getEdgeAttr(int id, std::string key) const {
    Edge* e = this->findEdge(id);
    if (e == nullptr) {
        throw std::invalid_argument("边不存在");
    }
    return e->getAttribute(key);
}

bool UndirectedGraph::setVertexAttr(int id, std::string key, std::any value) {
    Vertex* v = this->findVertex(id);
    if (v == nullptr) {
        return false;
    }
    return v->setAttribute(key, value);
}

bool UndirectedGraph::setEdgeAttr(int u, int v, std::string key, std::any value) {
    Edge* e0 = this->findEdge(u, v);
    Edge* e1 = this->findEdge(v, u);
    if (e0 == nullptr || e1 == nullptr) {//事实上，根据addEdge的实现，u,v可以互换，因此这里只需要判断u,v的一种情况即可,而这两者也必然同时存在或同时不存在，此处为了鲁棒性
        return false;
    }
    e0->setAttribute(key, value);
    e1->setAttribute(key, value);
    return true;
}

bool UndirectedGraph::setEdgeAttr(int id, std::string key, std::any value) {
    Edge* e = this->findEdge(id);
    if (e == nullptr) {
        return false;
    }
    int u = e->getU();
    int v = e->getV();
    Edge* e0 = this->findEdge(u, v);
    Edge* e1 = this->findEdge(v, u);
    e0->setAttribute(key, value);
    e1->setAttribute(key, value);
    return true;
}

bool UndirectedGraph::removeVertexAttr(int id, std::string key) {
    Vertex* v = this->findVertex(id);
    if (v == nullptr) {
        return false;
    }
    return v->deleteAttribute(key);
}

bool UndirectedGraph::removeEdgeAttr(int u, int v, std::string key) {
    Edge* e0 = this->findEdge(u, v);
    Edge* e1 = this->findEdge(v, u);
    if (e0 == nullptr || e1 == nullptr) {//事实上，根据addEdge的实现，u,v可以互换，因此这里只需要判断u,v的一种情况即可,而这两者也必然同时存在或同时不存在，此处为了鲁棒性
        return false;
    }
    return e0->deleteAttribute(key) && e1->deleteAttribute(key);
}

bool UndirectedGraph::removeEdgeAttr(int id, std::string key) {
    Edge* e = this->findEdge(id);
    if (e == nullptr) {
        return false;
    }
    int u = e->getU();
    int v = e->getV();
    Edge* e0 = this->findEdge(u, v);
    Edge* e1 = this->findEdge(v, u);
    return e0->deleteAttribute(key) && e1->deleteAttribute(key);
}

bool UndirectedGraph::clearVertexAttrs(int v) {
    Vertex* vertex = this->findVertex(v);
    if (vertex == nullptr) {
        return false;
    }
    return vertex->clearAttributes();
}

bool UndirectedGraph::clearEdgeAttrs(int u, int v) {
    Edge* e0 = this->findEdge(u, v);
    Edge* e1 = this->findEdge(v, u);
    if (e0 == nullptr || e1 == nullptr) {//事实上，根据addEdge的实现，u,v可以互换，因此这里只需要判断u,v的一种情况即可,而这两者也必然同时存在或同时不存在，此处为了鲁棒性
        return false;
    }
    return e0->clearAttributes() && e1->clearAttributes();
}

bool UndirectedGraph::clearEdgeAttrs(int id) {
    Edge* e = this->findEdge(id);
    if (e == nullptr) {
        return false;
    }
    int u = e->getU();
    int v = e->getV();
    Edge* e0 = this->findEdge(u, v);
    Edge* e1 = this->findEdge(v, u);
    return e0->clearAttributes() && e1->clearAttributes();
}

Graph* UndirectedGraph::clone() const {
    UndirectedGraph* graph = new UndirectedGraph();
    for ( Vertex* v : this -> vertices_ ) {
        graph -> addVertex(v->getId());
        graph -> findVertex(v->getId())->setAttributesMap(v->getAttributesMap());
    }
    for ( Edge* e : this -> edges_ ) {
        int u = e -> getU();
        int v = e -> getV();
        int w = e -> getWeight();
        graph -> addEdge(u, v, w);
        graph -> findEdge(u, v)->setAttributesMap(e->getAttributesMap());
        graph -> findEdge(u, v)->setId(e->getId());
    }
    return graph;
}
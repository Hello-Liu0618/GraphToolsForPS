#pragma once

#include "edge.h"
#include "vertex.h"

#include <iostream>
#include <string>
#include <vector>
#include <any>

//定义 Graph 类作为抽象基类，不提供具体的邻接表和边集容器，只提供最基本操作的接口
class Graph {
public:
    virtual ~Graph() {}
    virtual bool addVertex(int v) = 0;//insert a new vertex with id
    virtual bool addEdge(int u, int v) = 0;//insert a new edge (u,v)
    virtual bool addEdge(int u, int v, int weight) = 0;//insert a new edge (u,v) with weight
    virtual bool setEdgeId(Edge* e, int id) = 0;//set the id of edge e

    virtual int degree(int v) const = 0;//return the degree of vertex v

    virtual Graph* subgraph_by_vertices(const std::vector<int>& vertices) const = 0;//return a vertices-induced subgraph
    virtual Graph* subgraph_by_edges(const std::vector<Edge*>& edges) const = 0;//return a edges-induced subgraph
    //虽然此处输入为指针的集合，但是在实现中做到了按照编号查找，因此输入指针不需要与实际对象绑定，只需要保证指针有效即可，实际上保证了封装性


    virtual int numVertices() const = 0;//return the number of vertices
    virtual int numEdges() const = 0;//return the number of edges
    virtual bool isDirected() const = 0;//return true if the graph is directed, false otherwise

    virtual std::vector<int> neighbors(int v) const = 0;//return the neighbors of vertex v
    virtual std::vector<Edge*> edges() const = 0;//return all edges
    virtual std::vector<int> vertices() const = 0;//return all vertices

    virtual Vertex* findVertex(int id) const = 0;//return the vertex with id
    virtual Edge* findEdge(int u, int v) const = 0;//return the edge (u,v)
    virtual Edge* findEdge(int id) const = 0;//return the edge with id

    virtual std::any getVertexAttr(int v, std::string key) const = 0;//return the attribute of vertex v with name key
    virtual std::any getEdgeAttr(int u, int v, std::string key) const = 0;//return the attribute of edge (u,v) with name key
    virtual std::any getEdgeAttr(int id, std::string key) const = 0;//return the attribute of edge with id with name key

    virtual bool setVertexAttr(int v, std::string key, std::any value) = 0;//set the attribute of vertex v with name key to value
    virtual bool setEdgeAttr(int u, int v, std::string key, std::any value) = 0;//set the attribute of edge (u,v) with name key to value
    virtual bool setEdgeAttr(int id, std::string key, std::any value) = 0;//set the attribute of edge with id with name key to value
    virtual bool removeVertexAttr(int v, std::string key) = 0; // 删除顶点v的属性
    virtual bool removeEdgeAttr(int u, int v, std::string key) = 0; // 删除边(u,v)的属性
    virtual bool removeEdgeAttr(int id, std::string key) = 0; // 删除边id的属性 
    virtual bool clearVertexAttrs(int v) = 0; // 清除顶点v的属性
    virtual bool clearEdgeAttrs(int u, int v) = 0; // 清除边(u,v)的属性
    virtual bool clearEdgeAttrs(int id) = 0; // 清除边id的属性

    virtual Graph* clone() const = 0;//return a deep copy of the graph
};
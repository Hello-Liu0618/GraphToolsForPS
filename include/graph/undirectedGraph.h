#pragma once

#include "graph.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <any>

class UndirectedGraph : public Graph {//无向简单图类，不含自环，不含重边
    protected:
        std::unordered_map<int, std::unordered_set<Vertex*>> adjList_; // 邻接表，为无向简单图类特有
        std::unordered_set<Edge*> edges_; // 边集合，为无向简单图类特有
        std::vector<Vertex*> vertices_; // 顶点集合，为无向简单图类特有
        //UndirectedGraph* original_graph_ = nullptr; // 原图，为无向简单图类特有, 用于子图算法
        const bool is_directed_ = false; // 是否为有向图
    public:
        ~UndirectedGraph() override; // 析构函数

        bool addVertex(int v) override; // 向图中添加顶点
        bool addEdge(int u, int v) override; // 向图中添加边
        bool addEdge(int u, int v, int weight) override; // 向图中添加边伴随权重
        bool setEdgeId(Edge* e, int id) override; // 设置边的id

        int degree(int v) const override; // 顶点v的度

        Graph* subgraph_by_vertices(const std::vector<int>& vertices) const override; // 返回由指定顶点集合诱导的子图
        Graph* subgraph_by_edges(const std::vector<Edge*>& edges) const override; // 返回由指定边集合诱导的子图
        
        int numVertices() const override; // 图中顶点数
        int numEdges() const override; // 图中边数
        bool isDirected() const override; // 是否为有向图
        
        std::vector<int> neighbors(int v) const override; // 顶点v的邻居
        std::vector<Edge*> edges() const override; // 图中所有边
        std::vector<int> vertices() const override; // 图中所有顶点

        Vertex* findVertex(int id) const override; // 返回指定id的顶点
        Edge* findEdge(int u, int v) const override; // 返回指定两个端点的边
        Edge* findEdge(int id) const override; // 返回指定id的边

        std::any getVertexAttr(int v, std::string key) const override; // 获取顶点v的属性
        std::any getEdgeAttr(int u, int v, std::string key) const override; // 获取边(u,v)的属性
        std::any getEdgeAttr(int id, std::string key) const override; // 获取边id的属性

        bool setVertexAttr(int v, std::string key, std::any value) override; // 设置顶点v的属性
        bool setEdgeAttr(int u, int v, std::string key, std::any value) override; // 设置边(u,v)的属性
        bool setEdgeAttr(int id, std::string key, std::any value) override; // 设置边id的属性
        bool removeVertexAttr(int v, std::string key) override; // 删除顶点v的属性
        bool removeEdgeAttr(int u, int v, std::string key) override; // 删除边(u,v)的属性
        bool removeEdgeAttr(int id, std::string key) override; // 删除边id的属性
        bool clearVertexAttrs(int v) override; // 清除顶点v的属性
        bool clearEdgeAttrs(int u, int v) override; // 清除边(u,v)的属性
        bool clearEdgeAttrs(int id) override; // 清除边id的属性

        Graph* clone() const override;// 克隆图，返回深拷贝
};
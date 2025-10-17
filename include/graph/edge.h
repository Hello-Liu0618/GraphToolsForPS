#pragma once

#include <iostream>
#include <string>
#include <any>
#include <unordered_map>
#include <utility>

class Edge {
private:
    int id_ = -1;//唯一标识符,很多时候并不使用
    int u_ = -1;//起点
    int v_ = -1;//终点
    int weight_ = 1;//权重
    std::unordered_map<std::string, std::any> attributes_;


public:
    Edge(int id, std::pair<int, int> uv, int weight);
    Edge(int id, std::pair<int, int> uv);
    Edge(std::pair<int, int> uv, int weight);
    Edge(std::pair<int, int> uv);
    Edge(const Edge& other);
    ~Edge();

    int getId() const;
    int getU() const;
    int getV() const;
    int getWeight() const;
    std::any getAttribute(const std::string &key) const;//获取某一种属性
    std::unordered_map<std::string, std::any> getAttributesMap();//获取属性表

    bool isSame(const Edge &e);

    bool setId(int id);
    bool setWeight(int weight);
    bool setAttribute(const std::string &key, std::any value);//设置某一种属性
    bool setAttributesMap(const std::unordered_map<std::string, std::any> &attr);
    bool deleteAttribute(const std::string &key);//删除某一种属性
    bool clearAttributes();

    std::string toString() const;
};
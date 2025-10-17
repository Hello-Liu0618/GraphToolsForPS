#pragma once

#include <iostream>
#include <unordered_map>
#include <any>
#include <string>

//using namespace std;

class Vertex {
private:
    int id_;//唯一标识符
    //int degree;//度数
    std::unordered_map<std::string, std::any> attributes_;//属性表
    //degree/d/low/color/high/parent/child/dist/predecessor/visited/time/key/value...

public:
    //三种初始化方法
    Vertex();
    Vertex(const Vertex& other);
    Vertex(int id_);
    ~Vertex();

    //获取属性
    int getId() const;
    std::any getAttribute(const std::string &key) const;//获取某一个属性
    std::unordered_map<std::string, std::any> getAttributesMap();//获取属性表

    //设置属性
    bool setId(int id_);
    bool setAttribute(const std::string &key, std::any value);//设置某一个属性
    bool setAttributesMap(const std::unordered_map<std::string, std::any> &attributes_);//复制设置属性表
    bool deleteAttribute(const std::string &key);//删除某一个属性
    bool clearAttributes();

    //返回该点的字符串描述
    std::string toString() const;
};
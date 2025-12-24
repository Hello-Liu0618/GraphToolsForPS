#include "../include/graph/vertex.h"

#include <iostream>
#include <unordered_map>
#include <any>
#include <string>
#include <sstream>

//using namespace std;

//默认构造函数
Vertex::Vertex() {
    this -> id_ = -1; //默认id为-1，表示未设置
}

Vertex::Vertex(const Vertex& other) {//复制构造
    this -> id_ = other.id_;
    //this -> attributes_ = other.attributes_;
    //std::unordered_map<std::string, std::any> attr;
    for ( auto it = other.attributes_.begin() ; it != other.attributes_.end() ; it++ ) {
        this->attributes_[it -> first] = it -> second;
    }
}

Vertex::Vertex(int id) {
    this -> id_ = id;
}

Vertex::~Vertex() {
    //析构函数
    attributes_.clear();
}

//获取属性
int Vertex::getId() const {
    return id_;
}

//获取某一个属性
std::any Vertex::getAttribute(const std::string& key) const {
    auto it = attributes_.find(key);
    if (it != attributes_.end()) {
        return it -> second;//返回对应的属性
    }
    throw std::invalid_argument("Attribute not found");
}

//获取属性表
std::unordered_map<std::string, std::any> Vertex::getAttributesMap() {
    std::unordered_map<std::string, std::any> attr;
    for ( auto it = attributes_.begin() ; it != attributes_.end() ; it++ ) {
        attr[it -> first] = it -> second;
    }
    return attr;
}

//设置属性
bool Vertex::setId(int id) {
    if (id <= 0){
        return false;//id必须大于0
    }
    this -> id_ = id;
    return true;
}

//设置某一个属性
bool Vertex::setAttribute(const std::string &key, std::any value) {
    if ( key.empty() ) {
        return false;//key不能为空
    }
    attributes_[key] = value;
    return true;
}

//设置属性表
bool Vertex::setAttributesMap(const std::unordered_map<std::string, std::any>& attributes) {
    if (attributes.empty()) {
        return false;//输入属性表不能为空
    }
    attributes_.clear();
    for ( auto it = attributes.begin() ; it != attributes.end() ; it++ ) {
        this->attributes_[it -> first] = it -> second;
    }
    return true;    
}

//删除某一个属性
bool Vertex::deleteAttribute(const std::string& key) {
    if (key.empty()) {
        return false;//key不能为空
    }
    if (attributes_.find(key) == attributes_.end()) {
        return false;//属性不存在
    }
    attributes_.erase(key);
    return true;
}

//清空属性表
bool Vertex::clearAttributes() {
    attributes_.clear();
    return true;
}

//返回该点的字符串描述
std::string Vertex::toString() const {
    std::ostringstream oss;
    oss << "Vertex id: " << id_ << "\n";
    oss << "Attributes: " << "\n";
    for (auto it = attributes_.begin(); it != attributes_.end(); it++) {
        oss << it->first << " : ";
        if (it->second.type() == typeid(int)) {
            oss << std::any_cast<int>(it->second);
        } else if (it->second.type() == typeid(std::string)) {
            oss << std::any_cast<std::string>(it->second);
        } else if (it->second.type() == typeid(bool)) {
            oss << std::boolalpha << std::any_cast<bool>(it->second);
        } else if (it->second.type() == typeid(long)) {
            oss << std::any_cast<long>(it->second);
        } else if (it->second.type() == typeid(long long)) {
            oss << std::any_cast<long long>(it->second);
        } else if (it->second.type() == typeid(float)) {
            oss << std::any_cast<float>(it->second);
        } else if (it->second.type() == typeid(double)) {
            oss << std::any_cast<double>(it->second);
        } else {
            oss << "Unsupported type";
        }
        oss << "\n";
    }
    return oss.str();
}

#include "../include/graph/edge.h"

#include <iostream>
#include <string>
#include <sstream>

//构造与析构函数
Edge::Edge(int id, std::pair<int, int> uv, int weight) {
    this -> id_ = id;
    this -> u_ = uv.first;
    this -> v_ = uv.second;
    this -> weight_ = weight;
}
Edge::Edge(int id, std::pair<int, int> uv) {
    this -> id_ = id;
    this -> u_ = uv.first;
    this -> v_ = uv.second;
}
Edge::Edge(std::pair<int, int> uv, int weight) {
    this -> u_ = uv.first;
    this -> v_ = uv.second;
    this -> weight_ = weight;
}
Edge::Edge(std::pair<int, int> uv) {
    this -> u_ = uv.first;
    this -> v_ = uv.second;
}
Edge::Edge(const Edge& other) {
    this -> id_ = other.id_;
    this -> u_ = other.u_;
    this -> v_ = other.v_;
    this -> weight_ = other.weight_;
    for ( auto it = other.attributes_.begin() ; it != other.attributes_.end() ; it++ ) {
        this -> attributes_[it -> first] = it -> second;
    }
}
Edge::~Edge() {
    attributes_.clear();
}

//获取属性
int Edge::getId() const {
    return this -> id_;
}
int Edge::getU() const {
    return this -> u_;
}
int Edge::getV() const {
    return this -> v_;
}
int Edge::getWeight() const {
    return this -> weight_;
}
//获取某一种属性
std::any Edge::getAttribute(const std::string &key) const{
    auto it = attributes_.find(key);
    if ( it != attributes_.end() ) {
        return it -> second;
    }
    throw std::out_of_range("Attribute not found");
}
//获取属性表
std::unordered_map<std::string, std::any> Edge::getAttributesMap() {
    std::unordered_map<std::string, std::any> attr;
    for ( auto it = attributes_.begin() ; it != attributes_.end() ; it++ ) {
        attr[it -> first] = it -> second;
    }
    return attr;
}

//判断是否相同
bool Edge::isSame(const Edge &e) {
    return (this -> id_ == e.getId() && this -> u_ == e.getU() && this -> v_ == e.getV() && this -> weight_ == e.getWeight());
}

//设置id
bool Edge::setId(int id) {
    if ( id <= 0 ) {
        return false;//id必须大于0
    }
    this -> id_ = id;
    return true;
}

bool Edge::setWeight(int weight) {
    this -> weight_ = weight;
    return true;
}

//设置某一种属性
bool Edge::setAttribute(const std::string &key, std::any value) {
    if ( key.empty() ) {
        return false;//key不能为空
    }
    this -> attributes_[key] = value;
    return true;
}

//设置属性表
bool Edge::setAttributesMap(const std::unordered_map<std::string, std::any> &attr) {
    if ( attr.empty() ) {
        return false;//属性表不能为空
    }
    attributes_.clear();
    for ( auto it = attr.begin() ; it != attr.end() ; it++ ) {
        this -> attributes_[it -> first] = it -> second;
    }
    return true;
}

bool Edge::deleteAttribute(const std::string &key) {
    if ( key.empty() ) {
        return false;//key不能为空
    }
    auto it = attributes_.find(key);
    if ( it != attributes_.end() ) {
        attributes_.erase(it);
        return true;
    }
    return false;//属性不存在
}

bool Edge::clearAttributes() {
    attributes_.clear();
    return true;
}

//返回该边的字符串描述
std::string Edge::toString() const {
    std::ostringstream oss;
    oss << "id: " << id_ << "\n";
    oss << "from vertexU: " << u_ << " to vertexV: " << v_ << "\n";
    oss << "weight: " << weight_ << "\n";
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